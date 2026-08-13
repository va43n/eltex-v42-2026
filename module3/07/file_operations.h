#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "constants.h"

#define FILE_IS_NOT_EXISTS 1000

int check_if_file_exists(char* buffer);
int read_user_input(char* buffer);
int read_another_bytes_of_file(char* file_name, ssize_t* bytes, char* buffer,
                               ssize_t* actual_number_of_bytes,
                               int* is_file_ended);
