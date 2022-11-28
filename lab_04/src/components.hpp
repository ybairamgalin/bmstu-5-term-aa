#pragma once

#include <functional>
#include <map>
#include <string>
#include <stdexcept>
#include <algorithm>

#include <fmt/format.h>

#include "models/response.hpp"
#include "models/request.hpp"

#include "utils/to_string.hpp"

namespace components {

using Endpoint = std::function<Response(const Request&)>;

class EndpointProvider {
 public:
  const Endpoint& Get(
      const RequestType request_type,
      const std::string& path) const {
    std::string key = utils::ToString(request_type) + path;

    if (!endpoints_.count(key)) {
      throw std::runtime_error(
          fmt::format("No endpoints to handle {} {} set",
                      utils::ToString(request_type), path));
    }

    return endpoints_.at(key);
  }
  void Register(
      const Endpoint& endpoint,
      const RequestType request_type,
      const std::string& path) {
    endpoints_.insert_or_assign(utils::ToString(request_type) + path, endpoint);
  }

 private:
  std::map<std::string, Endpoint> endpoints_;
};

}  // namespace components
