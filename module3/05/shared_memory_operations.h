#ifndef SHARED_MEMORY_OPERATIONS_H
#define SHARED_MEMORY_OPERATIONS_H

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "item.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define PATHNAME_MEMORY "/manufacturer_shm"
#define PATHNAME_SEMAPHORE "/manufacturer_sem"

#define SHARED_MEMORY_SIZE 64

typedef struct {
  int shm_fd;
  item* items;
  sem_t* sem;
} shm_key;

int shared_memory_check_if_created();
int shared_memory_create(shm_key* key);
int shared_memory_connect(shm_key* key);
int shared_memory_write(shm_key key, int (*func)(item*));
int shared_memory_get_and_process(shm_key key, int (*func)(item*));
int shared_memory_is_all_processed(shm_key key);
int shared_memory_disconnect(shm_key key);
int shared_memory_delete(shm_key key);

#endif
