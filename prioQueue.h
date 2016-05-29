#define QUEUE_MAX 256

struct prioQueue{
	struct huffman_char huff_queue[QUEUE_MAX];
	int front;
	int rear;
};

/*
Initializes a new priority queue object and returns a pointer to it.
@return Pointer to the newly created priority queue
*/
struct prioQueue* createQueue();

/*
Deletes a priority queue and free's its space from memory.
*/
void deleteQueue(struct prioQueue* priorityQueue);

