#include <sys/select.h>

#include "group_chat.h"

int do_client_activity(char* address) {
  my_socket s;

  struct sigaction sa;
  sa.sa_handler = handle_any_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTSTP, &sa, NULL);

  printf("I'm a client and the server is: %s\n", address);

  if (socket_connect(&s, address) == FAILURE) return FAILURE;

  fd_set rfds, rfds_start;
  int maxfd = (s.s > STDIN_FILENO ? s.s : STDIN_FILENO) + 1;
  FD_ZERO(&rfds);
  FD_SET(STDIN_FILENO, &rfds_start);
  FD_SET(s.s, &rfds_start);

  if (send_welcome_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (send_welcome_message)\n");
    return FAILURE;
  }

  char file_name[BUFFER_SIZE];
  int sending_the_file = FALSE;
  ssize_t bytes = 0;
  int is_file_ended = TRUE;

  while (is_signal != SIGINT) {
    if (is_signal == SIGTSTP) {
      if (sending_the_file == TRUE) {
        printf("File sending is stopped\n");
        sending_the_file = FALSE;
      } else {
        printf("Enter file name: ");
        sending_the_file = TRUE;
      }
      is_signal = FALSE;
    }
    if (sending_the_file == TRUE && is_file_ended == FALSE) {
      if (send_part_of_file(s, file_name, &bytes, &is_file_ended,
                            &sending_the_file) == FAILURE)
        return FAILURE;
      continue;
    }

    rfds = rfds_start;

    int ret = select(maxfd, &rfds, NULL, NULL, NULL);
    if (ret == -1) {
      if (errno == EINTR) continue;
      fprintf(stderr, "ERROR: do_client_activity (select).\n");
      perror("select");
      break;
    }

    if (FD_ISSET(STDIN_FILENO, &rfds)) {
      if (sending_the_file == TRUE) {
        if (read_user_input(file_name) == FAILURE) return FAILURE;
        file_name[strlen(file_name) - 1] = '\0';
        is_file_ended = FALSE;
      } else if (socket_send(s, NULL, 0, TEXT, NULL) == FAILURE)
        break;
    }

    if (FD_ISSET(s.s, &rfds)) {
      message msg;
      if (socket_receive(s, &msg) == FAILURE) break;
      if (msg.mode == FILE) {
        int fd = open(msg.file_name, O_WRONLY | O_CREAT | O_APPEND,
                      S_IRUSR | S_IWUSR);
        write(fd, msg.m, msg.data_len);
        close(fd);
        printf("%ld bytes of data was received for %s.\n", msg.data_len,
               msg.file_name);
      } else if (msg.mode == DISCONNECT) {
        printf("Server has closed the connection...\n");
        return FAILURE;
      } else
        printf("%s", msg.m);
    }
  }

  if (send_goodbye_message(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (send_goodbye_message)\n");
    return FAILURE;
  }

  if (socket_disconnect(s) == FAILURE) {
    fprintf(stderr, "ERROR: do_client_activity (socket_disconnect)\n");
    return FAILURE;
  }

  return SUCCESS;
}

int send_welcome_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "New user is joined...\n");
  return socket_send(s, buffer, BUFFER_SIZE, CONNECT, NULL);
}

int send_goodbye_message(my_socket s) {
  char buffer[BUFFER_SIZE];
  strcpy(buffer, "User is disconnected...\n");
  return socket_send(s, buffer, BUFFER_SIZE, DISCONNECT, NULL);
}

int send_part_of_file(my_socket s, char* file_name, ssize_t* bytes,
                      int* is_file_ended, int* sending_the_file) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  int res = read_another_bytes_of_file(file_name, bytes, buffer, &bytes_read,
                                       is_file_ended);
  if (res == FAILURE)
    return FAILURE;
  else if (res == FILE_IS_NOT_EXISTS) {
    *sending_the_file = FALSE;
    *is_file_ended = TRUE;
    *bytes = 0;
    return SUCCESS;
  }
  if (*is_file_ended == TRUE) {
    *sending_the_file = FALSE;
    *bytes = 0;
    return SUCCESS;
  }
  if (socket_send(s, buffer, (size_t)bytes_read, FILE, file_name) == FAILURE)
    return FAILURE;

  printf("%ld bytes of the file are sent...\n", bytes_read);

  return SUCCESS;
}
