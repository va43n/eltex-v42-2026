#include "factory.h"

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char mode;
  if (parse_input(argc, argv, &mode) == SUCCESS) {
    if (start_corresponding_mode(mode) == SUCCESS) {
      printf("Disconnected\n");
    }
  }

  return 0;
}