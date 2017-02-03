#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "lockfreequeue.hpp"

int main(int argc, char** argv) {
  assert(argc > 2);
  const size_t WORKERS = std::stoi(std::string(argv[1]));
  const size_t ITERATION = std::stoi(std::string(argv[2]));

  LockFreeQueue queue;
  std::vector<std::thread> threads(WORKERS);

  auto begin = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < WORKERS; i++) {
    threads[i] = std::thread([&]() {
      std::vector<int> nums(ITERATION, 0);
      for (auto& num : nums) {
        queue.enqueue(&num);
      }
      void* ptr = nullptr;
      for (auto __ : nums) {
        while (ptr == nullptr) {
          ptr = queue.dequeue();
        }
        (void)(__);
      }
    });
  }

  for (size_t i = 0; i < WORKERS; i++) threads[i].join();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count();
  std::cout << WORKERS << " " << duration << " "
            << (WORKERS * ITERATION) * 1000 / duration << std::endl;
}

// @TODO How to test whether a queue lockfree? how to emulate preemption without
// modifying source?
