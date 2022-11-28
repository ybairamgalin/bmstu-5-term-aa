#pragma once

#include <vector>
#include <optional>
#include <string>
#include <sstream>

#include "models/request.hpp"

namespace utils {

template<typename T>
struct To{};

Request Parse(std::string str, To<Request>);
RequestType Parse(const std::string& str, To<RequestType>);

}  // namespace utils
