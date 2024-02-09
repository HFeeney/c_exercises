// Hayden Feeney
// hbfeeney@uw.edu
// Copyright 2023 Hayden Feeney

#include <stdlib.h>
#include <stdio.h>

// Computes an approximation of Pi using Nilakantha's series.
//   term - the term the series should compute up to, a value >= 0
double nilakantha(int term) {
    double result = 3.0;
    int sign = 1;
    for (int i = 1; i <= term; i++) {
        result += sign
                    * (4.0 / ((2.0 * i) * (2.0 * i + 1.0) * (2.0 * i + 2.0)));
        sign *= -1;
    }
    return result;
}



int main(int argc, char* argv[]) {
    // If there are no arguments, indicate failure. In the case of multiple
    // arguments, the first is accepted.
    if (argc <= 1) {
        printf("Error: no arguments passed!\n");
        return EXIT_FAILURE;
    }

    char* nextc;
    int input = (int) strtol(argv[1], &nextc, 10);

    // If input starts with something other than an int, or contains decimal
    // point or other character after initial integer, fail
    if (nextc == argv[1] || *nextc != 0) {
        printf("Error: invalid input\n");
        return EXIT_FAILURE;
    }

    // If input is negative, fail
    if (input < 0) {
        printf("Error: negative input\n");
        return EXIT_FAILURE;
    }

    double result = nilakantha(input);

    printf("Our estimate of Pi is %.20f\n", result);

    return EXIT_SUCCESS;
}
