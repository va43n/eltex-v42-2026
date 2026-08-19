#include "taxi.h"

int main() {
  pids p;

  if (create_pids_array(&p, getpid()) == FAILURE) return 0;

  while (TRUE) {
    if (perform_command(&p) == FAILURE) break;
  }

  free_pids_array(p);

  return 0;
}