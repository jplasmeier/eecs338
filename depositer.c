#include "main.h"

void depositer(int amt)
{
	int semid,shmid;
	struct account *shared;
	shmid = shmget(SHMKEY, 0, 0);
	semid = semget(SEMKEY, NUM_SEMS, 0777);
	shared = (struct account *)shmat(shmid, 0, 0);  
	int pid=getpid();
	int amount = amt; 
   
	printf("Depositing child process %d is going to add $%d.\n",pid,amount);

	P(semid, mutex);
	shared->balance = shared->balance + amount;
	if (shared->waitingCount == 0) {
		printf("No pending withdrawls, proceed\n");
		V(semid, mutex);
	}
	else if (shared->head->next->data > shared->balance) {
		printf("Attempted to service first waiting withdrawl of %d, but balance is only %d\n", shared->head->next->data, shared->balance);
		V(semid, mutex);
	}
	else {
		printf("Signalling waiting withdrawer\n");
		V(semid, waitingWithdrawers);	
	}
	printf("Applied amount of %d by Process %d. Current Balance: %d\n",amount,pid,shared->balance);
	V(semid, mutex);
}
