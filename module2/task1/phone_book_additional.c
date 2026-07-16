#include "phone_book.h"

size_t _get_unique_index(phone_book* head) {
  phone_book* ptr = head->next;
  size_t biggest_index = FIRST_UNIQUE_INDEX;

  while (ptr != NULL) {
    if (ptr->index < biggest_index) {
      biggest_index = ptr->index + 1;
      break;
    }

    biggest_index = ptr->index + 1;
    ptr = ptr->next;
  }

  return biggest_index;
}

void _free_one_page(phone_book* pb) {
  if (pb->phone_numbers != NULL) {
    for (size_t i = 0; i < pb->phone_numbers_n; i++) free(pb->phone_numbers[i]);
    free(pb->phone_numbers);
  }
  if (pb->socials != NULL) free(pb->socials);
}

int _check_phone_number(char* pn) {
  regex_t regex;
  int reti;

  reti = regcomp(&regex, "^\\+?[0-9\\-]+$", REG_EXTENDED);
  if (reti) {
    return ERROR;
  }

  reti = regexec(&regex, pn, 0, NULL, 0);

  regfree(&regex);

  return !reti;
}