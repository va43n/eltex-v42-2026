#include "echo_reply.h"

int do_client_activity(char *client_address, char *server_address,
                       uint16_t client_port, uint16_t server_port) {
  printf("server: %s:%d;\nyou:    %s:%d\n\n", server_address, server_port,
         client_address, client_port);

  int signals[] = {SIGINT};
  setup_signal_handler(handle_SIGINT, signals, 1);

  int fd;
  if (raw_socket_create(&fd) == FAILURE)
    return FAILURE;

  int result = SUCCESS;

  fd_set fds, fds_start;
  int max_fd = (fd > STDIN_FILENO ? fd : STDIN_FILENO) + 1;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds_start);
  FD_SET(fd, &fds_start);

  char message_type;
  char data[BUFFER_SIZE];
  printf("Send message:\n");
  while (is_signal == FALSE) {
    fds = fds_start;
    if ((result = wait_for_something_to_select(&fds, max_fd)) != SUCCESS)
      break;

    if (FD_ISSET(STDIN_FILENO, &fds)) {
      if ((result = raw_socket_send_data(fd, NULL, 0, MESSAGE_TYPE_TEXT,
                                         client_address, server_address,
                                         client_port, server_port)) == FAILURE)
        break;
    }

    if (FD_ISSET(fd, &fds)) {
      result =
          raw_socket_receive_data(fd, data, &message_type, server_address,
                                  client_address, &server_port, &client_port);
      if (result == SUCCESS) {
        printf("%s\n", data);
      } else if (result != EMPTY)
        break;
    }
  }

  raw_socket_send_data(fd, "i'm leaving", 12, MESSAGE_TYPE_DISCONNECT,
                       client_address, server_address, client_port,
                       server_port);

  raw_socket_close(fd);

  return result;
}

int wait_for_something_to_select(fd_set *fds, int max_fd) {
  int ret = select(max_fd, fds, NULL, NULL, NULL);
  if (ret == -1) {
    if (errno == EINTR) {
      fprintf(stderr,
              "wait_for_something_to_select - interrupted by signal.\n");
      return INTERRUPTION;
    }
    fprintf(stderr, "ERROR: wait_for_something_to_select (select).\n");
    perror("select");
    return FAILURE;
  }
  return SUCCESS;
}