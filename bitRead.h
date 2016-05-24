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