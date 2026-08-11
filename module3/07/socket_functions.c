#include "socket_functions.h"

int socket_connect(my_socket *s, char *address) {
  if ((s->s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "ERROR: socket_connect - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  struct hostent *server = gethostbyname(address);
  if (server == NULL) {
    fprintf(stderr,
            "ERROR: parse_server_address - cannot parse server address.\n");
    herror("gethostbyname");
    return FAILURE;
  }

  memset(&(s->server_addr), 0, sizeof(s->server_addr));
  s->server_addr.sin_family = AF_INET;
  s->server_addr.sin_port = htons(PORT);
  memcpy(&(s->server_addr.sin_addr.s_addr), server->h_addr_list[0],
         server->h_length);

  if (connect(s->s, (struct sockaddr *)&(s->server_addr),
              sizeof(s->server_addr)) < 0) {
    fprintf(stderr, "ERROR: socket_connect - cannot connect to a server.\n");
    perror("connect");
    return FAILURE;
  }

  return SUCCESS;
}

int socket_listen(my_socket *s) {
  if ((s->s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "ERROR: socket_listen - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  memset(&(s->server_addr), 0, sizeof(s->server_addr));
  s->server_addr.sin_family = AF_INET;
  s->server_addr.sin_port = htons(PORT);
  s->server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(s->s, (struct sockaddr *)&(s->server_addr), sizeof(s->server_addr)) <
      0) {
    fprintf(stderr, "ERROR: socket_listen - cannot create a server.\n");
    perror("connect");
    return FAILURE;
  }

  listen(s->s, SERVER_USERS);

  return SUCCESS;
}

int socket_accept(my_socket s, char **address, int *client_fd) {
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(struct sockaddr_in);
  *client_fd = accept(s.s, (struct sockaddr *)&(cli_addr), &clilen);

  if (*client_fd < 0) {
    fprintf(stderr, "ERROR: socket_accept - Cannot accept new client.\n");
    return FAILURE;
  }

  struct hostent *host = gethostbyaddr((char *)&cli_addr.sin_addr, 4, AF_INET);
  if (host == NULL) {
    fprintf(stderr, "ERROR: socket_accept - unknown client address.\n");
    herror("gethostbyaddr");
    return FAILURE;
  }
  strcpy(*address, inet_ntoa(cli_addr.sin_addr));

  return SUCCESS;
}

int socket_close_server(int *client_fds, size_t nfds) {
  for (size_t i = 0; i < nfds; i++) {
    close(client_fds[i]);
  }

  return SUCCESS;
}

int socket_disconnect(my_socket s) {
  close(s.s);

  return SUCCESS;
}

int socket_send(my_socket s, char *buffer, size_t len, int message_type) {
  if (len == 0) {
    char real_buffer[BUFFER_SIZE];

    if (fgets(real_buffer, BUFFER_SIZE, stdin) == NULL) {
      if (errno == EINTR) {
        is_signal = TRUE;
        return FAILURE;
      }
      fprintf(stderr,
              "ERROR: socket_send - cannot build message for sending.\n");
      return FAILURE;
    }

    printf("\033[A\033[K");

    size_t real_len = strlen(real_buffer);
    if (real_len == 1 && real_buffer[0] == '\n') return SUCCESS;

    len = real_len;
    buffer = real_buffer;
  }

  message msg;
  strcpy(msg.m, buffer);
  msg.mode = message_type;

  if (send(s.s, &msg, sizeof(msg), 0) < 0) {
    fprintf(stderr, "ERROR: socket_send - cannot send message.\n");
    perror("send");
    return FAILURE;
  }

  return SUCCESS;
}

int socket_receive(int s, message *msg) {
  int n = recv(s, msg, sizeof(message), 0);
  if (n < 0) {
    fprintf(
        stderr,
        "ERROR: socket_receive - cannot properly receive someone's message.\n");
    perror("recv");
    return FAILURE;
  }

  return SUCCESS;
}
