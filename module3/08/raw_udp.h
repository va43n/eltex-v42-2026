#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024

static volatile int is_signal = FALSE;

// raw_udp.c
int search_for_udp();
int create_raw_socket(int* fd);
int receive_data(int fd, time_t start);
void check_number_of_bytes_in_line(int* bytes);
void print_bytes_char(unsigned char* buffer, size_t* start_pos,
                      size_t number_of_bytes, int* bytes);
char* print_ip(char* ip, int ip_int);

// signal_handler.c
void handle_SIGINT(int sig);