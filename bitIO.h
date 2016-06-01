#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* 
Reads the next bit from the file described by fd into buffer
@return 0 on success, 1 on failure
@param fd File descriptor of file to read from
@param bitNum integer value from 0-7 describing which bit to read. Incremented on each call.
@param buffer pointer to buffer to write the bit to 
*/
int bitRead(int fd, int bitNum, void* buffer);

/*
Writes the specified string into the file described by fd such that
each string character is one bit.
@param fd File descriptor of file to write to
@param bits The specified string, must contain only "0"s and "1"s
@return The number of bytes written
*/
int bitWrite(int fd, char* bits);