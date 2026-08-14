#include "echo_reply.h"

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char mode;
  unsigned int destination_address, source_address;
  int port;
  if (parse_input(argc, argv, &mode, &destination_address, &source_address,
                  &port) == FAILURE)
    return FAILURE;

  int result;
  if (mode == CLIENT)
    result = do_client_activity(destination_address, source_address, port);
  else if (mode == SERVER)
    result = do_server_activity(source_address);

  if (result == SUCCESS)
    printf("Disconnected\n");
  else
    printf("Something went wrong in this group chat session...\n");

  return 0;
}