#include "echo_reply.h"

int do_server_activity(unsigned int source_address) {
  printf("you are server: %x:%d\n\n", source_address, SERVER_PORT);

  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_socket(&fd) == FAILURE)
    return FAILURE;

  printf("Packet receiving is started...\n");
  while (is_signal == FALSE) {
    if (receive_data(fd, 0, source_address) == FAILURE)
      break;
  }

  return SUCCESS;
}