#pragma once

#include <chrono>
#include <utility>
#include <functional>
#include <type_traits>

namespace utils {

using TimeUnit = std::chrono::microseconds;

template <typename F, typename... Args>
auto GetExecutionTime(F function, Args&&... args) {
  const auto start_time = std::chrono::high_resolution_clock::now();
  const auto return_value = function(std::forward<Args>(args)...);
  const auto end_time = std::chrono::high_resolution_clock::now();
  return std::pair(
      std::chrono::duration_cast<TimeUnit>(end_time - start_time),
      return_value);
}

template <typename... Args>
auto GetExecutionTime(void(*function)(Args...), Args&&... args) {
  const auto start_time = std::chrono::high_resolution_clock::now();
  function(std::forward<Args>(args)...);
  const auto end_time = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<TimeUnit>(end_time - start_time);
}

} // namespace utils
