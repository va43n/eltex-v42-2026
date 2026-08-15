#include "echo_reply.h"

int do_server_activity(char *server_address, int server_port) {
  printf("you are server: %s:%d\n\n", server_address, server_port);

  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  char data[BUFFER_SIZE];
  int client_port;
  char client_address[IPV4_LENGTH];

  printf("Packet receiving is started...\n");
  while (is_signal == FALSE) {
    client_port = 0;
    strcpy(client_address, ANY_ADDRESS_STR);
    int res = receive_data(fd, data, client_address, server_address,
                           &client_port, &server_port);
    if (res == FAILURE)
      break;
    else if (res == SUCCESS) {
      printf("data: %s\n", data);
      strcat(data, " hello!");
      if (send_data(fd, data, strlen(data), server_address, client_address,
                    server_port, client_port) == FAILURE)
        return FAILURE;
    }
  }

  return close_socket(fd);
}