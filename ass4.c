#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

#define SEMKEY 77

/* This code tests semaphores.  It forks two processes which both wait
on a semaphore initialized to zero.  The parent then signals the semaphore,
and only one of the children continues. The parent will clean up and exit
when the child processes both terminate.
 
  Use gcc (not cc) to compile this code.
 
  You can use unix commands (not system calls) ipcs and ipcrm from the 
keyboard to list the existing semaphores and to remove them. */

int semkey;
int birds = 4;
/*  This is the implementation of the wait() primitive. */
void P(int semaphore)
{
    struct sembuf psembuf;

    psembuf.sem_op = -1;
    psembuf.sem_flg = 0;
    psembuf.sem_num = semaphore;
    semop(semkey,&psembuf,1);
    return;
}

/* This is the implementation of the signal() primitive. */
void V(int semaphore)
{
    struct sembuf vsembuf;

    vsembuf.sem_op = 1;
    vsembuf.sem_flg = 0;
    vsembuf.sem_num = semaphore;
    semop(semkey,&vsembuf,1);
    return;
}

void add_child()
{
    int pid=getpid();
    printf("Child process %d is starting up...\n",pid);
    P(0);
    birds = birds + 5;
    printf("Child process %d has cleared the semaphore!\n",pid);
    V(0);
    exit(0);
}

void dec_child()
{
    int pid=getpid();
    printf("Child process %d is starting up...\n",pid);
    P(0);
    birds = 0;
    printf("Child process %d has cleared the semaphore!\n",pid);
    V(0);
    exit(0);
}


union semun {
        int             val;            /* value for SETVAL */
        struct semid_ds *buf;           /* buffer for IPC_STAT & IPC_SET */
        unsigned short  *array;         /* array for GETALL & SETALL */
};

int main()
{
    int pid1,pid2;
    u_short seminit[1];
    key_t key;
    union semun semctlarg;

    key=ftok(".", SEMKEY);
    semkey=semget(key,1,0777|IPC_CREAT);
    seminit[0]=0;
    semctlarg.array=seminit;
    semctl(semkey,1,SETALL,semctlarg);

    if ((pid1=fork())==0)
	add_child();
    if ((pid2=fork())==0)
	dec_child();

    printf("Child processes are %d and %d.\n",pid1,pid2);
    printf("Parent process is now waiting a while.\n");
    sleep(5);
    printf("Parent is now signalling the semaphore...\n");
    V(0);

    printf("Parent is now waiting for the children to exit.\n");
    wait(NULL);
    wait(NULL);
    printf("Children have exited.  Cleaning up...\n");
    semctl(semkey,1,IPC_RMID,0);
    return 0;
}




