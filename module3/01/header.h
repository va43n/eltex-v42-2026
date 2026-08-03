#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define FIFO_FLAG_SYMBOL 'p'

// prepare.c
int parse_input(int argc, char* argv[], char*** file_names, char*** pipe_names,
                size_t* size);
int check_if_flag(char* argv[], int index);
int put_str_in_array(char*** array, size_t size, char* str);