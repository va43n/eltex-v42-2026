#include "group_chat.h"

volatile int is_signal = FALSE;

void handle_SIGINT(int sig) {is_signal = sig;}
void handle_any_signal(int sig) { is_signal = sig; }