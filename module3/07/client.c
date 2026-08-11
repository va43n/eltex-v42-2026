#include "group_chat.h"

int do_client_activity(char* address) {
  my_socket s;

  signal(SIGINT, handle_SIGINT);
  printf("I'm a client and the server is: %s\n", s.server_addr);

  if (socket_connect(&s, address) == FAILURE) return FAILURE;

  fd_set rfds, rfds_start;
  int maxfd = (s.s > STDIN_FILENO ? s.s : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(s.s, &rfds_start);

  if (send_welcome_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (send_welcome_message)\n");
    return FAILURE;
  }

  while (!is_signal) {
    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        is_signal = 1;
        continue;
      }
      fprintf(stderr, "ERROR: do_client_activity (select).\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (socket_send(s, NULL, 0) == FAILURE) break;
    }

    if (FD_ISSET(s.s, &rfds)) {
      if (socket_receive(s) == FAILURE) break;
    }
  }

  if (send_goodbye_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (send_goodbye_message)\n");
    return FAILURE;
  }

  if (socket_disconnect(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (socket_disconnect)\n");
    return FAILURE;
  }

  return SUCCESS;
}
