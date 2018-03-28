#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int semalloc(key_t key, int valInit);
int semfree(int semid);
void P(int semid);
void V(int semid);
void* shmalloc(key_t key, int size);
int shmfree(key_t key);