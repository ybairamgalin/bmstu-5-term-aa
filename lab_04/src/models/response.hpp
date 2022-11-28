#pragma once

#include <string>
#include <map>

#include "fmt/format.h"

namespace {

const std::map<int, std::string> kCodeToStatus{
  std::pair(200, "OK"),
  std::pair(400, "Bad Request"),
  std::pair(500, "Internal Server Error"),
};

const std::map<int, std::string> kCodeToContentType{
    std::pair(200, "text/html"),
    std::pair(400, "text"),
    std::pair(500, "text"),
};

}

struct Response {
  int status_code;
  std::string message;
};

std::string BuildResponseString(const Response& response);
