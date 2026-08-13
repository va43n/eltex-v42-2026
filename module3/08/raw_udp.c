#include "raw_udp.h"

int search_for_udp() {
  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_raw_socket(&fd) == FAILURE) return FAILURE;

  while (is_signal == FALSE) {
    if (receive_data(fd) == FAILURE) return FAILURE;
  }

  return SUCCESS;
}

int create_raw_socket(int *fd) {
  if ((*fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
    fprintf(stderr, "ERROR: create_raw_socket - cannot create a socket.\n");
    perror("socket");
    return FAILURE;
  }

  return SUCCESS;
}

int receive_data(int fd) {
  unsigned char buffer[BUFFER_SIZE];
  struct sockaddr_in recv_addr;
  socklen_t addr_len = sizeof(recv_addr);
  int n = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr,
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

  struct iphdr *ip = (struct iphdr *)buffer;
  struct udphdr *udp = (struct udphdr *)(buffer + ip->ihl * 4);

  int bytes = 0;

  print_bytes(ip->ihl, BYTE, &bytes);
  print_bytes(ip->tos, BYTE, &bytes);
  print_bytes(ip->tot_len, BYTE * 2, &bytes);
  print_bytes(ip->id, BYTE * 2, &bytes);
  print_bytes(ip->frag_off, BYTE * 2, &bytes);
  print_bytes(ip->ttl, BYTE, &bytes);
  print_bytes(ip->protocol, BYTE, &bytes);
  print_bytes(ip->check, BYTE * 2, &bytes);
  print_bytes(ip->saddr, BYTE * 4, &bytes);
  print_bytes(ip->daddr, BYTE * 4, &bytes);

  print_bytes(udp->source, BYTE * 2, &bytes);
  print_bytes(udp->dest, BYTE * 2, &bytes);
  print_bytes(udp->len, BYTE * 2, &bytes);
  print_bytes(udp->check, BYTE * 2, &bytes);

  size_t ip_len = 20;
  size_t udp_len = ntohs(udp->len);
  size_t max_data_size =
      (udp_len > BUFFER_SIZE ? BUFFER_SIZE : udp_len) - ip_len - 8;

  for (size_t i = 0; i < max_data_size; i++) {
    print_bytes(buffer[20 + 8 + i] - '0', BYTE, &bytes);
  }

  printf("\n\n");

  return SUCCESS;
}

void print_bytes(int value, int number_of_bits, int *bytes) {
  int mask = 0xFF;
  int temp;

  for (int i = 0; i < number_of_bits / BYTE; i++) {
    temp = value;
    for (int j = number_of_bits / BYTE - i; j < number_of_bits / BYTE; j++)
      temp >>= BYTE;
    temp = temp & mask;
    printf("%02x ", temp);

    (*bytes)++;
    check_number_of_bytes_in_line(bytes);
  }
}

void check_number_of_bytes_in_line(int *bytes) {
  if (*bytes == 8)
    printf(" ");
  else if (*bytes == 16) {
    printf("\n");
    *bytes = 0;
  }
}