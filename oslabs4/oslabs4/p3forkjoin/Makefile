all: main

CFLAGS= -pthread -fcommon
CC=gcc $(CFLAGS)

main: forkpool
	$(CC) -c -o main.o main.c
	$(CC) -o forkjoin forkpool.o main.o

forkpool:
	$(CC) -c -o forkpool.o forkpool.c

clean:
	rm *.o
	rm forkjoin
