#import "semex.h"

void child(int amt)
{
    int semid,shmid;
    struct account *shared;
    shmid = shmget(SHMKEY, 0, 0);
    semid = semget(SEMKEY, NUM_SEMS, 0777);
    shared = (struct account *)shmat(shmid, 0, 0);  
    int pid=getpid();
    int amount = amt; 
    
    printf("Child process %d is going to add $%d.\n",pid,amount);
    P(semid, mutex);
    shared->balance = shared->balance + amount;
    printf("Applied amount of %d by Process %d. Current Balance: %d\n",amount,pid,shared->balance);
    V(semid, mutex);
    exit(0);
}

int main()
{
    /* INITIALIZATIONS */
    int shmid,semid,pid1,pid2;
    u_short seminit[1];
    key_t key;
    union semun semctlarg;
    struct account *shared;
    srand(time(NULL));
    key=ftok(".", SEMKEY);
    seminit[0]=0;
    semctlarg.array=seminit;
    semctl(SEMKEY,1,SETALL,semctlarg);
    semid = semget(SEMKEY, NUM_SEMS, 0777 | IPC_CREAT);
    shmid = shmget(SHMKEY, sizeof(struct account), 0777 | IPC_CREAT);
    shared=(struct account *)shmat(shmid, 0, 0); 
    shared->balance = 0;

    /* CUSTOMER CREATION */
    int a,b;
    a = (rand() % 500) - 250;
    sleep(3);
    b = (rand() % 500) - 250;

    if ((pid1=fork())==0)
	child(a);
    if ((pid2=fork())==0)
	child(b);


    /* EXECUTION */
    printf("Balance is initially: %d\n", shared->balance); 
    printf("Child processes are %d and %d.\n",pid1,pid2);
    printf("Parent process is now waiting a while.\n");
    sleep(5);
    printf("Parent is now signalling the semaphore...\n");
    V(semid, mutex);

    printf("Parent is now waiting for the children to exit.\n");
    wait(NULL);
    wait(NULL);
    printf("Children have exited. Final balance: $%d. Cleaning up...\n",);
    semctl(SEMKEY,1,IPC_RMID,0);
    return 0;
}
