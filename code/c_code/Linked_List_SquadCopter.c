#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

struct Node{		//Node structure
	int data;
	struct Node* next;
};

typedef struct Node node_t;

typedef struct{		//LinkedList structure
	node_t* head;
}Linked_List;

Linked_List LL;
pthread_mutex_t mutex; //mutex
#define NUM_THREADS 3

void Print_List(Linked_List* LinkedList);               //prints the data of each Link
void Insert_Node_First(Linked_List* LinkedList,int data);
void Insert_Node_Last(Linked_List* LinkedList,int data);

void* task1(void* args){
    pthread_mutex_lock(&mutex);
    long t=(long)args;
    printf("Task %ld started\n",t);
    for(int i=0;i<5;i++){
        Insert_Node_First(&LL,i+10*t);
    }
    printf("Task %ld finished\n",t);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* task2(void* args){
    pthread_mutex_lock(&mutex);
    long t=(long)args;
    printf("Task %ld started\n",t);
    for(int i=0;i<5;i++){
        Insert_Node_Last(&LL,i+10*t);
    }
    printf("Task %ld finished\n",t);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void Insert_Node_First(Linked_List* LinkedList,int data){
	node_t* newNode=(node_t*)malloc(sizeof(node_t));		
	newNode->data=data;	 
    newNode->next = LinkedList->head;                   
	LinkedList->head=newNode;	
}

void Insert_Node_Last(Linked_List* LinkedList,int data){
	node_t* newNode=(node_t*)malloc(sizeof(node_t));		
	newNode->data=data;
    newNode->next=NULL;
    if(LinkedList->head == NULL){
        LinkedList->head=newNode;
        return;
    }
    node_t* current=LinkedList->head;
    while(current->next!=NULL){
        current=current->next;
    }
    current->next=newNode;	
}

void Print_List(Linked_List* LinkedList){
	node_t* current=LL.head;
	if(current!=NULL){
        current=LL.head;
        while(current->next!=NULL){
             printf("[%i]->",current->data);
            current=current->next;
        }
        printf("[%i]",current->data);
    }
}
void free_Malloc(void){
    node_t* current = LL.head;
    if(current->next==NULL){
        free(current);
        LL.head=NULL;         
    }
    else{
        while(LL.head->next!=NULL){
            current=LL.head;
            while(current->next->next!=NULL){
                current=current->next;
            }
            free(current->next);
            current->next=NULL;
        }
        free(LL.head);
        LL.head=NULL;
    }
}

int main(){
    LL.head=NULL;
    printf("Creating Thread 1 \n");
    pthread_mutex_init(&mutex,NULL);
    
    pthread_t threads[NUM_THREADS];
    long t;
    for (t = 0; t < NUM_THREADS; t++){
    if (pthread_create(&threads[t], NULL,task1, (void *)t)){
        printf("Error creating thread %ld\n", t);
        return 1;
        }
    }
    for (t = 0; t < NUM_THREADS; t++){
        pthread_join(threads[t], NULL);
    }
    Print_List(&LL);
    free_Malloc();   
    return 0;
}