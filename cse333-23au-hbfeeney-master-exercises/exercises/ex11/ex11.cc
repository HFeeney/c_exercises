/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include <iostream>
#include <unistd.h>
#include "SocketUtil.h"

static const int BUF_SIZE = 512;

// Prints a message to stderr indicating proper use of this program and exits.
void Usage();

// Attempts to accept a connection from the provided socket and return the
// connection's file descriptor. This method handles recoverable errors. The
// caller is responsible for closing the connection.
//  fd - a file descriptor for the socket to accept connections from
// Returns the socket file descriptor for the connected client on success, and
// -1 on failure.
int WrappedAccept(int fd);

// Create a listening socket, accept a connection from a client,
// and write all the data the client sends to stdout.
int main(int argc, char** argv) {
  // Validate input.
  if (argc != 2) {
    Usage();
  }

  // Attempt to begin listening on this port.
  int af;
  int fd = Listen(argv[1], &af);
  if (fd == -1) {
    return EXIT_FAILURE;
  }

  // Attempt to accept a connection from the client.
  int c_fd = WrappedAccept(fd);
  if (c_fd == -1) {
    // Clean up and fail.
    close(fd);
    return EXIT_FAILURE;
  }

  // Read all data sent by the client, writing it to stdout continuously.
  unsigned char buf[BUF_SIZE];
  int bytes_read = 0;
  while ((bytes_read = WrappedRead(c_fd, buf, BUF_SIZE)) != 0) {
    if (bytes_read == -1) {
      // Clean up and fail.
      close(c_fd);
      close(fd);
      return EXIT_FAILURE;
    }

    // Write all read bytes to stdout.
    if (WrappedWrite(STDOUT_FILENO, buf, bytes_read) < bytes_read) {
      // Clean up and fail.
      close(c_fd);
      close(fd);
      return EXIT_FAILURE;
    }
  }

  // Clean up.
  close(c_fd);
  close(fd);
  return EXIT_SUCCESS;
}

int WrappedAccept(int fd) {
  struct sockaddr_storage c_addr;
  socklen_t c_addrlen = sizeof(c_addr);
  int c_fd = -1;
  while ((c_fd = accept(fd, reinterpret_cast<sockaddr*>(&c_addr), &c_addrlen))
          == -1) {
    if (errno != EINTR && errno != EAGAIN && errno!= EWOULDBLOCK) {
      return -1;
    }
  }
  return c_fd;
}

void Usage() {
  std::cerr << "Usage:" << std::endl;
  std::cerr << "./ex11 port_number" << std::endl;
  exit(EXIT_FAILURE);
}

