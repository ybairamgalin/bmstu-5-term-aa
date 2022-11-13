#include <iostream>
#include <fstream>

#include "algorithms/matrix_multiplication.hpp"
#include "utils/execution_time.hpp"

namespace {

std::vector<utils::TimeUnit> GetAllTimes(std::ifstream& input_file) {
  size_t input_count;
  input_file >> input_count;
  std::vector<utils::TimeUnit> result{};
  for (std::size_t i = 0; i < input_count; ++i) {
    models::Matrix<int> first_matrix{};
    models::Matrix<int> second_matrix{};
    input_file >> first_matrix >> second_matrix;

    const auto exec_result_simple = utils::GetExecutionTime(
        algorithms::MultiplySimple<int>, first_matrix, second_matrix);
    const auto exec_result_vinograd = utils::GetExecutionTime(
        algorithms::MultiplyVinograd<int>, first_matrix, second_matrix);
    const auto exec_result_vinograd_optimized = utils::GetExecutionTime(
        algorithms::MultiplyVinogradOptimized<int>,
        first_matrix, second_matrix);
    result.push_back(exec_result_simple.first);
    result.push_back(exec_result_vinograd.first);
    result.push_back(exec_result_vinograd_optimized.first);

    std::cout << "Time measured for matrix " << first_matrix.GetSize().first
              << "x" << first_matrix.GetSize().second;
  }

  return result;
}

}  // namespace

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: TEST-FILENAME OUT-FILENAME, argc=" << argc << "\n";
    return 1;
  }

  std::ifstream test_file;
  test_file.open(argv[1]);
  if (!test_file.is_open()) {
    std::cerr << "Could not open test file\n";
    return 1;
  }

  const auto time_results = GetAllTimes(test_file);

  std::ofstream result_file;
  result_file.open(argv[2]);
  if (!result_file.is_open()) {
    std::cerr << "Could not open result file\n";
    return 1;
  }

  for (const auto& result : time_results) {
    result_file << result.count() << "\n";
  }

  return 0;
}
