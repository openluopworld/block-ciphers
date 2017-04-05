IDIR=./../include
CC=gcc
CFLAGS=-Wall -Os -I$(IDIR)

present_test.out: present_test.o present.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f present_test.out
	rm -f *.o