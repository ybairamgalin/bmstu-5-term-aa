#include <vector>
#include <functional>

#include <gtest/gtest.h>

#include "algorithms/matrix_multiplication.hpp"
#include "models/matrix.hpp"

namespace {

class TestMatrixMultiplication : public ::testing::Test {
 protected:
  using ValueType = int;
  using MultiplyFunction = std::function<models::Matrix<ValueType>(
      const models::Matrix<ValueType>&, const models::Matrix<ValueType>&)>;

  static void ExecuteAllMultiplicationTests(
      MultiplyFunction&& multiply_function) {
    TestGeneral(multiply_function);
    Test1x1(multiply_function);
    TestOddDimensions(multiply_function);
  }

 private:
  static void TestGeneral(MultiplyFunction& multiply_function) {
    models::Matrix<ValueType> first{3, 4};
    models::Matrix<ValueType> second{4, 1};

    first[0] = { 1, 2, -1, 2};
    first[1] = { 3, 0, 4, -2};
    first[2] = { 2, 3, 3, 5};

    second[0] = { -1 };
    second[1] = { 2 };
    second[2] = { 1 };
    second[3] = { 3 };

    const auto result = multiply_function(first, second);
    EXPECT_EQ(result.GetSize().first, 3);
    EXPECT_EQ(result.GetSize().second, 1);
    EXPECT_EQ(result[0][0], 8);
    EXPECT_EQ(result[1][0], -5);
    EXPECT_EQ(result[2][0], 22);
  }

  static void Test1x1(MultiplyFunction& multiply_function) {
    models::Matrix<ValueType> first{1, 1};
    models::Matrix<ValueType> second{1, 1};

    first[0][0] = 4;
    second[0][0] = -5;

    const auto result = multiply_function(first, second);
    EXPECT_EQ(result.GetSize().first, 1);
    EXPECT_EQ(result.GetSize().second, 1);
    EXPECT_EQ(result[0][0], -20);
  }

  static void TestOddDimensions(MultiplyFunction& multiply_function) {
    models::Matrix<ValueType> first{3, 3};
    models::Matrix<ValueType> second{3, 3};

    first[0] = { 5, 3, -7 };
    first[1] = { -1, 6, -3 };
    first[2] = { 2, -4, 1 };

    second[0] = { 4, -1, 3 };
    second[1] = { 4, -2, -6 };
    second[2] = { 2, 0, 3};

    const auto result = multiply_function(first, second);
    EXPECT_EQ(result.GetSize().first, 3);
    EXPECT_EQ(result.GetSize().second, 3);
    EXPECT_EQ(result[0][0], 18);
    EXPECT_EQ(result[0][1], -11);
    EXPECT_EQ(result[0][2], -24);
    EXPECT_EQ(result[1][0], 14);
    EXPECT_EQ(result[1][1], -11);
    EXPECT_EQ(result[1][2], -48);
    EXPECT_EQ(result[2][0], -6);
    EXPECT_EQ(result[2][1], 6);
    EXPECT_EQ(result[2][2], 33);
  }
};

}  // namespace

TEST(TestMatrix, init) {
  models::Matrix<int> matrix{5, 6};
  EXPECT_EQ(matrix[0][0], 0);
  EXPECT_EQ(matrix[4][5], 0);
}

TEST(TestMatrix, size) {
  models::Matrix<double> matrix{11, 10};
  EXPECT_EQ(matrix.GetSize().first, 11);
  EXPECT_EQ(matrix.GetSize().second, 10);
}

TEST(TestMatrix, bad_size) {
  EXPECT_THROW(
      {models::Matrix<int> matrix(10, 0);},
      std::runtime_error);
}

TEST_F(TestMatrixMultiplication, simple_multiplication) {
  ExecuteAllMultiplicationTests(
      std::function(algorithms::MultiplySimple<ValueType>));
}

TEST_F(TestMatrixMultiplication, vinograd_multiplication) {
  ExecuteAllMultiplicationTests(
      std::function(algorithms::MultiplyVinograd<ValueType>));
}

TEST_F(TestMatrixMultiplication, vinograd_improved_multimpication) {
  ExecuteAllMultiplicationTests(
      std::function(algorithms::MultiplyVinogradOptimized<ValueType>));
}
