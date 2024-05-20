#include "queue.h"
#include "lab3.h"

// Exercise 2: implement a concurrent queue

// TODO: define your synchronization variables here
// (hint: don't forget to initialize them)

// Synchronization variables
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t busyQueue = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;


int activeDispatch = 0;
int activeWorker = 0;

int count = 0;
req_t *currHead;

// add a new request to the end of the queue
// NOTE: queue must be implemented as a monitor
void enqueue(queue_t *q, req_t *t) {
    pthread_mutex_lock(&queueMutex);
    while (activeWorker > 0){
		pthread_cond_wait(&busyQueue, &queueMutex);
	}
    activeDispatch++;
    req_t *curTail = q->tail;
    if (curTail == NULL) {
        pthread_cond_signal(&empty);
        q->head = t;
        q->tail = t;
    } else {
        curTail->next = t;
        q->tail = t;
    }
    count += 1;
    activeDispatch--;
    pthread_mutex_unlock(&queueMutex);
}

// pop a request from the head of the queue.
// if the queue is empty, the thread should wait.
// NOTE: queue must be implemented as a monitor
req_t* dequeue(queue_t *q) {
    pthread_mutex_lock(&queueMutex);
    while (count <= 0 || activeDispatch > 0) {
        pthread_cond_wait(&empty, &queueMutex); // Wait for dispatcher threads to finish
    }
    activeWorker++;
    currHead = q->head;
    req_t *newHead = currHead->next;
    if (newHead != NULL) {
        q->head = newHead;
    } else {
        q->head = NULL;
        q->tail = NULL;
        pthread_cond_signal(&busyQueue);
    }
    count--;
    activeWorker--;
    pthread_mutex_unlock(&queueMutex);
    return currHead;
}

// return the number of requests in the queue.
// NOTE: queue must be implemented as a monitor
int queue_count(queue_t *q) {
	pthread_mutex_lock(&queueMutex);
    pthread_mutex_unlock(&queueMutex);
    return count;
}

