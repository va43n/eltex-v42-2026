#include <sys/epoll.h>

#include "taxi.h"

void do_child_process_activity(pid_t parent) {
  char buffer[BUFFER_SIZE];
  mqd_t mq;

  pid_t pid = getpid();

  unsigned int priority;

  while (queue_check_if_created(parent, pid) == FALSE);
  queue_connect(&mq, parent, pid);

  while (TRUE) {
    queue_recv_message(mq, buffer, &priority);
    printf("I'm a child and I get a message '%s' with priority %d\n", buffer,
           priority);
    memset(buffer, 0, BUFFER_SIZE);
    strcpy(buffer, STATUS_AVAILABLE);
    queue_send_message(mq, buffer, 5);
  }

  _exit(EXIT_SUCCESS);
}