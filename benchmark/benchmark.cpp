#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#include "lockfreequeue.hpp"
#include "mutex_queue.hpp"

template <typename T>
size_t benchmark(size_t workers) {
  T queue;
  std::vector<std::thread> threads(workers);
  std::atomic<bool> finish_flag(false);
  std::atomic<size_t> total_count(0);

  pthread_barrier_t start_barrier;
  int res = pthread_barrier_init(&start_barrier, NULL, workers + 1);
  if (res != 0) {
    perror("main thread barrier");
  }


  for (size_t i = 0; i < workers; i++) {
    threads[i] = std::thread([&, i]() {
      size_t count = 0;
      pthread_barrier_wait(&start_barrier);
      while(!finish_flag){
        queue.enqueue(nullptr);
        queue.dequeue();
        count += 2;
      }
      
      total_count += count;
    });
  }


  auto begin = std::chrono::high_resolution_clock::now();
  pthread_barrier_wait(&start_barrier);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000*2));
  finish_flag.store(true);

  for (size_t i = 0; i < workers; i++) threads[i].join();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count();
  auto ops = (total_count * 1000) / duration;
  return ops;
}

int main(int argc, char** argv) {
  assert(argc > 1);
  const size_t WORKERS = std::stoi(std::string(argv[1]));

  size_t ops = benchmark<LockFreeQueue>(WORKERS);
  std::cout << "OptimisticLockfree: " << WORKERS << " " << ops << std::endl;

  ops = benchmark<MutexQueue>(WORKERS);
  std::cout << "Mutex: " << WORKERS << " " << ops << std::endl;
}
