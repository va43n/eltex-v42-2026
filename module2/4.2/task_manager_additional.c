#include "task_manager.h"

int _simple_task_function(int priority, int value) {
  printf("Task %d (value = %d) completed\n", priority, value);

  return SUCCESS;
}

int _size_t_sort(const void* a, const void* b) {
  int val_a = *(size_t*)a;
  int val_b = *(size_t*)b;

  if (val_a < val_b) return -1;
  if (val_a > val_b) return 1;
  return 0;
}

size_t _get_unique_index(queue* head) {
  if (head->next == NULL) return FIRST_UNIQUE_INDEX;

  queue* ptr = head->next;
  size_t* indices = NULL;
  size_t n = 0;

  while (ptr != NULL) {
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
  size_t expected = FIRST_UNIQUE_INDEX;
  for (size_t i = 0; i < n; i++) {
    if (indices[i] > expected) {
      free(indices);
      return expected;
    }
    expected++;
  }

  n = indices[n - 1] + 1;
  free(indices);

  return n;
}