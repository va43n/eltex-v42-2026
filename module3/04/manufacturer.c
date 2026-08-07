#include "factory.h"

#define MANUFACTURER_SLEEP_TIME 3
#define MANUFACTURER_CHUNKS 10

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

  item my_item;

  int is_full = FALSE;
  int is_everything_processed = FALSE;
  while (!is_signal) {
    for (int i = 0; i < MANUFACTURER_CHUNKS && !is_full; i++) {
      my_item = generate_item();
      if (shared_memory_write(key, my_item) == FAILURE) {
        is_full = TRUE;
      }
    }

    if (is_full) {
      item* read_item = NULL;
      do {
        if (shared_memory_read(key, read_item) == FAILURE) return FAILURE;
        if (read_item != NULL &&
            check_if_item_is_processed(read_item) == FALSE) {
          is_everything_processed = TRUE;
          break;
        }
      } while (read_item != NULL);

      if (is_everything_processed) {
        printf("...All of the items have been processed...\n");
        break;
      }
    }

    sleep(MANUFACTURER_SLEEP_TIME);
  }

  return SUCCESS;
}