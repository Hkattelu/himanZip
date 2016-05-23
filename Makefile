all: bitIO

bitIO:
	gcc -Wall -Werror bitIO.c -o bitIO

clean:
	rm -f *~ *.o bitIO
