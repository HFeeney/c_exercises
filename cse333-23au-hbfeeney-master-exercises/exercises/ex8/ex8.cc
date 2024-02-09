/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using std::ifstream;
using std::string;
using std::map;

// Reads a single token of the indicated type from in, returning it through the
// output parameter.
//   T      - the type of token to parse from the file stream
//   in     - a reference to an opened input file stream to read tokens from
//   output - an output parameter that the token will be returned through
// Returns true when a token is successfully read and stored in output, and
// false otherwise.
template <typename T>
static bool ReadValue(ifstream& in, T* const output);

// This program prints output indicating the frequency of each word found in
// the provided file, in the form "word freq" where word is the word, and freq
// is the number of times it was found in the file, with different words
// printed on different lines.
int main(int argc, char** argv) {
  // Validate input.
  if (argc < 2) {
    std::cerr << "Please provide a filename to open." << std::endl;
    return EXIT_FAILURE;
  }

  // Attempt to open the file stream. Throw an error if it failed to open.
  ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "Failed to open file." << std::endl;
    return EXIT_FAILURE;
  }

  // Record tokens read from the stream. Also, create a map recording the
  // frequencies of each token in the file.
  map<string, int> freq;
  string token;

  // Update the map as long as there are tokens left in the file.
  while (ReadValue(in, &token)) {
    freq[token]++;
  }


  // Iterate through the map and print tokens and their frequencies.
  for (auto& kv : freq) {
    std::cout << kv.first << " " << kv.second << std::endl;
  }

  return EXIT_SUCCESS;
}

template <typename T>
static bool ReadValue(ifstream& in, T* const output) {
  // Advance stream past all newline characters.
  while (in.peek() == '\n') {
    in.get();
  }

  // Return a token of input as long as in can still be read from.
  if (in.good()) {
    in >> *output;
    return true;
  }

  return false;
}
