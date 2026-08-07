#include "item.h"

int check_if_item_is_processed(item *my_item) {
  if ((*my_item).size == 0) return TRUE;

  return FALSE;
}

item generate_item() {
  item my_item;
  my_item.size = ARRAY_SIZE;
  for (int i = 0; i < ARRAY_SIZE; i++) {
    my_item.array[i] = rand();
  }

  return my_item;
}