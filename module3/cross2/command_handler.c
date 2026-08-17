#include "taxi.h"

int perform_command() {
  int (*commands[])(int, char[][BUFFER_SIZE]) = COMMANDS_ARRAY;
  char input_buffer[BUFFER_SIZE];
  char tokens[MAX_TOKENS_NUMBER][BUFFER_SIZE];
  int number_of_tokens = MAX_TOKENS_NUMBER, command_position;

  if (get_input(input_buffer) == FAILURE) return FAILURE;
  tokenize_input(input_buffer, &number_of_tokens, tokens);
  if (define_command(tokens[0], &command_position) == FAILURE) return FAILURE;

  return commands[command_position](number_of_tokens, tokens);
}

int define_command(char* command, int* pos) {
  if (strcmp(command, CREATE_DRIVER) == 0)
    *pos = CREATE_DRIVER_POS;
  else if (strcmp(command, SEND_TASK) == 0)
    *pos = SEND_TASK_POS;
  else if (strcmp(command, GET_STATUS) == 0)
    *pos = GET_STATUS_POS;
  else if (strcmp(command, GET_DRIVERS) == 0)
    *pos = GET_DRIVERS_POS;
  else {
    fprintf(stderr, "ERROR: define_command - command not found.\n");
    *pos = COMMAND_NOT_FOUND;
    return FAILURE;
  }

  return SUCCESS;
}

int create_driver(int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 1) {
    fprintf(
        stderr,
        "ERROR: create_driver - this command doesn't imply any parameters.\n");
    return FAILURE;
  }

  printf("%s: Creating the driver...\n", argv[0]);

  return SUCCESS;
}

int send_task(int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 3) {
    fprintf(stderr,
            "ERROR: send_task - there should be 2 parameters in the "
            "input:\n\tdriver's pid (uint32_t)\n\ttask timer (uint32_t)\n");
    return FAILURE;
  }

  unsigned int pid, task_timer;
  if (parse_str_to_uint(argv[1], &pid) == FAILURE) return FAILURE;
  if (parse_str_to_uint(argv[2], &task_timer) == FAILURE) return FAILURE;

  printf("%s: Sending task to driver (%u) that takes (%u) seconds...\n",
         argv[0], pid, task_timer);

  return SUCCESS;
}

int get_status(int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 2) {
    fprintf(stderr,
            "ERROR: get_status - there should be 1 parameter in the "
            "input:\n\tdriver's pid (uint32_t)\n");
    return FAILURE;
  }

  unsigned int pid;
  parse_str_to_uint(argv[1], &pid);

  printf("%s: Getting status from driver (%u)...\n", argv[0], pid);

  return SUCCESS;
}

int get_drivers(int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 1) {
    fprintf(
        stderr,
        "ERROR: get_drivers - this command doesn't imply any parameters.\n");
    return FAILURE;
  }

  printf("%s: Getting the drivers...\n", argv[0]);

  return SUCCESS;
}

int parse_str_to_uint(char* str, unsigned int* number) {
  int result = sscanf(str, "%u", number);

  if (result == 0) {
    fprintf(stderr, "ERROR: parse_str_to_uint - %s in not a uint.\n", str);
    return FAILURE;
  }
  if (str[0] == '-') {
    fprintf(stderr, "ERROR: parse_str_to_uint - %s is a negative number.\n",
            str);
    return FAILURE;
  }

  return SUCCESS;
}