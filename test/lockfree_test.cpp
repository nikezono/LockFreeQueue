#include <thread>
#include <vector>
#include "gtest/gtest.h"

#include "lockfreequeue.hpp"

constexpr static size_t WORKERS = 10;
constexpr static size_t ITERATION = 10000;

TEST(LockFreeQueue, init) { LockFreeQueue queue; }

TEST(LockFreeQueue, enqueue) {
  LockFreeQueue queue;
  int i;
  queue.enqueue(&i);
}

TEST(LockFreeQueue, dequeue) {
  LockFreeQueue queue;
  int i;
  queue.enqueue(&i);
  void* ret_ptr = queue.dequeue();
  ASSERT_EQ(&i, ret_ptr);
}

TEST(LockFreeQueue, fcfs) {
  LockFreeQueue queue;
  std::vector<int> nums(ITERATION, 0);
  for (auto& num : nums) {
    queue.enqueue(&num);
  }
  for (auto& num : nums) {
    void* ret_ptr = queue.dequeue();
    ASSERT_EQ(&num, ret_ptr);
  }
}

TEST(LockFreeQueue, multithread) {
  LockFreeQueue queue;
  std::thread threads[WORKERS];

  for (size_t i = 0; i < WORKERS; i++) {
    threads[i] = std::thread([&]() {
      std::vector<int> nums(ITERATION, 0);
      for (auto& num : nums) {
        queue.enqueue(&num);
      }
    });
  }
  for (size_t i = 0; i < WORKERS; i++) threads[i].join();

  size_t count = 0;
  void* ptr = queue.dequeue();
  while (ptr != nullptr) {
    ++count;
    ptr = queue.dequeue();
  }
  ASSERT_EQ(WORKERS * ITERATION, count);
}

// @TODO How to test whether a queue lockfree? how to emulate preemption without
// modifying source?
