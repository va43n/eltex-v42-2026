#include "header.h"

int main(int argc, char* argv[]) {
  char **file_names, **pipe_names;
  size_t size;

  if (parse_input(argc, argv, &file_names, &pipe_names, &size) == SUCCESS) {
    for (size_t i = 0; i < size; i++) {
      printf("'%s': '%s'\n", file_names[i], pipe_names[i]);
    }

    copy_files_with_ipc(file_names, pipe_names, size);
  }

  return 0;
}