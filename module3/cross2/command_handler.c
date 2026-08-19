#include "taxi.h"

int perform_command(pids* p) {
  int (*commands[])(pids*, int, char[][BUFFER_SIZE]) = COMMANDS_ARRAY;
  char input_buffer[BUFFER_SIZE];
  char tokens[MAX_TOKENS_NUMBER][BUFFER_SIZE];
  int number_of_tokens = MAX_TOKENS_NUMBER, command_position;

  if (get_input(input_buffer) == FAILURE) return FAILURE;
  tokenize_input(input_buffer, &number_of_tokens, tokens);
  if (define_command(tokens[0], &command_position) == FAILURE) return FAILURE;

  return commands[command_position](p, number_of_tokens, tokens);
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

int create_driver(pids* p, int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 1) {
    fprintf(
        stderr,
        "ERROR: create_driver - this command doesn't imply any parameters.\n");
    return FAILURE;
  }

  printf("%s: Creating the driver...\n", argv[0]);

  pid_t child = fork();
  if (child == 0) {
    do_child_process_activity(p->parent);
  } else {
    mqd_t mq1, mq2;
    queue_create(p->parent, child);
    queue_connect(&mq1, p->parent, child, 1, O_RDONLY);
    queue_connect(&mq2, p->parent, child, 2, O_WRONLY);
    printf("The driver is created with pid (%u)\n\n", child);

    if (add_pid_to_array(p, child, mq1, mq2) == FAILURE) return FAILURE;
  }

  return SUCCESS;
}

int send_task(pids* p, int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 3) {
    fprintf(stderr,
            "ERROR: send_task - there should be 2 parameters in the "
            "input:\n\tdriver's pid (uint32_t)\n\ttask timer (uint32_t)\n");
    return FAILURE;
  }

  unsigned int child, task_timer;
  if (parse_str_to_uint(argv[1], &child) == FAILURE) return FAILURE;
  if (parse_str_to_uint(argv[2], &task_timer) == FAILURE) return FAILURE;

  printf("%s: Sending task that takes (%u) seconds to driver (%u)...\n",
         argv[0], task_timer, child);

  size_t pos;
  if (find_pid_in_array(*p, child, &pos) == FAILURE) {
    printf("Driver (%u) is not found...\n", child);
  } else {
    unsigned int priority;
    char msg[BUFFER_SIZE];
    strcpy(msg, argv[2]);
    queue_send_message(p->mqs[pos][1], msg, PARENT_PRIORITY);
    queue_recv_message(p->mqs[pos][0], msg, &priority);

    printf("%s\n\n", msg);
  }

  return SUCCESS;
}

int get_status(pids* p, int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 2) {
    fprintf(stderr,
            "ERROR: get_status - there should be 1 parameter in the "
            "input:\n\tdriver's pid (uint32_t)\n");
    return FAILURE;
  }

  unsigned int child;
  parse_str_to_uint(argv[1], &child);

  printf("%s: Getting status from driver (%u)...\n", argv[0], child);

  size_t pos;
  if (find_pid_in_array(*p, child, &pos) == FAILURE) {
    printf("Driver (%u) is not found...\n", child);
  } else {
    unsigned int priority;
    char msg[BUFFER_SIZE];
    strcpy(msg, "0");
    queue_send_message(p->mqs[pos][1], msg, PARENT_PRIORITY);
    queue_recv_message(p->mqs[pos][0], msg, &priority);

    printf("%s\n\n", msg);
  }

  return SUCCESS;
}

int get_drivers(pids* p, int argc, char argv[][BUFFER_SIZE]) {
  if (argc != 1) {
    fprintf(
        stderr,
        "ERROR: get_drivers - this command doesn't imply any parameters.\n");
    return FAILURE;
  }

  printf("%s: Getting the drivers...\n", argv[0]);

  for (size_t i = 0; i < p->len; i++) {
    printf("%zu. Driver (%u) ", i + 1, p->pids[i]);

    unsigned int priority = CHILD_PRIORITY;
    char msg[BUFFER_SIZE];
    strcpy(msg, "0");
    queue_send_message(p->mqs[i][1], msg, PARENT_PRIORITY);
    queue_recv_message(p->mqs[i][0], msg, &priority);

    printf("%s\n", msg);
  }
  printf("\n");

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
