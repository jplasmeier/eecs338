#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
    int pid,x=0, status1, status2;
    printf("Parent will spawn two child processes...\n");
    if ((pid=fork())==0)
     {
         pid=getpid();
         printf("First child process  %d is starting up...\n",pid);
 
         x=1;

         printf("First child process  %d has modified x as %d! \n",pid,x);
         printf("First child process  %d is going into a sleep!\n",pid);
         sleep(10);
         printf("First child process  %d is awake now!\n",pid);
         printf("The value of x is still %d.\n ",x);
         printf("      Second child has modified a different copy of x!\n");
         printf("First child process  %d is exiting!\n",pid);
         exit(1);
      }

 
    else if ((pid=fork())==0)
 
      {
        pid=getpid();
        printf("Second child process  %d is starting up...\n",pid);
 
        x=2;
 
        printf("Second child process  %d has modified x as %d!\n",pid,x);
        printf("Second child process  %d exiting...\n",pid);
        exit(2);
       };
 
    printf("Parent is going into a wait for the children to exit...\n");
    wait(&status1);
    wait(&status2);
 
    sleep(15);
    printf("Children have exited, and parent is awake....\n");
 
    printf("The value of x is still %d.\n",x);
    printf("        Children have modified different copies of x!\n");
 
    printf("Parent exiting...\n");
}


