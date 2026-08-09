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

static volatile int is_signal = TRUE;

// socket_functions.c
int socket_connect();
int socket_disconnect();

int socket_send();
int socket_receive();

int socket_delete();