#include <gtest/gtest.h>

#include "algorithms/levenshtein_distance.hpp"

namespace {

inline void TestAlgorithms(
    const std::string& first,
    const std::string& second,
    int expected_distance) {
  EXPECT_EQ(algorithm::DamerauLevenshteinDistance(first, second),
            expected_distance);
  EXPECT_EQ(algorithm::DamerauLevenshteinDistanceRecursive(first, second),
            expected_distance);
  EXPECT_EQ(algorithm::DamerauLevenshteinDistanceRecursiveCached(first, second),
            expected_distance);
}

} // namespace

TEST(TestDistance, Empty) {
  TestAlgorithms("", "", 0);
}

TEST(TestDistance, Simple) {
  const std::string first = "cat";
  const std::string second = "hat";
  TestAlgorithms(first, second, 1);
}

TEST(TestDistance, MinimalChange) {
  const std::string first = "hello";
  const std::string second = "hallo";
  TestAlgorithms(first, second, 1);
}

TEST(TestDistance, MinimalTransposition) {
  const std::string first = "apple";
  const std::string second = "aplpe";
  TestAlgorithms(first, second, 1);
}

TEST(TestDistance, GeneralDistance) {
  const std::string first = "qwerty";
  const std::string second = "queue";
  TestAlgorithms(first, second, 4);
}

TEST(TestDistance, EqualStrings) {
  const std::string first = "wolf";
  const std::string second = "wolf";
  TestAlgorithms(first, second, 0);
}

TEST(TestDistance, OneEmpty) {
  const std::string first = "word";
  const std::string second = "";
  TestAlgorithms(first, second, 4);
}

TEST(TestDistance, AnotherGeneral) {
  const std::string first = "minsubishi";
  const std::string second = "mercedes-benz";
  TestAlgorithms(first, second, 11);
}
