#include "header.h"

int copy_files_with_ipc(char** file_names, char** pipe_names, size_t size) {
  // int ppid = getpid();
  pid_t* pids = (pid_t*)malloc(sizeof(pid_t) * size);
  size_t pipes_size, fifos_size;
  size_t** mask;

  find_out_what_channels_to_create(pipe_names, size, &mask, &pipes_size,
                                   &fifos_size);
  int** pipes_pc = (int**)malloc(sizeof(int*) * pipes_size);
  int** pipes_cp = (int**)malloc(sizeof(int*) * pipes_size);
  char** fifos_pc = (char**)malloc(sizeof(char*) * fifos_size);
  char** fifos_cp = (char**)malloc(sizeof(char*) * fifos_size);

  size_t pipes_i = 0, fifos_i = 0;
  for (size_t i = 0; i < size; i++) {
    size_t len = strlen(pipe_names[i]);
    if (len == 0) {
      pipes_pc[pipes_i] = (int*)malloc(sizeof(int) * 2);
      if (pipe(pipes_pc[pipes_i]) == -1) {
        perror("pipe pc");
        exit(EXIT_FAILURE);
      }

      pipes_cp[pipes_i] = (int*)malloc(sizeof(int) * 2);
      if (pipe(pipes_cp[pipes_i]) == -1) {
        perror("pipe cp");
        exit(EXIT_FAILURE);
      }

      pipes_i++;
    } else {
      fifos_pc[fifos_i] = (char*)malloc(sizeof(char) * (len + 4));
      fifos_cp[fifos_i] = (char*)malloc(sizeof(char) * (len + 4));
      sprintf(fifos_pc[fifos_i], "%s_%s", pipe_names[i], "pc");
      sprintf(fifos_cp[fifos_i], "%s_%s", pipe_names[i], "cp");

      if (mkfifo(fifos_pc[fifos_i], 0666) == -1) {
        perror("mkfifo pc");
        exit(EXIT_FAILURE);
      }
      if (mkfifo(fifos_cp[fifos_i], 0666) == -1) {
        perror("mkfifo cp");
        exit(EXIT_FAILURE);
      }

      fifos_i++;
    }
  }
  int rv;

  for (size_t i = 0; i < size; i++) {
    switch (pids[i] = fork()) {
      case -1:
        perror("fork");
        exit(EXIT_FAILURE);
      case 0:
        // char write_buf[BUFFER];
        char read_buf[BUFFER];
        int fd_in, fd_out;

        if (mask[i][0] == PIPE) {
          close(pipes_cp[mask[i][1]][0]);
          close(pipes_pc[mask[i][1]][1]);

          fd_in = pipes_pc[mask[i][1]][0];
          fd_out = pipes_cp[mask[i][1]][1];
        } else {
          fd_in = open(fifos_pc[i], O_RDONLY);
          fd_out = open(fifos_cp[i], O_WRONLY);
        }

        write(fd_out, "i'm ready", strlen("i'm ready") + 1);
        read(fd_in, read_buf, sizeof(read_buf));
        write(fd_out, "i'm ready", strlen("i'm ready") + 1);

        char* new_file_name =
            (char*)malloc(sizeof(char) * (strlen(read_buf) + 6));
        strcpy(new_file_name, read_buf);
        strcat(new_file_name, ".copy");

        int fd = open(new_file_name, O_WRONLY | O_CREAT | O_TRUNC,
                      S_IRUSR | S_IWUSR);

        while (TRUE) {
          ssize_t bytes_read = read(fd_in, read_buf, sizeof(read_buf));
          if (strcmp(read_buf, STOP_MESSAGE) == 0) break;
          // if (bytes_read < BUFFER) read_buf[bytes_read] = '\0';

          write(fd, read_buf, bytes_read);
        }
        close(fd);

        _exit(EXIT_SUCCESS);
    }
  }

  for (size_t i = 0; i < size; i++) {
    char write_buf[BUFFER];
    char read_buf[BUFFER];
    int fd_in, fd_out;

    if (mask[i][0] == PIPE) {
      close(pipes_pc[mask[i][1]][0]);
      close(pipes_cp[mask[i][1]][1]);

      fd_in = pipes_cp[mask[i][1]][0];
      fd_out = pipes_pc[mask[i][1]][1];
    } else {
      fd_out = open(fifos_pc[i], O_WRONLY);
      fd_in = open(fifos_cp[i], O_RDONLY);
    }

    read(fd_in, read_buf, sizeof(read_buf));
    write(fd_out, file_names[i], strlen(file_names[i]) + 1);
    read(fd_in, read_buf, sizeof(read_buf));

    int fd = open(file_names[i], O_RDONLY);
    ssize_t bytes_read;
    while ((bytes_read = read(fd, write_buf, BUFFER)) > 0) {
      write(fd_out, write_buf, bytes_read);
    }
    write(fd_out, STOP_MESSAGE, strlen(STOP_MESSAGE) + 1);
    close(fd);
  }

  for (size_t i = 0; i < size; i++) {
    wait(&rv);
  }

  return SUCCESS;
}

int find_out_what_channels_to_create(char** pipe_names, size_t size,
                                     size_t*** mask, size_t* pipes_size,
                                     size_t* fifos_size) {
  *mask = (size_t**)malloc(sizeof(size_t*) * size);
  *pipes_size = 0;
  *fifos_size = 0;
  for (size_t i = 0; i < size; i++) {
    (*mask)[i] = (size_t*)malloc(sizeof(size_t) * 2);
    if (strlen(pipe_names[i]) == 0) {
      (*mask)[i][0] = PIPE;
      (*mask)[i][1] = *pipes_size;
      (*pipes_size)++;
    } else {
      (*mask)[i][0] = FIFO;
      (*mask)[i][1] = *fifos_size;
      (*fifos_size)++;
    }
  }

  return SUCCESS;
}