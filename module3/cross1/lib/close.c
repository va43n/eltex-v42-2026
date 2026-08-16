#include "raw_socket_operations.h"

int raw_socket_close(int fd) {
  if (close(fd) < 0) {
    fprintf(stderr, "ERROR: raw_socket_close - cannot close the socket.\n");
    perror("close");
    return FAILURE;
  }

  return SUCCESS;
}