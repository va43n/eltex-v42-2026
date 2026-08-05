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
    fprintf(stderr, "ERROR: DO_SUBSCRIBER_ACTIVITY - some error occured while "
                    "working with queue.\n");
    perror("msgget");
    return FAILURE;
  }

  message my_msg, recv_msg;
  set_some_message_parameters(&my_msg, MT_SUBSCRIBE, getpid(), 1);

  fd_set rfds, rfds_start;
  int maxfd = (msgid > STDIN_FILENO ? msgid : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(msgid, &rfds_start);

  while (!is_signal) {
    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) {
        is_signal = 1;
        continue;
      }
      fprintf(stderr, "ERROR: DO_SUBSCRIBER_ACTIVITY - some error occured while trying "
                      "to wait for some user activity.\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (get_message_and_send(msgid, &my_msg, 2) == FAILURE)
        break;
      if (strcmp(my_msg.payload, SUBSCRIBE_TEXT) == 0)
        my_msg.message_type = MT_SUBSCRIBE;
      else if (strcmp(my_msg.payload, UNSUBSCRIBE_TEXT) == 0)
        my_msg.message_type = MT_UNSUBSCRIBE;
      else break;
    }

    if (FD_ISSET(msgid, &rfds)) {
      if (recv_message(msgid, &recv_msg) == FAILURE)
        break;
      printf("TOPIC: '%s'\nPAYLOAD: '%s'\n\n", recv_msg.topic, recv_msg.payload);
      fflush(stdout);
    }
  }

  return SUCCESS;
}