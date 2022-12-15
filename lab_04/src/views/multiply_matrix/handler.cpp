#include "handler.hpp"

#include <strstream>

#include "fmt/format.h"

#include "models/response.hpp"
#include "models/request.hpp"
#include "models/matrix.hpp"

namespace {

template<typename ValueType>
models::Matrix<ValueType> MultiplySimple(
    const models::Matrix<ValueType>& first,
    const models::Matrix<ValueType>& second) {
  if (first.GetSize().second != second.GetSize().first) {
    throw std::runtime_error("Inappropriate matrix dimensions to execute "
        "multiplication");
  }

  const auto result_size = std::pair(first.GetSize().first,
                                     second.GetSize().second);
  models::Matrix<ValueType> result{result_size.first, result_size.second};
  for (std::size_t i = 0; i < result_size.first; ++i) {
    for (std::size_t j = 0; j < result_size.second; ++j) {
      for (std::size_t k = 0; k < first.GetSize().second; ++k) {
        result[i][j] += first[i][k] * second[k][j];
      }
    }
  }

  return result;
}

template<typename ValueType>
std::string ToString(const models::Matrix<ValueType>& matrix) {
  std::string result{};

  result += fmt::format(
      "{} {}\n", matrix.GetSize().first, matrix.GetSize().second);

  for (size_t i = 0; i < matrix.GetSize().first; ++i) {
    for (size_t j = 0; j < matrix.GetSize().second; ++j) {
      result += fmt::format("{} ", matrix[i][j]);
    }
    result += "\n";
  }

  return result;
}

}  // namespace

namespace views::multiply_matrix {

Response Handler::HandleRequest(const Request& request) {
  if (!request.body.has_value()) {
    return Response{400, "Body of request is empty"};
  }

  std::stringstream body_string_stream{};
  body_string_stream << request.body->c_str();

  models::Matrix<int> left{};
  models::Matrix<int> right{};
  models::Matrix<int> result{};
  try {
    body_string_stream >> left >> right;
    result = MultiplySimple(left, right);
  } catch (const std::runtime_error& e) {
    return Response{400, "Bad matrix format"};
  }

  return Response{200, ToString(result)};
}

}  // namespace views::get_tasks
