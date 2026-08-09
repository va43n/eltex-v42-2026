#include "socket_functions.h"

int socket_connect(my_socket *s) {
  if ((s->s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    fprintf(stderr, "ERROR: socket_connect - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  int broadcast = 1;
  if (setsockopt(s->s, SOL_SOCKET, SO_BROADCAST, &broadcast,
                 sizeof(broadcast)) < 0) {
    fprintf(stderr,
            "ERROR: socket_connect - cannot add broadcast permissions to "
            "socket.\n");
    perror("setsockopt");
    close(s->s);
    return FAILURE;
  }

  int reuse = 1;
  if (setsockopt(s->s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    fprintf(stderr,
            "ERROR: socket_connect - cannot add reusability to a socket.\n");
    perror("setsockopt");
    return FAILURE;
  }

  memset(&(s->recv_addr), 0, sizeof(s->recv_addr));
  s->recv_addr.sin_family = AF_INET;
  s->recv_addr.sin_port = htons(PORT);
  s->recv_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(s->s, (struct sockaddr *)&s->recv_addr, sizeof(s->recv_addr)) < 0) {
    fprintf(
        stderr,
        "ERROR: socket_connect - cannot bind the receive address to socket.\n");
    perror("bind");
    close(s->s);
    return FAILURE;
  }

  memset(&(s->broadcast_addr), 0, sizeof(s->broadcast_addr));
  s->broadcast_addr.sin_family = AF_INET;
  s->broadcast_addr.sin_port = htons(PORT);
  s->broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

  return SUCCESS;
}

int socket_disconnect(my_socket s) {
  close(s.s);

  return SUCCESS;
}

int socket_send(my_socket s) {
  char buffer[BUFFER_SIZE];

  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
    if (errno == EINTR) {
      is_signal = TRUE;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: socket_send - cannot build message for sending.\n");
    return FAILURE;
  }

  printf("\033[A\033[K");

  size_t len = strlen(buffer);
  if (len == 1 && buffer[0] == '\n') return SUCCESS;

  if (sendto(s.s, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&s.broadcast_addr,
             sizeof(s.broadcast_addr)) < 0) {
    fprintf(stderr, "ERROR: socket_send - cannot send message.\n");
    perror("sendto");
    return FAILURE;
  }

  return SUCCESS;
}

int socket_receive(my_socket s) {
  char buffer[BUFFER_SIZE];
  socklen_t addr_len = sizeof(s.recv_addr);
  int n = recvfrom(s.s, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&s.recv_addr,
                   &addr_len);
  if (n < 0) {
    fprintf(
        stderr,
        "ERROR: socket_receive - cannot properly receive someone's message.\n");
    perror("recvfrom");
    return FAILURE;
  }

  printf("%s> %s", inet_ntoa(s.recv_addr.sin_addr), buffer);

  return SUCCESS;
}
