all: himanZip

himanZip:
	gcc -Wall -Werror himanZip.c -o himanZip

clean:
	rm -f *~ *.o himanZip
