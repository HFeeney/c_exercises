/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include "./Point3d.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void printPoint(Point3d* pointPtr) {
    printf("Point x: %d, ", pointPtr->x);
    printf("Point y: %d, ", pointPtr->y);
    printf("Point z: %d\n", pointPtr->z);
}

int main(int argc, char* argv[]) {
    // test Point3d_Allocate
    Point3d* pointPtr = Point3d_Allocate(1, 2, 3);
    if (pointPtr == NULL) {
        return EXIT_FAILURE;
    }

    if (pointPtr->x != 1) {
        printf("Allocate incorrectly assigned x!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->y != 2) {
        printf("Allocate incorrectly assigned y!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->z != 3) {
        printf("Allocate incorrectly assigned z!\n");
        return EXIT_FAILURE;
    }

    // test Point3d_Scale
    Point3d_Scale(pointPtr, 7);
    if (pointPtr->x != 7) {
        printf("Scale incorrectly modified x!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->y != 14) {
        printf("Scale incorrectly modified y!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->z != 21) {
        printf("Scale incorrectly modified z!\n");
        return EXIT_FAILURE;
    }

    // test Point3d_Origin
    *pointPtr = Point3d_GetOrigin();
    if (pointPtr->x != 0) {
        printf("GetOrigin incorrectly assigned x!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->y != 0) {
        printf("GetOrigin incorrectly assigned y!\n");
        return EXIT_FAILURE;
    }

    if (pointPtr->z != 0) {
        printf("GetOrigin incorrectly assigned z!\n");
        return EXIT_FAILURE;
    }

    // free the memory associated with the point
    free(pointPtr);

    return EXIT_SUCCESS;
}
