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
  struct in_addr *addr = (struct in_addr *)server->h_addr_list[0];

  memset(&(s->server_addr), 0, sizeof(s->server_addr));
  s->server_addr.sin_family = AF_INET;
  s->server_addr.sin_port = htons(PORT);
  s->server_addr.sin_addr.s_addr = addr;

  if (connect(s->s, (struct sockaddr *)&(s->server_addr),
              sizeof(s->server_addr)) < 0) {
    fprintf(stderr, "ERROR: socket_connect - cannot coonect to a server.\n");
    perror("connect");
    return FAILURE;
  }

  return SUCCESS;
}

int socket_disconnect(my_socket s) {
  close(s.s);

  return SUCCESS;
}

int socket_send(my_socket s, char *buffer, size_t len) {
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

  if (send(s.s, buffer, len, 0) < 0) {
    fprintf(stderr, "ERROR: socket_send - cannot send message.\n");
    perror("send");
    return FAILURE;
  }

  return SUCCESS;
}

int socket_receive(my_socket s) {
  message msg;
  int n = recv(s.s, &msg, BUFFER_SIZE, 0);
  if (n < 0) {
    fprintf(
        stderr,
        "ERROR: socket_receive - cannot properly receive someone's message.\n");
    perror("recvfrom");
    return FAILURE;
  }

  printf("%s> %s", inet_ntoa(msg.addr.sin_addr), msg.m);

  return SUCCESS;
}
