//Buffer used to convert characters to bit strings
char* bitstringBuff = NULL;

/* 
Reads the next bit from the file described by fd into buffer
@return 0 on success, -1 on failure
@param fd File descriptor of file to read from
@param bitNum Number of bits to read into buffer
@param buffer pointer to buffer to write the bits to 
*/
int bitRead(int fd, int bitNum, char* buffer);

/*
Writes the specified string into the file described by fd such that
each string character is one bit.
@param fd File descriptor of file to write to
@param bits The specified string, must contain only "0"s and "1"s
@return The number of bytes written
*/
int bitWrite(int fd, char* bits);

/*
Return the bitstring representation of a character
@param the specified character
@return the bitstring representation
*/
char* charToBitString(char x);

/*
Return the character represented by a bitstring
@param the specified bitstring
@return the character represented by bitstring
*/
char bitStringToChar(char* bitString);