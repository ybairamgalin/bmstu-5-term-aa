#include "fmt/format.h"

#include "models/request.hpp"
#include "models/response.hpp"

namespace views {

class Handler {
 public:
  static Response HandleRequest(const Request& request);
};

}  // namespace views
