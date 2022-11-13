#pragma once

#include <vector>
#include <stdexcept>
#include <istream>

namespace models {

template <typename ValueType>
class Matrix {
 public:
  using Row = std::vector<ValueType>;
  using MatrixType = std::vector<Row>;

  Matrix()
      : values_(std::vector(1, std::vector<ValueType>(1))) {}

  Matrix(std::size_t rows, std::size_t columns)
      : values_(std::vector(rows, std::vector<ValueType>(columns))) {
    static_assert(std::is_arithmetic_v<ValueType>,
        "ValueType should be an arithmetic type in Matrix");

    if (rows == 0 || columns == 0) {
      throw std::runtime_error("Each dimension should be at least 1");
    }
  }

  Row& operator[](std::size_t index) { return values_[index]; }

  const Row& operator[](std::size_t index) const { return values_[index]; }

  [[nodiscard]] std::pair<std::size_t, std::size_t> GetSize() const {
    return std::pair(values_.size(), values_[0].size());
  }

 private:
  MatrixType values_;
};

template<typename ValueType>
std::istream& operator>>(std::istream& input, Matrix<ValueType>& matrix) {
  std::size_t rows, columns;
  input >> rows >> columns;

  matrix = Matrix<ValueType>{rows, columns};
  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      input >> matrix[i][j];
    }
  }

  return input;
}

}  // namespace models
