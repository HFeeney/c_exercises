/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include<memory>
#include "LinkedIntList.h"

namespace intlist333 {

LinkedIntList::LinkedIntList()
    : num_elements_(0),
      head_(std::shared_ptr<Node>()),
      tail_(std::weak_ptr<Node>()) {}

void LinkedIntList::Push(const int payload) {
  // Create a new node, and initialize its payload.
  std::shared_ptr<Node> node(new Node());
  node->payload = payload;

  // Initialize the next field based on which case we are handling.
  // Potentially reassign tail pointer.
  if (num_elements_ == 0) {
    // Empty list case
    // The tail is reassigned in this case!
    tail_ = node;
  } else {
    // List has >= 1 elements
    // The current head's previous should point at this node.
    // The node's next pointer should point to the current head.
    head_->prev = node;
    node->next = head_;
  }

  // Reassign head to the added node, increment size.
  head_ = node;
  num_elements_++;
}

void LinkedIntList::Append(const int payload) {
  // Create a new node, and initialize its payload.
  std::shared_ptr<Node> node(new Node());
  node->payload = payload;

  // Initialize the prev field based on which case we are handling.
  // Potentially reassign head pointer.
  if (num_elements_ == 0) {
    // Empty list case
    // The head is reassigned in this case!
    head_ = node;
  } else {
    // List has >= 1 elements
    // The current tail's next pointer should point to the node.
    // The node's prev pointer should point to the current tail.
    tail_.lock()->next = node;
    node->prev = tail_;
  }

  // Reassign tail to the added node, increment size.
  tail_ = node;
  num_elements_++;
}

bool LinkedIntList::Pop(int* const payload_ptr) {
  if (num_elements_ == 0) {
    // Empty list case - nothing is done.
    return false;
  } else {
    // Return the value through the payload ptr.
    *payload_ptr = head_->payload;

    if (num_elements_ == 1) {
      // Single element case. The list is now empty.
      tail_ = head_ = nullptr;
    } else {
      // > 1 element case. Reassign head. Note: its previous will now dangle.
      head_ = head_->next;
    }

    // Decrement size.
    num_elements_--;

    // Indicate success!
    return true;
  }
}

bool LinkedIntList::Slice(int* const payload_ptr) {
  if (num_elements_ == 0) {
    // Empty list case. Do nothing.
    return false;
  } else {
    // Return the value through the payload ptr.
    *payload_ptr = tail_.lock()->payload;

    if (num_elements_ == 1) {
      // Single element case. The list is now empty.
      tail_ = head_ = nullptr;
    } else {
      // > 1 element case. Reassign tail and its next pointer.
      tail_ = tail_.lock()->prev;
      tail_.lock()->next = nullptr;
    }

    // Decrement size, indicate success!
    num_elements_--;
    return true;
  }
}

}   // namespace intlist333
