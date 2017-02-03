#ifndef LOCKFREEQUEUE_HPP
#define LOCKFREEQUEUE_HPP

#include <atomic>
#include <mutex>  // @TODO FORDEBUG

class LockFreeQueue {
  struct Node {
    void* value;
    std::atomic<Node*> next;
    Node(void* v) : value(v), next(nullptr) {}
  };

 public:
  LockFreeQueue()
      : sentinel_(new Node(nullptr)), head_(sentinel_), tail_(sentinel_) {}
  ~LockFreeQueue() { delete sentinel_; }

  void enqueue(void* v) {
    std::unique_lock<std::mutex> queuelock(debug_lock_);

    Node* node = new Node(v);
    Node* last = tail_.load();
    Node* next = last->next.load();
    last->next.compare_exchange_strong(next, node);
    tail_.store(node);
  }

  void* dequeue() {
    for (;;) {
      std::unique_lock<std::mutex> queuelock(debug_lock_);

      Node* first = head_.load();
      Node* next = first->next.load();
      if (next == nullptr) return nullptr;
      head_.store(next);
      return next->value;
    }
  }

 private:
  Node* sentinel_;
  std::atomic<Node*> head_;
  std::atomic<Node*> tail_;

  std::mutex debug_lock_;
};

#endif  // LOCKFREEQUEUE_HPP
