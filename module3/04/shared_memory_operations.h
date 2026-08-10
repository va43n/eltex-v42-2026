#ifndef SHARED_MEMORY_OPERATIONS_H
#define SHARED_MEMORY_OPERATIONS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "item.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define PATHNAME "manufacturer"
#define PROJ_ID 124

#define SHARED_MEMORY_SIZE 64

union semun {
  int val;
  struct semid_ds* buf;
  unsigned short* array;
  struct seminfo* __buf;
};

#define P \
  { 0, -1, 0 }
#define V \
  { 0, 1, 0 }

typedef struct {
  key_t key;
  int shmid;
  int semid;
} shm_key;

int shared_memory_check_if_created();
int shared_memory_create(shm_key* key);
int shared_memory_connect(shm_key* key);
int shared_memory_write(shm_key key, int (*func)(item*));
int shared_memory_get_and_process(shm_key key, int (*func)(item*));
int shared_memory_is_all_processed(shm_key key);
int shared_memory_delete(shm_key key);
int shared_memory_disconnect(shm_key key);

#endif
