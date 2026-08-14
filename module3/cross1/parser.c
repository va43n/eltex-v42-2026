#include "echo_reply.h"

int parse_input(int argc, char *argv[], char *mode,
                unsigned int *destination_address, unsigned int *source_address,
                int *port) {
  if (argc != 2 && argc != 4) {
    fprintf(
        stderr,
        "ERROR: parse_input - input should contain 1 or 3 parameters -\n"
        "1. user identifier: '-s' - server\n\tor\n1. user identifier: '-c' - "
        "client;\n2. server IP address;\n3. client port (> 1024 and not %d).\n",
        SERVER_PORT);
    return FAILURE;
  }

  if (parse_flag(argv[1], mode) == FAILURE)
    return FAILURE;
  if (*mode == CLIENT) {
    if (argc != 4) {
      fprintf(stderr,
              "ERROR: parse_input - client input should contain 2 parameters: "
              "flag and server address.\n");
      return FAILURE;
    }
    if (parse_address_from_str(argv[2], destination_address) == FAILURE)
      return FAILURE;
    if (parse_port(port, argv[3]) == FAILURE)
      return FAILURE;
  }
  if (get_source_address(source_address) == FAILURE)
    return FAILURE;

  return SUCCESS;
}

int parse_flag(char *flag, char *mode) {
  if (strlen(flag) != 2 || flag[0] != '-' ||
      (flag[1] != CLIENT && flag[1] != SERVER)) {
    fprintf(stderr,
            "ERROR: parse_flag - flag should be made out of 2 symbols: '-' and "
            "user identifier: 'c' - client, 's' - server.\n");
    return FAILURE;
  }

  *mode = flag[1];

  return SUCCESS;
}

int parse_address_from_str(char *address, unsigned int *address_int) {
  struct hostent *server = gethostbyname(address);
  if (server == NULL) {
    fprintf(stderr,
            "ERROR: parse_address_from_str - cannot parse server address.\n");
    herror("gethostbyname");
    return FAILURE;
  }

  *address_int = 0;
  char *token = strtok(address, ".");
  *address_int |= atoi(token);
  for (int i = 1; i < 4; i++) {
    token = strtok(NULL, ".");
    *address_int <<= 8;
    *address_int |= atoi(token);
  }

  return SUCCESS;
}

int parse_port(int *port_int, char *port) {
  *port_int = atoi(port);
  if (*port_int < 1024 || *port_int > 65535) {
    fprintf(stderr, "ERROR: parse_port - port is not valid.\n");
    return FAILURE;
  }

  if (*port_int == SERVER_PORT) {
    fprintf(stderr, "ERROR: parse_port - it's a server's port.\n");
    return FAILURE;
  }

  return SUCCESS;
}

int get_source_address(unsigned int *address) {
  struct ifaddrs *interfaces = NULL;
  struct ifaddrs *ifa = NULL;
  char ip_buffer[INET_ADDRSTRLEN];

  if (getifaddrs(&interfaces) == -1) {
    fprintf(stderr,
            "ERROR: get_source_address - cannot get client addresses.\n");
    perror("getifaddrs");
    return FAILURE;
  }

  for (ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;

    if (ifa->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
      inet_ntop(AF_INET, &(addr->sin_addr), ip_buffer, sizeof(ip_buffer));
      if (strcmp(ip_buffer, LOCALHOST_STR) == 0)
        continue;

      unsigned int addr_int;
      if (parse_address_from_str(ip_buffer, &addr_int) == FAILURE)
        return FAILURE;
      *address = addr_int;
      break;
    }
  }

  return SUCCESS;
}