#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define PATHNAME "manufacturer"
#define PROJ_ID 124

#define MANUFACTURER 'm'
#define CONSUMER 'c'

static volatile int is_signal = 0;

// parser.c
int parse_input(int argc, char *argv[], char *mode);
int parse_flag(char *flag, char *mode);
int start_corresponding_mode(char mode);

// manufacturer.c
int do_manufacturer_activity();

// consumer.c
int do_consumer_activity();

// signal.handler.c
void handle_SIGINT(int sig);
