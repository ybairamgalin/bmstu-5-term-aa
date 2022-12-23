#include "server.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <functional>
#include <memory>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <pqxx/pqxx>

#include "models/request.hpp"
#include "models/response.hpp"
#include "utils/parse.hpp"
#include "views/handler.hpp"
#include "pool/thread_pool.hpp"
#include "components.hpp"

namespace {

constexpr const int kServerPortNumber = 5002;

constexpr const int kParseThreadCount = 2;
constexpr const int kDbThreadCount = 8;
constexpr const int kResultThreadCount = 2;

inline constexpr const char kConnectionString[] =
    "host=localhost dbname=lab_01";

using Task = std::function<void()>;

StaticThreadPool<Task> thread_pool_parse{kParseThreadCount};
StaticThreadPool<Task> thread_pool_db_connect{kDbThreadCount};
StaticThreadPool<Task> thread_pool_response{kResultThreadCount};

void ProcessResult(
    int socket_descriptor,
    const std::vector<std::string>& car_numbers) {
  Response response = [&](){
    if (car_numbers.empty()) {
      return Response{400, "Bas request"};
    }
    std::string response_string = "{\n";
    for (const auto& number : car_numbers) {
      response_string += fmt::format("\t'{}',\n", number);
    }
    response_string += "}\n";

    return Response{200, response_string};
  }();

  const auto raw_response = BuildResponseString(response);
  write(socket_descriptor, raw_response.c_str(), raw_response.size());
}

void ProcessDb(
    int socker_descriptor,
    Request request) {
  if (!request.body.has_value()) {
    thread_pool_response.Submit([=](){
      ProcessResult(socker_descriptor, {});
    });
  }

  int count_from_requests;
  try {
    count_from_requests = std::stoi(request.body.value());
  } catch (const std::exception& e) {
    thread_pool_response.Submit([=](){
      ProcessResult(socker_descriptor, {});
    });
    return;
  }

  std::vector<std::string> result;
  const auto query = fmt::format(
      "select state_number "
      "from public.cars "
      "order by state_number "
      "limit {}", count_from_requests);
  try {
    pqxx::connection db_connection(kConnectionString);
    pqxx::work transaction{db_connection};
    for (auto [number] : transaction.stream<std::string>(query)) {
      result.push_back(number);
    }
  } catch (const std::exception& e) {
    std::cout << e.what();
    return;
  }

  thread_pool_response.Submit([=](){
    ProcessResult(socker_descriptor, result);
  });
}

void ProcessParse(int socket_descriptor) {
  constexpr const size_t buffer_size = 524288;
  std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);
  bzero(buffer.get(), buffer_size);

  if (recv(socket_descriptor, buffer.get(),
           buffer_size, 0) == -1) {
    throw std::runtime_error("Read error");
  }

  Request request{};
  try {
    std::cout << buffer.get();
    request = utils::Parse(buffer.get(), utils::To<Request>());
  } catch (std::exception& error) {
    std::cerr << "Error occurred while handling request : " << error.what();
  }

  thread_pool_db_connect.Submit([=](){
    ProcessDb(socket_descriptor, request);
  });
}

}

Server::Server()
    : socket_descriptor_(socket(AF_INET, SOCK_STREAM, 0)) {
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
    thread_pool_parse.Submit([new_sfd]() {
      ::ProcessParse(new_sfd);
    });
  }
}
