#include "task_manager.h"

int main() {
  queue* head = task_manager_init(NULL);

  task_manager_print(head);

  task_manager_add_task(head, 0, 10);
  task_manager_add_task(head, 100, 15);
  task_manager_add_task(head, 0, 1);

  task_manager_print(head);

  task_manager_add_task(head, 10, -4);
  task_manager_add_task(head, 1, 2);
  task_manager_add_task(head, 255, 255);

  task_manager_print(head);

  printf("start executing\n");

  task_manager_execute_task_with_certain_priority(head, 0);

  task_manager_print(head);

  task_manager_execute_task_with_certain_priority_or_higher(head, 255);
  task_manager_execute_task_with_certain_priority_or_higher(head, 255);

  task_manager_print(head);

  task_manager_execute_task_with_certain_priority_or_higher(head, 0);
  if (task_manager_execute_task_with_certain_priority_or_higher(head, 0) ==
      PRIORITY_IS_NOT_FOUND)
    printf("no more 0 or higher\n");

  task_manager_print(head);

  task_manager_execute_task_with_certain_priority_or_higher(head, 8);
  if (task_manager_execute_task_with_certain_priority_or_higher(head, 8) ==
      PRIORITY_IS_NOT_FOUND)
    printf("no more 8 or higher\n");

  task_manager_print(head);

  task_manager_free(head);

  return 0;
}