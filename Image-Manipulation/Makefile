CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g
SUBDIR=/Out/
LIBS = -lm

main: main.o ppm_io.o
	$(CC)  main.o ppm_io.o  $(LIBS) -o project


main.o: main.c ppm_io.h
	$(CC) $(CFLAGS)  -c main.c $(LIBS)

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c $(LIBS)

clean:
	rm -f *.o


