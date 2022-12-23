#include "handler.hpp"

#include <iostream>
#include <fstream>

#include "models/response.hpp"
#include "models/request.hpp"

namespace {

}  // namespace

namespace views {


Response Handler::HandleRequest(const Request&) {
  std::fstream index;
  index.open("/Users/yaroslavbairamgalin/Desktop/Studies/BMSTU/"
      "5 term/aa/aa_labs/lab_04/nginx/www/index.html", std::ios::in);

  if (!index.is_open()) {
    return Response{500, "Could not read file"};
  }

  std::string content;
  std::string buf;
  while (getline(index, buf)) {
    content += buf;
  }

  return Response{200, content};
}

}  // namespace views