#include "parse.hpp"

#include <map>
#include <vector>
#include <string>
#include <optional>
#include <algorithm>

#include "models/request.hpp"

namespace {

struct RequestLine {
  std::string path;
  std::string type;
  std::string http_version;
};

RequestLine ParseRequestLine(const std::string& request_line) {
  RequestLine result{};

  try {
    std::istringstream string_stream(request_line);
    std::string buf;
    getline(string_stream, buf, ' ');
    result.type = buf;
    getline(string_stream, buf, ' ');
    result.path = buf;
    getline(string_stream, buf, ' ');
    result.http_version = buf;
  } catch (std::exception& e) {
    std::cerr << e.what();
    throw;
  }

  return result;
}

std::map<std::string, std::string> ParseHeaders(
    const std::vector<std::string>& split_request) {
  std::map<std::string, std::string> headers{};
  if (split_request.empty()) {
    return headers;
  }

  for (size_t i = 1; i < split_request.size(); ++i) {
    if (split_request[i].empty()) {
      break ;
    }

    const auto key = split_request[i].substr(0, split_request[i].find(": "));
    const auto value =
        split_request[i].substr(split_request[i].find(": ") + 2,
                                split_request[i].size() - 1);
    headers[key] = value;
  }

  return headers;
}

std::optional<std::string> ParseBody(
    const std::vector<std::string>& split_request) {
  size_t empty_line_index = 0;
  while (empty_line_index < split_request.size() &&
         !split_request[empty_line_index].empty()) {
    ++empty_line_index;
  }

  if (empty_line_index == split_request.size()) {
    return std::nullopt;
  }
  if (empty_line_index + 1 == split_request.size()) {
    return std::nullopt;
  }

  std::string request_body{};
  for (size_t i = empty_line_index + 1; i < split_request.size(); ++i) {
    request_body += split_request[i] + "\n";
  }

  return request_body;
}

}  // namespace

namespace utils {

Request Parse(std::string str, To<Request>) {
  std::vector<std::string> request_split{};
  str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());

  std::istringstream string_stream(str);
  std::string buf;
  while (getline(string_stream, buf, '\n')) {
    request_split.push_back(std::move(buf));
  }
  if (request_split.empty()) {
    throw std::runtime_error("Parsing : empty request");
  }

  Request result{};
  const auto request_line = ParseRequestLine(request_split[0]);
  result.type = Parse(request_line.type, To<RequestType>());
  result.http_version = request_line.http_version;
  result.path = request_line.path;
  result.headers = ParseHeaders(request_split);
  result.body = ParseBody(request_split);

  return result;
}

RequestType Parse(const std::string& str, To<RequestType>) {
  std::string str_copy = str;
  std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(), ::toupper);

  if (str_copy == "POST") {
    return RequestType::kPost;
  } else if (str_copy == "GET") {
    return RequestType::kGet;
  } else if (str_copy == "DELETE") {
    return RequestType::kDelete;
  }

  throw std::runtime_error("Bad request type or such type cannot be handled");
}

}  // namespace utils
