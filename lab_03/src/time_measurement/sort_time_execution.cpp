#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string_view>

#include "algorithms/sort.hpp"
#include "utils/execution_time.hpp"

namespace {

constexpr const char kOutFile[] =
    "/Users/yaroslavbairamgalin/Desktop/Studies/BMSTU/5 term/aa/aa_labs/"
    "lab_03/src/time_measurement/sort_time_result.txt";

}  // namespace

namespace {

std::vector<int> GetSortedVector(size_t size) {
  std::vector<int> vector{};
  for (size_t i = 0; i < size; ++i) {
    vector.push_back(int(i));
  }

  return vector;
}

std::vector<int> GetReversedSortedVector(size_t size) {
  std::vector<int> vector{};
  for (size_t i = 0; i < size; ++i) {
    vector.push_back(int(size - i));
  }

  return vector;
}

std::vector<int> GetRandomVector(size_t size, int max_value) {
  std::vector<int> vector{};
  for (size_t i = 0; i < size; ++i) {
    vector.push_back(rand() % max_value);
  }

  return vector;
}

void SaveTimeToFile(
    std::ofstream& file, const std::vector<utils::TimeUnit>& vector) {
  for (const auto& time_value : vector) {
    file << time_value.count() << " ";
  }
  file << "\n";
}

}  // namespace

int main() {
  const std::vector<size_t> array_sizes{
      1, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};

  std::vector<utils::TimeUnit> radix_times_sorted{};
  std::vector<utils::TimeUnit> beads_times_sorted{};
  std::vector<utils::TimeUnit> binary_tree_times_sorted{};
  std::vector<utils::TimeUnit> radix_times_reversed{};
  std::vector<utils::TimeUnit> beads_times_reversed{};
  std::vector<utils::TimeUnit> binary_tree_times_reversed{};
  std::vector<utils::TimeUnit> radix_times_random{};
  std::vector<utils::TimeUnit> beads_times_random{};
  std::vector<utils::TimeUnit> binary_tree_times_random{};
  for (const auto& array_size : array_sizes) {
    std::cout << "Measuring time for array size " << array_size << std::endl;

    auto vector_radix = GetSortedVector(array_size);
    auto radix_time = utils::GetExecutionTime(
        algorithms::RadixSort, vector_radix.begin(), vector_radix.end());
    radix_times_sorted.push_back(radix_time);

    auto vector_beads = GetSortedVector(array_size);
    auto beads_time = utils::GetExecutionTime(
        algorithms::BeadSort, vector_beads.begin(), vector_beads.end());
    beads_times_sorted.push_back(beads_time);

    auto vector_binary_tree = GetSortedVector(array_size);
    auto binary_tree_time = utils::GetExecutionTime(
        algorithms::BinaryTreeSort,
        vector_binary_tree.begin(),vector_binary_tree.end());
    binary_tree_times_sorted.push_back(binary_tree_time);

    std::cout << "SORTED DONE\n";

    vector_radix = GetReversedSortedVector(array_size);
    radix_time = utils::GetExecutionTime(
        algorithms::RadixSort, vector_radix.begin(), vector_radix.end());
    radix_times_reversed.push_back(radix_time);

    vector_beads = GetReversedSortedVector(array_size);
    beads_time = utils::GetExecutionTime(
        algorithms::BeadSort, vector_beads.begin(), vector_beads.end());
    beads_times_reversed.push_back(beads_time);

    vector_binary_tree = GetReversedSortedVector(array_size);
    binary_tree_time = utils::GetExecutionTime(
        algorithms::BinaryTreeSort,
        vector_binary_tree.begin(),vector_binary_tree.end());
    binary_tree_times_reversed.push_back(binary_tree_time);

    std::cout << "REVERSED DONE\n";

    vector_radix = GetRandomVector(array_size, int(array_size));
    radix_time = utils::GetExecutionTime(
        algorithms::RadixSort, vector_radix.begin(), vector_radix.end());
    radix_times_random.push_back(radix_time);

    vector_beads = GetRandomVector(array_size, int(array_size));
    beads_time = utils::GetExecutionTime(
        algorithms::BeadSort, vector_beads.begin(), vector_beads.end());
    beads_times_random.push_back(beads_time);

    vector_binary_tree = GetRandomVector(array_size, int(array_size));
    binary_tree_time = utils::GetExecutionTime(
        algorithms::BinaryTreeSort,
        vector_binary_tree.begin(),vector_binary_tree.end());
    binary_tree_times_random.push_back(binary_tree_time);

    std::cout << "RANDOM DONE\n";
  }

  std::ofstream out_file;
  out_file.open(kOutFile);
  for (const auto& array_size : array_sizes) {
    out_file << array_size << " ";
  }
  out_file << "\n";
  SaveTimeToFile(out_file, radix_times_sorted);
  SaveTimeToFile(out_file, beads_times_sorted);
  SaveTimeToFile(out_file, binary_tree_times_sorted);
  SaveTimeToFile(out_file, radix_times_reversed);
  SaveTimeToFile(out_file, beads_times_reversed);
  SaveTimeToFile(out_file, binary_tree_times_reversed);
  SaveTimeToFile(out_file, radix_times_random);
  SaveTimeToFile(out_file, beads_times_random);
  SaveTimeToFile(out_file, binary_tree_times_random);
  out_file.close();

  return 0;
}
