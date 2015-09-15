#include "semex.h"



int main(void)
{

	printf("Starting now:\n");
	
	/* INITIALIZATIONS */
	int shmid,semid,pid1,pid2,pid3,pid4,pid5,pid6,pid7;
	u_short seminit[NUM_SEMS];
	key_t key;
	union semun semctlarg;
	struct account *shared;
	srand(time(NULL));
	key=ftok(".", SEMKEY);
	seminit[mutex]=1; /* mutex */
	seminit[waitingWithdrawers]=0; /* waitingWithdrawers */
	semctlarg.array=seminit;
	semctl(SEMKEY,1,SETALL,semctlarg);
	semid = semget(SEMKEY, NUM_SEMS, 0777 | IPC_CREAT);
	shmid = shmget(SHMKEY, sizeof(struct account), 0777 | IPC_CREAT);
	shared=(struct account *)shmat(shmid, 0, 0); 
	shared->balance = 0;
	node *start,*temp;
        start = (node *)malloc(sizeof(node)); 
	shared->head = start;
        temp = start;
        temp -> next = NULL;

	if ((pid1 = fork()) == 0) {
		execl("depositer.bin", "100", "100", 0);
		exit(EXIT_SUCCESS);
	}
	else if ((pid2 = fork()) == 0) {
		execl("withdrawer.bin", "50", "50", 0);
		exit(EXIT_SUCCESS);
	}
	else if ((pid3 = fork()) == 0) {
		execl("depositer.bin", "200", "200", 0);
		exit(EXIT_SUCCESS);
	}
	else if ((pid4 = fork()) == 0) {
		execl("depositer.bin", "300", "300", 0);
		exit(EXIT_SUCCESS);
	};

	
/*	if ((pid1=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("depositer.bin", "100", (char*)NULL);	
		exit(EXIT_SUCCESS);
	}
	else if ((pid2=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("depositer.bin", "200", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
	else if ((pid3=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("withdrawer.bin", "150", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
	else if ((pid4=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("withdrawer.bin", "15", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
	else if ((pid5=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("depositer.bin", "300", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
	else if ((pid6=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("depositer.bin", "400", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
	else if ((pid7=fork())==0) {
		printf("Created child %d", getpid()); 
		execl("withdrawer.bin", "250", (char*)NULL);
		exit(EXIT_SUCCESS);
	}
*/
	/* EXECUTION */
	printf("Balance is initially: %d\n", shared->balance); 
	printf("Child processes are %d and %d.\n",pid1,pid2);
	printf("Parent process is now waiting a while.\n");
	sleep(5);
	printf("Parent is now signalling the semaphore...\n");
	V(semid, mutex);
	printf("Parent is now waiting for the children to exit.\n");
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);
	wait(0);

	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
	kill(pid3, SIGKILL);
	kill(pid4, SIGKILL);
/*	kill(pid5, SIGKILL);
	kill(pid6, SIGKILL);
	kill(pid7, SIGKILL); */
	printf("Children have exited. Final balance: $%d. Cleaning up...\n", shared->balance);
	shmctl(shmid, IPC_RMID, 0);
	semctl(SEMKEY,1,IPC_RMID,0);
	printf("Closing...");
	return 0;
}
