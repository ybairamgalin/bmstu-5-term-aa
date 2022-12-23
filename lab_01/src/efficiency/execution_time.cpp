#include <iostream>
#include <vector>

#include "algorithms/levenshtein_distance.hpp"
#include "utils/execution_time.hpp"

namespace {

using WordsToTest = std::vector<std::pair<std::string, std::string>>;

void AddWordsToTest(WordsToTest& words_to_test) {
  words_to_test.emplace_back(
      "Similar",
      "Similar");
  words_to_test.emplace_back(
      "kjFFkjRhtZ",
      "ZFdbYi4nQd");
  words_to_test.emplace_back(
      "words diff",
      "wrods dfif");
}

void AddComplexWordsToTest(WordsToTest& words_to_test) {
  words_to_test.emplace_back(
      "Two similar sentences",
      "Two similar sentences");
  words_to_test.emplace_back(
      "NVyhNfPnYykGwDZETiI5",
      "GD34eG0rIZ73qGorddY5");
  words_to_test.emplace_back(
      "some real words with errors",
      "smoe real wrods wiht erors");
}

}  // namespace

int main() {
  WordsToTest words_to_test;
  AddWordsToTest(words_to_test);

  for (const auto& [first_word, second_word] : words_to_test) {
    const auto time_matrix = utils::GetExecutionTime(
        algorithm::DamerauLevenshteinDistance, first_word, second_word);
    const auto time_recursive = utils::GetExecutionTime(
        algorithm::DamerauLevenshteinDistanceRecursive,
        first_word, second_word);
    const auto time_recursive_cached = utils::GetExecutionTime(
        algorithm::DamerauLevenshteinDistanceRecursiveCached,
        first_word, second_word);

    std::cout << first_word << "\n" << second_word << "\n";
    std::cout << "Matrix: " << time_matrix.first.count() << "\n";
    std::cout << "Recursive: " << time_recursive.first.count() << "\n";
    std::cout << "Recursive cached: "
              << time_recursive_cached.first.count() << "\n";
    std::cout << "\n";
  }

  WordsToTest complex_word_to_test;
  AddComplexWordsToTest(complex_word_to_test);
  for (const auto& [first_word, second_word] : complex_word_to_test) {
    const auto time_matrix = utils::GetExecutionTime(
        algorithm::DamerauLevenshteinDistance, first_word, second_word);
    const auto time_recursive_cached = utils::GetExecutionTime(
        algorithm::DamerauLevenshteinDistanceRecursiveCached,
        first_word, second_word);

    std::cout << first_word << "\n" << second_word << "\n";
    std::cout << "Matrix: " << time_matrix.first.count() << "\n";
    std::cout << "Recursive cached: "
              << time_recursive_cached.first.count() << "\n";
    std::cout << "\n";
  }

  return 0;
}
