IDIR=./../include
CC=gcc
CFLAGS=-Wall -Os -I$(IDIR)

simon_test.out: simon_test.o simon.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f simon_test.out
	rm -f *.o