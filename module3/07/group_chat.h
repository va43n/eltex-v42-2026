#include "socket_functions.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

// chat.c
int handle_group_chat();
int send_welcome_message(my_socket s);
int send_goodbye_message(my_socket s);

// signal_handler.c
void handle_SIGINT(int sig);