#include "group_chat.h"

int do_server_activity() {
  my_socket listen_socket;

  signal(SIGINT, handle_SIGINT);
  printf("I'm a server\n");

  if (socket_listen(&listen_socket) == FAILURE) return FAILURE;

  struct pollfd* fds;
  size_t nfds = 1, actual_size = 1;
  fds = (struct pollfd*)malloc(sizeof(struct pollfd) * nfds);

  fds[0].fd = listen_socket.s;
  fds[0].events = POLLIN;

  char** addresses = NULL;
  my_socket* client_fds = NULL;

  int ret;
  while (!is_signal) {
    ret = poll(fds, nfds, -1);
    if (!ret) {
      fprintf(stderr,
              "ERROR: do_server_activity - something when wrong while waiting "
              "for some activity.\n");
      return FAILURE;
    }

    if (fds[0].revents & POLLIN) {
      char* address = malloc(sizeof(char) * IPV4_STR_LENGTH);
      int client_fd;
      printf("Someone is trying to connect...\n");
      if (socket_accept(listen_socket, &address, &client_fd) == FAILURE) {
        socket_disconnect(listen_socket);
        free_clients(addresses, client_fds, fds, nfds);
        return FAILURE;
      }
      add_client(&addresses, &client_fds, &fds, &nfds, &actual_size, address,
                 client_fd);
      free(address);
    }
    size_t i = 1;
    while (i < nfds) {
      if (fds[i].revents & POLLIN) {
        message msg;
        if (socket_receive(client_fds[i - 1], &msg) == SUCCESS) {
          char new_message[BUFFER_SIZE + IPV4_STR_LENGTH + 2];
          snprintf(new_message, BUFFER_SIZE + IPV4_STR_LENGTH + 2, "%s> %s",
                   addresses[i - 1], msg.m);
          printf("Client[%ld] (%s, %d) said: %s", i, addresses[i - 1],
                 client_fds[i - 1].s, msg.m);
          for (size_t i = 1; i < nfds; i++) {
            if (socket_send(client_fds[i - 1], new_message, strlen(new_message),
                            TEXT) == FAILURE) {
              socket_disconnect(listen_socket);
              free_clients(addresses, client_fds, fds, nfds);
              return FAILURE;
            }
          }
        }
        if (msg.mode == DISCONNECT) {
          close(fds[i].fd);
          remove_client(&addresses, &client_fds, &fds, &nfds, i - 1);
          continue;
        }
      }
      i++;
    }
  }

  socket_disconnect(listen_socket);
  free_clients(addresses, client_fds, fds, nfds);

  return SUCCESS;
}

void add_client(char*** addresses, my_socket** client_fds, struct pollfd** fds,
                size_t* nfds, size_t* actual_size, char* address,
                int client_fd) {
  (*nfds)++;
  size_t new_index = *nfds - 1 - 1;
  if (*actual_size <= new_index + 1) {
    (*actual_size)++;

    char** tmp =
        (char**)realloc(*addresses, sizeof(char*) * (*actual_size - 1));
    *addresses = tmp;
    (*addresses)[new_index] = (char*)malloc(sizeof(char) * IPV4_STR_LENGTH);

    my_socket* tmp2 = (my_socket*)realloc(
        *client_fds, sizeof(my_socket) * (*actual_size - 1));
    *client_fds = tmp2;

    struct pollfd* tmp3 =
        (struct pollfd*)realloc(*fds, sizeof(struct pollfd) * *actual_size);
    *fds = tmp3;
  }

  (*addresses)[new_index] = (char*)malloc(IPV4_STR_LENGTH);
  strcpy((*addresses)[new_index], address);

  (*client_fds)[new_index].s = client_fd;

  (*fds)[new_index + 1].fd = client_fd;
  (*fds)[new_index + 1].events = POLLIN;
}

void remove_client(char*** addresses, my_socket** client_fds,
                   struct pollfd** fds, size_t* nfds, size_t index) {
  if (*nfds <= index + 1) return;
  close((*fds)[index + 1].fd);
  free((*addresses)[index]);
  for (size_t i = index; i < *nfds - 2; i++) {
    (*addresses)[i] = (*addresses)[i + 1];
    (*client_fds)[i].s = (*client_fds)[i + 1].s;
    (*fds)[i + 1] = (*fds)[i + 2];
  }
  (*nfds)--;
}

void free_clients(char** addresses, my_socket* client_fds, struct pollfd* fds,
                  size_t nfds) {
  if (nfds - 1 != 0) {
    close(fds[0].fd);
    for (size_t i = 0; i < nfds - 1; i++) {
      close(fds[i + 1].fd);
      free(addresses[i]);
    }
    free(addresses);
    free(client_fds);
    free(fds);
  }
}