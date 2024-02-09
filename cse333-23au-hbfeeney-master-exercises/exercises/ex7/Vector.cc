/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <iostream>
#include "Vector.h"

namespace vector333 {

Vector::Vector() {
  components = new float[NUM_COMPONENTS]{0.0, 0.0, 0.0};
}

Vector::Vector(float x, float y, float z) {
  components = new float[NUM_COMPONENTS]{x, y, z};
}

Vector::Vector(const Vector& other) {
  components = new float[NUM_COMPONENTS];
  components[0] = other.components[0];
  components[1] = other.components[1];
  components[2] = other.components[2];
}

Vector::~Vector() {
  delete[] components;
}

Vector& Vector::operator=(const Vector& other) {
  if (this != &other) {
    components[0] = other.components[0];
    components[1] = other.components[1];
    components[2] = other.components[2];
  }
  return *this;
}

Vector& Vector::operator+=(const Vector& other) {
    components[0] += other.components[0];
    components[1] += other.components[1];
    components[2] += other.components[2];
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    components[0] -= other.components[0];
    components[1] -= other.components[1];
    components[2] -= other.components[2];
    return *this;
}

Vector Vector::operator+(const Vector& other) const {
  Vector res(*this);
  res += other;
  return res;
}

Vector Vector::operator-(const Vector& other) const {
  Vector res(*this);
  res -= other;
  return res;
}

float Vector::operator*(const Vector& other) const {
  return components[0] * other.components[0]
       + components[1] * other.components[1]
       + components[2] * other.components[2];
}

Vector Vector::operator*(const float k) const {
  Vector res(*this);
  res.components[0] *= k;
  res.components[1] *= k;
  res.components[2] *= k;
  return res;
}

Vector operator*(const float k, const Vector& v) {
  Vector res(v);
  res.components[0] *= k;
  res.components[1] *= k;
  res.components[2] *= k;
  return res;
}

std::ostream& operator<<(std::ostream& out, const Vector& v) {
  out << "(" << v.components[0] << "," << v.components[1] << ","
      << v.components[2] << ")";
  return out;
}

}  // namespace vector333
