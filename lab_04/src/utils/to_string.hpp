#pragma once

#include <string>
#include <stdexcept>

#include "models/request.hpp"

namespace utils {

std::string ToString(const RequestType request_type) {
  if (request_type == RequestType::kPost) {
    return "POST";
  } else if (request_type == RequestType::kGet) {
    return "GET";
  } else if (request_type == RequestType::kDelete) {
    return "DELETE";
  }

  throw std::runtime_error("Request type string not specified");
}

}  // namespace utils
