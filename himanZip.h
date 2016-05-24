#include <stdlib.h>

#define USAGE(name) do{									\
	printf(										\
		"%s [-hv] [-o OUTPUT] FILE \n"	\
		"-h 				Displays this help menu, and returns EXIT_SUCCESS.\n"  \
		"-v 				Verbose print relevant compression information.\n"  \
		"FILE 				This is the name of the file to compress.\n"  \
		"-o OUTPUT 			The file name after compression (defaults to FILE.hzip).\n"  \
		,(name)	\
		); \
	} while(0)

struct {
	char data;
	struct treenode* left;
	struct treenode* right;
} treenode;
