#include "taxi.h"

int main() {
  while (TRUE) {
    if (perform_command() == FAILURE) break;
  }

  return 0;
}