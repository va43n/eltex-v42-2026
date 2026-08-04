#include "p2p.h"

int check_if_queue_is_created(char *queue_name, int *status) {
  char buffer[SIZE];

  create_actual_queue_name(buffer, queue_name, "_1");
  mqd_t mq = mq_open(buffer, O_RDONLY);
  if (mq == (mqd_t)-1) {
    *status = CREATOR;
    return FALSE;
  }
  mq_close(mq);

  create_actual_queue_name(buffer, queue_name, "_2");
  mq = mq_open(buffer, O_RDONLY);
  if (mq == (mqd_t)-1) {
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
  queue_attr.mq_maxmsg = 32;
  queue_attr.mq_msgsize = SIZE;

  create_actual_queue_name(buffer, queue_name, "_1");
  if ((mq = mq_open(buffer, O_CREAT | O_RDWR | O_NONBLOCK, 0600,
                    &queue_attr)) == (mqd_t)-1) {
    fprintf(stderr, "ERROR: CREATE_QUEUE - some error occured.\n");
    perror("Creating queue error");
    return FAILURE;
  }
  mq_close(mq);

  create_actual_queue_name(buffer, queue_name, "_2");
  if ((mq = mq_open(buffer, O_CREAT | O_RDWR | O_NONBLOCK, 0600,
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
  while (TRUE) {
    if (recv_message(read) == FAILURE || send_message(write) == FAILURE)
      break;
  }

  return SUCCESS;
}

int recv_message(mqd_t read) {
  char buffer[SIZE];
  ssize_t bytes_read = mq_receive(read, buffer, SIZE, NULL);
  if (bytes_read == -1) {
    fprintf(stderr, "ERROR: RECV_MESSAGE - some error occured.\n");
    perror("cannot receive");
    return FAILURE;
  }

  buffer[bytes_read] = '\0';
  printf("\nother user> %s\n", buffer);
  fflush(stdout);

  return SUCCESS;
}

int send_message(mqd_t write) {
  char buffer[SIZE];

  if (fgets(buffer, SIZE, stdin) != NULL) {
    if (mq_send(write, buffer, strlen(buffer) + 1, 0) == -1) {
      fprintf(stderr, "ERROR: SEND_MESSAGE - some error occured.\n");
      perror("Sending message error");
      return FAILURE;
    }
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