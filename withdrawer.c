#include "semex.h"

void insert(node *pointer, int data)
{
        /* Iterate through the list till we encounter the last node.*/
        while(pointer->next!=NULL)
        {
                pointer = pointer -> next;
        }
        /* Allocate memory for the new node and put data in it.*/
        pointer->next = (node *)malloc(sizeof(node));
        pointer = pointer->next;
        pointer->data = data;
        pointer->next = NULL;
}
int find(node *pointer, int key)
{
        pointer =  pointer -> next; //First node is dummy node.
        /* Iterate through the entire linked list and search for the key. */
        while(pointer!=NULL)
        {
                if(pointer->data == key) //key is found.
                {
                        return 1;
                }
                pointer = pointer -> next;//Search in the next node.
        }
        /*Key is not found */
        return 0;
}
void delete(node *pointer, int data)
{
        /* Go to the node for which the node next to it has to be deleted */
        while(pointer->next!=NULL && (pointer->next)->data != data)
        {
                pointer = pointer -> next;
        }
        if(pointer->next==NULL)
        {
                printf("Element %d is not present in the list\n",data);
                return;
        }
        /* Now pointer points to a node and the node next to it has to be removed */
        node *temp;
        temp = pointer -> next;
        /*temp points to the node which has to be removed*/
        pointer->next = temp->next;
        /*We removed the node which is next to the pointer (which is also temp) */
        free(temp);
        /* Beacuse we deleted the node, we no longer require the memory used for it . 
           free() will deallocate the memory.
         */
        return;
}
void print(node *pointer)
{
        if(pointer==NULL)
        {
                return;
        }
        printf("%d ",pointer->data);
        print(pointer->next);
}

int main(int argc, char *argv[])
{
	int semid,shmid;
	struct account *shared;
	shmid = shmget(SHMKEY, 0, 0);
	semid = semget(SEMKEY, NUM_SEMS, 0777);
	shared = (struct account *)shmat(shmid, 0, 0);  
	int pid=getpid();
	int amount = atoi(argv[1]);
	
   
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
	return 0;
}
