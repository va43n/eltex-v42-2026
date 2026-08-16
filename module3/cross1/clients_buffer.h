#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111
#define EMPTY -110

#define BUFFER_SIZE 1024

typedef struct {
  char address[INET_ADDRSTRLEN];
  uint16_t port;
  unsigned int message_counter;
} client;

typedef struct {
  client *cb;
  size_t len;
  size_t actual_len;
} clients_buffer;

#define START_BUFFER_SIZE 2

int create_clients_buffer(clients_buffer *cb);
client create_client(char *address, uint16_t port,
                     unsigned int message_counter);
int add_client_to_buffer(clients_buffer *cb, client c);
int find_client_in_buffer(clients_buffer cb, client c, size_t *pos);
int increment_one_of_the_clients(clients_buffer cb, size_t pos);
int remove_client_from_buffer(clients_buffer cb, size_t pos);
int free_clients_buffer(clients_buffer cb);