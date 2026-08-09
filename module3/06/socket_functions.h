#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#define SUCCESS 111
#define FAILURE -111

#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 256

static volatile int is_signal = FALSE;

typedef struct {
  int s;
  struct sockaddr_in recv_addr;
  struct sockaddr_in broadcast_addr;
} my_socket;

#define PORT 12345

// socket_functions.c
int socket_connect(my_socket* s);
int socket_disconnect(my_socket s);

int socket_send(my_socket s);
int socket_receive(my_socket s);
