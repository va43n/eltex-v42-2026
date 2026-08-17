#include <errno.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define INTERRUPTION 110
#define FAILURE -111

#define BUFFER_SIZE 1024
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
int perform_command();
int define_command(char* command, int* pos);
int create_driver(int argc, char argv[][BUFFER_SIZE]);
int send_task(int argc, char argv[][BUFFER_SIZE]);
int get_status(int argc, char argv[][BUFFER_SIZE]);
int get_drivers(int argc, char argv[][BUFFER_SIZE]);
int parse_str_to_uint(char* str, unsigned int* number);