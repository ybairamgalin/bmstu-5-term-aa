#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "models/request.hpp"
#include "models/response.hpp"
#include "utils/parse.hpp"
#include "views/get_tasks/handler.hpp"
#include "views/handler.hpp"
#include "pool/thread_pool.hpp"
#include "components.hpp"

namespace {

int kServerPortNumber = 5002;

}

class Server {
  using Task = std::function<void()>;
 public:
  Server()
      : socket_descriptor_(socket(AF_INET, SOCK_STREAM, 0)),
        endpoint_provider_(components::EndpointProvider()),
        thread_pool_(24) {
    endpoint_provider_.Register(
        views::Handler::HandleRequest,
        RequestType::kGet, "/");
    endpoint_provider_.Register(
        views::get_tasks::Handler::HandleRequest,
        RequestType::kPost, "/get_tasks");

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

  void StartListening() {
    listen(socket_descriptor_, 100);

    sockaddr_in client_address{};
    socklen_t client_length = sizeof(client_address);
    std::cout << "Listening on port " << kServerPortNumber << "\n";

    while (true) {
      std::cout << "Request number " << ++count_ << "\n";
      const auto new_sfd = accept(
          socket_descriptor_, (sockaddr*)&client_address, &client_length);
      if (new_sfd < 0) {
        throw std::runtime_error("Accept error");
      }

//      ProcessRequest(new_sfd);
//      close(new_sfd);

      thread_pool_.Submit([this, new_sfd]() {
        ProcessRequest(new_sfd);
        close(new_sfd);
      });
    }
  }

  void Stop() {
    close(socket_descriptor_);
  }

 private:
  int socket_descriptor_;
  int count_ = 0;
  components::EndpointProvider endpoint_provider_;
  StaticThreadPool<Task> thread_pool_;


  void ProcessRequest(int new_socket_descriptor) const {
    char buffer[4096];
    bzero(buffer, sizeof(buffer));

    if (read(new_socket_descriptor, buffer, sizeof(buffer)) == -1) {
      throw std::runtime_error("Read error");
    }

    Request request{};
    Response response{};
    try {
      request = utils::Parse(buffer, utils::To<Request>());
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
};

int main() {
  Server server{};

  try {
    server.StartListening();
  } catch (std::runtime_error& error) {
    std::cerr << error.what();
    exit(1);
  }

  return 0;
}
