#include "publisher_broker_subscriber.h"

int main(int argc, char *argv[]) {
  char mode;

  if (parse_input(argc, argv, &mode) == SUCCESS) {
    if (start_corresponding_mode(mode) == SUCCESS) {
      //
    } else {
      //
    }
  }

  return 0;
}