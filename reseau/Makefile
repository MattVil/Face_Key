all: server

server: server.o util.o
	gcc server.o util.o -o server

server.o: server.c util.h
	gcc -c server.c

util.o: util.c util.h
	gcc -c util.c

clean_object:
	rm -f util.o server.o

clean:
	rm -f util.o server.o server