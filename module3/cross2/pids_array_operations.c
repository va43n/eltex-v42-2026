#include "taxi.h"

int create_pids_array(pids *p, pid_t ppid) {
  p->actual_len = START_BUFFER_SIZE;
  p->len = 0;
  p->parent = ppid;
  p->pids = (pid_t *)malloc(p->actual_len * sizeof(pid_t));
  p->mqs = (mqd_t **)malloc(p->actual_len * sizeof(mqd_t *));
  for (size_t i = 0; i < p->actual_len; i++) {
    p->mqs[i] = (mqd_t *)malloc(2 * sizeof(mqd_t));
  }

  return SUCCESS;
}

int add_pid_to_array(pids *p, pid_t pid, mqd_t mq1, mqd_t mq2) {
  p->len++;
  if (p->len > p->actual_len) {
    p->actual_len = p->len + START_BUFFER_SIZE;
    pid_t *tmp = (pid_t *)realloc(p->pids, p->actual_len * sizeof(pid_t));
    p->pids = tmp;

    mqd_t **tmp_2 = (mqd_t **)realloc(p->mqs, p->actual_len * sizeof(mqd_t *));
    p->mqs = tmp_2;
    for (size_t i = p->len - 1; i < p->actual_len; i++) {
      p->mqs[i] = (mqd_t *)malloc(2 * sizeof(mqd_t));
    }
  }

  p->pids[p->len - 1] = pid;
  p->mqs[p->len - 1][0] = mq1;
  p->mqs[p->len - 1][1] = mq2;

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

// int remove_pid_from_array(pids p, size_t pos) {
//   kill(p.pids[pos], SIGINT);
//   for (size_t i = pos + 1; i < p.len; i++) {
//     p.pids[i - 1] = p.pids[i];
//     p.mqs[i - 1][0] = p.mqs[i][0];
//     p.mqs[i - 1][1] = p.mqs[i][1];
//   }

//   return SUCCESS;
// }

int free_pids_array(pids p) {
  for (size_t i = 0; i < p.len; i++) {
    kill(p.pids[i], SIGINT);
    queue_disconnect(p.mqs[i][0]);
    queue_disconnect(p.mqs[i][1]);
    queue_delete(p.parent, p.pids[i]);
  }
  free(p.pids);

  for (size_t i = 0; i < p.actual_len; i++) {
    free(p.mqs[i]);
  }
  free(p.mqs);

  return SUCCESS;
}