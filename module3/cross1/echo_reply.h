#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024
#define IPV4_LENGTH INET_ADDRSTRLEN

#define LOCALHOST_STR "127.0.0.1"

#define SERVER 's'
#define SERVER_PORT 9876

#define CLIENT 'c'

static volatile int is_signal = FALSE;

typedef struct {
  int source_port;
  char buffer[BUFFER_SIZE];
} message;

// parser.c
int parse_input(int argc, char *argv[], char *mode,
                unsigned int *destination_address, unsigned int *source_address,
                int *port);
int parse_flag(char *flag, char *mode);
int parse_address_from_str(char *address, unsigned int *address_int);
int get_source_address(unsigned int *address);
int parse_port(int *port_int, char *port);

// socket_operations.c
int create_socket(int *fd);
int receive_data(int fd, unsigned int destination_address,
                 unsigned int source_address);
int send_data(int fd, int destination_port, int source_port);
char *create_ip_string(char *ip, int ip_int);

// signal_handler.c
void handle_SIGINT(int sig);

// server.c
int do_server_activity(unsigned int source_address);

// client.c
int do_client_activity(unsigned int destination_address,
                       unsigned int source_address, int port);
