#include "p2p.h"

int check_if_queue_is_created(char *queue_name, int *status) {
  char buffer1[SIZE], buffer2[SIZE];

  create_actual_queue_name(buffer1, queue_name, "_2");
  mqd_t mq = mq_open(buffer1, O_RDONLY);
  if (mq == (mqd_t)-1) {
    *status = CREATOR;
    return FALSE;
  }
  mq_close(mq);

  create_actual_queue_name(buffer2, queue_name, "_1");
  mq = mq_open(buffer2, O_RDONLY);
  if (mq == (mqd_t)-1) {
    mq_unlink(buffer1);
    *status = CREATOR;
    return FALSE;
  }
  mq_close(mq);

  *status = USER;
  return TRUE;
}

int create_queue(char *queue_name) {
  char buffer[SIZE];
  mqd_t mq;
  struct mq_attr queue_attr;
  queue_attr.mq_flags = 0;
  queue_attr.mq_maxmsg = 10;
  queue_attr.mq_msgsize = SIZE;
  queue_attr.mq_curmsgs = 0;

  create_actual_queue_name(buffer, queue_name, "_1");
  if ((mq = mq_open(buffer, O_CREAT | O_RDWR | O_NONBLOCK, 0644,
                    &queue_attr)) == (mqd_t)-1) {
    fprintf(stderr, "ERROR: CREATE_QUEUE - some error occured.\n");
    perror("Creating queue error");
    return FAILURE;
  }
  mq_close(mq);

  create_actual_queue_name(buffer, queue_name, "_2");
  if ((mq = mq_open(buffer, O_CREAT | O_RDWR | O_NONBLOCK, 0644,
                    &queue_attr)) == (mqd_t)-1) {
    fprintf(stderr, "ERROR: CREATE_QUEUE - some error occured.\n");
    perror("Creating queue error");
    return FAILURE;
  }
  mq_close(mq);

  return SUCCESS;
}

int connect_to_queue(char *queue_name, int status, mqd_t *read, mqd_t *write) {
  char buffer[SIZE];
  if (status == CREATOR) {
    create_actual_queue_name(buffer, queue_name, "_1");
    *read = mq_open(buffer, O_RDONLY | O_NONBLOCK);
    create_actual_queue_name(buffer, queue_name, "_2");
    *write = mq_open(buffer, O_WRONLY | O_NONBLOCK);
  } else {
    create_actual_queue_name(buffer, queue_name, "_2");
    *read = mq_open(buffer, O_RDONLY | O_NONBLOCK);
    create_actual_queue_name(buffer, queue_name, "_1");
    *write = mq_open(buffer, O_WRONLY | O_NONBLOCK);
  }

  return SUCCESS;
}

int start_handler(mqd_t read, mqd_t write) {
  signal(SIGINT, handle_SIGINT);

  fd_set rfds, rfds_start;
  int maxfd = (read > STDIN_FILENO ? read : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(read, &rfds_start);

  while (!is_signal) {
    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        is_signal = 1;
        continue;
      }
      fprintf(stderr, "ERROR: start_handler - some error occured while trying "
                      "to wait for some user activity.\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (get_message_and_send(write) == FAILURE)
        break;
    }

    if (FD_ISSET(read, &rfds)) {
      if (recv_message(read) == FAILURE)
        break;
    }
  }

  if (is_signal)
    send_message(write, "exit", SIGINT);

  return SUCCESS;
}

int recv_message(mqd_t read) {
  char buffer[SIZE];
  unsigned int priority;

  ssize_t bytes = mq_receive(read, buffer, SIZE, &priority);
  if (bytes == -1) {
    if (errno == EINTR) {
      is_signal = 1;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: RECV_MESSAGE - some error occured.\n");
    perror("cannot receive");
    return FAILURE;
  }
  buffer[bytes] = '\0';
  if (priority == SIGINT) {
    printf("...other user disconnected...\n");
    fflush(stdout);
    return FAILURE;
  }
  printf("other user> %s", buffer);
  fflush(stdout);

  return SUCCESS;
}

int get_message_and_send(mqd_t write) {
  char buffer[SIZE];

  if (fgets(buffer, SIZE, stdin) == NULL) {
    if (errno == EINTR) {
      is_signal = 1;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: get_message_and_send - sent message is empty.\n");
    return FAILURE;
  }

  if (send_message(write, buffer, 0) == FAILURE)
    return FAILURE;

  return SUCCESS;
}

int send_message(mqd_t write, char *buffer, unsigned int priority) {
  if (mq_send(write, buffer, strlen(buffer) + 1, priority) == -1) {
    if (errno == EINTR) {
      is_signal = 1;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: send_message - some error occured.\n");
    perror("Sending message error");
    return FAILURE;
  }
  return SUCCESS;
}

int delete_queue(char *queue_name) {
  char buffer[SIZE];
  create_actual_queue_name(buffer, queue_name, "_1");
  mq_unlink(buffer);
  create_actual_queue_name(buffer, queue_name, "_2");
  mq_unlink(buffer);

  return SUCCESS;
}

void create_actual_queue_name(char *buffer, char *queue_name, char *addition) {
  snprintf(buffer, SIZE, "%s%s", queue_name, addition);
}