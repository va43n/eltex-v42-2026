#include "publisher_broker_subscriber.h"

int check_if_queue_is_created(key_t key) {
  int msgid = msgget(key, 0);

  if (msgid == -1) {
    if (errno == ENOENT) return FALSE;
  }

  return TRUE;
}

int set_some_message_parameters(message *my_msg, int message_type, int pid,
                                long priority) {
  my_msg->mtype = priority;
  my_msg->message_type = message_type;
  my_msg->pid = pid;

  return SUCCESS;
}

int get_message_and_send(int write, message *my_msg,
                         unsigned int number_of_inputs_needed) {
  static unsigned int number_of_inputs = 0;
  char buffer[PAYLOAD_SIZE];

  if (fgets(buffer, PAYLOAD_SIZE, stdin) == NULL) {
    if (errno == EINTR) {
      is_signal = 1;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: get_message_and_send - sent message is empty.\n");
    return FAILURE;
  }

  number_of_inputs++;
  if (number_of_inputs == 1) {
    if (number_of_inputs_needed == 1)
      strcpy(my_msg->payload, buffer);
    else
      strcpy(my_msg->topic, buffer);
  } else if (number_of_inputs == 2) {
    strcpy(my_msg->payload, buffer);
  }

  if (number_of_inputs >= number_of_inputs_needed) {
    number_of_inputs = 0;
    if (send_message(write, *my_msg) == FAILURE) return FAILURE;
  }

  return SUCCESS;
}

int send_message(int write, message my_msg) {
  size_t buf_size = sizeof(message) - sizeof(long);
  if (msgsnd(write, &my_msg, buf_size, 0) == -1) {
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

int recv_message(int write, message *my_msg) {
  size_t buf_size = sizeof(message) - sizeof(long);
  if (msgrcv(write, &my_msg, buf_size, my_msg->mtype, 0) == -1) {
    if (errno == EINTR) {
      is_signal = 1;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: recv_message - some error occured.\n");
    perror("Receiving message error");
    return FAILURE;
  }
  return SUCCESS;
}