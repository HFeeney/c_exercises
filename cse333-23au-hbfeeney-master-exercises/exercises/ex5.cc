// Hayden Feeney
// hbfeeney@uw.edu
// Copyright 2023 Hayden Feeney

#include <iostream>
#include <cstdlib>

using std::cout;
using std::cerr;
using std::cin;
using std::endl;

// This method prints all positive factors of the input number.
//   input - a positive integer value to print the factors of
void printFactors(const int input);

// This program prompts the user for an integer and prints out all factors
// of that integer.
int main(int argc, char* argv[]) {
  // Prompt the user for a positive integer.
  cout << "Which positive integer would you like me to factorize? ";
  int input;
  cin >> input;

  // Handle failed attempts to retrieve input, and invalid integer input.
  if (cin.fail() || input < 1) {
    cout << "Invalid input, please enter only positive integers." << endl;
    return EXIT_FAILURE;
  }

  // Pass input to factor printing function.
  printFactors(input);

  return EXIT_SUCCESS;
}

void printFactors(const int input) {
  // 1 is always a factor of any positive integer.
  cout << "1";

  // Loop through all integers up to the input value (excluding 1).
  for (int i = 2; i <= input; i++) {
    if (input % i == 0) {
      // Print the value since it is a factor of input.
      cout << " " << i;
    }
  }

  // Output a final end line character.
  cout << endl;
}
