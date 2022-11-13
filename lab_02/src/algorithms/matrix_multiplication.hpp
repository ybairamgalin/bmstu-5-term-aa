#pragma once

#include <stdexcept>

#include "models/matrix.hpp"

namespace algorithms {

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
models::Matrix<ValueType> MultiplyVinograd(
    const models::Matrix<ValueType>& first,
    const models::Matrix<ValueType>& second) {
  if (first.GetSize().second != second.GetSize().first) {
    throw std::runtime_error("Inappropriate matrix dimensions to execute "
                             "multiplication");
  }
  // странные концтрукции по типу j = j + 2 нужны, чтобы отследить как микро
  // оптимизации влияют на производительность

  typename models::Matrix<ValueType>::Row row_factors(first.GetSize().first);
  for (std::size_t i = 0; i < first.GetSize().first; ++i) {
    for (std::size_t j = 0; j < first.GetSize().second - 1; j = j + 2) {
      row_factors[i] = row_factors[i] + first[i][j] * first[i][j + 1];
    }
  }
  typename models::Matrix<ValueType>::Row
      column_factors(second.GetSize().second);
  for (std::size_t i = 0; i < second.GetSize().second; ++i) {
    for (std::size_t j = 0; j < second.GetSize().first - 1; j = j + 2) {
      column_factors[i] = column_factors[i] + second[j][i] * second[j + 1][i];
    }
  }

  const auto result_size = std::pair(first.GetSize().first,
                                     second.GetSize().second);
  models::Matrix<ValueType> result{result_size.first, result_size.second};
  for (std::size_t i = 0; i < result_size.first; ++i) {
    for (std::size_t j = 0; j < result_size.second; ++j) {
      result[i][j] = -row_factors[i] - column_factors[j];
      for (std::size_t k = 0; k < first.GetSize().second - 1; k = k + 2) {
        result[i][j] = result[i][j] + (first[i][k + 1] + second[k][j]) *
                                      (first[i][k] + second[k + 1][j]);
      }
    }
  }

  if (first.GetSize().second % 2 == 1) {
    for (std::size_t i = 0; i < result_size.first; ++i) {
      for (std::size_t j = 0; j < result_size.second; ++j) {
        result[i][j] =
            result[i][j] + first[i][first.GetSize().second - 1] *
                           second[second.GetSize().first - 1][j];
      }
    }
  }

  return result;
}

template<typename ValueType>
models::Matrix<ValueType> MultiplyVinogradOptimized(
    const models::Matrix<ValueType>& first,
    const models::Matrix<ValueType>& second) {
  if (first.GetSize().second != second.GetSize().first) {
    throw std::runtime_error("Inappropriate matrix dimensions to execute "
        "multiplication");
  }

  typename models::Matrix<ValueType>::Row row_factors(first.GetSize().first);
  for (std::size_t i = 0; i < first.GetSize().first; ++i) {
    const auto upper_index = first.GetSize().second - 1;
    for (std::size_t j = 0; j < upper_index; j += 2) {
      row_factors[i] += first[i][j] * first[i][j + 1];
    }
  }
  typename models::Matrix<ValueType>::Row
      column_factors(second.GetSize().second);
  for (std::size_t i = 0; i < second.GetSize().second; ++i) {
    const auto upper_index = second.GetSize().first - 1;
    for (std::size_t j = 0; j < upper_index; j += 2) {
      column_factors[i] += second[j][i] * second[j + 1][i];
    }
  }

  const auto result_size = std::pair(first.GetSize().first,
                                     second.GetSize().second);
  models::Matrix<ValueType> result{result_size.first, result_size.second};
  for (std::size_t i = 0; i < result_size.first; ++i) {
    for (std::size_t j = 0; j < result_size.second; ++j) {
      result[i][j] = -row_factors[i] - column_factors[j];
      const auto upper_index = first.GetSize().second - 1;
      for (std::size_t k = 0; k < upper_index; k += 2) {
        result[i][j] += (first[i][k + 1] + second[k][j]) *
                        (first[i][k] + second[k + 1][j]);
      }
    }
  }

  if (first.GetSize().second % 2 == 1) {
    for (std::size_t i = 0; i < result_size.first; ++i) {
      for (std::size_t j = 0; j < result_size.second; ++j) {
        result[i][j] += first[i][first.GetSize().second - 1] *
                        second[second.GetSize().first - 1][j];
      }
    }
  }

  return result;
}

}  // namespace algorithms
