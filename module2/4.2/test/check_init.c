#include "test.h"

int some_function(int priority, int value) {
  printf("%d\n", priority + value);
  return SUCCESS;
}

START_TEST(check_init_1) {
  queue *head = task_manager_init(NULL);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);

  head = task_manager_init(some_function);
  ck_assert_int_eq(head->next == NULL, 1);

  ck_assert_int_eq(task_manager_free(head), SUCCESS);

  ck_assert_int_eq(task_manager_free(NULL), ERROR);
}

Suite *check_init() {
  Suite *s = suite_create("check_init");
  TCase *tc = tcase_create("init test cases for task_manager.a");

  tcase_add_test(tc, check_init_1);
  suite_add_tcase(s, tc);

  return s;
}