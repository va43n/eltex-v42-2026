#include "publisher_broker_subscriber.h"

int parse_input(int argc, char *argv[], char *mode) {
  if (argc != 2) {
    fprintf(stderr, "ERROR: PARSE_INPUT - there should be exactly 1 argument "
                    "in the input - flag.\n");
    return FAILURE;
  }

  return parse_flag(argv[1], mode);
}

int parse_flag(char *flag, char *mode) {
  if (strlen(flag) != 2) {
    fprintf(stderr, "ERROR: PARSE_FLAG - the length of the flag should be 2: "
                    "the first symbol is '-', the second - an identifier.\n");
    return FAILURE;
  }

  if (flag[0] != '-') {
    fprintf(
        stderr,
        "ERROR: PARSE_FLAG - the first symbol of the flag should be '-'.\n");
    return FAILURE;
  }

  if (flag[1] != PUBLISHER && flag[1] != BROKER && flag[1] != SUBSCRIBER) {
    fprintf(stderr, "ERROR: PARSE_FLAG - the second symbol of the flag should "
                    "be an identifier:\n\t'p' - publisher;\n\t'b' - "
                    "broker;\n\t's' - subscriber.\n");
    return FAILURE;
  }

  *mode = flag[1];

  return SUCCESS;
}

int start_corresponding_mode(char mode) {
  if (mode == PUBLISHER) {
    return do_publisher_activity();
  } else if (mode == BROKER) {
    return do_broker_activity();
  } else if (mode == SUBSCRIBER) {
    return do_subscriber_activity();
  }

  fprintf(stderr, "ERROR: START_CORRESPONDING_MODE - mode not found.\n");
  return FAILURE;
}