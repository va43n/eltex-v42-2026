#include "shared_memory_operations.h"

int shared_memory_check_if_created() { return FALSE; }

int shared_memory_create(shm_key* key) { return SUCCESS; }

int shared_memory_connect(shm_key* key) { return SUCCESS; }

int shared_memory_write(shm_key key, item my_item) { return SUCCESS; }

int shared_memory_read(shm_key key, item* my_item) { return SUCCESS; }

int shared_memory_delete(shm_key key) { return SUCCESS; }