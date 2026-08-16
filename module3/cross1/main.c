#include "echo_reply.h"

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char mode;
  char source_address[INET_ADDRSTRLEN], destination_address[INET_ADDRSTRLEN];
  uint16_t port;
  if (parse_input(argc, argv, &mode, source_address, destination_address,
                  &port) == FAILURE)
    return FAILURE;

  int result = 0;
  if (mode == CLIENT)
    result = do_client_activity(source_address, destination_address, port,
                                SERVER_PORT);
  else if (mode == SERVER)
    result = do_server_activity(source_address, SERVER_PORT);

  if (result != FAILURE)
    printf("Disconnected succesfully\n");
  else
    printf("Something went wrong in this session...\n");

  return 0;
}