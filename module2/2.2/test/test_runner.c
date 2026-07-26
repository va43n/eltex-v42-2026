#include "test.h"

int main() {
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, check_calculation());

  srunner_run_all(sr, CK_NORMAL);

  srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}