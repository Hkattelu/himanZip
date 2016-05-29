#include "prioQueue.h"

struct prioQueue* createQueue(){

	struct prioQueue priorityQueue* = malloc(sizeof(struct priorityQueue));
	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct priorityQueue));
	priorityQueue->front = -1;
	priorityQueue->rear = -1;
	return priorityQueue;

}

void deleteQueue(struct prioQueue* priorityQueue){

	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct priorityQueue) + 2*sizeof(int));
	free(priorityQueue);
	
}

