#include <sys/epoll.h>
#include <sys/timerfd.h>

#include "taxi.h"

#define MAX_EVENTS 2

int is_signal = FALSE;

void do_child_process_activity(pid_t parent) {
  char buffer[BUFFER_SIZE];
  mqd_t rd, wr;
  unsigned int priority;
  pid_t pid = getpid();

  int tfd;
  if (create_timer(&tfd) == FAILURE) _exit(EXIT_FAILURE);

  int epollfd = epoll_create1(0);
  struct epoll_event ev, events[MAX_EVENTS];

  ev.events = EPOLLIN;
  ev.data.fd = tfd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, tfd, &ev);

  while (queue_check_if_created(parent, pid) == FALSE);
  queue_connect(&rd, parent, pid, 2, O_RDONLY);
  queue_connect(&wr, parent, pid, 1, O_WRONLY);

  ev.events = EPOLLIN;
  ev.data.fd = rd;
  epoll_ctl(epollfd, EPOLL_CTL_ADD, rd, &ev);

  while (!is_signal) {
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == rd) {
        queue_recv_message(rd, buffer, &priority);

        double time_remaining;
        if (check_timer(tfd, &time_remaining) == FAILURE) {
          is_signal = TRUE;
          break;
        }

        if (time_remaining > 0) {
          memset(buffer, 0, BUFFER_SIZE);
          sprintf(buffer, "%s %lf", STATUS_BUSY, time_remaining);
          queue_send_message(wr, buffer, CHILD_PRIORITY);
        } else {
          unsigned int seconds = atoi(buffer);
          memset(buffer, 0, BUFFER_SIZE);
          if (seconds == 0) {
            strcpy(buffer, STATUS_AVAILABLE);
          } else {
            if (start_timer(tfd, seconds) == FAILURE) {
              is_signal = TRUE;
              break;
            }

            strcpy(buffer, "Ok");
          }
          queue_send_message(wr, buffer, CHILD_PRIORITY);
        }
      } else if (events[i].data.fd == tfd) {
        uint64_t expr;
        ssize_t s = read(tfd, &expr, sizeof(expr));
        if (s == sizeof(expr)) {
          printf("%u is available again\n", pid);
        }
      }
    }
  }

  printf("I'm stopping.\n");
  close(epollfd);
  close(tfd);
  close(rd);
  close(wr);

  _exit(EXIT_SUCCESS);
}

int create_timer(int* tfd) {
  *tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
  if (*tfd == -1) {
    fprintf(stderr, "Cannot create timer.\n");
    perror("timerfd_create");
    return FAILURE;
  }

  return SUCCESS;
}

int start_timer(int tfd, unsigned int seconds) {
  struct itimerspec its;
  its.it_value.tv_sec = seconds;
  its.it_value.tv_nsec = 0;
  its.it_interval.tv_sec = 0;
  its.it_interval.tv_nsec = 0;

  if (timerfd_settime(tfd, 0, &its, NULL) == -1) {
    fprintf(stderr, "ERROR: start_timer - cannot start timer.\n");
    perror("timerfd_settime");
    return FAILURE;
  }

  return SUCCESS;
}

int check_timer(int tfd, double* time_remaining) {
  struct itimerspec its;

  if (timerfd_gettime(tfd, &its) == -1) {
    printf("ERROR: check_timer - cannot check time.\n");
    perror("timerfd_gettime");
    return FAILURE;
  }

  *time_remaining = its.it_value.tv_sec + its.it_value.tv_nsec / 1000000000.0;

  return SUCCESS;
}