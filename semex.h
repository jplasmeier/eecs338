#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>

#define SEMKEY 77
#define SHMKEY 77

#define NUM_SEMS   2
#define mutex 0
#define waitingWithdrawers 1


void P(int semid, int semaphore)
{
	struct sembuf psembuf;
	printf("Probieren\n");
	psembuf.sem_op = -1;
	psembuf.sem_flg = 0;
	psembuf.sem_num = semaphore;
	semop(semid, &psembuf, 1);
	return;
}

void V(int semid, int semaphore)
{
	struct sembuf vsembuf;
	printf("Verhogen\n");
	vsembuf.sem_op = 1;
	vsembuf.sem_flg = 0;
	vsembuf.sem_num = semaphore;
	semop(semid, &vsembuf, 1);
	return;
}

union semun {
	/* value for SETVAL */
	int val;
	/* buffer for IPC_STAT, IPC_SET */
	struct semid_ds *buf;
	/* array for GETALL, SETALL */
	unsigned short *array;
	/* Linux specific part: */
	/* buffer for IPC_INFO */
	struct seminfo *__buf;
};
