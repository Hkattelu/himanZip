//Usage statement
#define USAGE(name) do{									\
	printf(										\
		"%s [-hv] FILE \n"	\
		"-h\tDisplays this help menu, and returns EXIT_SUCCESS.\n"  \
		"-v\tVerbose print relevant compression information.\n"  \
		"-d\tDecompress FILE. File must be of type .hzip\n"  \
		"FILE\tThis is the name of the file to compress (becomes FILE.hzip).\n"  \
		,(name)	\
		); \
	} while(0)

//Seems like a good arbitary limit
#define MAX_ENCODING_SIZE 500

//List of characters and frequencies
struct huffman_list* encoding_list = NULL;

//Huffman tree used to encode characters
struct huffman_char* huffman_tree = NULL;

//Buffer used to convert characters to bit strings
char* bitstringBuff = NULL;

/*
Check if character c exists in our encoding list.
@param c The character to check
@return If it exists, return a pointer to the struct containing the character. Otherwise, NULL
*/
struct huffman_list* checkEncodingList(char c);

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
@param Queue Pointer to empty priority queue
@return Pointer to the root of the huffman tree. NULL upon error
*/
struct huffman_char* generateHuffmanTree(struct prioQueue* Queue);

/*
Assigns encodings to characters in the encoding list according
to a specified huffman tree.
@param hufftree The root of the specified huffman tree
@param encoding The empty string
@param length Zero
*/
void assignEncodings(struct huffman_char* hufftree, char* encoding, int length);

/*
Return the string representation of a huffman tree.
@param hufftree Pointer to the root of the specified huffman tree
@param returnString Buffer to place the string into
*/
void huffmanTreeToString(struct huffman_char* hufftree, char* returnString);

/*
Return the bitstring representation of a character
@param the specified character
@return the bitstring representation
*/
char* charToBitString(char x);