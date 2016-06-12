#include "structs.h"

#include "prioQueue.c"

int main(int argc, char** argv){

	struct prioQueue* pQueue = createQueue();

	int i;
	for(i = 0;i<QUEUE_MAX;i++){
		struct huffman_char* tempChar = malloc(sizeof(struct huffman_char));
		tempChar->frequency = i;
		insertQueue(tempChar,pQueue);
		free(tempChar);
	}

	for(i = 0;i< QUEUE_MAX;i++){
		removeQueue(pQueue);
	}

	for(i = 0;i<QUEUE_MAX;i++){
		struct huffman_char* tempChar = malloc(sizeof(struct huffman_char));
		tempChar->frequency = i;
		insertQueue(tempChar,pQueue);
		free(tempChar);
	}


	deleteQueue(pQueue);

}