#include <iostream>
#include <stdexcept>

#include <server/server.hpp>

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
