#include "publisher_broker_subscriber.h"

int do_broker_activity() {
  printf("I'm a broker\n");

  key_t key = ftok(PATHNAME, PROJ_ID);
  if (check_if_queue_is_created(key)) {
    fprintf(stderr,
            "ERROR: DO_BROKER_ACTIVITY - the queue is already exists.\n");
    return FAILURE;
  }
  int msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) {
    fprintf(stderr,
            "ERROR: DO_BROKER_ACTIVITY - some error occured while "
            "working with queue.\n");
    perror("msgget");
    return FAILURE;
  }

  participant* subscribers = NULL;
  participant* publishers = NULL;
  size_t subs_n = 0, pubs_n = 0;

  message recv_msg;
  set_some_message_parameters(&recv_msg, MT_PUBLISHER, getpid(), 1);

  while (!is_signal) {
    if (recv_message(msgid, &recv_msg) == FAILURE) {
      is_signal = TRUE;
      continue;
    }
    if (recv_msg.message_type == MT_PUBLISHER) {
      size_t index;
      if (participant_find_by_pid(publishers, pubs_n, recv_msg.pid, &index) ==
          FAILURE) {
        participant_push_back(&publishers, &pubs_n, recv_msg.pid,
                              recv_msg.topic, recv_msg.payload);
        printf("...New publisher added...\n");
        participant_print(publishers, pubs_n, PUBLISHER_NAME);
      } else {
        participant_add_message_by_index(&publishers, index, recv_msg.topic,
                                         recv_msg.payload);
        printf("...Publisher №%ld created new theme...\n", index);
        participant_print(publishers, pubs_n, PUBLISHER_NAME);
      }
      if (broker_send_message_to_all_subscribers(msgid, recv_msg, subscribers,
                                                 subs_n) == FAILURE) {
        is_signal = TRUE;
        continue;
      }
    } else if (recv_msg.message_type == MT_SUBSCRIBER &&
               strcmp(recv_msg.payload, SUBSCRIBE_TEXT) == 0) {
      size_t index;
      if (participant_find_by_pid(subscribers, subs_n, recv_msg.pid, &index) ==
          FAILURE) {
        participant_push_back(&subscribers, &subs_n, recv_msg.pid,
                              recv_msg.topic, recv_msg.payload);
        printf("...New subscriber added...\n");
        participant_print(subscribers, subs_n, SUBSCRIBER_NAME);
      } else if (participant_check_if_topic_exists(subscribers, index,
                                                   recv_msg.topic) == FALSE) {
        participant_add_message_by_index(&subscribers, index, recv_msg.topic,
                                         recv_msg.payload);
        printf("...Subscriber №%ld subscribed to a new theme...\n", index);
        participant_print(subscribers, subs_n, SUBSCRIBER_NAME);
      }
    } else if (recv_msg.message_type == MT_SUBSCRIBER &&
               strcmp(recv_msg.payload, UNSUBSCRIBE_TEXT) == 0) {
      size_t index;
      if (participant_find_by_pid(subscribers, subs_n, recv_msg.pid, &index) ==
          SUCCESS) {
        participant_remove_topic_by_index(&subscribers, index, recv_msg.topic);
        printf("...Subscriber №%ld unsubscribed from some theme...\n", index);
        participant_print(subscribers, subs_n, SUBSCRIBER_NAME);
      }
    }
  }

  participant_send_signal_to_all_participants(subscribers, subs_n, SIGINT);
  participant_send_signal_to_all_participants(publishers, pubs_n, SIGINT);
  participant_free(subscribers, subs_n);
  participant_free(publishers, pubs_n);

  msgctl(msgid, IPC_RMID, NULL);

  return SUCCESS;
}

int broker_send_message_to_all_subscribers(int write, message my_msg,
                                           participant* subscribers,
                                           size_t subs_t) {
  message broker_message;
  broker_message.message_type = MT_PUBLISHER;
  broker_message.pid = my_msg.pid;
  strcpy(broker_message.topic, my_msg.topic);
  strcpy(broker_message.payload, my_msg.payload);

  for (size_t i = 0; i < subs_t; i++) {
    if (participant_check_if_topic_exists(subscribers, i, my_msg.topic) ==
        TRUE) {
      broker_message.mtype = subscribers[i].pid;
      if (send_message(write, broker_message) == FAILURE) {
        return FAILURE;
      }
    }
  }

  return SUCCESS;
}
