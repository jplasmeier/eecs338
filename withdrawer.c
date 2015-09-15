#include "main.h"

void main(int argc, char** argv)
{
	int semid,shmid;
	struct account *shared;
	shmid = shmget(SHMKEY, 0, 0);
	semid = semget(SEMKEY, NUM_SEMS, 0777);
	shared = (struct account *)shmat(shmid, 0, 0);  
	int pid=getpid();
	int amount = argv[0];
   
	printf("Child process %d is going to withdraw $%d.\n",pid,amount);
	
	P(semid, mutex);
	if (amount <= shared->balance && shared->waitingCount == 0) {
		shared->balance = shared->balance - amount;
		printf("Withdraw of $%d processed. Current Balance: $%d\n", amount, shared->balance);	
		V(semid,mutex);
	}
	else {
		shared->waitingCount = shared->waitingCount + 1 ;
		printf("Going to insert %d to the list", amount);
		insert(shared->head, amount); /*append process to list*/
		V(semid, mutex);
		P(semid, waitingWithdrawers);
		shared->balance	= shared->balance - amount;
		printf("Previously waiting Withdraw of $%d processed. Current Balance: $%d\n", amount, shared->balance);	
		delete(shared->head, amount);	/*remove process from list*/
		shared->waitingCount = shared->waitingCount - 1;
		if (shared->waitingCount > 1 && shared->head->next->data < shared->balance) {
			V(semid, waitingWithdrawers);
		}
		else {
			printf("All waiting withdrawers service. Resuming normal operations. \n");
			V(semid, mutex); //this signal corresponds to the departing depositer's wait.
		}
	}
	printf("Critical Section Cleared. Applied amount of %d by Process %d. Current Balance: %d\n",amount,pid,shared->balance);
	V(semid, mutex);
}
