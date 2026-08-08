#include "factory.h"

#define CONSUMER_SLEEP_TIME 1

int do_consumer_activity() {
  printf("I'm a consumer\n");

  if (shared_memory_check_if_created() == FALSE) {
    fprintf(stderr,
            "ERROR: do_consumer_activity - the memory is not created yet.\n");
    return FAILURE;
  }

  shm_key key;
  if (shared_memory_connect(&key) == FAILURE) return FAILURE;

  item *my_item = NULL;
  int is_item_found = FALSE;
  while (!is_signal) {
    do {
      if (shared_memory_read(key, my_item) == FAILURE) return FAILURE;
      if (my_item != NULL && check_if_item_is_processed(my_item) == FALSE) {
        is_item_found = TRUE;
        break;
      }
    } while (my_item != NULL);
    if (is_item_found == FALSE) {
      printf("All of the items are already processed...\n");
      break;
    }
    if (shared_memory_process(key, my_item, consumer_process_item) == FAILURE)
      return FAILURE;
    is_item_found = FALSE;
    sleep(CONSUMER_SLEEP_TIME);
  }

  return SUCCESS;
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
  printf("New item processed:\n\tmax = %d\n\tmin = %d\n\n", max, min);

  return SUCCESS;
}