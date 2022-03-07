CC=gcc
CFLAGS=-std=gnu99

createwav: createwav.o makefile
	$(CC) $(CFLAGS) -o $@ createwav.o -lm
	
clean:
	rm -f createwav *.o
