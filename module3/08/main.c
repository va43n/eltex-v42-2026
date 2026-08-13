#include "raw_udp.h"

int main(int argc, char* argv[]) {
  int mask;

  if (parse_input(argc, argv, &mask) == FAILURE) return FAILURE;
  if (search_for_udp(mask) == FAILURE) return FAILURE;

  return 0;
}