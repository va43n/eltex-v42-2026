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
#define EMPTY -110

#define BUFFER_SIZE 1024
#define IPV4_LENGTH INET_ADDRSTRLEN

#define LOCALHOST_STR "127.0.0.1"

#define SERVER 's'
#define SERVER_PORT 9876

#define CLIENT 'c'

static volatile int is_signal = FALSE;

typedef struct {
  char buffer[BUFFER_SIZE];
  struct sockaddr_in addr;
  size_t buffer_len;
  size_t addr_len;
} message;

// parser.c
int parse_input(int argc, char *argv[], char *mode, char *source_address,
                char *destination_address, int *port);
int parse_flag(char *flag, char *mode);
int check_address(char *address);
int get_source_address(char *address);
int parse_port(int *port_int, char *port);

// socket_operations.c
int create_socket(int *fd);
int receive_data(int fd, char *data, char *source_address,
                 char *destination_address, int *source_port,
                 int *destination_port);
int send_data(int fd, char *data, size_t len, char *source_address,
              char *destination_address, int source_port, int destination_port);
char *create_ip_string(char *ip, int ip_int);
message build_message(char *message_text, char *source_address,
                      char *destination_address, int source_port,
                      int destination_port);
int get_input(char *buffer, size_t *len);

// signal_handler.c
void handle_SIGINT(int sig);

// server.c
int do_server_activity(char *source_address);

// client.c
int do_client_activity(char *destination_address, char *source_address,
                       int port);
