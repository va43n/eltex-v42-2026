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
  set_some_message_parameters(&my_msg, MT_SUBSCRIBER, getpid(), 1);
  set_some_message_parameters(&recv_msg, MT_SUBSCRIBER, getpid(), getpid());

  printf(
      "To send message:\n\t1) enter the topic\n\t2) press 'Enter'\n\t3) enter "
      "the type of message: '%s' to subscribe, '%s' to unsubscribe\n\t4) press "
      "'Enter' again\n", SUBSCRIBE_TEXT, UNSUBSCRIBE_TEXT);
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr,
            "ERROR: do_subscriber_activity - cannot create child process.\n");
    return FAILURE;
  }
  if (pid == 0) {
    char buffer[PAYLOAD_SIZE];
    while (!is_signal) {
      printf("> ");
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
    }
  } else {
    while (!is_signal) {
      if (recv_message(msgid, &recv_msg) == FAILURE) break;
      printf(
          "-------\nSome publisher wrote a message on one of your desired "
          "topics:\n");
      printf("TOPIC: '%s'\nPAYLOAD:\n%s\n-------\n\n", recv_msg.topic,
             recv_msg.payload);
    }
    kill(pid, SIGKILL);
  }

  return SUCCESS;
}