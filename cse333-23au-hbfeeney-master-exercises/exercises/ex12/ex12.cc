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

#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <string>
#include <ctime>

#include "SimpleQueue.h"

using std::cout;
using std::endl;
using std::string;
using std::thread;

static constexpr int kNumSnacks = 6;
static SimpleQueue queue;
static unsigned int seed = time(nullptr);
static pthread_mutex_t write_lock;

// Thread safe print that prints the given str on a line
void thread_safe_print(const string& str) {
  pthread_mutex_lock(&write_lock);
  // Only one thread can hold the lock at a time, making it safe to
  // use cout. If we didn't lock before using cout, the order of things
  // put into the stream could be mixed up.
  cout << str << endl;
  pthread_mutex_unlock(&write_lock);
}

// Produces kNumSnacks snacks of the given type
// You should NOT modify this method at all
void producer(const string& snack_type) {
  for (int i = 0; i < kNumSnacks; i++) {
    queue.Enqueue(snack_type);
    thread_safe_print(snack_type + " ready!");
    int sleep_time = rand_r(&seed) % 500 + 1;
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
  }
}

// Eats 2 * kNumSnacks snacks
// You should NOT modify this method at all
void consumer() {
  for (int i = 0; i < kNumSnacks * 2; i++) {
    bool successful = false;
    string snack_type;
    while (!successful) {
      while (queue.IsEmpty()) {
        // Sleep for a bit and then check again
        int sleep_time = rand_r(&seed) % 800 + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
      }
      successful = queue.Dequeue(&snack_type);
    }
    thread_safe_print(snack_type + " eaten!");
  }
}

void* start_producer_fn(void* product_name_ptr) {
  string* product = reinterpret_cast<string*>(product_name_ptr);
  producer(*product);
  delete product;
  return nullptr;
}

void* start_consumer_fn(void*) {
  consumer();
  return nullptr;
}

pthread_t create_thd(void* (*thd_main)(void*), string* const arg) {
  pthread_t id;
  if (pthread_create(&id, nullptr, thd_main,
        reinterpret_cast<void*>(arg))) {
    return -1;
  }
  return id;
}

int main(int argc, char** argv) {
  pthread_mutex_init(&write_lock, nullptr);
  // Your task: Make the two producers and the single consumer
  // all run concurrently (hint: use pthreads)

  pthread_t pir_thd_id = create_thd(start_producer_fn, new string("piroshki"));
  pthread_t nal_thd_id = create_thd(start_producer_fn, new string("nalysnyky"));
  pthread_t con_thd_id = create_thd(start_consumer_fn, nullptr);
  pthread_join(pir_thd_id, nullptr);
  pthread_join(nal_thd_id, nullptr);
  pthread_join(con_thd_id, nullptr);

  pthread_mutex_destroy(&write_lock);
  return EXIT_SUCCESS;
}
