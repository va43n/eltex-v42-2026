#include "group_chat.h"

int main(int argc, char* argv[]) {
  setbuf(stdout, NULL);

  char mode, *address;
  if (parse_input(argc, argv, &mode, &address) == FAILURE) return EXIT_FAILURE;

  int result;
  if (mode == CLIENT)
    result = do_client_activity(address);
  else if (mode == SERVER)
    result = do_server_activity();

  if (result == SUCCESS)
    printf("Disconnected\n");
  else
    printf("Something went wrong in this group chat session...\n");

  return EXIT_SUCCESS;
}