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

int create_raw_socket(int* fd) {
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
  int n = recvfrom(fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&recv_addr,
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

  int bytes = 0;
  size_t cur_pos = 0;

  print_bytes_char(buffer, &cur_pos, 1, &bytes);
  print_bytes_char(buffer, &cur_pos, 1, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 1, &bytes);
  print_bytes_char(buffer, &cur_pos, 1, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 4, &bytes);
  print_bytes_char(buffer, &cur_pos, 4, &bytes);

  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);
  print_bytes_char(buffer, &cur_pos, 2, &bytes);

  struct iphdr* ip = (struct iphdr*)buffer;
  unsigned int ihl = ip->ihl * 4;

  struct udphdr* udp = (struct udphdr*)(buffer + ihl);
  unsigned int udp_len = ntohs(udp->len),
               udp_data = udp_len - sizeof(struct udphdr);
  size_t max_data_len = n - sizeof(struct udphdr) > udp_data
                            ? udp_data
                            : n - sizeof(struct udphdr);

  cur_pos = ihl + sizeof(struct udphdr);
  for (size_t i = 0; i < max_data_len; i++) {
    print_bytes_char(buffer, &cur_pos, 1, &bytes);
  }

  printf("\n\n");

  return SUCCESS;
}

void print_bytes_char(unsigned char* buffer, size_t* start_pos,
                      size_t number_of_bytes, int* bytes) {
  for (size_t i = *start_pos; i < *start_pos + number_of_bytes; i++) {
    int num = buffer[i] - '\0';
    printf("%02x ", num);

    (*bytes)++;
    check_number_of_bytes_in_line(bytes);
  }
  *start_pos += number_of_bytes;
}

void check_number_of_bytes_in_line(int* bytes) {
  if (*bytes == 8)
    printf(" ");
  else if (*bytes == 16) {
    printf("\n");
    *bytes = 0;
  }
}