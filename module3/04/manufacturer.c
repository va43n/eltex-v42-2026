#include "factory.h"

#define MANUFACTURER_SLEEP_TIME 3
#define MANUFACTURER_CHUNK_SIZE 10

int do_manufacturer_activity() {
  printf("I'm a manufacturer\n");

  srand(time(NULL));

  if (shared_memory_check_if_created() == TRUE) {
    fprintf(
        stderr,
        "ERROR: do_manufacturer_activity - the memory is already created.\n");
    return FAILURE;
  }

  shm_key key;
  if (shared_memory_create(&key) == FAILURE) return FAILURE;

  int is_full = FALSE;
  while (is_signal == FALSE) {
    if (is_full == FALSE) printf("> Starting to create another chunk...\n");
    for (int i = 0; i < MANUFACTURER_CHUNK_SIZE && is_full == FALSE; i++) {
      if (shared_memory_write(key, manufacturer_fill_item) == FAILURE) {
        printf("\t%d new items are added.\n", i + 1);
        printf("> All of the items are created...\n");
        is_full = TRUE;
      }
    }
    if (is_full == FALSE)
      printf("\t%d new items are added.\n", MANUFACTURER_CHUNK_SIZE);

    if (is_full) {
      printf("> Checking if there any item that is not processed...\n");
      if (!shared_memory_is_all_processed(key)) {
        printf("\tThere is still an item that is not processed yet.\n");
      } else {
        printf("> Everything is processed - stopping...\n");
        break;
      }
    }

    unsigned int sec = sleep(MANUFACTURER_SLEEP_TIME);
    if (sec > 0) is_signal = TRUE;
  }

  shared_memory_disconnect(key);
  if (shared_memory_delete(key) == FAILURE) return FAILURE;

  return SUCCESS;
}

int manufacturer_fill_item(item* my_item) {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    my_item->array[i] = rand();
  }
  my_item->size = ARRAY_SIZE;
  my_item->next = 0;

  return SUCCESS;
}
