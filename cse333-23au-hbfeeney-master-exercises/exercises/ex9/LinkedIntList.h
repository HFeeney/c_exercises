/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#ifndef LINKEDINTLIST_H_
#define LINKEDINTLIST_H_

#include <memory>
#include "IntList.h"

namespace intlist333 {

// LinkedIntList is an implementation of
// the IntList abstract class. Implemented using
// a doubly linked list
class LinkedIntList : public IntList {
 public:
  // Constructs an empty LinkedIntList.
  LinkedIntList();

  // Destructor should be kept default
  // since smart pointers should clean
  // everything up for us
  virtual ~LinkedIntList() = default;

  // Disable copy constructor.
  LinkedIntList(const LinkedIntList& other) = delete;

  // Disable assignment operator.
  LinkedIntList& operator=(const LinkedIntList& other) = delete;

  // Returns the number of elements in the LinkedIntList.
  int num_elements() const override { return num_elements_; }

  // Pushes the given integer onto the front of the List.
  void Push(const int payload) override;

  // Pushes the given integer on to the back of the List.
  void Append(const int payload) override;

  // Removes the first element in the List and returns it through payload_ptr.
  // Returns true if removal is successful and false if an element cannot be
  // removed.
  bool Pop(int* const payload_ptr) override;

  // Removes the last element in the List and returns it through payload_ptr.
  // Returns true if removal is successful and false if an element cannot be
  // removed.
  bool Slice(int* const payload_ptr) override;

 private:
  int num_elements_;
  struct Node {
    int payload;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;
  };
  std::shared_ptr<Node> head_;
  std::weak_ptr<Node> tail_;
};

}  // namespace intlist333

#endif  // LINKEDINTLIST_H_
