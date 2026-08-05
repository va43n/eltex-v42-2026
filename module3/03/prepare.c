#include "p2p.h"

int parse_input(int argc, char *argv[], char **queue_name) {
  if (argc != 2) {
    fprintf(stderr, "ERROR: PARSE_INPUT - input should contain exactly 1 "
                    "parameter - name of queue.\n");
    return FAILURE;
  }

  *queue_name = argv[1];

  return SUCCESS;
}