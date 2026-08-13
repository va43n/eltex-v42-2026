#include "raw_udp.h"

int parse_input(int argc, char* argv[], int* mask) {
  *mask = 0;
  if (argc == 1) return SUCCESS;

  if (argc != 2) {
    fprintf(stderr,
            "ERROR: parse_input - input should contain either 0 or 1 "
            "parameter: set of flags.\n");
    return FAILURE;
  }

  if (parse_flag(argv[1], mask) == FAILURE) return FAILURE;

  return SUCCESS;
}

int parse_flag(char* flag, int* mask) {
  size_t len = strlen(flag);

  if (len < 2 || flag[0] != '-') {
    fprintf(stderr,
            "ERROR: parse_flag - flag should be made out of at least 2 "
            "symbols, in which the first one is the '-'.\n");
    return FAILURE;
  }

  for (size_t i = 1; i < len; i++) {
    switch (flag[i]) {
      case PUT_IN_FILE:
        *mask |= PUT_IN_FILE_MASK;
        break;
      case FILTER_06:
        *mask |= FILTER_06_MASK;
        break;
      case FILTER_DNS:
        *mask |= FILTER_DNS_MASK;
        break;
      default:
        fprintf(stderr, "ERROR, parse_flag - unknown flag.\n");
        return FAILURE;
    }
  }

  return SUCCESS;
}