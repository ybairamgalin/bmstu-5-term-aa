#include "components.hpp"

#include <string>
#include <stdexcept>
#include <algorithm>

#include "utils/to_string.hpp"

#include "models/response.hpp"
#include "models/request.hpp"

namespace components {

const Endpoint& EndpointProvider::Get(
    RequestType request_type,
    const std::string& path) const {
  std::string key = utils::ToString(request_type) + path;

  if (!endpoints_.count(key)) {
    throw std::runtime_error(
        fmt::format("No endpoints to handle {} {} set",
                    utils::ToString(request_type), path));
  }

  return endpoints_.at(key);
}

void EndpointProvider::Register(
    const Endpoint& endpoint,
    RequestType request_type,
    const std::string& path) {
  endpoints_.insert_or_assign(utils::ToString(request_type) + path, endpoint);
}

}  // namespace components
