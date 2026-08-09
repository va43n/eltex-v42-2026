#include "group_chat.h"

int handle_group_chat() {
  my_socket s;

  if (socket_connect(&s) == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (socket_connect)\n");
    return FAILURE;
  }

  fd_set rfds, rfds_start;
  int maxfd = (s.s > STDIN_FILENO ? s.s : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(s.s, &rfds_start);

  printf("The chat is started...\n");
  while (!is_signal) {
    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        is_signal = 1;
        continue;
      }
      fprintf(stderr, "ERROR: handle_group_chat (select).\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (socket_send(s) == FAILURE) break;
    }

    if (FD_ISSET(s.s, &rfds)) {
      if (socket_receive(s) == FAILURE) break;
    }
  }

  if (socket_disconnect(s) == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (socket_disconnect)\n");
    return FAILURE;
  }

  return SUCCESS;
}