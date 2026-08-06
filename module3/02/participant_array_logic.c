#include "publisher_broker_subscriber.h"

int participant_push_back(participant** ps, size_t* size, int pid, char* topic,
                          char* payload) {
  (*size)++;
  participant* tmp = (participant*)realloc(*ps, sizeof(participant) * *size);
  *ps = tmp;
  (*ps)[*size - 1].pid = pid;
  (*ps)[*size - 1].size = 1;

  (*ps)[*size - 1].topics = (char**)malloc(sizeof(char*) * 1);
  (*ps)[*size - 1].topics[0] =
      (char*)malloc(sizeof(char) * (strlen(topic) + 1));
  strcpy((*ps)[*size - 1].topics[0], topic);

  (*ps)[*size - 1].payloads = (char**)malloc(sizeof(char*) * 1);
  (*ps)[*size - 1].payloads[0] =
      (char*)malloc(sizeof(char) * (strlen(payload) + 1));
  strcpy((*ps)[*size - 1].payloads[0], payload);

  return SUCCESS;
}

int participant_add_message_by_index(participant** ps, size_t index,
                                     char* topic, char* payload) {
  (*ps)[index].size++;
  char** tmp =
      (char**)realloc((*ps)[index].topics, sizeof(char*) * (*ps)[index].size);
  (*ps)[index].topics = tmp;

  (*ps)[index].topics[(*ps)[index].size - 1] =
      (char*)malloc(sizeof(char) * (strlen(topic) + 1));
  strcpy((*ps)[index].topics[(*ps)[index].size - 1], topic);

  tmp =
      (char**)realloc((*ps)[index].payloads, sizeof(char*) * (*ps)[index].size);
  (*ps)[index].payloads = tmp;

  (*ps)[index].payloads[(*ps)[index].size - 1] =
      (char*)malloc(sizeof(char) * (strlen(payload) + 1));
  strcpy((*ps)[index].payloads[(*ps)[index].size - 1], payload);

  return SUCCESS;
}

int participant_find_by_pid(participant* ps, size_t size, int pid,
                            size_t* index) {
  for (size_t i = 0; i < size; i++) {
    if (ps[i].pid == pid) {
      *index = i;
      return SUCCESS;
    }
  }

  return FAILURE;
}

int participant_remove_topic_by_index(participant** ps, size_t index,
                                      char* topic) {
  for (size_t i = 0; i < (*ps)[index].size; i++) {
    if (strcmp((*ps)[index].topics[i], topic) == 0) {
      free((*ps)[index].topics[i]);
      free((*ps)[index].payloads[i]);
      for (size_t j = i; j < (*ps)[index].size - 1; j++) {
        (*ps)[index].topics[j] = (*ps)[index].topics[j + 1];
        (*ps)[index].payloads[j] = (*ps)[index].payloads[j + 1];
      }
      (*ps)[index].size--;
    }
  }

  return SUCCESS;
}

int participant_check_if_topic_exists(participant* ps, size_t index,
                                      char* topic) {
  for (size_t i = 0; i < ps[index].size; i++) {
    if (strcmp(ps[index].topics[i], topic) == 0) {
      return TRUE;
    }
  }

  return FALSE;
}

int participant_free(participant* ps, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < ps[i].size; j++) {
      free(ps[i].topics[j]);
      free(ps[i].payloads[j]);
    }
    free(ps[i].topics);
    free(ps[i].payloads);
  }
  free(ps);

  return SUCCESS;
}

int participant_print(participant* ps, size_t size, char* participant_type) {
  for (size_t i = 0; i < size; i++) {
    printf("\t%s №%ld (%d)\n", participant_type, i + 1, ps[i].pid);
    for (size_t j = 0; j < ps[i].size; j++) {
      printf("-------\nMessage №%ld\n", j + 1);
      printf("Topic: %s\n", ps[i].topics[j]);
      printf("Payload: %s\n", ps[i].payloads[j]);
    }
    printf("-------\n\n");
  }

  return SUCCESS;
}

int participant_send_signal_to_all_participants(participant* ps, size_t size,
                                                int signal) {
  for (size_t i = 0; i < size; i++) {
    if (kill(ps[i].pid, signal) == -1) {
      fprintf(stderr,
              "ERROR: participant_send_signal_to_all_participants - something "
              "went wrong while sending the signal.\n");
      perror("kill");
    }
  }
  return SUCCESS;
}