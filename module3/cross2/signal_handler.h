#include <signal.h>
#include <stdlib.h>

#include "constants.h"

extern volatile int is_signal;

void setup_signal_handler(void (*func)(int), int *signals,
                          size_t number_of_signals);
void handle_signal(int sig);