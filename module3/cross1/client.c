#include "echo_reply.h"

int do_client_activity(char *client_address, char *server_address,
                       uint16_t client_port, uint16_t server_port) {
  printf("server: %s:%d;\nyou:    %s:%d\n\n", server_address, server_port,
         client_address, client_port);

  int signals[] = {SIGINT};
  setup_signal_handler(handle_SIGINT, signals, 1);

  int fd;
  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  fd_set fds, fds_start;
  int max_fd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds_start);
  FD_SET(fd, &fds_start);

  char data[BUFFER_SIZE];
  printf("Send message:\n");
  while (is_signal == FALSE) {
    fds = fds_start;
    if (wait_for_something_to_select(&fds, max_fd) == FAILURE)
      break;

    if (FD_ISSET(STDIN_FILENO, &fds)) {
      if (send_data(fd, NULL, 0, client_address, server_address, client_port,
                    server_port) == FAILURE)
        break;
    }

    if (FD_ISSET(fd, &fds)) {
      int res = receive_data(fd, data, server_address, client_address,
                             &server_port, &client_port);
      if (res == FAILURE)
        break;
      else if (res == SUCCESS) {
        printf("%s\n", data);
      }
    }
  }

  return close_socket(fd);
}

int wait_for_something_to_select(fd_set *fds, int max_fd) {
  int ret = select(max_fd, fds, NULL, NULL, NULL);
  if (ret == -1) {
    if (errno == EINTR) {
      fprintf(stderr,
              "wait_for_something_to_select - interrupted by signal.\n");
      return FAILURE;
    }
    fprintf(stderr, "ERROR: wait_for_something_to_select (select).\n");
    perror("select");
    return FAILURE;
  }
  return SUCCESS;
}