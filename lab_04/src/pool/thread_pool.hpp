#pragma once

#include <thread>
#include <functional>
#include <deque>
#include <vector>
#include <stdexcept>


#include "pool/blocking_queue.hpp"

template<typename Task>
class StaticThreadPool {
 public:
  explicit StaticThreadPool(int thread_count)
      : thread_count_(thread_count) {
    StartWorkers();
  }

  void Submit(Task task) {
    blocking_queue_.Add(std::move(task));
  }

  void Join() {
    for (size_t i = 0; i < thread_count_; ++i) {
      blocking_queue_.Add({});
    }
  }

 private:
  void StartWorkers() {
    if (!pool_.empty()) {
      throw std::runtime_error("Bad start workers call");
    }

    for (int i = 0; i < thread_count_; ++i) {
      std::cout << "Starting worker " << i  << "\n";
      pool_.emplace_back([this]() {
        WorkerRoutine();
      });
    }
  }

  void WorkerRoutine() {
    while(true) {
      auto task = blocking_queue_.Take();
      if (!task) {
        break;
      }
      task();
    }
  }

  const int thread_count_;
  BlockingQueue<Task> blocking_queue_;
  std::vector<std::thread> pool_;
};
