#include "ipcTools.h"

int semalloc(key_t key, int valInit){
	int semid;
	if ((semid = semget(key, 0, 0)) == -1){
		if ((semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0600)) == -1){
			//perror("Semaphore creation failed");
			return -1;
		}
	}
	if (semctl(semid, 0, SETVAL, valInit) == -1){
		//perror("Semaphore init failed");
		semfree(semid);
		return -1;
	}
	return semid;
}

int semfree(int semid){
	if (semctl(semid, 0, IPC_RMID, 0) == -1){
		//perror("Semaphore destruction failed");
		return -1;
	}
	return 0;
}

void P(int semid){
	struct sembuf sembuf;
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;
	if (semop(semid, &sembuf, 1) == -1)
		perror("Operation P failed");
}

void V(int semid){
	struct sembuf sembuf;
	sembuf.sem_num = 0;
	sembuf.sem_op = 1;
	sembuf.sem_flg = 0;
	if (semop(semid, &sembuf, 1) == -1)
		perror("Operation V failed");
}

void* shmalloc(key_t key, int size){
	void *adress;
	int shmid;

	if((shmid = shmget(key, 0, 0600)) == -1){
		if ((shmid = shmget(key, size, IPC_CREAT|IPC_EXCL|0600)) == -1)
			return (void*) -1;
	}

	if((adress = shmat(shmid, adress, 0)) == (void*)-1){
		shmfree(key);
		return (void*) -1;
	}

	return adress;
}

int shmfree(key_t key){
	int shmid;
	if((shmid = shmget(key, 0, 0600)) == -1){
		return -1;
	}
	else{
		return shmctl(shmid, IPC_RMID, 0);
	}
}