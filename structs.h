#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define QUEUE_MAX 300

//Priority Queue Structure
struct prioQueue{
	struct huffman_char* huff_queue[QUEUE_MAX];
	int front;
	int rear;
};

//Structure used to map characters to frequencies. Also used to create huffman tree
struct huffman_char{
	char character;
	int frequency;
	char* encoding;
	struct huffman_char* left;
	struct huffman_char* right;
};

//Structure used to create a linkedlist of huffman chars
struct huffman_list{
	struct huffman_char huff;
	struct huffman_list* next;
};