// Hayden Feeney
// hbfeeney@uw.edu
// Copyright 2023 Hayden Feeney

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

// Prints a message indicating the specified number of bytes starting from
// a given address
//   data - a pointer to the beginning of the data to print
//   byte_len - the number of bytes to print
void DumpBytes(void* data, int32_t byte_len) {
    printf("The %u bytes starting at %p are:", byte_len, data);

    uint8_t* curr_byte = (uint8_t*) data;
    for (int32_t i = 0; i < byte_len; i++) {
        printf(" %02" PRIx8, *(curr_byte + i));
    }

    printf("\n");
}

// Swaps the elements found at the specified indices of the input array
//   arr - the input array
//   i - the index of the first element
//   j - the index of the second element
void swap(uint8_t arr[], int i, int j) {
    uint8_t temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// Prints a message indicating the content of the first array in the format
// of DumpBytes (see above). Copies data from the first array to the second
// and sorts it in non-decreasing order. Arrays must be the same length
//   input - array of data to be copied and sorted
//   output - array the copied and sorted data will be stored in
//   len - the length of both arrays
void CopyAndSort(uint8_t input[], uint8_t output[], int len) {
    DumpBytes(input, sizeof(input));

    // copy elements of input over to output
    for (int i = 0; i < len; i++) {
        output[i] = input[i];
    }

    // perform insertion sort on output
    for (int i = 1; i < len; i++) {
        for (int j = 0; j < i; j++) {
            if (output[j] > output[i]) {
                swap(output, i, j);
            }
        }
    }
}

int main(int argc, char* arv[]) {
  int32_t int_val = 1;
  float   float_val = 1.0f;
  uint8_t arr_unsorted[] = {3, 2, 0, 8, 17, 6, 10, 7, 8, 1, 12};
  uint8_t arr_sorted[]   = {0, 0, 0, 0,  0, 0,  0, 0, 0, 0,  0};

  DumpBytes(&int_val, sizeof(int_val));
  DumpBytes(&float_val, sizeof(float_val));
  DumpBytes(arr_unsorted, sizeof(arr_unsorted));
  CopyAndSort(arr_unsorted, arr_sorted, sizeof(arr_sorted));
  DumpBytes(arr_sorted, sizeof(arr_sorted));

  return EXIT_SUCCESS;
}

