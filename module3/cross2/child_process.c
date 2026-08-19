#include <sys/epoll.h>

#include "taxi.h"

void do_child_process_activity(pid_t parent) {
  char buffer[BUFFER_SIZE];
  mqd_t mq1, mq2;

  pid_t pid = getpid();

  unsigned int priority;

  while (queue_check_if_created(parent, pid) == FALSE);
  queue_connect(&mq1, parent, pid, 2, O_RDONLY);
  queue_connect(&mq2, parent, pid, 1, O_WRONLY);

  while (TRUE) {
    queue_recv_message(mq1, buffer, &priority);
    // printf("I'm a child and I got a message '%s' with priority %d\n", buffer,
    //  priority);
    memset(buffer, 0, BUFFER_SIZE);
    strcpy(buffer, STATUS_AVAILABLE);
    queue_send_message(mq2, buffer, CHILD_PRIORITY);
  }

  _exit(EXIT_SUCCESS);
}