#include "echo_reply.h"

int do_client_activity(char *source_address, char *destination_address,
                       int port) {
  printf("server: %s:%d;\nyou:    %s:%d\n\n", destination_address, SERVER_PORT,
         source_address, port);

  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  char data[BUFFER_SIZE];
  int server_port = SERVER_PORT;

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
      if (send_data(fd, NULL, 0, source_address, destination_address, port,
                    server_port) == FAILURE)
        break;
    }

    if (FD_ISSET(fd, &rfds)) {
      int res = receive_data(fd, data, destination_address, source_address,
                             &server_port, &port);
      if (res == FAILURE)
        break;
      else if (res == SUCCESS) {
        printf("data: %s\n", data);
      }
    }
  }

  close(fd);

  return SUCCESS;
}