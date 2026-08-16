#include "clients_buffer.h"

int create_clients_buffer(clients_buffer *cb) {
  cb->actual_len = START_BUFFER_SIZE;
  cb->len = 0;
  cb->cb = (client *)malloc(cb->actual_len * sizeof(client));
  if (cb->cb == NULL) {
    fprintf(stderr, "ERROR: create_clients_buffer - cannot allocate memory.\n");
    return FAILURE;
  }

  return SUCCESS;
}

client create_client(const char *const address, uint16_t port,
                     unsigned int message_counter) {
  client c;
  strcpy(c.address, address);
  c.port = port;
  c.message_counter = message_counter;

  return c;
}

int add_client_to_buffer(clients_buffer *cb, client c) {
  cb->len++;
  if (cb->len > cb->actual_len) {
    cb->actual_len = cb->len + START_BUFFER_SIZE;
    client *tmp = (client *)realloc(cb->cb, cb->len * sizeof(client));
    if (tmp == NULL) {
      fprintf(stderr,
              "ERROR: add_client_to_buffer - cannot reallocate memory.\n");
      free_clients_buffer(*cb);
      return FAILURE;
    }
    cb->cb = tmp;
  }

  strcpy(cb->cb[cb->len - 1].address, c.address);
  cb->cb[cb->len - 1].port = c.port;
  cb->cb[cb->len - 1].message_counter = c.message_counter;

  return SUCCESS;
}

int find_client_in_buffer(clients_buffer cb, client c, size_t *pos) {
  for (size_t i = 0; i < cb.len; i++) {
    if (strcmp(cb.cb[i].address, c.address) == 0 && cb.cb[i].port == c.port) {
      *pos = i;
      return SUCCESS;
    }
  }

  return FAILURE;
}

int increment_one_of_the_clients(clients_buffer cb, size_t pos) {
  cb.cb[pos].message_counter++;

  return SUCCESS;
}

int remove_client_from_buffer(clients_buffer cb, size_t pos) {
  memset(&(cb.cb[pos]), 0, sizeof(client));
  for (size_t i = pos + 1; i < cb.len; i++) {
    strcpy(cb.cb[i - 1].address, cb.cb[i].address);
    cb.cb[i - 1].port = cb.cb[i].port;
    cb.cb[i - 1].message_counter = cb.cb[i].message_counter;
  }

  return SUCCESS;
}

int free_clients_buffer(clients_buffer cb) {
  free(cb.cb);
  if (cb.cb == NULL) {
    fprintf(stderr, "ERROR: free_clients_buffer - cannot free buffer.\n");
    return FAILURE;
  }
  return SUCCESS;
}