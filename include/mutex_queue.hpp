#ifndef MUTEXQUEUE_HPP
#define MUTEXQUEUE_HPP

#include <mutex>

class MutexQueue {
  struct Node {
    void* value;
    Node* next;
    Node(void* v) : value(v), next(nullptr) {}
  };

 public:
  MutexQueue() : head_(new Node(nullptr)), tail_(head_) {}
  ~MutexQueue() {
    auto* node = head_;
    while (node) {
      auto* old = node;
      node = node->next;
      delete old;
    }
  }

  void enqueue(void* v) {
    std::unique_lock<std::mutex> lock(latch_);
    auto* node = new Node(v);
    tail_->next = node;
    tail_ = node;
  }

  void* dequeue() {
    std::unique_lock<std::mutex> lock(latch_);
    if (head_->next != nullptr) {
      auto* node = head_;
      head_ = head_->next;
      delete node;
      return head_->value;
    } else {
      return nullptr;
    }
  }

 private:
  Node* head_;
  Node* tail_;
  std::mutex latch_;
};

#endif  // LOCKFREEQUEUE_HPP
