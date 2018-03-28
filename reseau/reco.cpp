#include "ipcTools.h"

key_t semkey;
pid_t pidFork;
int semid, shmkey, *fd;
int tube[2];
char tubeBuffer[100];

int main(){
	printf("Initialisation de la reconnaissance\n");
	//Sem & SharedMem Init
	if ((semkey = ftok("sem.key", 1)) == -1){
		perror("Semkey creation failed");
		exit(-1);
	}
	printf("Semkey ok\n");
	if ((shmkey = ftok("shm.key", 1)) == -1){
		perror("Semkey creation failed");
		exit(-1);
	}
	printf("SHMKey Ok\n");
	if ((semid = semalloc(semkey, 0)) == -1){
		perror("Semaphore creation failed");
		exit(-1);
	}
	printf("Sem Ok\n");
	fd = (int*) shmalloc(shmkey, sizeof(int));
	printf("SHM Ok\n");
	printf("REC_SEMID: %d\n", semid);
	printf("Chargement du model\n");
	P(semid);
	tube[0] = *fd;
	V(semid);
	P(semid);
	tube[1] = *fd;
	printf("Tube Ok\n");
	P(semid);

	//Reception Signal
	printf("Prises des photos\n");
	sprintf(tubeBuffer, "Valide");
	write(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("Validation...\n");
	sprintf(tubeBuffer, "0.50");
	write(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("Pourcentage ...\n");
	sprintf(tubeBuffer, "0.50 0.50 0.50 0.50 0.50");
	write(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("Array ...\n");
	printf("Fin transmission\n");
	P(semid);

	close(tube[1]);
	return 0;
}