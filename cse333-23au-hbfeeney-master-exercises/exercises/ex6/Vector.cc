/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include "Vector.h"

Vector::Vector() : x_(0.0f), y_(0.0f), z_(0.0f) {}

Vector::Vector(float x, float y, float z) : x_(x), y_(y), z_(z) {}

Vector::Vector(const Vector& other) {
  x_ = other.x_;
  y_ = other.y_;
  z_ = other.z_;
}

Vector& Vector::operator=(const Vector& other) {
  if (this != &other) {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
  }
  return *this;
}

Vector& Vector::operator+=(const Vector& other) {
  x_ += other.x_;
  y_ += other.y_;
  z_ += other.z_;
  return *this;
}

Vector& Vector::operator-=(const Vector& other) {
  x_ -= other.x_;
  y_ -= other.y_;
  z_ -= other.z_;
  return *this;
}

float Vector::operator*(const Vector& other) const {
  return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}
