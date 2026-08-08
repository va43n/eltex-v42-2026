#ifndef FACTORY_H
#define FACTORY_H

#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "item.h"
#include "shared_memory_operations.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define MANUFACTURER 'm'
#define CONSUMER 'c'

static volatile int is_signal = 0;

// parser.c
int parse_input(int argc, char *argv[], char *mode);
int parse_flag(char *flag, char *mode);
int start_corresponding_mode(char mode);

// manufacturer.c
int do_manufacturer_activity();
int manufacturer_fill_item(item *my_item);
int manufacturer_check_if_item_is_processed(item *my_item);

// consumer.c
int do_consumer_activity();
int consumer_process_item(item *my_item);

// signal.handler.c
void handle_SIGINT(int sig);

#endif