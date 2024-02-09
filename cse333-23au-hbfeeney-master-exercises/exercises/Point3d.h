/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#ifndef POINT3D_H_
#define POINT3D_H_
#include <stdint.h>

// A simple representation of a point in 3d space.
// A Point3d contains three integers representing the point's coordinates.
typedef struct point3d {
    int32_t x;
    int32_t y;
    int32_t z;
} Point3d;

// Allocates space for a Point3d on the heap, initializing it with the input
// values for x, y, and z. Returns a pointer to the Point3d. In the event of a
// failed allocation, prints a message to the console and the returned pointer
// is NULL.
Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z);

// Scales the Point3d pointed to by pointPtr by factor. In the event that
// pointPtr is NULL, prints an error message.
void Point3d_Scale(Point3d* pointPtr, int32_t factor);

// Returns a Point3d initialized with coordinates corresponding to the origin.
Point3d Point3d_GetOrigin();

#endif  // POINT3D_H_
