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
			case 'h':
				USAGE(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				verbose = 1;
				parseCount++;
				break; 
			case 'd':
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

	if(decompress > 0){
		printf("Still have to do decompression.\n");
		exit(EXIT_SUCCESS);
	}

	/* Open File */

	file = argv[argc-1];
	int fd;
	if((fd = open(file,O_RDONLY)) < 1){
		fprintf(stderr,"Failed to open file");
		exit(EXIT_FAILURE);
	}

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
	void* pQueue = (void *) createQueue(); //use a void pointer to avoid compiler errors
	huffman_tree = generateHuffmanTree(pQueue);
	struct prioQueue* prQueue = (struct prioQueue*) pQueue;

	/* Obtain bit encodings from huffman tree */ 
	char* huffencoding = calloc(1,MAX_ENCODING_SIZE * sizeof(char));
	assignEncodings(huffman_tree,huffencoding);
	free(huffencoding); 

	if(verbose == 1) printEncodingList();

	/* Create Zip file */
	int cfd;
	if((cfd = open(file,O_RDWR | O_CREAT)) < 0){
		fprintf(stderr, "Failed to open file\n");
		exit(EXIT_FAILURE);
	}

	if(lseek(fd, 0, SEEK_SET) < 0){
		fprintf(stderr,"Could not restore file with lseek.\n");
		exit(EXIT_FAILURE);
	}

	char* buf = calloc(1,sizeof(char));
	char* toWrite = calloc(1,10000*sizeof(char));

	/* Begin writing encoding */
	while(read(fd,buf,1) > 0){
		if(strlen(toWrite) > (10000- MAX_ENCODING_SIZE) && strlen(toWrite) % 8 == 0){
			bitWrite(cfd,toWrite);
			memset(toWrite,0,2000*sizeof(char));
		}
		strcat(toWrite,checkEncodingList(*buf)->huff.encoding);
	}
	bitWrite(cfd,toWrite);

	/* Free resources and close files */
	free(buf);
	free(toWrite);
	freeEncodingList();
	deleteQueue(prQueue);
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

	//Loop through the list to find the character
	while(temp != NULL){
		printf("Center: %p, Left: %p , Right: %p\n",(void*) temp, (void*) temp->huff.left,(void*) temp->huff.right);
		temp = temp->next;
	}

}

struct huffman_char* generateHuffmanTree(void* Queue){

	struct prioQueue* pQueue = (struct prioQueue*) Queue;
	struct huffman_list* temp = encoding_list;

	//Loop through the list, Insert each element into a priority queue
	while(temp != NULL){
		insertQueue(&temp->huff,pQueue);
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

void assignEncodings(struct huffman_char* hufftree, char* encoding){

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
		hufftree->encoding = calloc(1,(strlen(encoding)+1) * sizeof(char));
		strcpy(hufftree->encoding,encoding);
		encoding[strlen(encoding) - 1] = '\0'; //Push back the current encoding by 1 character
		return;
	}

	//Assign encoding for left subtree, 0 if you go left
	if(hufftree->left != NULL){
		strcat(encoding,"0");
		assignEncodings(hufftree->left,encoding);
	}

	//Assign encoding for right subtree, 1 if you go right
	if(hufftree->right != NULL){
		strcat(encoding,"1");
		assignEncodings(hufftree->right,encoding);
	}

	return;

}
