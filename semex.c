#include "semex.h"

void withdrawer(int amt)
{
	int semid,shmid;
	struct account *shared;
	shmid = shmget(SHMKEY, 0, 0);
	semid = semget(SEMKEY, NUM_SEMS, 0777);
	shared = (struct account *)shmat(shmid, 0, 0);  
	int pid=getpid();
	int amount = amt;
   
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
	exit(0);
}


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
	exit(0);
}

int main(void)
{
	/*LIST INITIALIZATION 
	struct wait_list *tmp;
	struct list_head *pos, *q;
	unsigned int i;
	struct wait_list waitingWithdraw;
	INIT_LIST_HEAD(&waitingWithdraw.list); */

	printf("Starting now:\n");
	
	node *start,*temp;
        start = (node *)malloc(sizeof(node)); 
        temp = start;
        temp -> next = NULL;

	/* INITIALIZATIONS */
	FILE *fp;
	int shmid,semid,pid1,pid2,pid3,pid4,pid5,pid6,pid7;
	u_short seminit[NUM_SEMS];
	key_t key;
	union semun semctlarg;
	struct account *shared;
	srand(time(NULL));
	key=ftok(".", SEMKEY);
	seminit[0]=1; /* mutex */
	seminit[1]=0; /* waitingWithdrawers */
	semctlarg.array=seminit;
	semctl(SEMKEY,1,SETALL,semctlarg);
	semid = semget(SEMKEY, NUM_SEMS, 0777 | IPC_CREAT);
	shmid = shmget(SHMKEY, sizeof(struct account), 0777 | IPC_CREAT);
	shared=(struct account *)shmat(shmid, 0, 0); 
	shared->balance = 0;
	shared->head = start;

	/* CUSTOMER CREATION */
	/* P(semid, mutex); */
	int a,b,c,d,e,f,g;
	a = 1000;
	sleep(2);
	b = 100;
	sleep(2);
	c = 100;
	sleep(2);
	d = 200;
	sleep(2);
	e = 500;
	sleep(2);
	f = 500;
	sleep(2);
	g = 200;
	
	if ((pid1=fork())==0) {
		depositer(a);	
	}
	if ((pid2=fork())==0) {
		depositer(b);
	}

	if ((pid3=fork())==0) {
		withdrawer(c);
	}
	if ((pid4=fork())==0) {
		withdrawer(d);
	}
	if ((pid5=fork())==0) {
		depositer(e);
	}
	if ((pid6=fork())==0) {
		depositer(f);
	}
	if ((pid7=fork())==0) {
		withdrawer(g);
	}
	/* EXECUTION */
	printf("Balance is initially: %d\n", shared->balance); 
	printf("Child processes are %d and %d.\n",pid1,pid2);
	printf("Parent process is now waiting a while.\n");
	sleep(5);
	printf("Parent is now signalling the semaphore...\n");
	V(semid, mutex);
	printf("Parent is now waiting for the children to exit.\n");
	wait(0);

	printf("First Child has exited. Final balance: $%d. Cleaning up...\n", shared->balance);
		
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
	kill(pid3, SIGKILL);
	kill(pid4, SIGKILL);
	kill(pid5, SIGKILL);
	kill(pid6, SIGKILL);
	kill(pid7, SIGKILL);

	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	shmctl(shmid, IPC_RMID, 0);
	semctl(SEMKEY,1,IPC_RMID,0);
	printf("Closing...");
	return 0;
}
