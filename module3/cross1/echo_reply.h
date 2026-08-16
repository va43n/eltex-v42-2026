#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#include "lib/raw_socket_operations.h"

#define SERVER 's'
#define CLIENT 'c'

static volatile int is_signal = FALSE;

// parser.c
int parse_input(int argc, char *argv[], char *mode, char *source_address,
                char *destination_address, uint16_t *source_port);
int parse_flag(const char *const flag, char *mode);
int check_address(const char *const address);
int get_source_address(char *address);
int parse_port(uint16_t *port_int, const char *const port);

// signal_handler.c
void setup_signal_handler(void (*func)(int), int *signals,
                          size_t number_of_signals);
void handle_SIGINT(int sig);

// server.c
int do_server_activity(char *server_address, uint16_t server_port);
void build_server_response(char *data, size_t message_counter);

// client.c
int do_client_activity(char *client_address, char *server_address,
                       uint16_t client_port, uint16_t server_port);
int wait_for_something_to_select(fd_set *fds, int max_fd);
