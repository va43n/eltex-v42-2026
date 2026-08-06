#include "publisher_broker_subscriber.h"

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);

  char mode;

  if (parse_input(argc, argv, &mode) == SUCCESS) {
    if (start_corresponding_mode(mode) == SUCCESS) {
      printf("Disconnected\n");
    } else {
      printf("Some error occured\n");
    }
  }

  return 0;
}