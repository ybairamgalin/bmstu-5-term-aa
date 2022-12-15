#include "levenshtein_distance.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <optional>

namespace algorithm {

int DamerauLevenshteinDistance(
    const std::string& first, const std::string& second) {
  using Row = std::vector<int>;
  using Matrix = std::vector<Row>;

  Matrix distances(first.size() + 1, Row(second.size() + 1));

  for (size_t i = 0; i < distances.size(); ++i) {
    distances[i][0] = int(i);
  }
  for (size_t j = 0; j < distances[0].size(); ++j) {
    distances[0][j] = int(j);
  }
  for (size_t i = 1; i < distances.size(); ++i) {
    for (size_t j = 1; j < distances[0].size(); ++j) {
      int cost = 1;
      if (first[i - 1] == second[i - 1]) {
        cost = 0;
      }
      distances[i][j] = std::min({
          distances[i - 1][j] + 1,
          distances[i][j - 1] + 1,
          distances[i - 1][j - 1] + cost});

      if (i > 1 && j > 1 &&
          first[i - 1] == second[j - 2] && first[i - 2] == second[j - 1]) {
        distances[i][j] = std::min({
            distances[i][j], distances[i - 2][j - 2] + cost});
      }
    }
  }

  return distances[first.size()][second.size()];
}

int DamerauLevenshteinDistanceRecursive(
    const std::string& first, const std::string& second) {
  if (first.empty()) {
    return int(second.size());
  }
  if (second.empty()) {
    return int(first.size());
  }

  std::function<int(int, int)> calculate_distance_recursively;
  calculate_distance_recursively = [&](int i, int j) -> int {
    if (i == 0 && j == 0) {
      return (first[i] != second[j]) ? 1 : 0;
    }
    if (i == 0) {
      return int(j);
    }
    if (j == 0) {
      return int(i);
    }

    int cost = (first[i] != second[j]) ? 1 : 0;

    int delete_operation_cost = calculate_distance_recursively(i - 1, j) + 1;
    int insert_operation_cost = calculate_distance_recursively(i, j - 1) + 1;
    int correspondence_operation_cost =
        calculate_distance_recursively(i - 1, j - 1) + cost;
    if (i > 1 && j > 1 &&
        first[i] == second[j - 1] && first[i - 1] == second[j]) {
      int swap_operation_cost =
          calculate_distance_recursively(i - 2, j - 2) + 1;
      return std::min({
          delete_operation_cost, insert_operation_cost,
          correspondence_operation_cost, swap_operation_cost});
    }

    return std::min({
        delete_operation_cost, insert_operation_cost,
        correspondence_operation_cost});
  };

  return calculate_distance_recursively(
      int(first.size() - 1), int(second.size() - 1));
}

int DamerauLevenshteinDistanceRecursiveCached(
    const std::string& first, const std::string& second) {
  using Row = std::vector<std::optional<int>>;
  using Matrix = std::vector<Row>;

  if (first.empty()) {
    return int(second.size());
  }
  if (second.empty()) {
    return int(first.size());
  }

  Matrix calculated_distances(first.size() + 1, Row(second.size() + 1));
  std::function<int(int, int)> calculate_distance_recursively;
  calculate_distance_recursively = [&](int i, int j) -> int {
    if (i == 0 && j == 0) {
      return (first[i] != second[j]) ? 1 : 0;
    }
    if (i == 0) {
      return int(j);
    }
    if (j == 0) {
      return int(i);
    }
    if (calculated_distances[i][j].has_value()) {
      return calculated_distances[i][j].value();
    }

    int cost = (first[i] != second[j]) ? 1 : 0;

    int delete_operation_cost = calculate_distance_recursively(i - 1, j) + 1;
    int insert_operation_cost = calculate_distance_recursively(i, j - 1) + 1;
    int correspondence_operation_cost =
        calculate_distance_recursively(i - 1, j - 1) + cost;
    if (i > 1 && j > 1 &&
        first[i] == second[j - 1] && first[i - 1] == second[j]) {
      int swap_operation_cost =
          calculate_distance_recursively(i - 2, j - 2) + 1;
      int result = std::min({
          delete_operation_cost, insert_operation_cost,
          correspondence_operation_cost, swap_operation_cost});
      calculated_distances[i][j] = result;
      return result;
    }

    int result = std::min({
        delete_operation_cost, insert_operation_cost,
        correspondence_operation_cost});
    calculated_distances[i][j] = result;

    return result;
  };

  return calculate_distance_recursively(
      int(first.size() - 1), int(second.size() - 1));
}

}  // namespace algorithm
