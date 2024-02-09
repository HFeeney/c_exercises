/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
 public:
  // Initializes the vector to the coordinates (0, 0, 0).
  Vector();

  // Initializes the vector with the given coordinates.
  //   x - the x coordinate to initialize the vector with
  //   y - the y coordinate to initialize the vector with
  //   z - the z coordinate to initialize the vector with
  Vector(float x, float y, float z);

  // Initializes the vector to have the same coordinates as the passed
  // vector.
  //   other - the vector whose coordinates will be copied
  Vector(const Vector& other);

  // Destructs this vector.
  ~Vector() = default;

  // Assigns this vector's coordinates to the values of the other.
  //   other - the vector whose coordinates will be copied
  // Returns a reference to the modified instance of this vector.
  Vector& operator=(const Vector& other);

  // Gets the x coordinate associated with this vector.
  float get_x() const { return x_; }

  // Gets the y coordinate associated with this vector.
  float get_y() const { return y_; }

  // Gets the z coordinate associated with this vector.
  float get_z() const { return z_; }

  // Increases each coordinate of this vector by the coordinates of the
  // passed vector.
  //   other - the vector whose coordinates will modify this vector's
  // Returns a reference to the now modified vector.
  Vector& operator+=(const Vector& other);

  // Decreases each coordinate of this vector by the coordinates of the
  // passed vector.
  //   other - the vector whose coordinates will modify this vector's
  // Returns a reference to the now modified vector.
  Vector& operator-=(const Vector& other);

  // Calculates the result of the dot product of this vector with the other.
  //   other - the vector to be dotted with this one
  // Returns the result of the dot product.
  float operator*(const Vector& other) const;

 private:
  float x_;
  float y_;
  float z_;
};

#endif  // VECTOR_H_
