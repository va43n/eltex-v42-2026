#include "task_manager.h"

queue* task_manager_init(int (*task_func)(int, int)) {
  if (task_func == NULL) task_func = _simple_task_function;

  queue* head = (queue*)calloc(1, sizeof(queue));
  head->t.task_func = task_func;
  head->next = NULL;

  return head;
}

int task_manager_free(queue* head) {
  if (head == NULL) return ERROR;

  queue* ptr = head;

  while (ptr != NULL) {
    queue* temp = ptr;
    ptr = ptr->next;
    free(temp);
  }

  return SUCCESS;
}

int task_manager_add_task(queue* head, int priority, int value) {
  if (head == NULL) return ERROR;

  size_t index = _get_unique_index(head);
  if (index == INDEX_NOT_FOUND) return ERROR;

  queue* q = (queue*)calloc(1, sizeof(queue));
  q->index = index;

  if (task_manager_set_priority(q, priority) &&
      task_manager_set_value(q, value) == SUCCESS) {
    q->t.task_func = head->t.task_func;
    queue* ptr = head;
    while (ptr->next != NULL && ptr->next->t.priority <= priority) {
      ptr = ptr->next;
    }
    q->next = ptr->next;
    ptr->next = q;
    return SUCCESS;
  }

  free(q);

  return ERROR;
}

int task_manager_remove_task(queue* head, size_t index) {
  if (head == NULL) return ERROR;

  queue *ptr = head->next, *prev = head;

  while (ptr != NULL) {
    if (ptr->index == index) {
      prev->next = ptr->next;
      ptr->next = NULL;
      free(ptr);

      return SUCCESS;
    }
    ptr = ptr->next;
    prev = prev->next;
  }

  return INDEX_NOT_FOUND;
}

int task_manager_print(queue* head) {
  if (head == NULL) return ERROR;

  queue* ptr = head->next;

  printf("===============================\n");
  printf("My task manager! \n");

  while (ptr != NULL) {
    task_manager_print_task(ptr);
    ptr = ptr->next;
  }
  printf("===============================\n\n");

  return SUCCESS;
}

int task_manager_print_task(queue* t) {
  if (t == NULL) return ERROR;

  printf("\n-------------------------------\n");
  printf("Unique index: %ld\n", t->index);
  printf("Priority: %d\n", t->t.priority);
  printf("Value: %d\n", t->t.value);
  printf("-------------------------------\n");

  return SUCCESS;
}

int task_manager_set_priority(queue* q, int priority) {
  if (priority < HIGHEST_PRIORITY || priority > LOWEST_PRIORITY) return ERROR;

  q->t.priority = priority;
  return SUCCESS;
}

int task_manager_set_value(queue* q, int value) {
  q->t.value = value;
  return SUCCESS;
}

int task_manager_execute_task_with_certain_priority(queue* head, int priority) {
  if (head == NULL) return ERROR;
  if (priority < HIGHEST_PRIORITY || priority > LOWEST_PRIORITY) return ERROR;

  queue* ptr = head->next;

  while (ptr != NULL) {
    if (ptr->t.priority == priority) {
      ptr->t.task_func(ptr->t.priority, ptr->t.value);
      task_manager_remove_task(head, ptr->index);
      return SUCCESS;
    }
    ptr = ptr->next;
  }

  return PRIORITY_IS_NOT_FOUND;
}

int task_manager_execute_task_with_certain_priority_or_higher(queue* head,
                                                              int priority) {
  if (head == NULL) return ERROR;
  if (priority < HIGHEST_PRIORITY || priority > LOWEST_PRIORITY) return ERROR;

  queue *ptr = head->next, *higher = NULL;

  while (ptr != NULL) {
    if (ptr->t.priority == priority) {
      ptr->t.task_func(ptr->t.priority, ptr->t.value);
      task_manager_remove_task(head, ptr->index);

      return SUCCESS;
    }
    if (ptr->t.priority < priority) higher = ptr;
    ptr = ptr->next;
  }

  if (higher != NULL) {
    higher->t.task_func(higher->t.priority, higher->t.value);
    task_manager_remove_task(head, higher->index);

    return SUCCESS;
  }

  return PRIORITY_IS_NOT_FOUND;
}

int tesk_manager_execute_first_task_in_queue(queue* head) {
  if (head == NULL) return ERROR;

  queue* ptr = head->next;

  if (ptr != NULL) {
    ptr->t.task_func(ptr->t.priority, ptr->t.value);
    task_manager_remove_task(head, ptr->index);
    return SUCCESS;
  }

  return INDEX_NOT_FOUND;
}