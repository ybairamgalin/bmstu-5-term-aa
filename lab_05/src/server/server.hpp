#pragma once

#include <iostream>
#include <string>
#include <functional>

#include <pqxx/pqxx>

#include "pool/thread_pool.hpp"
#include "components.hpp"

class Server {
  using Task = std::function<void()>;
 public:
  Server();

  void StartListening();

 private:

  int socket_descriptor_;
};
