#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "constants.h"

#define STATUS_AVAILABLE "AVAILABLE"
#define STATUS_BUSY "BUSY"

#define PARENT_PRIORITY 5
#define CHILD_PRIORITY 10

// queue_operations.c
int queue_check_if_created(pid_t ppid, pid_t pid);
int queue_create(pid_t ppid, pid_t pid);
int queue_connect(mqd_t *mq, pid_t ppid, pid_t pid, int number, int mode);
int queue_recv_message(mqd_t mq, char *buffer, unsigned int *priority);
int queue_send_message(mqd_t mq, char *buffer, unsigned int priority);
int queue_disconnect(mqd_t mq);
int queue_delete(pid_t ppid, pid_t pid);

void _create_actual_queue_name(char *buffer, pid_t ppid, pid_t pid, int number);
