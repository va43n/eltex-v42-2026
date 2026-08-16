#include "clients_buffer.h"
#include "echo_reply.h"

int do_server_activity(char *server_address, uint16_t server_port) {
  printf("you are the server: %s:%d\n\n", server_address, server_port);

  int signals[] = {SIGINT};
  setup_signal_handler(handle_SIGINT, signals, 1);

  clients_buffer cb;
  if (create_clients_buffer(&cb) == FAILURE)
    return FAILURE;

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
      size_t pos;
      client c = create_client(client_address, client_port, 1);
      if (find_client_in_buffer(cb, c, &pos) == FAILURE) {
        if (add_client_to_buffer(&cb, c) == FAILURE)
          return FAILURE;
        pos = cb.len - 1;
      } else
        increment_one_of_the_clients(cb, pos);

      printf("%s:%d> '%s' (message #%d)\n", client_address, client_port, data,
             cb.cb[pos].message_counter);
      build_server_response(data, cb.cb[pos].message_counter);

      if (send_data(fd, data, strlen(data), server_address, client_address,
                    server_port, client_port) == FAILURE)
        return FAILURE;
    }
  }

  free_clients_buffer(cb);

  return close_socket(fd);
}

void build_server_response(char *data, size_t message_counter) {
  unsigned int number_of_digits = 0;
  size_t tmp = message_counter;
  while (tmp > 0) {
    tmp /= 10;
    number_of_digits++;
  }

  size_t len = strlen(data);
  len = len < BUFFER_SIZE - number_of_digits - 1
            ? len
            : BUFFER_SIZE - number_of_digits - 1;
  char new_data[BUFFER_SIZE];
  memset(new_data, 0, BUFFER_SIZE);

  sprintf(new_data, "%*s %ld", (int)len, data, message_counter);
  strcpy(data, new_data);
}
