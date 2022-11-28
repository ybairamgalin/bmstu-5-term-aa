#pragma once

#include "fmt/format.h"

#include "models/request.hpp"
#include "models/response.hpp"

namespace views::get_tasks {

class Handler {
 public:
  static Response HandleRequest(const Request& request);
};

}  // namespace views::get_tasks
