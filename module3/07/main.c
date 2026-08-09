#include "group_chat.h"

int main() {
  setbuf(stdout, NULL);

  if (handle_group_chat() == SUCCESS) {
    printf("Disconnected\n");
  } else {
    printf("Something went wrong in this group chat session...\n");
  }

  return 0;
}