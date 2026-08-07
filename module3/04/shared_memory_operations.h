#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "item.h"

#define TRUE 1
#define FALSE 0

#define SUCCESS 111
#define FAILURE -111

#define PATHNAME "manufacturer"
#define PROJ_ID 124

#define SHARED_MEMORY_SIZE 64

typedef struct {
  key_t key;
  int shmid;
} shm_key;

int shared_memory_check_if_created();
int shared_memory_create(shm_key* key);
int shared_memory_connect(shm_key* key);
int shared_memory_write(shm_key key, item my_item);
int shared_memory_read(shm_key key, item* my_item);
int shared_memory_delete(shm_key key);
