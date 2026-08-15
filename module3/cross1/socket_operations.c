#include "echo_reply.h"

int create_socket(int *fd) {
  if ((*fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "ERROR: create_raw_socket - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  int val = 1;
  if (setsockopt(*fd, IPPROTO_IP, IP_HDRINCL, &val, sizeof(val)) < 0) {
    fprintf(stderr, "ERROR: socket_connect - cannot add permissions to "
                    "socket.\n");
    perror("setsockopt");
    close(*fd);
    return FAILURE;
  }

  return SUCCESS;
}

int receive_data(int fd, char *data, char *source_address,
                 char *destination_address, uint16_t *source_port,
                 uint16_t *destination_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));
  memset(data, 0, BUFFER_SIZE);

  socklen_t socklen;
  int n = recvfrom(fd, &(msg.buffer), BUFFER_SIZE, 0,
                   (struct sockaddr *)&(msg.addr), &(socklen));
  if (n < 0) {
    if (errno == EINTR) {
      fprintf(stderr, "receive_data - interrupted by signal.\n");
      return FAILURE;
    }
    fprintf(stderr,
            "ERROR: receive_data - cannot properly receive some data.\n");
    perror("recvfrom");
    return FAILURE;
  }

  struct iphdr *ip = (struct iphdr *)msg.buffer;
  unsigned int ihl = ip->ihl * 4;
  struct udphdr *udp = (struct udphdr *)(msg.buffer + ihl);

  if (filter_udp_packets(ip, udp, source_address, destination_address,
                         *source_port, *destination_port) == EMPTY)
    return EMPTY;

  char ip_buf[INET_ADDRSTRLEN];
  strcpy(data, msg.buffer + ihl + sizeof(struct udphdr));
  strcpy(source_address, create_ip_string(ip_buf, ntohl(ip->saddr)));
  strcpy(destination_address, create_ip_string(ip_buf, ntohl(ip->daddr)));
  *source_port = ntohs(udp->source);
  *destination_port = ntohs(udp->dest);

  return SUCCESS;
}

char *create_ip_string(char *ip, uint32_t ip_int) {
  memset(ip, 0, INET_ADDRSTRLEN);
  sprintf(ip, "%u.%u.%u.%u", (ip_int >> 24) & 0xFF, (ip_int >> 16) & 0xFF,
          (ip_int >> 8) & 0xFF, ip_int & 0xFF);

  return ip;
}

int filter_udp_packets(struct iphdr *ip, struct udphdr *udp,
                       char *source_address, char *destination_address,
                       uint16_t source_port, uint16_t destination_port) {
  uint32_t source_address_int = inet_addr(source_address),
           destination_address_int = inet_addr(destination_address);

  if ((ip->saddr != source_address_int && source_address_int != 0))
    return EMPTY;
  if ((ip->daddr != destination_address_int && destination_address_int != 0))
    return EMPTY;
  if ((ntohs(udp->source) != source_port && source_port != 0))
    return EMPTY;
  if ((ntohs(udp->dest) != destination_port && destination_port != 0))
    return EMPTY;

  return SUCCESS;
}

int get_input(char *buffer, size_t *len) {
  if (*len == 0) {
    memset(buffer, 0, BUFFER_SIZE);

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      if (errno == EINTR) {
        fprintf(stderr, "get_input - interrupted by signal.\n");
        return FAILURE;
      }
      fprintf(stderr, "ERROR: get_input - cannot build message for sending.\n");
      return FAILURE;
    }

    printf("\033[A\033[K");
  }
  size_t real_len = strlen(buffer);
  if (real_len == 1 && buffer[0] == '\n')
    return EMPTY;

  *len = real_len;
  if (buffer[*len - 1] == '\n')
    buffer[*len - 1] = '\0';

  return SUCCESS;
}

int send_data(int fd, char *data, size_t len, char *source_address,
              char *destination_address, uint16_t source_port,
              uint16_t destination_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));

  if (data)
    strcpy(msg.buffer, data);

  int res = get_input(msg.buffer, &len);
  if (res == FAILURE)
    return FAILURE;
  if (res == EMPTY)
    return SUCCESS;

  msg = build_message(msg.buffer, source_address, destination_address,
                      source_port, destination_port);

  if (sendto(fd, &(msg.buffer), msg.buffer_len, 0,
             (struct sockaddr *)&(msg.addr), msg.addr_len) < 0) {
    fprintf(stderr, "ERROR: send_data - cannot send message.\n");
    perror("sendto");
    return FAILURE;
  }

  return SUCCESS;
}

message build_message(char *message_text, char *source_address,
                      char *destination_address, uint16_t source_port,
                      uint16_t destination_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));

  size_t message_len = strlen(message_text), ip_len = sizeof(struct iphdr),
         udp_len = sizeof(struct udphdr);
  msg.buffer_len = ip_len + udp_len + message_len;

  struct iphdr *ip = (struct iphdr *)msg.buffer;
  ip->version = 4;
  ip->ihl = 5;
  ip->tot_len = htons(msg.buffer_len);
  ip->id = htons(source_port);
  ip->ttl = 64;
  ip->protocol = IPPROTO_UDP;
  ip->saddr = inet_addr(source_address);
  ip->daddr = inet_addr(destination_address);

  struct udphdr *udp = (struct udphdr *)(msg.buffer + ip_len);
  udp->source = htons(source_port);
  udp->dest = htons(destination_port);
  udp->len = htons(udp_len + message_len);

  memcpy(msg.buffer + ip_len + udp_len, message_text, message_len);

  msg.addr_len = sizeof(msg.addr);
  msg.addr.sin_family = AF_INET;
  msg.addr.sin_port = udp->dest;
  msg.addr.sin_addr.s_addr = ip->daddr;

  return msg;
}

int close_socket(int fd) {
  if (close(fd) < 0) {
    fprintf(stderr, "ERROR: close_socket - cannot close the socket.\n");
    perror("close");
    return FAILURE;
  }

  return SUCCESS;
}