#include "taxi.h"

int create_pids_array(pids *p) {
  p->actual_len = START_BUFFER_SIZE;
  p->len = 0;
  p->pids = (pid_t *)malloc(p->actual_len * sizeof(pid_t));
  if (p->pids == NULL) {
    fprintf(stderr, "ERROR: create_pids_array - cannot allocate memory.\n");
    return FAILURE;
  }

  return SUCCESS;
}

int add_pid_to_array(pids *p, pid_t pid) {
  p->len++;
  if (p->len > p->actual_len) {
    p->actual_len = p->len + START_BUFFER_SIZE;
    pid_t *tmp = (pid_t *)realloc(p->pids, p->len * sizeof(pid_t));
    if (tmp == NULL) {
      fprintf(stderr, "ERROR: add_pid_to_array - cannot reallocate memory.\n");
      free_pids_array(*p);
      return FAILURE;
    }
    p->pids = tmp;
  }

  p->pids[p->len - 1] = pid;

  return SUCCESS;
}

int find_pid_in_array(pids p, pid_t pid, size_t *pos) {
  for (size_t i = 0; i < p.len; i++) {
    if (p.pids[i] == pid) {
      *pos = i;
      return SUCCESS;
    }
  }

  return FAILURE;
}

int remove_pid_from_array(pids p, size_t pos) {
  kill(p.pids[pos], SIGKILL);
  for (size_t i = pos + 1; i < p.len; i++) {
    p.pids[i - 1] = p.pids[i];
  }

  return SUCCESS;
}

int free_pids_array(pids p) {
  for (size_t i = 0; i < p.len; i++) {
    kill(p.pids[i], SIGKILL);
  }
  free(p.pids);
  if (p.pids == NULL) {
    fprintf(stderr, "ERROR: free_pids_array - cannot free array.\n");
    return FAILURE;
  }
  return SUCCESS;
}