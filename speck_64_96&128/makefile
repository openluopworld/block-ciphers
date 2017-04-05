IDIR=./../include
CC=gcc
CFLAGS=-Wall -Os -I$(IDIR)

speck_test.out: speck_test.o speck.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f speck_test.out
	rm -f *.o