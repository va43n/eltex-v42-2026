#include <netinet/in.h>
#include <arpa/inet.h>

#include "group_chat.h"

int parse_input(int argc, char* argv[], char* mode) {
  if (argc < 2) {
    fprintf(stderr, "ERROR: parse_input - input should contain at least 1 parameter - user identifier: '-c' - client, '-s' - server.\n");
    return FAILURE;
  }

  if (parse_flag(argv[1], mode) == FAILURE) return FAILURE;
  if (*mode == CLIENT) {
    if (argc != 3) {
      fprintf(stderr, "ERROR: parse_input - client input should contain 2 parameters: flag and server address.\n");
      return FAILURE;
    }
    return parse_server_address(argv[2]);
  }

  return SUCCESS;
}

int parse_flag(char* flag, char* mode) {
  if (strlen(flag) != 2 || flag[0] != '-' || (flag[1] != CLIENT && flag[1] != SERVER)) {
    fprintf(stderr, "ERROR: parse_flag - flag should be made out of 2 symbols: '-' and user identifier: 'c' - client, 's' - server.\n");
    return FAILURE;
  }

  *mode = flag[1];

  return SUCCESS;
}

int parse_server_address(char* address) {
  struct in_addr ip_binary;
  if (inet_aton(address, &ip_binary) == 0) {
        fprintf(stderr, "ERROR: parse_server_address - cannot parse server address.\n");
        return FAILURE;
    }
    printf("%d, %X, %o\n", ip_binary.s_addr, ip_binary.s_addr, ip_binary.s_addr);

  return SUCCESS;
}