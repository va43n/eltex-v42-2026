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

  if (send_welcome_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (send_welcome_message)\n");
    return FAILURE;
  }

  signal(SIGINT, handle_SIGINT);

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
      if (socket_send(s, NULL, 0) == FAILURE) break;
    }

    if (FD_ISSET(s.s, &rfds)) {
      if (socket_receive(s) == FAILURE) break;
    }
  }

  if (send_goodbye_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (send_goodbye_message)\n");
    return FAILURE;
  }

  if (socket_disconnect(s) == FAILURE) {
    fprintf(stderr, "ERROR: handle_group_chat (socket_disconnect)\n");
    return FAILURE;
  }

  return SUCCESS;
}

int send_welcome_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "New user is joined...\n");
  return socket_send(s, buffer, BUFFER_SIZE);
}

int send_goodbye_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "User is disconnected...\n");
  return socket_send(s, buffer, BUFFER_SIZE);
}
