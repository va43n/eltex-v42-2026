#include <poll.h>
#include <signal.h>

#include "constants.h"
#include "socket_functions.h"

#define SERVER 's'
#define CLIENT 'c'

// signal_handler.c
void handle_any_signal(int sig);
void handle_SIGINT(int sig);

// parser.c
int parse_input(int argc, char* argv[], char* mode, char** address);
int parse_flag(char* flag, char* mode);
int parse_server_address(char* address);

// client.c
int do_client_activity(char* address);
int send_welcome_message(my_socket s);
int send_goodbye_message(my_socket s);
int send_part_of_file(my_socket s, char* file_name, ssize_t* bytes,
                      int* is_file_ended, int* sending_the_file);

// server.c
int do_server_activity();
void add_client(char*** addresses, my_socket** client_fds, struct pollfd** fds,
                size_t* nfds, size_t* actual_size, char* address,
                int client_fd);
void remove_client(char*** addresses, my_socket** client_fds,
                   struct pollfd** fds, size_t* nfds, size_t index);
void free_clients(char** addresses, my_socket* client_fds, struct pollfd* fds,
                  size_t nfds);