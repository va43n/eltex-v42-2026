#include "header.h"

int copy_files_with_ipc(size_t size) {
  pid_t *pids = (pid_t*)malloc(sizeof(pid_t) * size);
  int rv;

  for (size_t i = 0; i < size; i++) {
    switch (pids[i] = fork()) {
      case -1:
        perror("fork");
        exit(EXIT_FAILURE);
      case 0: /* Потомок */
        printf("CHILD: PID - %d\n", getpid());
        printf("CHILD: PPID - %d\n", getppid());
        _exit(EXIT_SUCCESS);
      default: /* Родитель */
        printf("PARENT: PID - %d\n", getpid());
        printf("PARENT: CHILD PID - %d\n", pids[i]);
        wait(&rv);
        printf("PARENT: RETURN STATUS FOR CHILD - %d\n", WEXITSTATUS(rv));
    }
  }

  return SUCCESS;
}