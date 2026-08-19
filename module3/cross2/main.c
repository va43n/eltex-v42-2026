#include "taxi.h"

int main() {
  pids p;

  if (create_pids_array(&p, getpid()) == FAILURE) return 0;

  int signals[] = {SIGINT};
  setup_signal_handler(handle_signal, signals, 1);
  while (!is_signal) {
    int res = perform_command(&p);
    if (res != SUCCESS && res != WRONG_COMMAND) break;
  }

  free_pids_array(p);

  return 0;
}