all: main

main.o: main.c
	gcc -c main.c

mlp.o: mlp.c mlp.h
	gcc -c mlp.c -lm

main: mlp.o main.o
	gcc main.o mlp.o -o main -lm

clear:
	rm -f *.o main
