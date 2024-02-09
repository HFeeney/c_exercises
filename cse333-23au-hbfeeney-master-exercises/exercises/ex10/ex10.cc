/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include "SocketUtil.h"

using std::string;

const int NUM_PORTS = 65535;
const int BUF_SIZE = 512;

// Prints a string describing the proper usage of this program.
void Usage();

// Open a local file, open a socket to a remote process,
// and send the file to other process.
int main(int argc, char** argv) {
  // Validate input.
  // There must be 3 arguments passed.
  if (argc < 4) {
    Usage();
    return EXIT_FAILURE;
  }

  // Attempt to parse the port number as an int.
  char* badCharPos;
  int port = std::strtol(argv[2], &badCharPos, 10);
  if (badCharPos < argv[2] + std::strlen(argv[2])
      || port < 0
      || port > NUM_PORTS) {
    std::cerr << "Invalid port number." << std::endl;
    Usage();
    return EXIT_FAILURE;
  }

  // Attempt to open the passed file.
  int file_fd = open(argv[3], O_RDONLY);
  if (file_fd == -1) {
    std::cerr << "Failed to open file." << std::endl;
    Usage();
    return EXIT_FAILURE;
  }

  // Great, inputs were valid (as far as we can tell).
  // Attempt to do DNS lookup of the servername.
  struct sockaddr_storage addr;
  size_t addrlen;
  if (!LookupName(argv[1], port, &addr, &addrlen)) {
    std::cerr << "Failed to perform DNS lookup of servername." << std::endl;
    // Clean up.
    close(file_fd);
    return EXIT_FAILURE;
  }

  // Attempt to establish a socket connection with the server.
  int sock_fd;
  if (!Connect(addr, addrlen, &sock_fd)) {
    std::cerr << "Failed to establish a connection with the server."
              << std::endl;
    // Clean up.
    close(file_fd);
    return EXIT_FAILURE;
  }

  // Awesome, now we can read from the input file and send the bytes.
  int read_res;
  unsigned char buf[BUF_SIZE];
  while ((read_res = WrappedRead(file_fd, buf, BUF_SIZE)) > 0) {
    // Handle read error.
    if (read_res == -1) {
      std::cerr << "Unrecoverable read error encountered." << std::endl;
      // Clean up.
      close(sock_fd);
      close(file_fd);
    }

    // Attempt to write all read bytes to the server.
    if (WrappedWrite(sock_fd, buf, read_res) < read_res) {
      std::cerr << "Unrecoverable write error encountered." << std::endl;
      // Clean up.
      close(sock_fd);
      close(file_fd);
    }
  }

  // We're done sending data! Close up the connection and clean up the input
  // file resources.
  close(sock_fd);
  close(file_fd);

  return EXIT_SUCCESS;
}

void Usage() {
  std::cerr << "Usage: ./ex10 servername portnumber filename" << std::endl;
}
