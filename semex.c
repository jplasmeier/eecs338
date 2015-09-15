#include "semex.h"

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
		execl("depositer.bin", &a);	
		exit(EXIT_SUCCESS);
	}
	if ((pid2=fork())==0) {
		execl("depositer.bin", &b);
		exit(EXIT_SUCCESS);
	}
	if ((pid3=fork())==0) {
		execl("withdrawer.bin", &c);
		exit(EXIT_SUCCESS);
	}
	if ((pid4=fork())==0) {
		execl("withdrawer.bin", &d);
		exit(EXIT_SUCCESS);
	}
	if ((pid5=fork())==0) {
		execl("depositer.bin", &e);
		exit(EXIT_SUCCESS);
	}
	if ((pid6=fork())==0) {
		execl("depositer.bin", &f);
		exit(EXIT_SUCCESS);
	}
	if ((pid7=fork())==0) {
		execl("withdrawer.bin", &g);
		exit(EXIT_SUCCESS);
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
