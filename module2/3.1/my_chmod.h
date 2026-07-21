#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SUCCESS 1
#define ERROR 0

#define USER 0700
#define GROUP 0070
#define OTHER 0007

#define READ 0b100100100
#define WRITE 0b010010010
#define EXEC 0b001001001

#define READ_SYMBOL 'r'
#define WRITE_SYMBOL 'w'
#define EXEC_SYMBOL 'x'
#define BLANK_SYMBOL '-'

#define USER_SYMBOL 'u'
#define GROUP_SYMBOL 'g'
#define OTHER_SYMBOL 'o'
#define ALL_SYMBOL 'a'

#define PLUS '+'
#define MINUS '-'
#define EQUAL '='

#define PLUS_MODE 1
#define MINUS_MODE -1
#define EQUAL_MODE 0

typedef struct {
  unsigned int numbers;
  char letters[9];
} mod;

int my_chmod_init(mod* const m, const char* const input);
int my_chmod_init_with_file(mod* const m, const char* const file_name);
int my_chmod_print(const mod m);
int my_chmod_change(mod* const m, const char* const format);

int _get_numbers_from_input(const char* const input, unsigned int* mask);
int _convert_numbers_to_letters(const unsigned int numbers,
                                char* const letters);
int _parse_complex_input(const char* const input, unsigned int* mask, int* mode);