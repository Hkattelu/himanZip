#define QUEUE_MAX 300

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
Insert a specified huffman pairing into a priority queue.
@param huffchar The pairing to insert
@param priorityQueue The priority queue to insert the pairing into
*/
void insertQueue(struct huffman_char huffchar,struct prioQueue* priorityQueue);

/*
Pop an element off of a specified priority queue.
@param priorityQueue The priority queue to pop
@return a pointer to the popped pairing. NULL if the queue is empty.
*/
struct huffman_char* removeQueue(struct prioQueue* priorityQueue);

/*
Deletes a priority queue and free's its space from memory.
@param priorityQueue the priority queue to delete
*/
void deleteQueue(struct prioQueue* priorityQueue);

/*
Return the number of elements currently in a queue
@param priorityQueue the queue to check
@return the number of elements in the queue
*/
int queueSize(struct prioQueue* priorityQueue);





