#include "response.hpp"

#include <iostream>

std::string BuildResponseString(const Response& response) {
  std::string response_string = fmt::format(
      "HTTP/1.1 {} {}\n"
      "Content-Length: {}\n"
      "Content-Type: {}\n"
      "\n"
      "{}\n",
      response.status_code,
      kCodeToStatus.at(response.status_code),
      response.message.size(),
      kCodeToContentType.at(response.status_code),
      response.message);

  return response_string;
}
