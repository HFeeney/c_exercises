/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include "./Point3d.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

Point3d* Point3d_Allocate(int32_t x, int32_t y, int32_t z) {
    Point3d* pointPtr = (Point3d*) malloc(sizeof(Point3d));
    if (pointPtr == NULL) {
        printf("Malloc failed to allocate space for a Point3d!\n");
        return NULL;
    }
    pointPtr->x = x;
    pointPtr->y = y;
    pointPtr->z = z;
    return pointPtr;
}

void Point3d_Scale(Point3d* pointPtr, int32_t factor) {
    if (pointPtr == NULL) {
        printf("Invalid pointer (NULL) input to Point3d_Scale!\n");
        return;
    }
    pointPtr->x *= factor;
    pointPtr->y *= factor;
    pointPtr->z *= factor;
}

Point3d Point3d_GetOrigin() {
    Point3d origin = {0, 0, 0};
    return origin;
}
