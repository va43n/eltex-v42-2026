#include <sys/select.h>

#include "group_chat.h"

int do_client_activity(char* address) {
  my_socket s;

  struct sigaction sa;
  sa.sa_handler = handle_any_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTSTP, &sa, NULL);

  printf("I'm a client and the server is: %s\n", address);

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

  int waiting_for_file_name = FALSE;

  while (is_signal != SIGINT) {
    if (is_signal == SIGTSTP) {
      if (waiting_for_file_name == TRUE) {
        printf("File sending is stopped\n");
        waiting_for_file_name = FALSE;
      }
      else {
        printf("Enter file name: ");
        waiting_for_file_name = TRUE;
      }
      is_signal = FALSE;
    }

    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        // is_signal = SIGINT;
        continue;
      }
      fprintf(stderr, "ERROR: do_client_activity (select).\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (socket_send(s, NULL, 0, TEXT) == FAILURE) break;
    }

    if (FD_ISSET(s.s, &rfds)) {
      message msg;
      if (socket_receive(s.s, &msg) == FAILURE) break;
      printf("%s", msg.m);
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

int send_welcome_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "New user is joined...\n");
  return socket_send(s, buffer, BUFFER_SIZE, CONNECT);
}

int send_goodbye_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "User is disconnected...\n");
  return socket_send(s, buffer, BUFFER_SIZE, DISCONNECT);
}
