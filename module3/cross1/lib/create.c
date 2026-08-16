#include "raw_socket_operations.h"

int raw_socket_create(int *fd) {
  if ((*fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "ERROR: create_raw_socket - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  int val = 1;
  if (setsockopt(*fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) {
    fprintf(stderr,
            "ERROR: socket_connect - cannot add permissions to "
            "socket.\n");
    perror("setsockopt");
    close(*fd);
    return FAILURE;
  }

  return SUCCESS;
}