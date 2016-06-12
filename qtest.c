#include "structs.h"

#include "prioQueue.c"

int main(int argc, char** argv){

	struct prioQueue* pQueue = createQueue();

	int i;
	for(i = QUEUE_MAX - 1;i > 0;i--){
		struct huffman_char* tempChar = malloc(sizeof(struct huffman_char));
		tempChar->frequency = i;
		insertQueue(tempChar,pQueue);
	}

	for(i = 0;i< QUEUE_MAX - 1;i++){
		struct huffman_char* temp = removeQueue(pQueue);
		printf("Value: %d\n", temp->frequency);
		free(temp);
	}

	deleteQueue(pQueue);
	return 0;
}
