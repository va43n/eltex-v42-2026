#include "shared_memory_operations.h"

int shared_memory_check_if_created() {
  key_t key = ftok(PATHNAME, PROJ_ID);

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

  int semid = semget(key->key, 1, 0666 | IPC_CREAT);
  if (semid == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_create - something went wrong while trying "
            "to create the semaphore.\n");
    perror("semget");
    return FAILURE;
  }
  key->semid = semid;
  printf("Semaphore semid is: %d\n", semid);

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
  head[0].next = 0;
  head[0].size = 0;
  for (int i = 1; i < SHARED_MEMORY_SIZE; i++) {
    head[i].next = NULL_ITEM;
    head[i].size = 0;
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

  int semid = semget(key->key, 1, 0666);
  if (semid == -1) {
    fprintf(stderr,
            "ERROR: shared_memory_connect - something went wrong while trying "
            "to connect to the semaphore.\n");
    perror("semget");
    return FAILURE;
  }
  key->semid = semid;

  return SUCCESS;
}

int shared_memory_write(shm_key key, int (*func)(item*)) {
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
  semop(key.semid, &p, 1);
  while (head[index].next != 0) {
    index = head[index].next;
  }
  if (index == SHARED_MEMORY_SIZE - 1) {
    printf("...The memory is full...\n");
    semop(key.semid, &v, 1);
    return FAILURE;
  }

  head[index].next = index + 1;
  int result = func(&(head[index + 1]));
  semop(key.semid, &v, 1);

  return result;
}

int shared_memory_get_and_process(shm_key key, int (*func)(item*)) {
  item* head = (item*)shmat(key.shmid, NULL, 0);
  if (head == (item*)-1) {
    fprintf(stderr,
            "ERROR: shared_memory_get_and_process - something went wrong "
            "while trying to "
            "get the first element from the shared memory.\n");
    perror("shmat");
    return FAILURE;
  }
  struct sembuf p = P, v = V;
  semop(key.semid, &p, 1);

  int cur = head[0].next;
  while (cur != 0 && cur != NULL_ITEM) {
    if (head[cur].size > 0) {
      int res = func(&(head[cur]));
      semop(key.semid, &v, 1);
      return res;
    }
    cur = head[cur].next;
  }

  semop(key.semid, &v, 1);
  return FAILURE;
}

int shared_memory_is_all_processed(shm_key key) {
  item* head = (item*)shmat(key.shmid, NULL, 0);
  if (head == (item*)-1) {
    fprintf(stderr,
            "ERROR: shared_memory_is_all_processed - something went wrong "
            "while trying to "
            "get the first element from the shared memory.\n");
    perror("shmat");
    return -1;
  }
  struct sembuf p = P, v = V;
  semop(key.semid, &p, 1);

  int cur = head[0].next;
  while (cur != 0 && cur != NULL_ITEM) {
    if (head[cur].size > 0) {
      semop(key.semid, &v, 1);
      return FALSE;
    }
    cur = head[cur].next;
  }

  semop(key.semid, &v, 1);
  return TRUE;
}

int shared_memory_disconnect(shm_key key) {
  printf("Disconnecting from shared memory %d and semaphore %d...\n", key.shmid,
         key.semid);

  return SUCCESS;
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
