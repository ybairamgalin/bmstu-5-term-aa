#pragma once

#include <deque>
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

template<typename Task>
class BlockingQueue {
 public:
  void Add(Task task) {
    std::lock_guard guard(mutex_);
    queue_.push_back(std::move(task));
    queue_not_empty_.notify_one();
  }

  Task Take() {
    std::unique_lock lock(mutex_);
    while (true) {
      if (!queue_.empty()) {
        return TakeLocked();
      } else {
        queue_not_empty_.wait(lock);
      }
    }
  }
 private:
  Task TakeLocked() {
    Task oldest = std::move(queue_.front());
    queue_.pop_front();
    return oldest;
  }

  std::deque<Task> queue_; // Guarded by mutex_
  std::mutex mutex_;
  std::condition_variable queue_not_empty_;
};
