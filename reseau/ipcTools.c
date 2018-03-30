/**
* \file ipcTools.c
* \brief tools for semaphore use
* \author  Matthieu Vilain, Quentin Gerard, Louis L'Haridon 
* \version 1.0
* \date 20/03/2018
*/
#include "ipcTools.h"

/**
* \fn semalloc(key_t key, int valInit)
* \brief create a semaphore
* \param key key to alloc
* \param valInit value to init
* \return semid the semaphore id
*/
int semalloc(key_t key, int valInit){
	int semid;
	if ((semid = semget(key, 0, 0)) == -1){
		if ((semid = semget(key, 1, IPC_CREAT|IPC_EXCL|0600)) == -1){
			perror("Semaphore creation failed");
			return -1;
		}
	}
	if (semctl(semid, 0, SETVAL, valInit) == -1){
		perror("Semaphore init failed");
		semfree(semid);
		return -1;
	}
	return semid;
}

/**
* \fn int semfree(int semid)
* \brief destroy a semaphore
* \param semid id of semaphore to kill
*/
int semfree(int semid){
	if (semctl(semid, 0, IPC_RMID, 0) == -1){
		perror("Semaphore destruction failed");
		return -1;
	}
	return 0;
}

/**
* \fn void P(int semid)
* \brief do P operation
* \param semid id of semaphore on which to do P operation
*/
void P(int semid){
	struct sembuf sembuf;
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;
	if (semop(semid, &sembuf, 1) == -1)
		perror("Operation P failed");
}

/**
* \fn void V(int semid)
* \brief do V operation
* \param semid id of semaphore on which to do V operation
*/
void V(int semid){
	struct sembuf sembuf;
	sembuf.sem_num = 0;
	sembuf.sem_op = 1;
	sembuf.sem_flg = 0;
	if (semop(semid, &sembuf, 1) == -1)
		perror("Operation V failed");
}

/**
* \fn void* shmalloc(key_t key, int size)
* \brief returns a pointer to a block of at least size bytes suitably aligned for any use
* \param key the key to alloc 
* \param size the size to alloc
* \return adress pointer to the allocated space
*/
void* shmalloc(key_t key, int size){
	void *adress = NULL;
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

/**
* \fn int shmfree(key_t key)
* \brief free a block of at least size bytes suitably aligned for any use
* \param key the key of the block to free 
* \return adress pointer to the allocated space
*/
int shmfree(key_t key){
	int shmid;
	if((shmid = shmget(key, 0, 0600)) == -1){
		return -1;
	}
	else{
		return shmctl(shmid, IPC_RMID, 0);
	}
}