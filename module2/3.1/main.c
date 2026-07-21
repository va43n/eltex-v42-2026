#include "my_chmod.h"

int main() {
  mod m1, m2, m3;

  if (!my_chmod_init(&m1, "rwx--xr-x")) {
    printf("ERROR 1\n");
  }
  if (!my_chmod_init(&m2, "247")) {
    printf("ERROR 2\n");
  }
  if (!my_chmod_init_with_file(&m3, ".clang-format")) {
    printf("ERROR 3\n");
  }

  my_chmod_print(m1);
  my_chmod_print(m2);
  my_chmod_print(m3);

  return 0;
}