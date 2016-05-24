#include "himanZip.h"
#include "bitRead.c"

int main(int argc, char** argv){

	/*Parse inputs*/

	//int verbose = 0;
	char* file = NULL; 

	int parseCount = 2;

	if(argc < parseCount) {
		USAGE(argv[0]); //Print help statement if not enough arguments
		exit(EXIT_FAILURE);
	}

	int i;
	for(i = 0; i < argc-1; i++){

		//-h flag prints help statement
		if(strcmp(argv[i],"-h") == 0){
			USAGE(argv[0]);
			exit(EXIT_SUCCESS);
		}  

		//-v enables verbose output
		if(strcmp(argv[i],"-v") == 0){
			//verbose = 1;
			parseCount++;
		}

	}

	if(argc > parseCount){
		USAGE(argv[0]);
		exit(EXIT_FAILURE);
	}


	/* Open File */

	file = argv[argc-1];
	int fd;
	if((fd = open(file,O_RDONLY)) < 1){
		fprintf(stderr,"Failed to open file");
		exit(EXIT_FAILURE);
	}


	/* Process the file to obtain an encoding list */

	char buff = '\0';
	struct huffman_list* encoding;

	//Read through the file and obtain frequences of each character
	while(read(fd,&buff,1) > 0){

		if((encoding = checkEncodingList(buff)) == NULL){

			//If the character does not exist in the list, append it to the start of the list
			struct huffman_list* newEncoding = malloc(sizeof(struct huffman_list));
			strcpy(&(newEncoding->huff.character),&buff);
			newEncoding->huff.frequency = 1;
			newEncoding->next = encoding_list;
			encoding_list = newEncoding;			

		} else {
			//If it does exist, simply increment the frequency
			encoding->huff.frequency++;
		}

	}

	/* Generate the huffman encodings of each character */

	printEncodingList();
	freeEncodingList();

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
		free(temp);
	}

}

void printEncodingList(){

	struct huffman_list* temp = encoding_list;

	//Loop through the list to find the character
	while(temp != NULL){
		printf("Char: %c , Freq: %d\n",temp->huff.character,temp->huff.frequency);
		temp = temp->next;
	}

}

struct treenode* generateHuffmanTree(){

	

	return NULL;

}