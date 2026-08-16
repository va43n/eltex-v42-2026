#include "raw_socket_operations.h"

int raw_socket_send_data(int fd, const char *const data, size_t len,
                         char message_type, char *source_address,
                         char *destination_address, uint16_t source_port,
                         uint16_t destination_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));

  if (data) strcpy(msg.buffer, data);

  int res = _raw_socket_get_input(msg.buffer, &len);
  if (res == FAILURE) return FAILURE;
  if (res == EMPTY) return SUCCESS;

  msg = _raw_socket_build_message(msg.buffer, message_type, source_address,
                                  destination_address, source_port,
                                  destination_port);

  if (sendto(fd, &(msg.buffer), msg.buffer_len, 0,
             (struct sockaddr *)&(msg.addr), msg.addr_len) < 0) {
    fprintf(stderr, "ERROR: raw_socket_send_data - cannot send message.\n");
    perror("sendto");
    return FAILURE;
  }

  return SUCCESS;
}

int _raw_socket_get_input(char *buffer, size_t *len) {
  if (*len == 0) {
    memset(buffer, 0, BUFFER_SIZE);

    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
      if (errno == EINTR) {
        fprintf(stderr, "_raw_socket_get_input - interrupted by signal.\n");
        return INTERRUPTION;
      }
      fprintf(
          stderr,
          "ERROR: _raw_socket_get_input - cannot build message for sending.\n");
      return FAILURE;
    }

    // printf("\033[A\033[K");
  }
  size_t real_len = strlen(buffer);
  if (real_len == 1 && buffer[0] == '\n') return EMPTY;

  *len = real_len;
  if (buffer[*len - 1] == '\n') buffer[*len - 1] = '\0';

  return SUCCESS;
}

message _raw_socket_build_message(const char *const message_text,
                                  char message_type, char *source_address,
                                  char *destination_address,
                                  uint16_t source_port,
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

  uint16_t message_id = message_type - '\0';
  ip->id = htons(message_id);

  ip->ttl = 64;
  ip->protocol = IPPROTO_UDP;
  ip->saddr = inet_addr(source_address);
  ip->daddr = inet_addr(destination_address);

  struct udphdr *udp = (struct udphdr *)(msg.buffer + ip_len);
  udp->uh_sport = htons(source_port);
  udp->uh_dport = htons(destination_port);
  udp->uh_ulen = htons(udp_len + message_len);

  memcpy(msg.buffer + ip_len + udp_len, message_text, message_len);

  msg.addr_len = sizeof(msg.addr);
  msg.addr.sin_family = AF_INET;
  msg.addr.sin_port = udp->uh_dport;
  msg.addr.sin_addr.s_addr = ip->daddr;

  return msg;
}