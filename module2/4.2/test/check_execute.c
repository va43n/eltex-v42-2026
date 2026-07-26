#include "test.h"

START_TEST(check_execute_1) {
  queue* head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_add_task(head, 0, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 100, 15), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 1), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 10, -4), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 1, 2), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 255, 255), SUCCESS);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0),
                   SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0),
                   SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0),
                   PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 22),
                   PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 1),
                   SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 255),
                   SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 256),
                   ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, -22),
                   ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(NULL, 10),
                   ERROR);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

START_TEST(check_execute_2) {
  queue* head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_add_task(head, 0, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 100, 15), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 1), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 10, -4), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 1, 2), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 255, 255), SUCCESS);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 255),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 255),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 2), INDEX_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 0),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 0),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 0),
      PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 8),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 8),
      PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 1000),
      ERROR);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, -1000),
      ERROR);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(NULL, 10),
      ERROR);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

START_TEST(check_execute_3) {
  queue* head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_add_task(head, 2, 10), SUCCESS);

  ck_assert_int_eq(head->next->index == 1, 1);
  ck_assert_int_eq(head->next->t.priority == 2, 1);
  ck_assert_int_eq(head->next->t.value == 10, 1);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 5),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 5),
      PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_add_task(head, 2, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 11), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 12), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 13), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 9), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 5, 5), SUCCESS);

  ck_assert_int_eq(head->next->index == 5, 1);
  ck_assert_int_eq(head->next->t.value == 9, 1);

  ck_assert_int_eq(head->next->next->index == 1, 1);
  ck_assert_int_eq(head->next->next->t.value == 10, 1);

  ck_assert_int_eq(head->next->next->next->index == 2, 1);
  ck_assert_int_eq(head->next->next->next->t.value == 11, 1);

  ck_assert_int_eq(head->next->next->next->next->index == 3, 1);
  ck_assert_int_eq(head->next->next->next->next->t.value == 12, 1);

  ck_assert_int_eq(head->next->next->next->next->next->index == 4, 1);
  ck_assert_int_eq(head->next->next->next->next->next->t.value == 13, 1);

  ck_assert_int_eq(head->next->next->next->next->next->next->index == 6, 1);
  ck_assert_int_eq(head->next->next->next->next->next->next->t.value == 5, 1);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 4),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 4), INDEX_NOT_FOUND);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 4),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 2),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 255),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 4),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 2), INDEX_NOT_FOUND);
  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 4),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

START_TEST(check_execute_4) {
  queue* head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_add_task(head, 2, 10), SUCCESS);

  ck_assert_int_eq(head->next->index == 1, 1);
  ck_assert_int_eq(head->next->t.priority == 2, 1);
  ck_assert_int_eq(head->next->t.value == 10, 1);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 5),
      SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(
      task_manager_execute_task_with_certain_priority_or_higher(head, 5),
      PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_add_task(head, 2, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 11), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 12), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 2, 13), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 9), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 5, 5), SUCCESS);

  ck_assert_int_eq(head->next->index == 5, 1);
  ck_assert_int_eq(head->next->t.value == 9, 1);

  ck_assert_int_eq(head->next->next->index == 1, 1);
  ck_assert_int_eq(head->next->next->t.value == 10, 1);

  ck_assert_int_eq(head->next->next->next->index == 2, 1);
  ck_assert_int_eq(head->next->next->next->t.value == 11, 1);

  ck_assert_int_eq(head->next->next->next->next->index == 3, 1);
  ck_assert_int_eq(head->next->next->next->next->t.value == 12, 1);

  ck_assert_int_eq(head->next->next->next->next->next->index == 4, 1);
  ck_assert_int_eq(head->next->next->next->next->next->t.value == 13, 1);

  ck_assert_int_eq(head->next->next->next->next->next->next->index == 6, 1);
  ck_assert_int_eq(head->next->next->next->next->next->next->t.value == 5, 1);

  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);
  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);
  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 2), INDEX_NOT_FOUND);
  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);
  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 4), INDEX_NOT_FOUND);
  ck_assert_int_eq(tesk_manager_execute_first_task_in_queue(head), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

#include <time.h>

int _task_sort(const void* a, const void* b) {
  task val_a = *(task*)a;
  task val_b = *(task*)b;

  if (val_a.priority <= val_b.priority)
    return -1;
  else
    return 1;
}

START_TEST(check_execute_5) {
  srand(time(NULL));
  queue* head = task_manager_init(NULL);

  size_t N = 1000;
  task* tasks = (task*)malloc(N * sizeof(task));

  for (size_t i = 0; i < N; i++) {
    tasks[i].priority = rand() % 255;
    tasks[i].value = rand();

    task_manager_add_task(head, tasks[i].priority, tasks[i].value);
  }

  qsort(tasks, N, sizeof(task), _task_sort);

  queue* ptr = head->next;
  for (size_t i = 0; i < N; i++, ptr = ptr->next) {
    ck_assert_int_eq(ptr->t.priority == tasks[i].priority, 1);
    ck_assert_int_eq(ptr->t.value == tasks[i].value, 1);
  }

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

Suite* check_execute() {
  Suite* s = suite_create("check_execute");
  TCase* tc = tcase_create("execute test cases for task_manager.a");

  tcase_add_test(tc, check_execute_1);
  tcase_add_test(tc, check_execute_2);
  tcase_add_test(tc, check_execute_3);
  tcase_add_test(tc, check_execute_4);
  tcase_add_test(tc, check_execute_5);
  suite_add_tcase(s, tc);

  return s;
}