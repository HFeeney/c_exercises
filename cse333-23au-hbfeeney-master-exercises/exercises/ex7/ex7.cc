/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <iostream>
#include <cstdlib>
#include "Vector.h"

using std::cout;
using std::cerr;
using std::endl;
using vector333::Vector;

static void VerifyDotProduct(const Vector& v1, const Vector& v2,
    const float k);

int main(int argc, char** argv) {
  // Test constructors, initialize unit vectors.
  cout << "Testing constructors..." << endl;
  const Vector ihat(1.0, 0.0, 0.0);
  const Vector jhat(0.0, 1.0, 0.0);
  const Vector khat(0.0, 0.0, 1.0);
  const Vector origin;
  const Vector allones(1.0, 1.0, 1.0);
  VerifyDotProduct(origin, allones, 0.0);
  VerifyDotProduct(ihat, jhat, 0.0);
  VerifyDotProduct(ihat, ihat, 1.0);

  // Test copy constructor, assignment.
  cout << "Testing copy constructors and assignments..." << endl;
  const Vector origin2 = origin;
  Vector allones2;
  allones2 = allones;
  VerifyDotProduct(origin2, allones2, 0.0);
  VerifyDotProduct(allones, allones2, 3.0);

  // Test increment and decrement.
  cout << "Testing increment and decrement..." << endl;
  Vector incrementTester;
  incrementTester += ihat;
  incrementTester += ihat;
  incrementTester -= jhat;
  incrementTester -= jhat;
  incrementTester += khat;
  VerifyDotProduct(incrementTester, incrementTester, 9.0);

  // Test addition, subtraction.
  cout << "Testing addition and subtraction..." << endl;
  Vector alltwos = allones + allones2;
  VerifyDotProduct(alltwos, allones, 6.0);
  VerifyDotProduct(allones, allones2, 3.0);
  const Vector onetwothree(1.0, 2.0, 3.0);
  VerifyDotProduct(onetwothree - alltwos, allones, 0.0);
  VerifyDotProduct(onetwothree - alltwos, alltwos - onetwothree, -2.0);

  // Test scaling on right and left.
  cout << "Testing scaling..." << endl;
  const Vector allthrees = 3.0 * allones;
  const Vector foureighttwelve = onetwothree * 4.0;
  VerifyDotProduct(allthrees, allones, 9.0);
  VerifyDotProduct(foureighttwelve, allones, 24.0);
  VerifyDotProduct(allthrees, foureighttwelve, 72.0);

  cout << "All tests passed!" << endl;
  return EXIT_SUCCESS;
}

static void VerifyDotProduct(const Vector& v1, const Vector& v2,
    const float k) {
  float prod = v1 * v2;
  if (prod != k) {
    cerr << "FAILED:" << endl;
    cerr << "Dot product of " << v1 << " and " << v2 << " was " << prod
      << ", not " << k << "." << endl;
    exit(EXIT_FAILURE);
  }
}
