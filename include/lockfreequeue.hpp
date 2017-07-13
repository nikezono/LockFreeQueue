#ifndef LOCKFREEQUEUE_HPP
#define LOCKFREEQUEUE_HPP

#include <atomic>

class LockFreeQueue {
  struct Node {
    void* value;
    std::atomic<Node*> next;
    Node* prev;
    Node(void* v) : value(v), next(nullptr), prev(nullptr) {}
    void set_next(Node* n) { next = n; }
    void set_prev(Node* n) { prev = n; }
  };

 public:
  LockFreeQueue()
      : sentinel_(new Node(nullptr)), head_(sentinel_), tail_(sentinel_) {}
  ~LockFreeQueue() {
    Node* last = tail_.load();
    while (last) {
      tail_.compare_exchange_weak(last, last->next);
      delete last;
      last = tail_.load();
    }
  }

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
      Node* first_prev = first->prev;
      if (first == head_.load()) {  // Check consistency
        if (last == first) return nullptr;
        if (!first_prev) {  // prev chain isnt consistent
          fix_list();       // helping
          continue;
        } else {
          if (head_.compare_exchange_weak(first, first_prev)) {
            return first_prev->value;
          }
        }
      }
    }
  }

  bool is_empty(){
    return head_.load() == tail_.load();
  }

 private:
  // fix_list can execute without lock or atomic ops. its idempotent.
  void fix_list() {
    Node* cur = tail_.load();
    Node* first = head_.load();
    Node* cur_next;
    while (first == head_.load() && cur != first) {
      cur_next = cur->next;
      cur_next->prev = cur;  // fix prev chain
      cur = cur_next;
    }
  }

 private:
  Node* sentinel_;
  std::atomic<Node*> head_;
  std::atomic<Node*> tail_;
};

#endif  // LOCKFREEQUEUE_HPP
