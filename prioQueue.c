#include "prioQueue.h"

struct prioQueue* createQueue(){

	struct prioQueue* priorityQueue = malloc(sizeof(struct prioQueue));
	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct huffman_char*));
	priorityQueue->front = -1;
	priorityQueue->rear = -1;
	return priorityQueue;

}

void insertQueue(struct huffman_char* huffchar,struct prioQueue* priorityQueue){

	if(priorityQueue->front == -1 || priorityQueue->front > priorityQueue->rear){

		//If queue is empty, insert at the start
		priorityQueue->huff_queue[0] = huffchar;
		priorityQueue->front = 0;
		priorityQueue->rear = 0;

	} else {

		//Go through the queue to find a spot to place the char
		int index = priorityQueue->front;
		while(huffchar->frequency < priorityQueue->huff_queue[index]->frequency && index <= priorityQueue->rear) 
			index++;

		//Storage variables
		int counter = index;
		struct huffman_char* tempchar = priorityQueue->huff_queue[counter];
		struct huffman_char* tempchar2 = priorityQueue->huff_queue[counter];

		//Copy over the rest of the list, then insert the char
		while(counter <= priorityQueue->rear){
			counter++;
			tempchar2 = priorityQueue->huff_queue[counter];
			priorityQueue->huff_queue[counter] = tempchar;
			tempchar = tempchar2;
		}

		//Set value and increment end index
		priorityQueue->huff_queue[index] = huffchar;
		priorityQueue->rear++;

	}

	return;

}

struct huffman_char* removeQueue(struct prioQueue* priorityQueue){

	if(priorityQueue->front == -1 || priorityQueue->front > priorityQueue->rear) return NULL;
	
	//Grab the front element
	priorityQueue->front++;
	return priorityQueue->huff_queue[priorityQueue->front-1];

}

void deleteQueue(struct prioQueue* priorityQueue){

	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct huffman_char*) + 2*sizeof(int));
	free(priorityQueue);

}

int queueSize(struct prioQueue* priorityQueue){
	return priorityQueue->rear - priorityQueue->front + 1;
}