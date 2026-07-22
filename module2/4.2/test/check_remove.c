#include "test.h"

START_TEST(check_remove_1) {
  queue* head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_remove_task(head, 1), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_add_task(head, 0, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 100, 15), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 1), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 10, -4), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 1, 2), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 255, 255), SUCCESS);

  ck_assert_int_eq(task_manager_remove_task(head, 3), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 1), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 6), SUCCESS);
  ck_assert_int_eq(task_manager_remove_task(head, 4), SUCCESS);

  ck_assert_int_eq(task_manager_add_task(head, 0, 10), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 0, 1), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 10, -4), SUCCESS);
  ck_assert_int_eq(task_manager_add_task(head, 255, 255), SUCCESS);

  task_manager_print(head);

  ck_assert_int_eq(head->next->index == 1, 1);
  ck_assert_int_eq(head->next->t.priority == 0, 1);
  ck_assert_int_eq(head->next->t.value == 10, 1);

  ck_assert_int_eq(head->next->next->index == 3, 1);
  ck_assert_int_eq(head->next->next->t.priority == 0, 1);
  ck_assert_int_eq(head->next->next->t.value == 1, 1);

  ck_assert_int_eq(head->next->next->next->index == 5, 1);
  ck_assert_int_eq(head->next->next->next->t.priority == 1, 1);
  ck_assert_int_eq(head->next->next->next->t.value == 2, 1);

  ck_assert_int_eq(head->next->next->next->next->index == 4, 1);
  ck_assert_int_eq(head->next->next->next->next->t.priority == 10, 1);
  ck_assert_int_eq(head->next->next->next->next->t.value == -4, 1);

  ck_assert_int_eq(head->next->next->next->next->next->index == 2, 1);
  ck_assert_int_eq(head->next->next->next->next->next->t.priority == 100, 1);
  ck_assert_int_eq(head->next->next->next->next->next->t.value == 15, 1);

  ck_assert_int_eq(head->next->next->next->next->next->next->index == 6, 1);
  ck_assert_int_eq(head->next->next->next->next->next->next->t.priority == 255, 1);
  ck_assert_int_eq(head->next->next->next->next->next->next->t.value == 255, 1);

  ck_assert_int_eq(task_manager_remove_task(head, 10), INDEX_NOT_FOUND);
  ck_assert_int_eq(task_manager_remove_task(head, 0), INDEX_NOT_FOUND);

  ck_assert_int_eq(task_manager_remove_task(NULL, 2), ERROR);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);
}

Suite *check_remove() {
  Suite *s = suite_create("check_remove");
  TCase *tc = tcase_create("remove test cases for task_manager.a");

  tcase_add_test(tc, check_remove_1);
  suite_add_tcase(s, tc);

  return s;
}