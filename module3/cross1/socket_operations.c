#include "echo_reply.h"

int create_socket(int *fd) {
  if ((*fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "ERROR: create_raw_socket - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  int broadcast = 1;
  if (setsockopt(*fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) <
      0) {
    fprintf(stderr,
            "ERROR: socket_connect - cannot add broadcast permissions to "
            "socket.\n");
    perror("setsockopt");
    close(*fd);
    return FAILURE;
  }

  return SUCCESS;
}

int receive_data(int fd, unsigned int source_address,
                 unsigned int destination_address) {
  message msg;
  struct sockaddr_in recv_addr;
  socklen_t addr_len = sizeof(recv_addr);
  int n = recvfrom(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&recv_addr,
                   &addr_len);
  if (n < 0) {
    if (errno == EINTR) {
      fprintf(stderr, "ERROR: receive_data - interrupted by signal.\n");
      is_signal = TRUE;
      return FAILURE;
    }
    fprintf(stderr,
            "ERROR: receive_data - cannot properly receive some data.\n");
    perror("recvfrom");
    return FAILURE;
  }

  // int bytes = 0;
  // size_t cur_pos = 0;

  // printf("Source port: %d\n", msg.source_port);

  struct iphdr *ip = (struct iphdr *)msg.buffer;
  unsigned int ihl = ip->ihl * 4;
  struct udphdr *udp = (struct udphdr *)(msg.buffer + ihl);

  FILE *file = stdout;

  char ip_buf[IPV4_LENGTH];
  fprintf(file, "%s:%d -> %s:%d (expected %s -> %s)\n",
          create_ip_string(ip_buf, ntohl(ip->saddr)), ntohs(udp->source),
          create_ip_string(ip_buf, ntohl(ip->daddr)), ntohs(udp->dest),
          create_ip_string(ip_buf, source_address),
          create_ip_string(ip_buf, destination_address));

  if ((ntohl(ip->saddr) != source_address && source_address != 0) ||
      (ntohl(ip->daddr) != destination_address && destination_address != 0))
    return SUCCESS;

  fprintf(file, "RECEIVED!!!!!\n");

  // int total_length = ntohs(ip->tot_len);
  // size_t max_data_len = n > total_length ? total_length : n;

  // for (size_t i = 0; i < max_data_len; i++) {
  //   print_bytes_char(file, buffer, &cur_pos, 1, &bytes);
  // }

  return SUCCESS;
}

int send_data(int fd, int destination_port, int source_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));

  if (fgets(msg.buffer, BUFFER_SIZE, stdin) == NULL) {
    if (errno == EINTR) {
      is_signal = TRUE;
      return FAILURE;
    }
    fprintf(stderr, "ERROR: send_data - cannot build message for sending.\n");
    return FAILURE;
  }
  msg.source_port = source_port;

  printf("\033[A\033[K");

  size_t real_len = strlen(msg.buffer);
  if (real_len == 1 && msg.buffer[0] == '\n')
    return SUCCESS;

  struct sockaddr_in addr;
  memset(&(addr), 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(destination_port);
  addr.sin_addr.s_addr = INADDR_BROADCAST;

  if (sendto(fd, &msg, sizeof(msg), 0, (struct sockaddr *)&addr, sizeof(addr)) <
      0) {
    fprintf(stderr, "ERROR: send_data - cannot send message.\n");
    perror("sendto");
    return FAILURE;
  }

  return SUCCESS;
}

char *create_ip_string(char *ip, int ip_int) {
  memset(ip, 0, IPV4_LENGTH);
  sprintf(ip, "%u.%u.%u.%u", (ip_int >> 24) & 0xFF, (ip_int >> 16) & 0xFF,
          (ip_int >> 8) & 0xFF, ip_int & 0xFF);

  return ip;
}