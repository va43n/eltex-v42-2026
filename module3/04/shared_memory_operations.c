#include "shared_memory_operations.h"

int shared_memory_check_if_created() {
  key_t key = ftok(PATHNAME, PROJ_ID);
  if (key == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_check_if_created - something went wrong "
            "while trying to check if the shared memory is created.\n");
    perror("ftok");
    return FAILURE;
  }

  if (shmget(key, sizeof(item) * SHARED_MEMORY_SIZE, 0666) == -1 &&
      errno == ENOENT)
    return FALSE;

  return TRUE;
}

int shared_memory_create(shm_key* key) {
  key_t k = ftok(PATHNAME, PROJ_ID);
  key->key = k;
  int shmid =
      shmget(key->key, sizeof(item) * SHARED_MEMORY_SIZE, 0666 | IPC_CREAT);
  if (shmid == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to create the shared memory.\n");
    perror("shmget");
    return FAILURE;
  }
  key->shmid = shmid;
  printf("Shared memory shmid is: %d\n", shmid);

  int semid = semget(key, 1, 0666 | IPC_CREAT);
  if (semid == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to create the semaphore.\n");
    perror("semget");
    return FAILURE;
  }
  key->semid = semid;
  printf("Semaphore semid is: %d\n", shmid);
  union semun arg;
  arg.val = 0;
  semctl(key->semid, 0, SETVAL, arg);

  item* head = (item*)shmat(key->shmid, NULL, 0);
  if (head == (item*)(-1)) {
    fprintf(stderr,
            "ERROR: shared_memory_write - something went wrong while trying to "
            "get the first element from the shared memory.\n");
    perror("shmat");
    return FAILURE;
  }
  for (int i = 0; i < SHARED_MEMORY_SIZE; i++) {
    head[0].next = NULL_ITEM;
  }
  struct sembuf v = V;
  semop(key->semid, &v, 1);

  return SUCCESS;
}

int shared_memory_connect(shm_key* key) {
  key_t k = ftok(PATHNAME, PROJ_ID);
  key->key = k;
  int shmid = shmget(key->key, sizeof(item) * SHARED_MEMORY_SIZE, 0666);
  if (shmid == -1) {
    if (errno == ENOENT) {
      fprintf(
          stderr,
          "ERROR: shared_memory_connect - shared memory is not created yet.\n");
      perror("shmget");
      return FAILURE;
    }
    fprintf(stderr,
            "ERROR: shared_memory_connect - something went wrong while trying "
            "to connect to the shared memory.\n");
    perror("shmget");
    return FAILURE;
  }
  key->shmid = shmid;

  return SUCCESS;
}

int shared_memory_write(shm_key key, item my_item,
                        int (*func)(item*, unsigned int)) {
  item* head = (item*)shmat(key.shmid, NULL, 0);
  int index;
  if (head == (item*)(-1)) {
    fprintf(stderr,
            "ERROR: shared_memory_write - something went wrong while trying to "
            "get the first element from the shared memory.\n");
    perror("shmat");
    return FAILURE;
  }

  struct sembuf p = P;
  struct sembuf v = V;

  index = 0;
  while (index != NULL_ITEM) {
    index = head[index].next;
    if (index == 0) {
      printf("...The memory is full...\n");
      return FAILURE;
    }
  }

  semop(key.semid, &p, 1);
  int result = func(&(head[index]), index);
  semop(key.semid, &v, 1);

  return SUCCESS;
}

int shared_memory_read(shm_key key, item* my_item) {
  static int index = 0;

  item* head = (item*)shmat(key.shmid, NULL, 0);
  if (head == (item*)(-1)) {
    fprintf(stderr,
            "ERROR: shared_memory_read - something went wrong while trying to "
            "get the first element from the shared memory.\n");
    perror("shmat");
    return FAILURE;
  }

  if (head[index].size == 0) {
    index = (index + 1) % 16;
    if (index == 0) {
      printf("...Shared memory is fully processed...\n");
      return FAILURE;
    }
  }

  return SUCCESS;
}

int shared_memory_process(shm_key key, item* my_item, int (*func)(item*)) {
  struct sembuf p = P;
  struct sembuf v = V;

  semop(key.semid, &p, 1);
  int result = func(my_item);
  semop(key.semid, &v, 1);

  return result;
}

int shared_memory_delete(shm_key key) {
  struct sembuf p = P;

  semop(key.semid, &p, 1);
  if (shmctl(key.shmid, IPC_RMID, NULL) < 0) {
    fprintf(stderr,
            "ERROR: shared_memory_delete - cannot delete shared memory.\n");
    perror("shmctl");
    return FAILURE;
  }
  semctl(key.semid, 0, IPC_RMID);

  return SUCCESS;
}