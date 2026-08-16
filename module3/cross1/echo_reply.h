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

#define LOCALHOST_STR "127.0.0.1"
#define ANY_ADDRESS_STR "0.0.0.0"

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
                char *destination_address, uint16_t *source_port);
int parse_flag(char *flag, char *mode);
int check_address(char *address);
int get_source_address(char *address);
int parse_port(uint16_t *port_int, char *port);

// socket_operations.c
int create_socket(int *fd);
int receive_data(int fd, char *data, char *source_address,
                 char *destination_address, uint16_t *source_port,
                 uint16_t *destination_port);
int send_data(int fd, char *data, size_t len, char *source_address,
              char *destination_address, uint16_t source_port,
              uint16_t destination_port);
char *create_ip_string(char *ip, uint32_t ip_int);
message build_message(char *message_text, char *source_address,
                      char *destination_address, uint16_t source_port,
                      uint16_t destination_port);
int get_input(char *buffer, size_t *len);
int close_socket(int fd);
int filter_udp_packets(struct iphdr *ip, struct udphdr *udp,
                       char *source_address, char *destination_address,
                       uint16_t source_port, uint16_t destination_port);

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
