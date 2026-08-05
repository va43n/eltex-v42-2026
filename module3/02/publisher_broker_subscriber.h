#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define PUBLISHER 'p'
#define BROKER 'b'
#define SUBSCRIBER 's'

#define SUCCESS 111
#define FAILURE -111

// parser.c
int parse_input(int argc, char *argv[], char *mode);
int parse_flag(char *flag, char *mode);
int start_corresponding_mode(char mode);

// publisher.c
int do_publisher_activity();

// broker.c
int do_broker_activity();

// subscriber.c
int do_subscriber_activity();
