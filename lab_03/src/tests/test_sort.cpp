#include <vector>

#include <gtest/gtest.h>

#include "algorithms/sort.hpp"

namespace {

class TestSort : public ::testing::Test {
 protected:
  using ValueType = int;
  using IteratorType = std::vector<ValueType>::iterator;
  using SortFunction = const std::function<void(IteratorType , IteratorType)>;

  void ExecuteAllSortTests(SortFunction& sort_function) {
    TestEmpty(sort_function);
    TestOneElement(sort_function);
    TestNotNegative(sort_function);
    TestSorted(sort_function);
    TestSortedReversed(sort_function);
    TestRepeatedElements(sort_function);
  }

 private:
  void TestEmpty(SortFunction& sort_function) {
    sort_function(vector_empty_.begin(), vector_empty_.end());
    EXPECT_EQ(vector_empty_.size(), 0);
  }

  void TestOneElement(SortFunction& sort_function) {
    sort_function(vector_one_element_.begin(), vector_one_element_.end());
    EXPECT_EQ(vector_one_element_.size(), 1);
    EXPECT_EQ(vector_one_element_[0], 0);
  }

  void TestNotNegative(SortFunction& sortFunction) {
    sortFunction(vector_not_negative_.begin(), vector_not_negative_.end());
    EXPECT_EQ(vector_not_negative_.size(), 11);
    for (size_t i = 0; i < vector_not_negative_.size(); ++i) {
      EXPECT_EQ(vector_not_negative_[i], i);
    }
  }

  void TestSorted(SortFunction& sortFunction) {
    sortFunction(vector_sorted_.begin(), vector_sorted_.end());
    EXPECT_EQ(vector_sorted_.size(), 10);
    for (size_t i = 0; i < vector_sorted_.size(); ++i) {
      EXPECT_EQ(vector_sorted_[i], i);
    }
  }

  void TestSortedReversed(SortFunction& sort_function) {
    sort_function(
        vector_sorted_reversed_.begin(), vector_sorted_reversed_.end());
    EXPECT_EQ(vector_sorted_reversed_.size(), 10);
    for (size_t i = 0; i < vector_sorted_reversed_.size(); ++i) {
      EXPECT_EQ(vector_sorted_reversed_[i], i);
    }
  }

  void TestRepeatedElements(SortFunction& sort_function) {
    sort_function(
          vector_repeated_elements_.begin(), vector_repeated_elements_.end());
    EXPECT_EQ(vector_repeated_elements_.size(), 8);
    std::vector<ValueType> expected_vector{0, 0, 1, 2, 3, 3, 4, 4};
    EXPECT_EQ(vector_repeated_elements_, expected_vector);
  }

  std::vector<ValueType> vector_empty_{};
  std::vector<ValueType> vector_one_element_{0};
  std::vector<ValueType> vector_not_negative_{10, 3, 1, 0, 2, 4, 5, 9, 8, 7, 6};
  std::vector<ValueType> vector_sorted_{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<ValueType> vector_sorted_reversed_{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  std::vector<ValueType> vector_repeated_elements_{0, 0, 4, 3, 2, 1, 4, 3};
};

}  // namespace

TEST_F(TestSort, bead_sort) {
  ExecuteAllSortTests(std::function(algorithms::BeadSort<IteratorType>));
}

TEST_F(TestSort, binary_tree_sort) {
  ExecuteAllSortTests(std::function(algorithms::BinaryTreeSort<IteratorType>));
}

TEST_F(TestSort, radix_sort) {
  ExecuteAllSortTests(std::function(algorithms::RadixSort<IteratorType>));
}
