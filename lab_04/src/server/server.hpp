#pragma once

#include <iostream>
#include <string>
#include <functional>

#include "pool/thread_pool.hpp"
#include "components.hpp"

class Server {
  using Task = std::function<void()>;
 public:
  Server();

  void StartListening();

 private:
  void ProcessRequest(int new_socket_descriptor) const;

  int socket_descriptor_;
  components::EndpointProvider endpoint_provider_;
  StaticThreadPool<Task> thread_pool_;
};
