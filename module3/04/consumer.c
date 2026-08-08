#include "factory.h"

#define CONSUMER_SLEEP_TIME 2

int do_consumer_activity() {
  printf("I'm a consumer\n");

  if (shared_memory_check_if_created() == FALSE) {
    fprintf(stderr,
            "ERROR: do_consumer_activity - the memory is not created yet.\n");
    return FAILURE;
  }

  shm_key key;
  if (shared_memory_connect(&key) == FAILURE) return FAILURE;

  while (is_signal == FALSE) {
    printf("> Looking for item that is not processed yet...\n");
    int result = shared_memory_get_and_process(key, consumer_process_item);
    if (result == SUCCESS) {
      printf("> Item was found and processed...\n");
    } else {
      printf("> Everything is processed - stopping...\n");
      break;
    }
    unsigned int sec = sleep(CONSUMER_SLEEP_TIME);
    if (sec > 0) is_signal = TRUE;
  }

  return shared_memory_disconnect(key);
}

int consumer_process_item(item *my_item) {
  int max = (*my_item).array[0], min = (*my_item).array[0];
  for (unsigned long i = 1; i < (*my_item).size; i++) {
    if (max < (*my_item).array[i])
      max = (*my_item).array[i];
    else if (min > (*my_item).array[i])
      min = (*my_item).array[i];
  }

  (*my_item).size = 0;
  printf("\tNew item processed:\n\tmax = %d\n\tmin = %d\n\n", max, min);

  return SUCCESS;
}