#include "phone_book.h"

int _size_t_sort(const void* a, const void* b) {
  int val_a = *(size_t*)a;
  int val_b = *(size_t*)b;

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

size_t _get_unique_index(phone_book* head) {
  if (head == NULL || head->next == head) return FIRST_UNIQUE_INDEX;

  phone_book* ptr = head->next;
  size_t* indices = NULL;
  size_t n = 0;

  while (ptr != head) {
    n++;
    size_t* temp = realloc(indices, sizeof(size_t) * n);
    if (temp == NULL) {
      free(indices);
      return INDEX_NOT_FOUND;
    }
    indices = temp;
    indices[n - 1] = ptr->index;

    ptr = ptr->next;
  }

  qsort(indices, n, sizeof(size_t), _size_t_sort);
  for (size_t i = 1; i < n; i++) {
    if (indices[i] - 1 > indices[i - 1]) {
      n = indices[i - 1] + 1;
      free(indices);
      return n;
    }
  }

  n = indices[n - 1] + 1;
  free(indices);

  return n;
}

void _free_one_page(phone_book* pb) {
  if (pb->numbers != NULL) {
    for (size_t i = 0; i < pb->numbers_n; i++) free(pb->numbers[i]);
    free(pb->numbers);
  }
  if (pb->socials != NULL) free(pb->socials);
}

int _check_number(char* pn) {
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

int _set_full_name(phone_book* pb, char full_name[]) {
  if (strlen(full_name) == 0) return ERROR;

  strcpy(pb->full_name, full_name);
  return SUCCESS;
}