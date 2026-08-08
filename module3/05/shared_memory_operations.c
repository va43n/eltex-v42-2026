#include "shared_memory_operations.h"

int shared_memory_check_if_created() {
  int shm_fd = shm_open(PATHNAME_MEMORY, O_RDWR, 0666);
  if (shm_fd == -1 && errno == ENOENT) return FALSE;

  return TRUE;
}

int shared_memory_create(shm_key* key) {
  int shm_fd = shm_open(PATHNAME_MEMORY, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to create the shared memory.\n");
    perror("shm_open");
    return FAILURE;
  }
  key->shm_fd = shm_fd;

  printf("Shared memory shm_fd is: %d\n", shm_fd);

  if (ftruncate(shm_fd, sizeof(item) * SHARED_MEMORY_SIZE) == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to allocate memory for shared memory.\n");
    perror("ftruncate");
    return FAILURE;
  }

  key->items = mmap(NULL, sizeof(item) * SHARED_MEMORY_SIZE,
                    PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (key->items == MAP_FAILED) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to connect to allocated memory.\n");
    perror("mmap");
    return FAILURE;
  }
  close(key->shm_fd);

  key->sem = sem_open(PATHNAME_SEMAPHORE, O_CREAT | O_EXCL, 0666, 1);
  if (key->sem == SEM_FAILED) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying"
            "to create the semaphore.\n");
    perror("sem_open");
    return FAILURE;
  }
  printf("Semaphore is created.\n");

  sem_wait(key->sem);

  key->items[0].next = 0;
  key->items[0].size = 0;
  for (int i = 1; i < SHARED_MEMORY_SIZE; i++) {
    key->items[i].next = NULL_ITEM;
    key->items[i].size = 0;
  }

  sem_post(key->sem);

  return SUCCESS;
}

int shared_memory_connect(shm_key* key) {
  int shm_fd = shm_open(PATHNAME_MEMORY, O_RDWR, 0666);
  if (shm_fd == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_connect - something went wrong while trying "
            "to open the shared memory.\n");
    perror("shm_open");
    return FAILURE;
  }
  key->shm_fd = shm_fd;

  key->items = mmap(NULL, sizeof(item) * SHARED_MEMORY_SIZE,
                    PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (key->items == MAP_FAILED) {
    fprintf(stderr,
            "ERROR: shared_memory_connect - something went wrong while trying "
            "to connect to allocated memory.\n");
    perror("mmap");
    return FAILURE;
  }
  close(key->shm_fd);

  key->sem = sem_open(PATHNAME_SEMAPHORE, 0);
  if (key->sem == SEM_FAILED) {
    fprintf(stderr,
            "ERROR: shared_memory_connect - something went wrong while trying"
            "to connect to the semaphore.\n");
    perror("sem_open");
    return FAILURE;
  }

  return SUCCESS;
}

int shared_memory_write(shm_key key, int (*func)(item*)) {
  unsigned int index = 0;
  sem_wait(key.sem);
  while (key.items[index].next != 0) {
    index = key.items[index].next;
  }
  if (index == SHARED_MEMORY_SIZE - 1) {
    printf("...The memory is full...\n");
    sem_post(key.sem);
    return FAILURE;
  }

  key.items[index].next = index + 1;
  int result = func(&(key.items[index + 1]));
  sem_post(key.sem);

  return result;
}

int shared_memory_get_and_process(shm_key key, int (*func)(item*)) {
  sem_wait(key.sem);

  int cur = key.items[0].next;
  while (cur != 0 && cur != NULL_ITEM) {
    if (key.items[cur].size > 0) {
      int res = func(&(key.items[cur]));
      sem_post(key.sem);
      return res;
    }
    cur = key.items[cur].next;
  }

  sem_post(key.sem);
  return FAILURE;
}

int shared_memory_is_all_processed(shm_key key) {
  sem_wait(key.sem);

  int cur = key.items[0].next;
  while (cur != 0 && cur != NULL_ITEM) {
    if (key.items[cur].size > 0) {
      sem_post(key.sem);
      return FALSE;
    }
    cur = key.items[cur].next;
  }

  sem_post(key.sem);
  return TRUE;
}

int shared_memory_disconnect(shm_key key) {
  sem_close(key.sem);

  if (munmap(key.items, sizeof(item) * SHARED_MEMORY_SIZE) == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_disconnect - some error occured while trying "
            "to detach shared memory.\n");
    perror("munmap");
    return FAILURE;
  }

  return SUCCESS;
}

int shared_memory_delete(shm_key key) {
  printf("Deleting shared memory %d and semaphore...\n", key.shm_fd);
  if (shm_unlink(PATHNAME_MEMORY) == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_delete - cannot delete shared memory.\n");
    perror("shm_unlink");
    return FAILURE;
  }
  sem_unlink(PATHNAME_SEMAPHORE);

  return SUCCESS;
}
