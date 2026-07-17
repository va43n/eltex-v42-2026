#include <check.h>

#include "test.h"

int main() {
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, check_add());
  srunner_add_suite(sr, check_remove());
  srunner_add_suite(sr, check_find());
  // srunner_add_suite(sr, check_set());
  srunner_add_suite(sr, check_save_load());

  srunner_run_all(sr, CK_NORMAL);

  srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}