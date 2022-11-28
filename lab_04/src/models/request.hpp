#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <optional>

enum class RequestType {
  kPost,
  kGet,
  kDelete,
};

struct Request {
  std::string path;
  RequestType type;
  std::string http_version;
  std::map<std::string, std::string> headers;
  std::optional<std::string> body;
};
