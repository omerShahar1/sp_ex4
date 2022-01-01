CC=gcc
FLAGS= -Wall -g

all: graph

graph: main.o
	$(CC) $(FLAGS) -o graph main.o -lm

main.o: main.c
	$(CC) $(FLAGS) -c main.c -lm


.PHONY: clean


clean:
	rm -f *.o graph main.o