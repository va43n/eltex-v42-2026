#include "gateway.h"

int _parse_ip(gateway* gw, const char* const ip) {
  (*gw).ip = 0;

  int dot_count = 0;
  int number_expected = 1;
  unsigned int number = 0;

  size_t len = strlen(ip);
  if (len < 7) {
    printf("ERROR: _parse_ip: IP is too short\n");
    return ERROR;
  }

  for (size_t i = 0; i < len; i++) {
    if (ip[i] == '.') {
      if (number_expected) {
        printf("ERROR: _parse_ip: . was not expected - invalid IP\n");
        return ERROR;
      }
      if (number > 255) {
        printf("ERROR: _parse_ip: one of the bytes > 255 - invalid IP\n");
        return ERROR;
      }
      (*gw).ip <<= 8;
      (*gw).ip += number;
      dot_count++;
      number_expected = 1;
      number = 0;
      if (dot_count >= 4) {
        printf("ERROR: _parse_ip: too many bytes in IP\n");
        return ERROR;
      }
    } else if (isdigit(ip[i])) {
      number = number * 10 + ip[i] - '0';
      number_expected = 0;
    } else {
      printf("ERROR: _parse_ip: unexpected symbol in IP\n");
      return ERROR;
    }
  }
  if (number_expected) {
    printf("ERROR: _parse_ip: . was not expected - invalid IP\n");
    return ERROR;
  }
  if (number > 255) {
    printf("ERROR: _parse_ip: one of the bytes > 255 - invalid IP\n");
    return ERROR;
  }
  (*gw).ip <<= 8;
  (*gw).ip += number;

  return SUCCESS;
}

int _parse_mask(gateway* gw, const char* const mask) {
  (*gw).mask = 0;

  size_t len = strlen(mask);
  if (len <= 1) {
    printf("ERROR: _parse_mask: mask is too short\n");
    return ERROR;
  }

  if (mask[0] == '/') {
    int mask_number = 0;
    int is_mask_found = 0;
    for (size_t i = 1; i < len; i++) {
      if (!isdigit(mask[i])) {
        printf("ERROR: _parse_mask: unexpected symbol in mask\n");
        return ERROR;
      }
      is_mask_found = 1;
      mask_number = mask_number * 10 + mask[i] - '0';
    }
    if (!is_mask_found) {
      printf("ERROR: _parse_mask: mask number was not found after /\n");
      return ERROR;
    }
    if (mask_number > 32) {
      printf(
          "ERROR: _parse_mask: number after / is too large - 32 is the max "
          "value\n");
      return ERROR;
    }
    mask_number--;
    for (int i = 31; i >= 31 - mask_number; i--) (*gw).mask |= 1 << i;
  } else {
    gateway temp_gw;
    if (!_parse_ip(&temp_gw, mask)) {
      printf(
          "ERROR: _parse_mask: when trying to parse mask by ip parser some "
          "error occured\n");
      return ERROR;
    }
    (*gw).mask = temp_gw.ip;
  }

  return SUCCESS;
}

unsigned int _generate_ip() {
  unsigned int some_ip = 0;
  some_ip |= (unsigned int)(rand() & 0b11111111) << 24;
  some_ip |= (unsigned int)(rand() & 0b11111111) << 16;
  some_ip |= (unsigned int)(rand() & 0b11111111) << 8;
  some_ip |= (unsigned int)(rand() & 0b11111111);

  return some_ip;
}
