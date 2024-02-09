/*
 * Copyright ©2023 Chris Thachuk.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Fall Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include "SimpleQueue.h"

#include <memory>
#include <pthread.h>

using std::string;
using std::shared_ptr;

SimpleQueue::SimpleQueue() {
  this->size_ = 0;
  this->front_.reset();
  this->end_.reset();
  pthread_mutex_init(&lock_, nullptr);
}

SimpleQueue::~SimpleQueue() {
  // Note: attempting to destroy locked mutex -> undefined behavior!
  pthread_mutex_destroy(&lock_);
}

void SimpleQueue::Enqueue(const string& item) {
  // Initialization of the new node is independent of current queue content.
  shared_ptr<Node> new_node(new Node());
  new_node->next.reset();
  new_node->item = item;

  // Object state is both read and written in this block. Acquire lock.
  pthread_mutex_lock(&lock_);
  if (this->end_) {
    this->end_->next = new_node;
  } else  {
    this->front_ = new_node;
  }
  this->end_ = new_node;
  this->size_++;
  pthread_mutex_unlock(&lock_);
  // Release lock.
}

bool SimpleQueue::Dequeue(string* const result) {
  // Data reads occur here, acquire lock.
  pthread_mutex_lock(&lock_);
  if (this->size_ == 0) {
    // Release lock.
    pthread_mutex_unlock(&lock_);
    return false;
  }
  *result = this->front_->item;
  if (this->end_ == this->front_) {
    this->end_ = this->front_ = this->front_->next;
  } else {
    this->front_ = this->front_->next;
  }
  this->size_--;
  pthread_mutex_unlock(&lock_);
  // Release lock.

  return true;
}

int SimpleQueue::Size() const {
  pthread_mutex_lock(&lock_);
  int s = this->size_;
  pthread_mutex_unlock(&lock_);
  return s;
}

bool SimpleQueue::IsEmpty() const {
  pthread_mutex_lock(&lock_);
  bool empty = this->size_ == 0;
  pthread_mutex_unlock(&lock_);
  return empty;
}
