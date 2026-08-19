#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "pids_array.h"

#define MAX_TOKENS_NUMBER 3

#define CREATE_DRIVER "create_driver"
#define CREATE_DRIVER_POS 0

#define SEND_TASK "send_task"
#define SEND_TASK_POS 1

#define GET_STATUS "get_status"
#define GET_STATUS_POS 2

#define GET_DRIVERS "get_drivers"
#define GET_DRIVERS_POS 3

#define COMMAND_NOT_FOUND -1

#define COMMANDS_ARRAY \
  { create_driver, send_task, get_status, get_drivers }

// user_input_operations.c
int get_input(char* buffer);
void tokenize_input(char* buffer, int* number_of_tokens,
                    char tokens[][BUFFER_SIZE]);

// command_handler.c
int perform_command(pids* p);
int define_command(char* command, int* pos);
int create_driver(pids* p, int argc, char argv[][BUFFER_SIZE]);
int send_task(pids* p, int argc, char argv[][BUFFER_SIZE]);
int get_status(pids* p, int argc, char argv[][BUFFER_SIZE]);
int get_drivers(pids* p, int argc, char argv[][BUFFER_SIZE]);
int parse_str_to_uint(char* str, unsigned int* number);

// child_process.c
void do_child_process_activity(pid_t parent);
int create_timer(int* tfd);
int start_timer(int tfd, unsigned int seconds);
int check_timer(int tfd, double* time_remaining);
