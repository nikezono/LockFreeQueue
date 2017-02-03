#ifndef LOCKFREEQUEUE_HPP
#define LOCKFREEQUEUE_HPP

#include <atomic>

class LockFreeQueue {
  struct Node {
    void* value;
    std::atomic<Node*> next;
    std::atomic<Node*> prev;
    Node(void* v) : value(v), next(nullptr), prev(nullptr) {}
    void set_next(Node* n) { next = n; }
    void set_prev(Node* n) { prev = n; }
  };

 public:
  LockFreeQueue()
      : sentinel_(new Node(nullptr)), head_(sentinel_), tail_(sentinel_) {}
  ~LockFreeQueue() { delete sentinel_; }

  void enqueue(void* v) {
    Node* node = new Node(v);

    for (;;) {
      Node* last = tail_.load();
      node->set_next(last);
      if (tail_.compare_exchange_weak(last, node)) {
        last->set_prev(node);
        break;
      }
    }
  }

  void* dequeue() {
    for (;;) {
      Node* first = head_.load();
      Node* last = tail_.load();
      if (next == nullptr) return nullptr;
      head_.store(next);
      return next->value;
    }
  }

 private:
  void fix_list() {}

 private:
  Node* sentinel_;
  std::atomic<Node*> head_;
  std::atomic<Node*> tail_;
};

#endif  // LOCKFREEQUEUE_HPP
