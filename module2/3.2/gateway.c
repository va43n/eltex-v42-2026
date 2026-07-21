#include "gateway.h"

int gateway_init(gateway* const gw, const char* const ip,
                 const char* const mask) {
  if (!_parse_ip(gw, ip)) return ERROR;
  if (!_parse_mask(gw, mask)) return ERROR;

  return SUCCESS;
}

int gateway_generate(const gateway gw, gateway_stats* stats) {
  if ((*stats).N == 0) {
    printf("ERROR: gateway_generate: N == 0\n");
    return ERROR;
  }

  srand(time(NULL));

  (*stats).in = 0;

  for (size_t i = 0; i < (*stats).N; i++) {
    unsigned int some_ip = (unsigned int)rand();
    if (gateway_check_ip(gw, some_ip)) (*stats).in++;
  }

  (*stats).percentage = (double)(*stats).in / (*stats).N * 100.0;

  printf(
      "Stats for %ld IP's:\n        Belongs: %ld\nDoes not belong: %ld\n     "
      "Percentage: %.3lf\n",
      (*stats).N, (*stats).in, (*stats).N - (*stats).in, (*stats).percentage);

  return SUCCESS;
}

int gateway_check_ip(const gateway gw, const unsigned int some_ip) {
  return ((gw.ip & gw.mask) ^ (some_ip & gw.mask)) == 0;
}

int gateway_print(const gateway gw) {
  int mask = 0;
  printf(" ip | mask\n");
  for (int i = 31; i > 31 - 8; i--) mask |= 1 << i;
  for (int i = 3; i >= 0; i--) {
    int ip_8 = (gw.ip >> (i * 8)) & 255;
    int mask_8 = (gw.mask >> (i * 8)) & 255;
    printf("%3d | %3d\n", ip_8, mask_8);
  }

  return SUCCESS;
}