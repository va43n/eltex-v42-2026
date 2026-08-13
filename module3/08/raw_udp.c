#include "raw_udp.h"

int search_for_udp(int mask) {
  int fd;

  struct sigaction sa;
  sa.sa_handler = handle_SIGINT;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);

  if (create_raw_socket(&fd) == FAILURE) return FAILURE;

  time_t start;
  time(&start);
  while (is_signal == FALSE) {
    if (receive_data(fd, start, mask) == FAILURE) return FAILURE;
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

int receive_data(int fd, time_t start, int mask) {
  static size_t packet_number = 1;

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

  struct iphdr* ip = (struct iphdr*)buffer;
  unsigned int ihl = ip->ihl * 4;
  struct udphdr* udp = (struct udphdr*)(buffer + ihl);

  if (mask & FILTER_06_MASK && (ntohs(udp->source) != FILTER_06_PORT ||
                                ntohs(udp->dest) != FILTER_06_PORT))
    return SUCCESS;

  if (mask & FILTER_DNS_MASK && (ntohs(udp->source) != FILTER_DNS_PORT &&
                                 ntohs(udp->dest) != FILTER_DNS_PORT))
    return SUCCESS;

  time_t end;
  time(&end);

  FILE* file = stdout;
  if (mask & PUT_IN_FILE_MASK) {
    char file_name[FILENAME_SIZE];
    sprintf(file_name, "%ld_%ld.udp", packet_number, (size_t)end);
    file = fopen(file_name, "w");
  }

  char ip_buf[IPV4_LENGTH];
  fprintf(file, "Received after %.0f seconds\n", difftime(end, start));
  fprintf(file, "%s:%d -> %s:%d\n", create_ip_string(ip_buf, ntohl(ip->saddr)),
          ntohs(udp->source), create_ip_string(ip_buf, ntohl(ip->daddr)),
          ntohs(udp->dest));

  int total_length = ntohs(ip->tot_len);
  size_t max_data_len = n > total_length ? total_length : n;

  for (size_t i = 0; i < max_data_len; i++) {
    print_bytes_char(file, buffer, &cur_pos, 1, &bytes);
  }

  if (!(mask & PUT_IN_FILE_MASK))
    printf("\n\n");
  else
    fclose(file);

  packet_number++;

  return SUCCESS;
}

void print_bytes_char(FILE* file, unsigned char* buffer, size_t* start_pos,
                      size_t number_of_bytes, int* bytes) {
  for (size_t i = *start_pos; i < *start_pos + number_of_bytes; i++) {
    int num = buffer[i] - '\0';
    fprintf(file, "%02x ", num);

    (*bytes)++;
    check_number_of_bytes_in_line(file, bytes);
  }
  *start_pos += number_of_bytes;
}

void check_number_of_bytes_in_line(FILE* file, int* bytes) {
  if (*bytes == 8)
    fprintf(file, " ");
  else if (*bytes == 16) {
    fprintf(file, "\n");
    *bytes = 0;
  }
}

char* create_ip_string(char* ip, int ip_int) {
  memset(ip, 0, IPV4_LENGTH);
  sprintf(ip, "%u.%u.%u.%u", (ip_int >> 24) & 0xFF, (ip_int >> 16) & 0xFF,
          (ip_int >> 8) & 0xFF, ip_int & 0xFF);

  return ip;
}