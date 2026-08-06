#include "publisher_broker_subscriber.h"

int do_subscriber_activity() {
  printf("I'm a subscriber\n");

  key_t key = ftok(PATHNAME, PROJ_ID);
  if (!check_if_queue_is_created(key)) {
    fprintf(stderr,
            "ERROR: DO_SUBSCRIBER_ACTIVITY - the queue does not exist.\n");
    return FAILURE;
  }
  int msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    fprintf(stderr,
            "ERROR: DO_SUBSCRIBER_ACTIVITY - some error occured while "
            "working with queue.\n");
    perror("msgget");
    return FAILURE;
  }

  message my_msg, recv_msg;
  set_some_message_parameters(&my_msg, MT_SUBSCRIBE, getpid(), 1);

  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr,
            "ERROR: do_subscriber_activity - cannot create child process.\n");
    return FAILURE;
  }
  if (pid == 0) {
    char buffer[PAYLOAD_SIZE];
    while (!is_signal) {
      if (scanf(buffer, "%s") == EOF) {
        if (errno == EINTR) {
          printf("ended in scanf\n");
          is_signal = TRUE;
          continue;
        } else {
          fprintf(stderr,
                  "ERROR: do_subscriber_activity - some error occured while "
                  "using scanf.\n");
          perror("scanf");
        }
      }
      if (get_message_and_send(msgid, &my_msg, 2) == FAILURE) break;
      if (strcmp(my_msg.payload, SUBSCRIBE_TEXT) == 0)
        my_msg.message_type = MT_SUBSCRIBE;
      else if (strcmp(my_msg.payload, UNSUBSCRIBE_TEXT) == 0)
        my_msg.message_type = MT_UNSUBSCRIBE;
    }
  } else {
    while (!is_signal) {
      if (recv_message(msgid, &recv_msg) == FAILURE) break;
      printf("TOPIC: %s\nPAYLOAD: %s\n\n", recv_msg.topic, recv_msg.payload);
    }
    kill(pid, SIGKILL);
  }

  return SUCCESS;
}