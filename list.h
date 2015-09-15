#include<stdio.h>
#include<stdlib.h>

typedef struct Node 
{
        int data;
        struct Node *next;
}node;


void* insert(node*, int);
int* find(node*, int);
void* delete(node*, int);
void* print(node*);

