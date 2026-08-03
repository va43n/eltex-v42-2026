#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
int check_if_file_exists(char* file_name);
int check_if_names_are_different(char** names, size_t size);

// ipc.c
int copy_files_with_ipc(size_t size);
//

// copy.c
//
//
