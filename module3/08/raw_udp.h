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
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define BUFFER_SIZE 1024

#define PUT_IN_FILE 'f'
#define PUT_IN_FILE_MASK 0b100
#define FILENAME_SIZE 64

#define FILTER_06 '6'
#define FILTER_06_MASK 0b010
#define FILTER_06_PORT 12345

#define FILTER_DNS 'd'
#define FILTER_DNS_MASK 0b001

#define IPV4_LENGTH 15

static volatile int is_signal = FALSE;

// raw_udp.c
int search_for_udp(int mask);
int create_raw_socket(int* fd);
int receive_data(int fd, time_t start, int mask);
void check_number_of_bytes_in_line(FILE* file, int* bytes);
void print_bytes_char(FILE* file, unsigned char* buffer, size_t* start_pos,
                      size_t number_of_bytes, int* bytes);
char* create_ip_string(char* ip, int ip_int);

// parser.c
int parse_input(int argc, char* argv[], int* mask);
int parse_flag(char* flag, int* mask);

// signal_handler.c
void handle_SIGINT(int sig);