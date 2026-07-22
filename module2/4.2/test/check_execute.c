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

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 0), PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 22), PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 1), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 255), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, 256), ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(head, -22), ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority(NULL, 10), ERROR);

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

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 255), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 255), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 2), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 0), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 0), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 3), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 0), PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 8), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 5), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 8), PRIORITY_IS_NOT_FOUND);

  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, 1000), ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(head, -1000), ERROR);
  ck_assert_int_eq(task_manager_execute_task_with_certain_priority_or_higher(NULL, 10), ERROR);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

Suite *check_execute() {
  Suite *s = suite_create("check_execute");
  TCase *tc = tcase_create("execute test cases for task_manager.a");

  tcase_add_test(tc, check_execute_1);
  tcase_add_test(tc, check_execute_2);
  suite_add_tcase(s, tc);

  return s;
}