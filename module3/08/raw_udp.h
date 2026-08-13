#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdio.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024
#define BYTE 8

static volatile int is_signal = FALSE;

// raw_udp.c
int search_for_udp();
int create_raw_socket(int* fd);
int receive_data(int fd);
void print_bytes(int value, int number_of_bits, int* bytes);
void check_number_of_bytes_in_line(int* bytes);

// signal_handler.c
void handle_SIGINT(int sig);