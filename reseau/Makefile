all: server

server: server.o util.o dbutil.o
	gcc server.o util.o dbutil.o -o server -lpq

server.o: server.c util.h
	gcc -c server.c

util.o: util.c util.h
	gcc -c util.c

dbutil.o: dbutil.c dbutil.h
	gcc -c dbutil.c -lpq

clean_object:
	rm -f util.o server.o

clean:
	rm -f util.o server.o server