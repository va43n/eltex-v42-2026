#include "group_chat.h"

int main(int argc, char* argv[]) {
  setbuf(stdout, NULL);

  char mode;
  if (parse_input(argc, argv, &mode) == FAILURE) return EXIT_FAILURE;

  if (handle_group_chat() == SUCCESS) {
    printf("Disconnected\n");
  } else {
    printf("Something went wrong in this group chat session...\n");
  }

  return EXIT_SUCCESS;
}