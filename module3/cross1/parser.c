#include "echo_reply.h"

int parse_input(int argc, char *argv[], char *mode, char *source_address,
                char *destination_address, uint16_t *port) {
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
              "ERROR: parse_input - client input should contain 3 parameters: "
              "flag, server address and client port.\n");
      return FAILURE;
    }

    if (check_address(argv[2]) == FAILURE)
      return FAILURE;
    strcpy(destination_address, argv[2]);

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

int check_address(char *address) {
  struct hostent *server = gethostbyname(address);
  if (server == NULL) {
    fprintf(stderr,
            "ERROR: parse_address_from_str - cannot parse server address.\n");
    herror("gethostbyname");
    return FAILURE;
  }

  return SUCCESS;
}

int parse_port(uint16_t *port_int, char *port) {
  *port_int = atoi(port);
  if (*port_int < 1024) {
    fprintf(stderr, "ERROR: parse_port - port is not valid.\n");
    return FAILURE;
  }

  if (*port_int == SERVER_PORT) {
    fprintf(stderr, "ERROR: parse_port - you cannot use server's port.\n");
    return FAILURE;
  }

  return SUCCESS;
}

int get_source_address(char *address) {
  struct ifaddrs *interfaces = NULL;
  struct ifaddrs *ifa = NULL;

  if (getifaddrs(&interfaces) == -1) {
    fprintf(stderr, "ERROR: get_source_address - cannot get your addresses.\n");
    perror("getifaddrs");
    return FAILURE;
  }

  for (ifa = interfaces; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL)
      continue;

    if (ifa->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
      inet_ntop(AF_INET, &(addr->sin_addr), address, INET_ADDRSTRLEN);
      if (check_address(address) == FAILURE)
        continue;
      if (strcmp(address, LOCALHOST_STR) == 0)
        continue;
      break;
    }
  }

  return SUCCESS;
}