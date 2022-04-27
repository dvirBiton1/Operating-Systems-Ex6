// A C program to demonstrate linked list based implementation of queue
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
 
// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
 
int done = 1;
// A linked list (LL) node to store a queue entry
typedef struct QNode {
	void* key;
	struct QNode* next;
}QNode, *pQNode;

// The queue, front stores the front node of LL and rear stores the
// last node of LL
typedef struct Queue {
	QNode *front, *rear;
}Queue, *pQueue;

// A utility function to create a new linked list node.
pQNode newNode(void* k)
{
	pQNode temp = (pQNode)malloc(sizeof(struct QNode));
	temp->key = k;
	temp->next = NULL;
	return temp;
}

// A utility function to create an empty queue
pQueue createQ()
{
	pQueue q = (pQueue)malloc(sizeof(struct Queue));
	q->front = q->rear = NULL;
	return q;
}
void destoryQ(pQueue q){
    while (q->front !=NULL)
    {
        deQ(q);
    }
    free(q); 
}
// The function to add a key k to q
void enQ(pQueue q, int k)
{
	// Create a new LL node
	pQNode temp = newNode(k);

	// If queue is empty, then new node is front and rear both
	if (q->rear == NULL) {
		q->front = q->rear = temp;
        printf("Signaling condition variable cond1\n");
        pthread_cond_signal(&cond1);
		return;
	}

	// Add the new node at the end of queue and change rear
	q->rear->next = temp;
	q->rear = temp;
}

// Function to remove a key from given queue q
void deQ(pQueue q)
{
     pthread_mutex_lock(&lock);
	// If queue is empty, return NULL.
    if (q->front == NULL) {
 
        // let's wait on condition variable cond1
        printf("Waiting on condition variable cond1\n");
        pthread_cond_wait(&cond1, &lock);
    }
	// Store previous front and move front one node ahead
	pQNode temp = q->front;
	q->front = q->front->next;
	// If front becomes NULL, then change rear also as NULL
	if (q->front == NULL)
		q->rear = NULL;
	free(temp);
    pthread_mutex_unlock(&lock);
}

// Driver Program to test anove functions
int main()
{
	pQueue q = createQ();
    pthread_t tid1, tid2;
    int x = 5;
    void * p[2] = {q, &x};
    pthread_create(&tid1, NULL, deQ, q);
    sleep(5);
    pthread_create(&tid2, NULL, enQ, p);
 
    // wait for the completion of thread 2
    pthread_join(tid2, NULL);
	enQ(q, 10);
	enQ(q, 20);
	deQ(q);
	deQ(q);
	enQ(q, 30);
	enQ(q, 40);
	enQ(q, 50);
	deQ(q);
	printf("Queue Front : %d \n", q->front->key);
	printf("Queue Rear : %d\n", q->rear->key);
    destoryQ(q);
	return 0;
}
