#include <thread>
#include <vector>
#include "gtest/gtest.h"

#include "lockfreequeue.hpp"
#include "mutex_queue.hpp"

constexpr static size_t WORKERS = 10;
constexpr static size_t ITERATION = 10000;


template <typename T>
class queue_test : public ::testing::Test {
 public:
  T queue;
};

typedef ::testing::Types<LockFreeQueue, MutexQueue> impls;

TYPED_TEST_CASE(queue_test, impls);

TYPED_TEST(queue_test, enqueue) {
  int i;
  this->queue.enqueue(&i);
}

TYPED_TEST(queue_test, dequeue) {
  int i;
  this->queue.enqueue(&i);
  void* ret_ptr = this->queue.dequeue();
  ASSERT_EQ(&i, ret_ptr);
}

TYPED_TEST(queue_test, is_empty) {
  int i;
  ASSERT_TRUE(this->queue.is_empty());
  this->queue.enqueue(&i);
  ASSERT_FALSE(this->queue.is_empty());
  this->queue.dequeue();
  ASSERT_TRUE(this->queue.is_empty());
}

TYPED_TEST(queue_test, fcfs) {
  std::vector<int> nums(ITERATION, 0);
  for (auto& num : nums) {
    this->queue.enqueue(&num);
  }
  for (auto& num : nums) {
    void* ret_ptr = this->queue.dequeue();
    ASSERT_EQ(&num, ret_ptr);
  }
}

TYPED_TEST(queue_test, multithread) {
  std::thread threads[WORKERS];

  for (size_t i = 0; i < WORKERS; i++) {
    threads[i] = std::thread([&]() {
      std::vector<int> nums(ITERATION, 0);
      for (auto& num : nums) {
        this->queue.enqueue(&num);
      }
    });
  }
  for (size_t i = 0; i < WORKERS; i++) threads[i].join();

  size_t count = 0;
  void* ptr = this->queue.dequeue();
  while (ptr != nullptr) {
    ++count;
    ptr = this->queue.dequeue();
  }
  ASSERT_EQ(WORKERS * ITERATION, count);
}

// @TODO How to test whether a queue lockfree? how to emulate preemption without
// modifying source?
