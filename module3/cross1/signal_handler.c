#include "echo_reply.h"

void setup_signal_handler(void (*func)(int), int *signals,
                          size_t number_of_signals) {
  struct sigaction sa;
  sa.sa_handler = func;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  for (size_t i = 0; i < number_of_signals; i++) {
    sigaction(signals[i], &sa, NULL);
  }
}

void handle_SIGINT(int sig) { is_signal = sig; }