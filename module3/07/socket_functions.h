#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "constants.h"
#include "file_operations.h"

#define CONNECT 35
#define DISCONNECT 36
#define FILE 37
#define TEXT 38

#define SERVER_USERS 5
#define IPV4_STR_LENGTH 15
#define FD_LENGTH 9

extern volatile int is_signal;
extern volatile int is_sending_file;

typedef struct {
  int s;
  struct sockaddr_in server_addr;
} my_socket;

typedef struct {
  int mode;
  size_t data_len;
  char m[BUFFER_SIZE];
  char file_name[BUFFER_SIZE];
  struct sockaddr_in addr;
} message;

#define PORT 12345

// socket_functions.c
int socket_connect(my_socket* s, char* address);
int socket_listen(my_socket* s);
int socket_accept(my_socket s, char** address, int* client_fd);
int socket_disconnect(my_socket s);

int socket_send(my_socket s, char* buffer, size_t len, int message_type,
                char* file_name);
int socket_receive(my_socket s, message* msg);
