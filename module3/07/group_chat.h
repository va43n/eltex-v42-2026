#include "socket_functions.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define SERVER 's'
#define CLIENT 'c'

// chat.c
int handle_group_chat();
int send_welcome_message(my_socket s);
int send_goodbye_message(my_socket s);

// signal_handler.c
void handle_SIGINT(int sig);

//parser.c 
int parse_input(int argc, char* argv[], char* mode);
int parse_flag(char* flag, char* mode);
int parse_server_address(char* address);