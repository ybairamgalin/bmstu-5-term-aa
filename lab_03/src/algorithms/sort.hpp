#pragma once

#include <algorithm>
#include <array>
#include <iterator>
#include <memory>
#include <stack>
#include <vector>

namespace {

template <typename ValueType>
struct Node {
  ValueType value{};
  std::shared_ptr<Node<ValueType>> left{};
  std::shared_ptr<Node<ValueType>> right{};
};

template <typename ValueType>
class Tree {
 public:
  void Insert(const ValueType& value);
  template <typename OutputIt>
  void ToContainer(OutputIt it);
 private:
  std::shared_ptr<Node<ValueType>> root_{};
};

} // namespace

namespace {

template <typename ValueType>
void Tree<ValueType>::Insert(const ValueType& value) {
  auto new_node = std::make_shared<Node<ValueType>>(Node<ValueType>{value});
  if (!root_) {
    root_ = new_node;
    return;
  }

  auto current_root = root_;
  auto inserted = false;
  while (!inserted) {
    if (current_root->value < new_node->value) {
      if (!current_root->right) {
        current_root->right = new_node;
        inserted = true;
      } else {
        current_root = current_root->right;
      }
    } else {
      if (!current_root->left) {
        current_root->left = new_node;
        inserted = true;
      } else {
        current_root = current_root->left;
      }
    }
  }
}

template <typename ValueType>
template <typename OutputIt>
void Tree<ValueType>::ToContainer(OutputIt it) {
  auto current_root = root_;
  std::stack<std::shared_ptr<Node<ValueType>>> stack{};

  while (!stack.empty() || current_root) {
    if (current_root) {
      stack.push(current_root);
      current_root = current_root->left;
    } else {
      current_root = stack.top();
      stack.pop();
      *(it++) = current_root->value;
      current_root = current_root->right;
    }
  }
}

template <typename RandomIt>
std::vector<std::vector<bool>> MakeBeads(
    RandomIt first, size_t container_size, size_t max_value) {
  using ValueType = typename std::remove_reference<decltype(*first)>::type;

  std::vector<std::vector<bool>> beads{};
  auto container_iterator = first;
  for (size_t i = 0; i < container_size; ++i) {
    if (*container_iterator < 0) {
      throw std::runtime_error("Cannot use negative elements in BeadSort");
    }

    std::vector<bool> beads_row(max_value, false);
    for (ValueType j = 0; j < *container_iterator; ++j) {
      beads_row[j] = true;
    }
    beads.push_back(beads_row);
    ++container_iterator;
  }

  return beads;
}

void ExecuteBeadsFall(
    std::vector<std::vector<bool>>& beads,
    size_t container_size, size_t max_value) {
  for (size_t i = 0; i < max_value; ++i) {
    size_t zero_count = 0;
    for (size_t j = 0; j < container_size; ++j) {
      if (beads[j][i]) {
        ++zero_count;
      }
    }
    for (size_t j = 0; j < zero_count; ++j) {
      beads[j][i] = true;
    }
    for (size_t j = zero_count; j < container_size; ++j) {
      beads[j][i] = false;
    }
  }
}

template <typename RandomIt>
void BeadsToContainer(RandomIt last, std::vector<std::vector<bool>>& beads) {
  using ValueType = typename std::remove_reference<decltype(*(last - 1))>::type;

  auto it = last;
  for (const auto& row : beads) {
    ValueType result_value = 0;
    for (const auto value : row) {
      if (value) {
        ++result_value;
      }
    }
    *(--it) = result_value;
  }
}

} // namespace

namespace algorithms {

template <typename RandomIt>
void BinaryTreeSort(RandomIt first, RandomIt last) {
  using ValueType = typename std::remove_reference<decltype(*first)>::type;

  if (first == last) {
    return;
  }
  Tree<ValueType> tree{};
  for (auto it = first; it != last; ++it) {
    tree.Insert(*it);
  }
  tree.ToContainer(first);
}

template <typename RandomIt>
void RadixSort(RandomIt first, RandomIt last) {
  using ValueType = typename std::remove_reference<decltype(*first)>::type;
  using Byte = unsigned char;
  constexpr const size_t kValuesInByte = 256;

  if (first == last) {
    return;
  }

  for (size_t i = 0; i < sizeof(ValueType); ++i) {
    std::array<std::vector<ValueType>, kValuesInByte> array;
    for (auto it = first; it != last; ++it) {
      const auto* current_byte = (Byte*)&(*it) + (sizeof(ValueType) - i - 1);
      const auto value = *current_byte;
      array[value].push_back(*it);
    }

    auto output_interator = first;
    for (size_t j = 0; j < kValuesInByte; ++j) {
      if (array[j].empty()) {
        continue;
      }
      for (auto& element : array[j]) {
        *(output_interator++) = element;
      }
    }
  }
}

template <typename RandomIt>
void BeadSort(RandomIt first, RandomIt last) {
  using ValueType = typename std::remove_reference<decltype(*first)>::type;
  static_assert(std::is_integral_v<ValueType>,
                "ValueType should be an integral type to use BeadSort");

  if (first == last) {
    return;
  }
  const auto container_size = std::distance(first, last);
  const auto max_value = *std::max_element(first, last);

  auto beads = MakeBeads(first, container_size, max_value);
  ExecuteBeadsFall(beads, container_size, max_value);
  BeadsToContainer(last, beads);
}

} // namespace algorithms
