#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define USAGE(name) do{									\
	printf(										\
		"%s [-hv] FILE \n"	\
		"-h 				Displays this help menu, and returns EXIT_SUCCESS.\n"  \
		"-v 				Verbose print relevant compression information.\n"  \
		"FILE 				This is the name of the file to compress (becomes FILE.hzip).\n"  \
		,(name)	\
		); \
	} while(0)

//Seems like a good arbitary limit
#define MAX_ENCODING_SIZE 50

struct treenode{
	char data;
	struct treenode* left;
	struct treenode* right;
};

struct huffman_char{
	char character;
	int frequency;
	struct huffman_char* next;
} ;

//List of characters and frequencies
struct huffman_char* encoding_list = NULL;

//Huffman tree used to encode characters
struct treenode* huffman_tree = NULL;

/*
Check if character c exists in our encoding list.
@param c The character to check
@return If it exists, return a pointer to the struct containing the character. Otherwise, NULL
*/
struct huffman_char* checkEncodingList(char c);

/*
Free's the memory malloc'd for the encoding list.
*/
void freeEncodingList();

/*
Prints the contents of the encoding list.
*/
void printEncodingList();

/*
Generates the huffman tree for the existing encoding list.
@return Pointer to the root of the huffman tree. NULL upon error
*/
struct treenode* generateHuffmanTree();