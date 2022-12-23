#pragma once

#include <functional>
#include <map>

#include <fmt/format.h>

#include "models/response.hpp"
#include "models/request.hpp"

namespace components {

using Endpoint = std::function<Response(const Request&)>;

class EndpointProvider {
 public:
  [[nodiscard]] const Endpoint& Get(
      RequestType request_type,
      const std::string& path) const;

  void Register(
      const Endpoint& endpoint,
      RequestType request_type,
      const std::string& path);

 private:
  std::map<std::string, Endpoint> endpoints_;
};

}  // namespace components
