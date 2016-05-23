#include "bitRead.h"

int bitRead(int fd, int bitNum, void* buffer){

	//Prepare buffer and read from file
	char bitBuff;
	memset(&bitBuff,0,sizeof(char));
	if(read(fd,&bitBuff,1) < 0) return -1;

	//Obtain the proper bit
	char mask = 0x01;
	mask = mask << (7 - bitNum);
	char result = (mask & bitBuff) >> (7 - bitNum);
	memcpy(buffer,&result,1);

	//Next bit for the next call
	bitNum++;
	if(bitNum != 8) lseek(fd,-1,SEEK_CUR);
	else bitNum = 0;

	return 0;

}

int main(int argc, char** argv){
	return 0;
}
