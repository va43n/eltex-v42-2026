#include "echo_reply.h"

int do_client_activity(char *client_address, char *server_address,
                       int client_port, int server_port) {
  printf("server: %s:%d;\nyou:    %s:%d\n\n", server_address, server_port,
         client_address, client_port);

  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  char data[BUFFER_SIZE];

  fd_set rfds, rfds_start;
  int maxfd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(fd, &rfds_start);

  printf("Send message:\n");
  while (is_signal == FALSE) {
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
      if (send_data(fd, NULL, 0, client_address, server_address, client_port,
                    server_port) == FAILURE)
        break;
    }

    if (FD_ISSET(fd, &rfds)) {
      int res = receive_data(fd, data, server_address, client_address,
                             &server_port, &client_port);
      if (res == FAILURE)
        break;
      else if (res == SUCCESS) {
        printf("data: %s\n", data);
      }
    }
  }

  return close_socket(fd);
}