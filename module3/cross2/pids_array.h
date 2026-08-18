#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.h"

typedef struct {
  pid_t *pids;
  size_t len;
  size_t actual_len;
} pids;

#define START_BUFFER_SIZE 10

int create_pids_array(pids *p);
int add_pid_to_array(pids *p, pid_t pid);
int find_pid_in_array(pids p, pid_t pid, size_t *pos);
int remove_pid_from_array(pids p, size_t pos);
int free_pids_array(pids p);