#include "raw_socket_operations.h"

int raw_socket_receive_data(int fd, char *data, char *message_type,
                            char *source_address, char *destination_address,
                            uint16_t *source_port, uint16_t *destination_port) {
  message msg;
  memset(&msg, 0, sizeof(msg));
  memset(data, 0, BUFFER_SIZE);

  socklen_t socklen = sizeof(msg.addr);
  int n = recvfrom(fd, &(msg.buffer), BUFFER_SIZE, 0,
                   (struct sockaddr *)&(msg.addr), &(socklen));
  if (n < 0) {
    if (errno == EINTR) {
      fprintf(stderr, "raw_socket_receive_data - interrupted by signal.\n");
      return INTERRUPTION;
    }
    fprintf(stderr, "ERROR: raw_socket_receive_data - cannot properly receive "
                    "some data.\n");
    perror("recvfrom");
    return FAILURE;
  }

  struct iphdr *ip = (struct iphdr *)msg.buffer;
  unsigned int ihl = ip->ihl * 4;
  struct udphdr *udp = (struct udphdr *)(msg.buffer + ihl);
  unsigned int uhl = sizeof(struct udphdr);

  if (_raw_socket_filter_packets(ip, udp, source_address, destination_address,
                                 *source_port, *destination_port) == EMPTY)
    return EMPTY;

  char ip_buf[INET_ADDRSTRLEN];
  strcpy(data, msg.buffer + ihl + uhl);
  strcpy(source_address,
         _raw_socket_create_ip_string(ip_buf, ntohl(ip->saddr)));
  strcpy(destination_address,
         _raw_socket_create_ip_string(ip_buf, ntohl(ip->daddr)));
  *source_port = ntohs(udp->uh_sport);
  *destination_port = ntohs(udp->uh_dport);

  *message_type = (char)(ntohs(ip->id) & 0xFF);

  return SUCCESS;
}

char *_raw_socket_create_ip_string(char *ip, uint32_t ip_int) {
  memset(ip, 0, INET_ADDRSTRLEN);
  sprintf(ip, "%u.%u.%u.%u", (ip_int >> 24) & 0xFF, (ip_int >> 16) & 0xFF,
          (ip_int >> 8) & 0xFF, ip_int & 0xFF);

  return ip;
}

int _raw_socket_filter_packets(const struct iphdr *const ip,
                               const struct udphdr *const udp,
                               char *source_address, char *destination_address,
                               uint16_t source_port,
                               uint16_t destination_port) {
  uint32_t source_address_int = inet_addr(source_address),
           destination_address_int = inet_addr(destination_address);

  if ((ip->saddr != source_address_int && source_address_int != 0))
    return EMPTY;
  if ((ip->daddr != destination_address_int && destination_address_int != 0))
    return EMPTY;
  if ((ntohs(udp->uh_sport) != source_port && source_port != 0))
    return EMPTY;
  if ((ntohs(udp->uh_dport) != destination_port && destination_port != 0))
    return EMPTY;

  return SUCCESS;
}