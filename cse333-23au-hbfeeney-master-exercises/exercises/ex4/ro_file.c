// Hayden Feeney
// hbfeeney@uw.edu
// Copyright 2023 Hayden Feeney

#include "ro_file.h"

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


/*** INTERNAL DATA TYPES AND CONSTANTS **************************************/

static const int RO_FILE_BUF_LEN = 512;  // do not modify

struct ro_file_st {
  int fd;         // The file descriptor we are currently managing.

  char* buf;      // Pointer to our internal buffer for this file.

  off_t buf_pos;  // The position in the file that the beginning of our
                  // internal buffer currently corresponds to.

  int buf_index;  // The index in our internal buffer that corresponds to the
                  // user's current position in the file.

  int buf_end;    // How many bytes currently in our internal buffer are
                  // actually from the file.
                  // To understand why this is important to track, consider
                  // the case when the file length is < RO_FILE_BUF_LEN.
};


/*** STATIC HELPER FUNCTION DECLARATIONS ************************************/

// Copy up to 'amount' bytes from the RO_FILE's internal buffer to 'out'.
// Returns the number of bytes copied.
static size_t flush_buffer(RO_FILE* file, char* out, int amount);

// Fill in the RO_FILE's internal buffer with as many bytes as can be read
// from the file.
// Returns the number of bytes copied into the buffer, or -1 on any error.
static ssize_t fill_buffer(RO_FILE* file);


/*** FUNCTION DEFINITIONS ***************************************************/

RO_FILE* ro_open(char* filename) {
  // 1. Allocate a new RO_FILE
  RO_FILE* file = (RO_FILE*) malloc(sizeof(RO_FILE));
  if (file == NULL) {
    return NULL;
  }

  // 2. Get the file descriptor for the file
  file->fd = open(filename, O_RDONLY);

  // 3. Allocate the internal buffer
  file->buf = (char*) malloc(RO_FILE_BUF_LEN * sizeof(char));
  if (file->buf == NULL) {
    return NULL;
  }

  // 4. Initialize the other fields (no reading done yet)
  file->buf_pos = 0;
  file->buf_index = 0;
  file->buf_end = 0;

  return file;
}

ssize_t ro_read(char* ptr, size_t len, RO_FILE* file) {
  // 1. If we have bytes in our internal buffer, flush as many as we can to
  //    'ptr'.
  size_t num_copied_out = flush_buffer(file, ptr, len);

  while (num_copied_out != len) {
    // 2. If we haven't provided the bytes requested, repopulate our buffer
    //    with bytes from the file.
    ssize_t num_filled = fill_buffer(file);
    if (num_filled == 0) {
      // reached EOF; no more bytes to copy
      break;
    } else if (num_filled < 0) {
      // pass error back to client
      return num_filled;
    }

    // 3. Copy filled bytes into 'ptr'.
    num_copied_out += flush_buffer(file, ptr+num_copied_out,
                                   len-num_copied_out);

    // 4. Repeat steps 2-3 until request is fulfilled.
  }

  return num_copied_out;
}

off_t ro_tell(RO_FILE* file) {
  if (file == NULL) {
    return -1;
  }
  return file->buf_pos + file->buf_index;
}

int ro_seek(RO_FILE* file, off_t offset, int whence) {
  // 1. Check validity of arguments, where applicable.
  if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
    // whence had invalid value.
    return 1;
  }

  // 2. Seek to specified offset from specified whence using lseek.
  //    No need to check if new position is already in our buffer.
  int newpos = lseek(file->fd, offset, whence);
  if (newpos < 0) {
    // Handle error as well as attempts to seek to before the beginning of
    // the file.
    return 1;
  }

  // 3. Update our buffer indicators
  file->buf_pos = newpos;
  file->buf_index = 0;
  file->buf_end = 0;

  return 0;
}

int ro_close(RO_FILE* file) {
  int error;
  if ((error = close(file->fd))) {
    // Pass error on to client.
    return error;
  }
  // Free the internal buffer and the file struct.
  free(file->buf);
  free(file);
  return 0;
}


/*** STATIC HELPER FUNCTION DEFINITIONS *************************************/

size_t flush_buffer(RO_FILE* file, char* out, int amount) {
  // 1. Copy/flush bytes to 'out' starting from the buffer index. The amount
  //    flushed should be the min of 'amount' and the remaining unflushed bytes
  //    in the buffer.

  // The number of unread bytes is the difference between the end of the
  // valid buffer data and the index of the buffer data read so far.
  int bytesremaining = file->buf_end - file->buf_index;
  // The number of bytes that will be flushed will be the minimum between
  // the number of unread bytes and the number requested.
  int flushcount = bytesremaining <= amount ? bytesremaining : amount;

  // Flush bytes by copying them over to the location pointed to by out.
  for (int i = 0; i < flushcount; i++) {
    out[i] = file->buf[file->buf_index + i];
  }

  // 2. Advance buffer index by the number of bytes flushed.
  file->buf_index += flushcount;

  // 3. Return the number of bytes flushed.
  return flushcount;
}

ssize_t fill_buffer(RO_FILE* file) {
  // The buffer will begin with data at buf_pos + buf_index
  file->buf_pos += file->buf_index;

  // The buffer will have had no bytes read, and no valid data.
  file->buf_index = 0;
  file->buf_end = 0;

  int numread = 0;                     // The number of bytes read
  int numremaining = RO_FILE_BUF_LEN;  // The number of bytes left in the
                                       // buffer to fill.
  // Continue to read bytes as long as there is space left in the buffer,
  // fewer bytes than requested are read, and eof has not been reached.
  while (numremaining > 0
      && (numread = read(file->fd, file->buf, numremaining)) < numremaining
      && numread != 0) {
    // Handle errors.
    if (numread == -1) {
      if (errno != EINTR && errno != EAGAIN) {
        return -1;
      }
      continue;
    }
    // Update values.
    numremaining -= numread;
    file->buf_end += numread;
  }
  // The number of bytes read is equivalent to the amount of valid buffer.
  return file->buf_end;
}
