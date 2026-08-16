#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "constants.h"

#define LOCALHOST_STR "127.0.0.1"
#define ANY_ADDRESS_STR "0.0.0.0"

#define MESSAGE_TYPE_TEXT 'T'
#define MESSAGE_TYPE_DISCONNECT 'D'

#define SERVER_PORT 9876

typedef struct {
  char buffer[BUFFER_SIZE];
  struct sockaddr_in addr;
  size_t buffer_len;
  size_t addr_len;
} message;

int raw_socket_create(int *fd);
int raw_socket_receive_data(int fd, char *data, char *message_type,
                            char *source_address, char *destination_address,
                            uint16_t *source_port, uint16_t *destination_port);
int raw_socket_send_data(int fd, const char *const data, size_t len,
                         char message_type, char *source_address,
                         char *destination_address, uint16_t source_port,
                         uint16_t destination_port);
int raw_socket_close(int fd);

int _raw_socket_get_input(char *buffer, size_t *len);
char *_raw_socket_create_ip_string(char *ip, uint32_t ip_int);
message _raw_socket_build_message(const char *const message_text,
                                  char message_type, char *source_address,
                                  char *destination_address,
                                  uint16_t source_port,
                                  uint16_t destination_port);
int _raw_socket_filter_packets(const struct iphdr *const ip,
                               const struct udphdr *const udp,
                               char *source_address, char *destination_address,
                               uint16_t source_port, uint16_t destination_port);