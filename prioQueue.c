#include "prioQueue.h"

struct prioQueue* createQueue(){

	struct prioQueue priorityQueue* = malloc(sizeof(struct priorityQueue));
	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct priorityQueue));
	priorityQueue->front = -1;
	priorityQueue->rear = -1;
	return priorityQueue;

}

void insertQueue(struct huffman_char huffchar,struct prioQueue* priorityQueue){

	if(priorityQueue->front == -1 || priorityQueue->front > priorityQueue->rear){

		//If queue is empty, insert at the start
		priorityQueue->huff_queue[0] = huffchar;
		front = 0;
		rear = 0;

	} else {

		//Go through the queue to find a spot to place the char
		int index = front;
		while(huffman_char.frequency < priorityQueue->huff_queue[index].frequency && index <= rear) index++;

		int counter = index;
		//Copy over the rest of the list, then insert the char
		while(counter < QUEUE_MAX){
			priorityQueue->huff_queue[counter+1] = priorityQueue->huff_queue[counter];
			counter++;
		}

		//Set value and increment end index
		priorityQueue->huff_queue[index] = huffchar;
		rear++;

	}

	return;

}

struct huffman_char* removeQueue(struct prioQueue* priorityQueue){

	if(priorityQueue->front == -1 || priorityQueue->front > priorityQueue->rear) return NULL;
	
	front++;
	return priorityQueue->huff_queue[front-1];

}

void deleteQueue(struct prioQueue* priorityQueue){

	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct priorityQueue) + 2*sizeof(int));
	free(priorityQueue);

}