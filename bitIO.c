#include "bitIO.h"

int bitRead(int fd, int bitNum, char* buffer){

	//Prepare buffer and read from file
	char bitBuff;
	memset(&bitBuff,0,sizeof(char));

	int ptr = 0;
	int readNum = bitNum / 8;
	int extra = bitNum % 8;

	while(readNum-- > 0){
		if(read(fd,&bitBuff,1) < 0) return -1;
		memcpy(&buffer[ptr],charToBitString(bitBuff),8);
		ptr += 8;
	}

	if(extra > 0){
		if(read(fd,&bitBuff,1) < 0) return -1;
		memcpy(&buffer[ptr],charToBitString(bitBuff),extra);
	}

	return 0;

}

int bitWrite(int fd, char* bits){

	int counter = 0;
	char one = (char) 1; 
	int len = strlen(bits);
	char buffer[1];
	buffer[0] = '\0';
	int retVal = 0;

	while(counter < len){

		if(bits[counter] == '1'){
			buffer[0] = buffer[0] | (one << (7 - (counter % 8)));
		} 
		counter++;
		if(counter % 8 == 0){
			write(fd,buffer,1);
			buffer[0] = '\0';
			retVal++;
		}
	}

	if(counter % 8 != 0){
		write(fd,buffer,1);
		retVal++;
	}

	return retVal;
}

char* charToBitString(char x){
	int i;
	//Grab each bit
	for(i = 7; i >= 0; i--){

		//Will evalute to true if the bit is 1
		if(x & (1 << i)) bitstringBuff[7-i] = '1';
		else bitstringBuff[7-i] = '0';
	}
	return bitstringBuff;
}

char bitStringToChar(char* bitString){
	char toReturn = '\0';
	int i;

	//Replace the bits of the null character by 1 if there is a 1 in the bitstring
	for(i = 7;i >= 0; i--) if(bitString[7-i] == '1') toReturn = toReturn | (0x01 << (7-i));
	return toReturn;
}