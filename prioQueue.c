#include "prioQueue.h"

struct prioQueue* createQueue(){

	struct prioQueue* priorityQueue = malloc(sizeof(struct prioQueue));
	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct huffman_char*));
	priorityQueue->size = 0;
	return priorityQueue;

}

void insertQueue(struct huffman_char* huffchar,struct prioQueue* priorityQueue){

	if(priorityQueue->size == 0){

		//If queue is empty, insert at the start
		priorityQueue->huff_queue[0] = huffchar;
		priorityQueue->size++;

	} else if (priorityQueue->size >= QUEUE_MAX - 1){

		fprintf(stderr,"Cannot insert more into full queue.\n");
		return;

	} else {

		//Insert element as close to the rear as possible
		int i;
		for(i = priorityQueue->size-1; i >= 0; i--){
			if(huffchar->frequency < priorityQueue->huff_queue[i]->frequency) priorityQueue->huff_queue[i+1] = priorityQueue->huff_queue[i];
			if(huffchar->frequency >= priorityQueue->huff_queue[i]->frequency) break;
		}
		priorityQueue->huff_queue[i+1] = huffchar;
		priorityQueue->size++;
		
	}

	return;

}

struct huffman_char* removeQueue(struct prioQueue* priorityQueue){

	if(priorityQueue->size == 0) return NULL;
	
	//Grab the front element
	struct huffman_char* toReturn = priorityQueue->huff_queue[0];

	//Push all elements back 1
	int i;
	for(i = 0; i < priorityQueue->size; i++){
		priorityQueue->huff_queue[i] = priorityQueue->huff_queue[i+1];
	}

	priorityQueue->size--;

	return toReturn;

}

void deleteQueue(struct prioQueue* priorityQueue){
	//Free space
	memset(priorityQueue->huff_queue,0,QUEUE_MAX * sizeof(struct huffman_char*) + sizeof(int));
	free(priorityQueue);

}

int queueSize(struct prioQueue* priorityQueue){
	return priorityQueue->size;
}