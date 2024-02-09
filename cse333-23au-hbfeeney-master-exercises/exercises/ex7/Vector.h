/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#ifndef VECTOR_H_
#define VECTOR_H_
#include <iostream>

namespace vector333 {

static const int NUM_COMPONENTS = 3;

class Vector {
 public:
  // Constructs a vector with coordinates (0, 0, 0).
  Vector();

  // Constructs a vector with coordinates (x, y, z).
  //  x, y, z - the x, y, and z coordinates of the vector
  Vector(float x, float y, float z);

  // Constructs a vector with the same coordinates as the passed vector.
  //  other - the vector whose coordinates will be copied
  Vector(const Vector& other);

  // Destructs this vector.
  ~Vector();

  // Assigns this vector's coordinates to be the same as the passed vector's.
  //  other - the vector whose coordinates will be copied
  // Returns this vector with reassigned values.
  Vector& operator=(const Vector& other);

  // Increments this vector's coordinates by each of the passed vector's.
  //  other - the vector whose coordinates will be increment this vector's
  // Returns this vector with incremented values.
  Vector& operator+=(const Vector& other);

  // Decrements this vector's coordinates by each of the passed vector's.
  //  other - the vector whose coordinates will be increment this vector's
  // Returns this vector with decremented values.
  Vector& operator-=(const Vector& other);

  // Computes the vector sum of this vector and the other.
  //  other - the vector to add with this one
  // Returns a new vector that is the vector sum of this and the other.
  Vector operator+(const Vector& other) const;

  // Computes the vector diffrence of this vector and the other.
  //  other - the vector to subtract from this one
  // Returns a new vector that is the vector difference of this and the other.
  Vector operator-(const Vector& other) const;

  // Computes the dot product of this vector and the other.
  //  other - the vector to dot with this one
  // Returns the dot product of this vector and the other.
  float operator*(const Vector& other) const;

  // Scales this vector by a scalar value.
  //  k - the scalar to scale this vector by
  // Returns a new vector resulting from scaling this vector by k.
  Vector operator*(const float k) const;

  // Scales a vector by a scalar value.
  //  k - the scalar to scale the input vector by
  //  v - the vector to scale by k
  // Returns a new vector resulting from scaling the vector by k.
  friend Vector operator*(const float k, const Vector& v);

  // Prints a vector to an output stream.
  //  out - the stream v should be printed to
  //  v   - the vector to print to the stream
  // Returns a reference to the stream v was printed to.
  friend std::ostream& operator<<(std::ostream& out, const Vector& v);

 private:
  float* components;
};

}  // namespace vector333

#endif  // VECTOR_H_
