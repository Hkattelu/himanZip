#include "structs.h"
#include "himanZip.h"
#include "bitIO.c"
#include "prioQueue.c"

int main(int argc, char** argv){

	/*Parse inputs*/

	int opt;
	int decompress = 0;
	int verbose = 0;
	char* file = NULL; 
	int parseCount = 2;

	if(argc < parseCount) {
		USAGE(argv[0]); //Print help statement if not enough arguments
		exit(EXIT_FAILURE);
	}

	while((opt = getopt(argc, argv, "hvd")) != -1){
		switch(opt){
			case 'h': //Help
				USAGE(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'v': //Verbose
				verbose = 1;
				parseCount++;
				break; 
			case 'd': //Decompress
				decompress = 1;
				parseCount++;
				break;
			case '?':
				//Invalid option
			default:
				//Not h c or v, check if there are 2 args
				if(argc == 2) break; 
				
				//Invalid option provided
				USAGE(argv[0]);
				exit(EXIT_FAILURE);
				break;
		}
	}

	/* Open File */

	//Check if the file exists
	file = argv[argc-1];

	struct stat sb1;
  	memset(&sb1,0,sizeof(sb1));

	if(stat(file,&sb1) < 0){
		fprintf(stderr,"%s does not exist, aborting compression.\n", file);
		exit(EXIT_FAILURE);
	}

	int fd;
	if((fd = open(file,O_RDONLY)) < 1){
		fprintf(stderr,"Failed to open file. Check hZip permissions.\n");
		exit(EXIT_FAILURE);
	}

	if(decompress > 0){

		//Check if the file is of type .hzip
		if((strstr(file,".hzip")) == NULL){
			fprintf(stderr,"File not of type .hzip, aborting.\n");
			exit(EXIT_FAILURE);
		}

		file[strlen(file) - 5] = '\0';

		//Check if the unzipped version of the file already exists
		struct stat sb;
	  	memset(&sb,0,sizeof(sb));

		if(stat(file,&sb) > -1){
			fprintf(stderr,"%s already exists, aborting compression.\n", file);
			exit(EXIT_FAILURE);
		}

		int cfd;
		if((cfd = open(file,O_RDWR | O_CREAT, 0666)) < 0){
			fprintf(stderr, "Failed to open file. \n");
			exit(EXIT_FAILURE);
		}

		//Start decompression
		
		/* Obtain the Huffman tree length from header bytes */
		char* lengthBuff = "hi";
		read(fd,lengthBuff,2);

		int huffTreeLength = (int) lengthBuff[0] & ((int) lengthBuff[1] << 8);
		bitstringBuff = malloc(9*sizeof(char));

		/* Obtain the Huffman tree from the file */
		char* huffBuffer = calloc(1,MAX_TREE_SIZE * sizeof(char));
		bitRead(fd,huffTreeLength,huffBuffer);
		struct huffman_char* decodeTree = stringToHuffmanTree(huffBuffer);

		/* Obtain the encoding length from header bytes */
		char* encodingLengthBuff = "boom";
		read(fd,encodingLengthBuff,4);
		int encodingLength = (int) encodingLengthBuff[0] 
							& ((int) encodingLengthBuff[1] << 8)
						    & ((int) encodingLengthBuff[2] << 16)
						     & ((int) encodingLengthBuff[3] << 24);

		/* Obtain the encoded bits from the file */
		char* decodeBuffer = calloc(1,MAX_ZIP_FILE_SIZE * sizeof(char));
		bitRead(fd,encodingLength,decodeBuffer);

		/* Finally, decompress the file */
		struct huffman_char* temp = decodeTree;
		while(encodingLength-- > 0){
			if(decodeBuffer[0] == '0'){
				temp = temp->left;
			} else if (decodeBuffer[0] == '1'){
				temp = temp->right;
			} else {
				fprintf(stderr, "Invalid encoding.\n");
				free(decodeBuffer);
				free(bitstringBuff);
				free(huffBuffer);
				close(fd);
				close(cfd);
				exit(EXIT_FAILURE);
			}

			if(temp->left == NULL && temp->right == NULL){
				write(cfd,&temp->character,1);
				temp = decodeTree;
			}

			decodeBuffer++;
		}

		/* Free up resources */
		free(decodeBuffer);
		free(bitstringBuff);
		free(huffBuffer);
		close(fd);
		close(cfd);
	
		return 0;	
	}

	//Check if the zipped version of the file already exists
	strcat(file,".hzip");
	struct stat sb;
  	memset(&sb,0,sizeof(sb));

	if(stat(file,&sb) > -1){
		fprintf(stderr,"%s already exists, aborting compression.\n", file);
		exit(EXIT_FAILURE);
	}

	/* Process the file to obtain an encoding list */

	char* buff = calloc(1,sizeof(char));
	struct huffman_list* encoding;

	//Read through the file and obtain frequences of each character
	while(read(fd,buff,1) > 0){

		if((encoding = checkEncodingList(*buff)) == NULL){

			//If the character does not exist in the list, append it to the start of the list
			struct huffman_list* newEncoding = malloc(sizeof(struct huffman_list));
			strncpy(&(newEncoding->huff.character),buff,1);
			newEncoding->huff.frequency = 1;
			newEncoding->huff.encoding = NULL;
			newEncoding->huff.left = NULL;
			newEncoding->huff.right = NULL;
			newEncoding->next = encoding_list;
			encoding_list = newEncoding;			

		} else {
			//If it does exist, simply increment the frequency
			encoding->huff.frequency++;
		}

	}
	free(buff);

	/* Generate the huffman tree */
	struct prioQueue* pQueue = createQueue(); //use a void pointer to avoid compiler errors
	huffman_tree = generateHuffmanTree(pQueue);

	/* Obtain bit encodings from huffman tree */ 
	char* huffencoding = calloc(1,MAX_ENCODING_SIZE * sizeof(char));
	assignEncodings(huffman_tree,huffencoding,0);
	free(huffencoding); 

	if(verbose == 1) printEncodingList();

	/* Create Zip file */
	int cfd;
	if((cfd = open(file,O_RDWR | O_CREAT, 0666)) < 0){
		fprintf(stderr, "Failed to open file. \n");
		exit(EXIT_FAILURE);
	}

	if(lseek(fd, 0, SEEK_SET) < 0){
		fprintf(stderr,"Could not restore file with lseek.\n");
		exit(EXIT_FAILURE);
	}

	char* buf = calloc(1,sizeof(char));
	char* toWrite = calloc(1,MAX_ZIP_FILE_SIZE*sizeof(char)); 
	bitstringBuff = calloc(1,9 * sizeof(char));

	/* Write the Huffman tree */
	char* huffBuff = calloc(1,MAX_TREE_SIZE*sizeof(char));
	huffmanTreeToString(huffman_tree,huffBuff);
	int huffLen = strlen(huffBuff);

	bitWrite(cfd,charToBitString((char) huffLen >> 8)); //Max tree size of 2^16
	bitWrite(cfd,charToBitString((char) huffLen)); //First write tree length for decompression
	bitWrite(cfd,huffBuff); //Now write tree itself

	/* Begin writing encoding */
	while(read(fd,buf,1) > 0) {
		strcat(toWrite,checkEncodingList(*buf)->huff.encoding);
	}

	//Write the encoding length in bits in a 4-byte header
	int encodingLen = strlen(toWrite);
	bitWrite(cfd,charToBitString((char) encodingLen >> 24));
	bitWrite(cfd,charToBitString((char) encodingLen >> 16));
	bitWrite(cfd,charToBitString((char) encodingLen >> 8));
	bitWrite(cfd,charToBitString((char) encodingLen));

	//Write the actual encoding
	bitWrite(cfd,toWrite);

	/* Free resources and close files */
	free(bitstringBuff);
	free(huffBuff);
	free(buf);
	free(toWrite);
	freeEncodingList();
	deleteQueue(pQueue);
	close(fd);
	close(cfd);

	return 0;
}

struct huffman_list* checkEncodingList(char c){

	struct huffman_list* temp = encoding_list;

	//Loop through the list to find the character
	while(temp != NULL){
		if(c == temp->huff.character) return temp;
		temp = temp->next;
	}

	return NULL; //The character is not in the list

}

void freeEncodingList(){

	if(encoding_list == NULL) return; //Nothing to free

	struct huffman_list* temp = encoding_list;

	//Loop through the list, freeing each node
	while(encoding_list != NULL){
		temp = encoding_list;
		encoding_list = encoding_list->next;
		if(temp->huff.encoding != NULL) free(temp->huff.encoding);
		free(temp);
	}

}

void printEncodingList(){

	struct huffman_list* temp = encoding_list;

	//Loop through the list to print characters
	while(temp != NULL){

		if(temp->huff.character != '*'){
			printf("Char: %c, Frequency: %d, Encoding: %s\n",
				temp->huff.character,
				temp->huff.frequency,
				temp->huff.encoding);
		}
		temp = temp->next;
	}

}

struct huffman_char* generateHuffmanTree(struct prioQueue* Queue){

	struct prioQueue* pQueue = (struct prioQueue*) Queue;
	struct huffman_list* temp = encoding_list;

	//Loop through the list, Insert each element into a priority queue
	while(temp != NULL){
		insertQueue(&(temp->huff),pQueue);
		temp = temp->next;
	}

	//Generate the huffman tree
	struct huffman_char* leftnode;
	struct huffman_char* rightnode;

	while(queueSize(pQueue) > 1){

		//Pop two nodes
		leftnode = removeQueue(pQueue);
		rightnode = removeQueue(pQueue);

		//Generate a new node whos frequency is the sum of the two popped nodes
		struct huffman_list* newNode = malloc(sizeof(struct huffman_list));
		strcpy(&(newNode->huff.character),"*");
		newNode->huff.frequency = leftnode->frequency + rightnode->frequency;
		newNode->huff.encoding = NULL;
		newNode->next = encoding_list;
		encoding_list = newNode;

		//Set the two popped nodes to be the children of the new node
		newNode->huff.left = leftnode;
		newNode->huff.right = rightnode;

		//Inset the new node into the queue
		insertQueue(&newNode->huff,pQueue);

	}

	//Obtain the root of the huffman tree
	return removeQueue(pQueue);

}

void assignEncodings(struct huffman_char* hufftree, char* encoding, int length){

	//If we 've reached a leaf node, assign it
	if(hufftree->left == NULL && hufftree->right == NULL){

		//Find the leaf node character in the encoding list
		struct huffman_list* temp = encoding_list;
		while(temp != NULL){

			if(&temp->huff == hufftree) break;
			temp = temp->next;
		}

		if(temp == NULL) return; //The character did not exist in the encoding list

		//If it exists in the list, copy over our existing encoding into it
		hufftree->encoding = calloc(1,(length+1) * sizeof(char));
		strcpy(hufftree->encoding,encoding);
		encoding[length - 1] = '\0'; //Push back the current encoding by 1 character
		return;
	}

	//Assign encoding for left subtree, 0 if you go left
	if(hufftree->left != NULL){
		strcat(encoding,"0");
		assignEncodings(hufftree->left,encoding,length+1);
	}

	//Assign encoding for right subtree, 1 if you go right
	if(hufftree->right != NULL){
		strcat(encoding,"1");
		assignEncodings(hufftree->right,encoding,length+1);
	}

	encoding[length - 1] = '\0';
	return;

}

void huffmanTreeToString(struct huffman_char* hufftree, char* returnString){

	if(hufftree->left == NULL && hufftree->right == NULL){
		//Leaf nodes are preceded by a "1"
		strcat(returnString,"1");
		strcat(returnString,charToBitString(hufftree->character));
	} else {
		//Non-leaves are preceded by a "0"
		strcat(returnString,"0");
	}

	if(hufftree->left != NULL) {
		huffmanTreeToString(hufftree->left,returnString); //Left subtree
	}

	if(hufftree->right != NULL) {
		huffmanTreeToString(hufftree->right,returnString); //Right subtree
	}

	return;
}

struct huffman_char* stringToHuffmanTree(char* huffString){

	if(huffString[0] == '1'){

		//1's represent leaf nodes and are succeeded by 8 bits representing the character.
		struct huffman_char* leaf = calloc(1,sizeof(struct huffman_char));
		leaf->character = bitStringToChar(++huffString);
		huffString += 8;
		return leaf;

	} else if (huffString[0] == '0'){

		//0's represent non-leaf nodes and are succeeded by the left subtree, then the right
		struct huffman_char* node = calloc(1,sizeof(struct huffman_char));

		node->left = stringToHuffmanTree(++huffString);
		node->right = stringToHuffmanTree(huffString);

		return node;
	}

	return NULL; //Invalid huffman string
	
}
