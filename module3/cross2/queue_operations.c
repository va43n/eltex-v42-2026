#include "queue_operations.h"

#include <mqueue.h>

#define QUEUE_NAME "/drivers"

int queue_check_if_created(pid_t ppid, pid_t pid) {
  char buffer[BUFFER_SIZE];
  _create_actual_queue_name(buffer, ppid, pid, 1);

  mqd_t mq = mq_open(buffer, O_RDONLY);
  if (mq == (mqd_t)-1) {
    return FALSE;
  }
  mq_close(mq);

  _create_actual_queue_name(buffer, ppid, pid, 2);
  mq = mq_open(buffer, O_RDONLY);
  if (mq == (mqd_t)-1) {
    return FALSE;
  }
  mq_close(mq);

  return TRUE;
}

int queue_create(pid_t ppid, pid_t pid) {
  char buffer[BUFFER_SIZE];
  _create_actual_queue_name(buffer, ppid, pid, 1);

  mqd_t mq;
  struct mq_attr queue_attr;
  queue_attr.mq_flags = 0;
  queue_attr.mq_maxmsg = 10;
  queue_attr.mq_msgsize = BUFFER_SIZE;
  queue_attr.mq_curmsgs = 0;

  if ((mq = mq_open(buffer, O_CREAT | O_RDWR, 0644, &queue_attr)) ==
      (mqd_t)-1) {
    fprintf(stderr, "ERROR: queue_create - some error occured 1.\n");
    perror("mq_open");
    return FAILURE;
  }
  mq_close(mq);

  _create_actual_queue_name(buffer, ppid, pid, 2);
  if ((mq = mq_open(buffer, O_CREAT | O_RDWR, 0644, &queue_attr)) ==
      (mqd_t)-1) {
    fprintf(stderr, "ERROR: queue_create - some error occured 2.\n");
    perror("mq_open");
    return FAILURE;
  }
  mq_close(mq);

  return SUCCESS;
}

int queue_connect(mqd_t *mq, pid_t ppid, pid_t pid, int number, int mode) {
  char buffer[BUFFER_SIZE];
  _create_actual_queue_name(buffer, ppid, pid, number);
  *mq = mq_open(buffer, mode);

  return SUCCESS;
}

int queue_recv_message(mqd_t mq, char *buffer, unsigned int *priority) {
  memset(buffer, 0, BUFFER_SIZE);
  ssize_t bytes = mq_receive(mq, buffer, BUFFER_SIZE, priority);
  if (bytes == -1) {
    fprintf(stderr, "ERROR: queue_recv_message - some error occured.\n");
    perror("mq_receive");
    return FAILURE;
  }

  return SUCCESS;
}

int queue_send_message(mqd_t mq, char *buffer, unsigned int priority) {
  if (mq_send(mq, buffer, BUFFER_SIZE, priority) == -1) {
    fprintf(stderr, "ERROR: queue_send_message - some error occured.\n");
    perror("mq_send");
    return FAILURE;
  }
  return SUCCESS;
}

int queue_disconnect(mqd_t mq) {
  mq_close(mq);

  return SUCCESS;
}

int queue_delete(pid_t ppid, pid_t pid) {
  char buffer[BUFFER_SIZE];

  _create_actual_queue_name(buffer, ppid, pid, 1);
  mq_unlink(buffer);

  _create_actual_queue_name(buffer, ppid, pid, 2);
  mq_unlink(buffer);

  return SUCCESS;
}

void _create_actual_queue_name(char *buffer, pid_t parent, pid_t child,
                               int number) {
  memset(buffer, 0, BUFFER_SIZE);
  snprintf(buffer, BUFFER_SIZE, "%s_%u_%u_%d", QUEUE_NAME, parent, child,
           number);
}