/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <cstdlib>
#include <iostream>
#include "Vector.h"

using std::cerr;
using std::cout;
using std::endl;

// Prints a message detailing the differences between the passed vector's
// coordinates and the expected values if they exist. Otherwise, nothing is
// printed.
//  v - the vector whose coordinates will be compared to the others
//  x - the expected x coordinate
//  y - the expected y coordinate
//  z - the expected z coordinate
static int VerifyValues(const Vector& v, float x, float y, float z);

// Increments each coordinate of the passed vector by 1.
//   v - the vector whose coordinates will be incremented
static void Increment(Vector v);

// Verifies that the passed vector's address matches the passed address.
//   v - a reference to a vector whose address will be checked
//   addr - the expected address of the vector reference.
// Returns whether the vector's addressed matched the passed address.
static bool VerifyAddress(const Vector& v, void* addr);

// This program tests the functionality of the Vector class and demonstrates
// properties of objects and their references.
int main(int argc, char* argv[]) {
  // Test default constructor.
  Vector v1;
  if (VerifyValues(v1, 0.0f, 0.0f, 0.0f)) {
    return EXIT_FAILURE;
  }

  // Test explicit constructor.
  Vector v2(5.0f, 6.0f, 3.0f);
  if (VerifyValues(v2, 5.0f, 6.0f, 3.0f)) {
    return EXIT_FAILURE;
  }

  // Test copy constructor.
  Vector v3(v2);
  if (VerifyValues(v3, 5.0f, 6.0f, 3.0f)) {
    return EXIT_FAILURE;
  }

  // Test assignment operator.
  Vector v4(90.0f, 100.0f, 20.0f);
  v3 = v4;
  if (VerifyValues(v3, 90.0f, 100.0f, 20.0f)) {
    return EXIT_FAILURE;
  }

  // Test +=, -=, *
  v4 += v2;
  if (VerifyValues(v4, 95.0f, 106.0f, 23.0f)) {
    return EXIT_FAILURE;
  }

  v1 += v2 += v4;
  if (VerifyValues(v1, 100.0f, 112.0f, 26.0f)) {
    return EXIT_FAILURE;
  }

  v1 -= v1;
  if (VerifyValues(v1, 0.0f, 0.0f, 0.0f)) {
    return EXIT_FAILURE;
  }

  if (v4 * Vector(0.0f, 0.0f, 0.0f) != 0.0f) {
    return EXIT_FAILURE;
  }

  if (Vector(1.0f, 2.0f, -3.0f) * Vector(2.0f, -1.0f, 8.0f) != -24.0f) {
    return EXIT_FAILURE;
  }

  // Test if objects are passed by reference or value.
  Vector myCoords(50.0f, 1000.0f, 0.0f);
  Increment(myCoords);
  if (myCoords.get_x() == 51.0f
      && myCoords.get_y() == 1001.0f
      && myCoords.get_z() == 1.0f) {
    cout << "Vector: pass-by-reference" << endl;
  } else {
    cout << "Vector: pass-by-value" << endl;
  }

  // Test if references point to the same memory location as the original
  // object's.
  if (VerifyAddress(v1, &v1)) {
    cout << "Ref: same address" << endl;
  } else {
    cout << "Ref: different address" << endl;
  }

  return EXIT_SUCCESS;
}

static int VerifyValues(const Vector& v, float x, float y, float z) {
  if (v.get_x() != x
      || v.get_y() != y
      || v.get_z() != z) {
    cerr << "The vector's coordinates are incorrect:" << endl;
    cerr << "Expected x: " << x << " Actual x: " << v.get_x() << endl;
    cerr << "Expected y: " << y << " Actual y: " << v.get_y() << endl;
    cerr << "Expected z: " << z << " Actual z: " << v.get_z() << endl;
    return -1;
  }
  return 0;
}

static void Increment(Vector v) {
  v += Vector(1.0f, 1.0f, 1.0f);
}

static bool VerifyAddress(const Vector& v, void* addr) {
  return &v == addr;
}
