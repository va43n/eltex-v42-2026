#include "echo_reply.h"

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char mode;
  char source_address[IPV4_LENGTH], destination_address[IPV4_LENGTH];
  uint16_t port;
  if (parse_input(argc, argv, &mode, source_address, destination_address,
                  &port) == FAILURE)
    return FAILURE;

  int result;
  if (mode == CLIENT)
    result = do_client_activity(source_address, destination_address, port,
                                SERVER_PORT);
  else if (mode == SERVER)
    result = do_server_activity(source_address, SERVER_PORT);

  if (result == SUCCESS)
    printf("Disconnected\n");
  else
    printf("Something went wrong in this group chat session...\n");

  return 0;
}