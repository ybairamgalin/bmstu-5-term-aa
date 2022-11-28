#include "handler.hpp"

#include <thread>
#include <chrono>

#include "models/response.hpp"
#include "models/request.hpp"

namespace {

}  // namespace

namespace views::get_tasks {


Response Handler::HandleRequest(const Request& request) {
  if (!request.body.has_value()) {
    return Response{400, "Body of request is empty"};
  }

  int lol = 0;
  for (int i = 0; i < 1000; ++i) {
    for (int j = 0; j < 1000; ++j) {
      for (int k = 0; k < 1000; ++k) {
        if (lol > 1000000000) {
          lol -= i * j * k;
        } else {
          lol += i * j * k;
        }
      }
    }
  }

  return Response{200, std::to_string(lol)};
}

}  // namespace views::get_tasks