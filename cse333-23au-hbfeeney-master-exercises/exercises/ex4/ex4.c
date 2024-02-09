// Hayden Feeney
// hbfeeney@uw.edu
// Copyright 2023 Hayden Feeney

#define _GNU_SOURCE
#include <stdio.h>    // for snprintf
#include <stdlib.h>   // for EXIT_SUCCESS, NULL
#include <string.h>   // for strrchr, strcmp, strlen
#include <stdbool.h>  // for bool
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "ro_file.h"
#define RBUFSIZE 256


/*** HELPER FUNCTION DECLARATIONS ******************************************/

// Returns whether or not a filename ends in ".txt".
bool IsTxtFile(char* filename);

// Concatenate the directory and file names into a full path. The caller is
// responsible for freeing the allocated string. Exits if an error occurs.
char* Concatenate(char* dirname, char* filename);

// Replace the last character from the supplied string if it is a slash with
// the null character. The input must be non-null and null terminated.
void RemoveEndingSlash(char* str);

/*
 * This program:
 * - Accepts a directory name as a command-line argument.
 * - Scans through the directory looking for all files whose names end with
 *   the four characters ".txt".
 * - For every such file found, write the contents of those files to stdout
 *   without adding any additional characters or formatting.
 *   Eventually reading the files with ro_file module.
 */
int main(int argc, char** argv) {
  // Indicate an error occurred for improper function use.
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments.\n");
    return EXIT_FAILURE;
  }

  // Remove ending slash from input directory for consistency.
  RemoveEndingSlash(argv[1]);

  // Record the name of the directory this program is working in. This will
  // be a malloc'ed string.
  char* workingdirname;

  // Attempt to open the directory passed by the user, first interpreting it
  // as an absolute path.
  DIR* dirp = opendir(argv[1]);
  if (dirp == NULL) {
    // Indicate that this path could not be interpreted as absolute.
    fprintf(stderr, "Failed to open as absolute path. Attempting to open ");
    fprintf(stderr, "folder as the current or subdirectory...\n");

    // Remove beginning slash from input.
    if (argv[1][0] == '/') {
      argv[1] = argv[1] + 1;
    }

    // Attempt to open a sub directory matching the parameter.
    workingdirname = Concatenate(".", argv[1]);
    dirp = opendir(workingdirname);

    // Handle failure.
    if (dirp == NULL) {
      perror("Failed to open directory");
      return EXIT_FAILURE;
    }
  } else {
    // The input directory name was a valid directory. Record this in
    // a malloc'ed string.
    int mallocsize = (strlen(argv[1]) + 1) * sizeof(char);
    workingdirname = (char*) malloc(mallocsize);
    snprintf(workingdirname, mallocsize, "%s", argv[1]);
  }

  struct dirent *entry;

  // Set errno to 0 to distinguish between failure to read and end of
  // directory. Loop through the contents of the directory.
  errno = 0;
  char readbuf[RBUFSIZE];
  while ((entry = readdir(dirp)) != NULL) {
    // Check whether the current file is a txt file.
    char* filename = Concatenate(workingdirname, entry->d_name);
    if (IsTxtFile(filename)) {
      // Attempt to open the file to read.
      RO_FILE* file = ro_open(filename);
      if (file == NULL) {
        perror("Failed to open file");
        if (closedir(dirp)) {
          perror("Failed to close directory");
        }
        free(filename);
        free(workingdirname);
        return EXIT_FAILURE;
      }

      int bytesread;
      while ((bytesread = ro_read(readbuf, RBUFSIZE, file)) < RBUFSIZE
            && !bytesread == 0) {
        if (bytesread == -1) {
          perror("Failed to read from file");
          ro_close(file);
          if (closedir(dirp)) {
            perror("Failed to close directory");
          }
          free(filename);
          free(workingdirname);
          return EXIT_FAILURE;
        }

        for (int i = 0; i < bytesread; i++) {
          printf("%c", readbuf[i]);
        }
      }

      ro_close(file);
    }
    free(filename);
  }

  // Report the error to read the directory entry.
  if (errno != 0) {
    perror("Failed to read directory entry");
    if (closedir(dirp)) {
      perror("Failed to close directory");
    }
    free(workingdirname);
    return EXIT_FAILURE;
  }

  if (closedir(dirp)) {
    perror("Failed to close directory");
    free(workingdirname);
    return EXIT_FAILURE;
  }

  free(workingdirname);
  return EXIT_SUCCESS;
}


/*** HELPER FUNCTION DEFINITIONS *******************************************/

bool IsTxtFile(char* filename) {
  char* dot = strrchr(filename, '.');
  return dot && !strcmp(dot, ".txt");
}

char* Concatenate(char* dirname, char* filename) {
  bool has_trailing_slash = dirname[strlen(dirname) - 1] == '/';
  // Concatenate directory and file name.
  size_t dlen = strlen(dirname);
  size_t flen = strlen(filename);
  // Malloc space for full path name:
  // dlen + strlen("/") + flen + strlen('\0') = dlen + flen + 2
  int size_to_malloc = has_trailing_slash ? dlen + flen + 1 : dlen + flen + 2;
  char* fullpath = (char*) malloc(sizeof(char) * (size_to_malloc));
  if (fullpath == NULL) {
    fprintf(stderr, "Error on malloc.\n");
    exit(EXIT_FAILURE);
  }
  if (has_trailing_slash) {
    snprintf(fullpath, size_to_malloc, "%s%s", dirname, filename);
  } else {
    snprintf(fullpath, size_to_malloc, "%s/%s", dirname, filename);
  }
  return fullpath;
}

void RemoveEndingSlash(char *str) {
  int lastc_idx = strlen(str) - 1;
  if (str[lastc_idx] == '/') {
    str[lastc_idx] = 0;
  }
}
