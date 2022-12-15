#include "server.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>
#include <memory>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "models/request.hpp"
#include "models/response.hpp"
#include "utils/parse.hpp"
#include "views/multiply_matrix/handler.hpp"
#include "views/handler.hpp"
#include "pool/thread_pool.hpp"
#include "components.hpp"

namespace {

int kServerPortNumber = 5002;

}

Server::Server()
    : socket_descriptor_(socket(AF_INET, SOCK_STREAM, 0)),
      endpoint_provider_(components::EndpointProvider()),
      thread_pool_(31) {
  endpoint_provider_.Register(
      views::Handler::HandleRequest,
      RequestType::kGet, "/");
  endpoint_provider_.Register(
      views::multiply_matrix::Handler::HandleRequest,
      RequestType::kPost, "/api/multiply_matrix");

  if (socket_descriptor_ == -1) {
    throw std::runtime_error("Socker error");
  }

  int enable = 1;
  const auto opt_rc = setsockopt(socket_descriptor_, SOL_SOCKET, SO_REUSEADDR,
                                 &enable, sizeof(int));
  if (opt_rc == -1) {
    throw std::runtime_error("Setopt error");
  }

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(kServerPortNumber);

  auto* converted_address = (sockaddr*)&address;
  if (bind(socket_descriptor_, converted_address, sizeof(address)) < 0) {
    throw std::runtime_error("Bind error");
  }

  std::cout << "Server is ready to accept connections\n";
}

void Server::StartListening() {
  listen(socket_descriptor_, 100);

  sockaddr_in client_address{};
  socklen_t client_length = sizeof(client_address);
  std::cout << "Listening on port " << kServerPortNumber << "\n";

  while (true) {
    const auto new_sfd = accept(
        socket_descriptor_, (sockaddr*)&client_address, &client_length);
    if (new_sfd < 0) {
      throw std::runtime_error("Accept error");
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    thread_pool_.Submit([this, new_sfd]() {
      ProcessRequest(new_sfd);
      close(new_sfd);
    });
  }
}

void Server::ProcessRequest(int new_socket_descriptor) const {
  constexpr const size_t buffer_size = 524288;
  std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);
  bzero(buffer.get(), buffer_size);

  if (recv(new_socket_descriptor, buffer.get(),
           buffer_size, 0) == -1) {
    throw std::runtime_error("Read error");
  }

  Request request{};
  Response response{};
  try {
    std::cout << buffer.get();
    request = utils::Parse(buffer.get(), utils::To<Request>());
    const auto endpoint = endpoint_provider_.Get(request.type, request.path);
    response = endpoint(request);
  } catch (std::exception& error) {
    std::cerr << "Error occurred while handling request : " << error.what();
    response = Response{500, "Server error"};
  }

  const auto response_string = BuildResponseString(response);
  const auto written_bytes = write(
      new_socket_descriptor, response_string.c_str(), response_string.size());

  if (written_bytes < 0) {
    std::cerr << "Write error";
  }
}
