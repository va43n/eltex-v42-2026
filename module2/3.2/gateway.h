#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SUCCESS 1
#define ERROR 0

typedef struct {
  unsigned int ip;
  unsigned int mask;
} gateway;

typedef struct {
  size_t N;
  size_t in;
  double percentage;
} gateway_stats;

int gateway_init(gateway* const gw, const char* const ip,
                 const char* const mask);
int gateway_generate(const gateway gw, gateway_stats* stats);
int gateway_check_ip(const gateway gw, const unsigned int some_ip);
int gateway_print(const gateway gw);

int _parse_ip(gateway* gw, const char* const ip);
int _parse_mask(gateway* gw, const char* const mask);