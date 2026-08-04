#include "p2p.h"

int main(int argc, char *argv[]) {
  int status;
  char *queue_name;
  mqd_t read, write;
  if (parse_input(argc, argv, &queue_name) == SUCCESS &&
      make_stdin_nonblocking() == SUCCESS) {
    if (!check_if_queue_is_created(queue_name, &status)) {
      if (create_queue(queue_name) == FAILURE)
        return 0;
    }
    if (connect_to_queue(queue_name, status, &read, &write) == SUCCESS) {
      start_handler(read, write);
      mq_close(read);
      mq_close(write);
      if (status == CREATOR) {
        delete_queue(queue_name);
      }
    }
  }

  return 0;
}