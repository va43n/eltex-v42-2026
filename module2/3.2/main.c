#include "gateway.h"

int main(int argc, char* argv[]) {
  if (argc < 4) {
    printf("Too few args. The arguments needed: IP, mask, N\n");
    return 1;
  }
  gateway gw;

  gateway_init(&gw, argv[1], argv[2]);
  gateway_print(gw);

  gateway_stats stats;
  char* endptr;
  unsigned long long value = strtoull(argv[3], &endptr, 10);
  if (endptr == argv[3]) {
    printf("N number could not be parsed\n");
    return 1;
  }
  stats.N = (size_t)value;

  gateway_generate(gw, &stats);

  return 0;
}