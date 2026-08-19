#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.h"
#include "queue_operations.h"

typedef struct {
  pid_t *pids;
  mqd_t *mqs;
  pid_t parent;
  size_t len;
  size_t actual_len;
} pids;

#define START_BUFFER_SIZE 10

int create_pids_array(pids *p, pid_t ppid);
int add_pid_to_array(pids *p, pid_t pid, mqd_t mq);
int find_pid_in_array(pids p, pid_t pid, size_t *pos);
int remove_pid_from_array(pids p, size_t pos);
int free_pids_array(pids p);