/*
 * Copyright ©2023 Hayden Feeney
 * hbfeeney@uw.edu
*/

#include "SocketUtil.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>

#include <iostream>

using std::cerr;
using std::endl;

bool LookupName(char* name,
                unsigned short port,
                struct sockaddr_storage* ret_addr,
                size_t* ret_addrlen) {
  // Create result pointer.
  struct addrinfo *result;

  // Create and zero out hints struct, initialize with proper hints.
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Attempt to do DNS lookup of the server.
  int retval;
  while ((retval = getaddrinfo(name, nullptr, &hints, &result)) != 0) {
    if (retval != EAI_AGAIN) {
      return false;
    }
  }
  if (result == nullptr) {
    // Clean up.
    freeaddrinfo(result);
    return false;
  }

  // Set the port number of the returned address.
  if (result->ai_family == AF_INET) {
    struct sockaddr_in* addr_ptr =
      reinterpret_cast<sockaddr_in*>(result->ai_addr);
    addr_ptr->sin_port = htons(port);
  } else if (result->ai_family == AF_INET6) {
    struct sockaddr_in6* addr_ptr =
      reinterpret_cast<sockaddr_in6*>(result->ai_addr);
    addr_ptr->sin6_port = htons(port);
  } else {
    // Clean up.
    freeaddrinfo(result);
    return false;
  }

  // Copy over the address through the return parameter.
  memcpy(ret_addr, result->ai_addr, result->ai_addrlen);

  // Return the address length.
  *ret_addrlen = result->ai_addrlen;

  // Clean up.
  freeaddrinfo(result);

  return true;
}

bool Connect(const struct sockaddr_storage& addr,
             const size_t& addrlen,
             int* ret_fd) {
  // Create a socket.
  int sock_fd = socket(addr.ss_family, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    return false;
  }

  // Connect using the socket.
  if (connect(sock_fd,
              reinterpret_cast<const sockaddr*>(&addr),
              addrlen)) {
    close(sock_fd);
    return false;
  }

  // Return the socket file descriptor.
  *ret_fd = sock_fd;

  return true;
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
