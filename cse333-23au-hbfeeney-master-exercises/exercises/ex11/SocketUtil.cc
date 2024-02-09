/*
* Copyright ©2023 Hayden Feeney
* hbfeeney@uw.edu
*/

#include "SocketUtil.h"

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <cstring>
#include <iostream>

using std::cerr;
using std::endl;

const int NUM_PORTS = 65535;

int Listen(char* portnum, int* sock_family) {
  // Attempt to parse argument as portnum.
  char* badCharPos;
  int port = std::strtol(portnum, &badCharPos, 10);
  if (badCharPos < portnum + std::strlen(portnum)
      || port > NUM_PORTS
      || port < 0) {
    std::cerr << "Invalid port number." << std::endl;
    return -1;
  }

  // Attempt to get addresses we can listen on this port from.
  struct addrinfo* results;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_flags |= AI_V4MAPPED;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  if (getaddrinfo(nullptr, portnum, &hints, &results)) {
    std::cerr << "Failed to retrieve valid IP addresses." << std::endl;
    return -1;
  }

  // We have a result. Attempt to create a socket using any of the results.
  int fd = -1;
  for (struct addrinfo* rp = results; rp != nullptr; rp = rp->ai_next) {
    // Attempt to create a socket with this IP result.
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd == -1) {
      // Socket creation failed! Continue on to the next result.
      continue;
    }

    // Attempt to bind the socket
    if (bind(fd, rp->ai_addr, rp->ai_addrlen)) {
      // Bind failed. Close the socket and try the next result.
      close(fd);
      fd = -1;
      continue;
    }

    // Bind succeeded! Return address family to caller and stop looping.
    *sock_family = rp->ai_family;
    break;
  }

  // Free the results struct.
  freeaddrinfo(results);

  // If fd is -1, we failed to create and bind a socket. Indicate failure.
  if (fd == -1) {
    return -1;
  }

  // Attempt to designate the socket as a listening socket.
  if (listen(fd, SOMAXCONN)) {
    close(fd);
    return -1;
  }

  // Success!
  return fd;
}

int WrappedRead(int fd, unsigned char* buf, int readlen) {
  int read_res;
  while ((read_res = read(fd, buf, readlen)) == -1) {
    if (errno != EINTR && errno != EAGAIN) {
      return -1;
    }
  }
  return read_res;
}

int WrappedWrite(int fd, unsigned char* buf, int writelen) {
  int write_res;
  while ((write_res = write(fd, buf, writelen)) == -1) {
    if (errno != EINTR && errno != EAGAIN) {
      return -1;
    }
  }
  return write_res;
}
