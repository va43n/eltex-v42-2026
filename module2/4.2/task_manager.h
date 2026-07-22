#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SUCCESS 1
#define ERROR 0

#define FIRST_UNIQUE_INDEX 1
#define INDEX_NOT_FOUND 0

#define PRIORITY_IS_NOT_FOUND -1

#define LOWEST_PRIORITY 255
#define HIGHEST_PRIORITY 0

typedef struct {
  int priority;
  int value;
  int (*task_func) (int, int);
} task;

typedef struct queue {
  size_t index;
  task t;
  struct queue* next;
} queue;

queue* task_manager_init(int (*task_func) (int, int));
int task_manager_free(queue* head);

int task_manager_add_task(queue* head, int priority, int value);
int task_manager_remove_task(queue* head, size_t index);

int task_manager_set_priority(queue* q, int priority);
int task_manager_set_value(queue* q, int value);

int task_manager_execute_task_with_certain_priority(queue* head, int priority);
int task_manager_execute_task_with_certain_priority_or_higher(queue* head, int priority);

int task_manager_print(queue* head);
int task_manager_print_task(queue* t);

int _simple_task_function(int priority, int value);

size_t _get_unique_index(queue* head);
int _size_t_sort(const void* a, const void* b);