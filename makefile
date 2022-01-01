CC=gcc
FLAGS= -Wall -g

all: test

test: main.o
	$(CC) $(FLAGS) -o test main.o -lm

main.o: main.c
	$(CC) $(FLAGS) -c main.c -lm


.PHONY: clean


clean:
	rm -f *.o test main.o