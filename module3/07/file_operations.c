#include "file_operations.h"

int check_if_file_exists(char* buffer) {
  struct stat path_stat;
  if (stat(buffer, &path_stat) != 0) {
    return FALSE;
  }
  return S_ISREG(path_stat.st_mode);
}

int read_user_input(char* buffer) {
  memset(buffer, 0, BUFFER_SIZE);
  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    fprintf(stderr, "ERROR: read_user_input - cannot get user message.\n");
    return FAILURE;
  }

  printf("\033[A\033[K");

  return SUCCESS;
}

int read_another_bytes_of_file(char* file_name, ssize_t* bytes, char* buffer,
                               ssize_t* actual_number_of_bytes,
                               int* is_file_ended) {
  if (check_if_file_exists(file_name) == FALSE) {
    printf("File %s doesn't exist.\n", file_name);
    return FILE_IS_NOT_EXISTS;
  }

  int fd = open(file_name, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr,
            "ERROR: read_another_bytes_of_file - cannot open a file.\n");
    perror("open");
    return FILE_IS_NOT_EXISTS;
  }

  if (lseek(fd, (off_t)(*bytes), SEEK_SET) == -1) {
    fprintf(
        stderr,
        "ERROR: read_another_bytes_of_file - cannot set position in file.\n");
    perror("lseek");
    close(fd);
    return FAILURE;
  }

  *actual_number_of_bytes = read(fd, buffer, BUFFER_SIZE);
  if (*actual_number_of_bytes == -1) {
    fprintf(
        stderr,
        "ERROR: read_another_bytes_of_file - cannot read bytes from file.\n");
    perror("read");
    close(fd);
    return FAILURE;
  }
  if (*actual_number_of_bytes == 0) *is_file_ended = TRUE;

  *bytes += *actual_number_of_bytes;

  close(fd);

  return SUCCESS;
}