#include "group_chat.h"

int handle_group_chat() {
  if (socket_connect() == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (socket_connect)\n");
    return FAILURE;
  }

  int read;

  fd_set rfds, rfds_start;
  int maxfd = (read > STDIN_FILENO ? read : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(read, &rfds_start);

  while (!is_signal) {
    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        is_signal = 1;
        continue;
      }
      fprintf(stderr,
              "ERROR: handle_group_chat (select).\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (socket_send() == FAILURE) break;
    }

    if (FD_ISSET(read, &rfds)) {
      if (socket_receive() == FAILURE) break;
    }
  }

  if (socket_disconnect() == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (socket_disconnect)\n");
    return FAILURE;
  }

  return SUCCESS;
}