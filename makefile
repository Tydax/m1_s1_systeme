CC=gcc
CFLAGS=-m32 -s -Wall -g

try.o: try.c try.h
	$(CC) -o try.o 

try_mup: try.o try.h try_mup.c
	$(CC) -o try_mup try_mup.c $(CLFAGS)

main: main.c
	$(CC) -o main main.c $(CFLAGS)