#include "echo_reply.h"

int do_server_activity(char *server_address, uint16_t server_port) {
  printf("you are the server: %s:%d\n\n", server_address, server_port);

  int signals[] = {SIGINT};
  setup_signal_handler(handle_SIGINT, signals, 1);

  int fd;
  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  char data[BUFFER_SIZE];
  uint16_t client_port;
  char client_address[INET_ADDRSTRLEN];

  printf("Packet receiving is started...\n");
  while (is_signal == FALSE) {
    client_port = 0;
    strcpy(client_address, ANY_ADDRESS_STR);

    int res = receive_data(fd, data, client_address, server_address,
                           &client_port, &server_port);
    if (res == FAILURE)
      break;
    else if (res == SUCCESS) {
      printf("%s:%d> %s\n", client_address, client_port, data);
      strcat(data, " hello!");
      if (send_data(fd, data, strlen(data), server_address, client_address,
                    server_port, client_port) == FAILURE)
        return FAILURE;
    }
  }

  return close_socket(fd);
}