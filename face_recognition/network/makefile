all: main

main.o: main.c
	gcc -c main.c

mlp.o: mlp.c mlp.h
	gcc -c mlp.c -lm

load_mnist.o: load_mnist.c load_mnist.h
	gcc -c load_mnist.c

main: mlp.o main.o load_mnist.o
	gcc main.o mlp.o load_mnist.o -o main -lm

clear:
	rm -f *.o main
