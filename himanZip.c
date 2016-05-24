#include "himanZip.h"
#include "bitRead.c"

int main(int argc, char** argv){

	//Parse inputs
	int i;
	/*int verbose = 0;
	char* output = NULL;
	char* file = NULL; */
	int parseCount = 2;

	if(argc < parseCount) {
		USAGE(argv[0]); //Print help statement if not enough arguments
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < argc-1; i++){

		if(strcmp(argv[i],"-h") == 0){
			USAGE(argv[0]);
			exit(EXIT_SUCCESS);
		}  //-h flag prints help statement

		if(strcmp(argv[i],"-v") == 0){
			//verbose = 1; // Enables verbose output
			parseCount++;
		}

		if(strcmp(argv[i],"-o") == 0){
			if(i+1 >= argc-1){
				USAGE(argv[0]);
				exit(EXIT_FAILURE);
			}

			//output = argv[i+1];
			parseCount += 2;
		}

	}

	if(argc > parseCount){
		USAGE(argv[0]);
		exit(EXIT_FAILURE);
	}

	//file = argv[argc-1];

	return 0;
}