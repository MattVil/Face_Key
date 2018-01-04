all: server client

server: server.o util.o dbutil.o
	gcc server.o util.o dbutil.o -o server -lpq

client: client.o util.o
	gcc client.o util.o -o client

server.o: server.c util.h
	gcc -c server.c

client.o: client.c util.h
	gcc -c client.c

util.o: util.c util.h
	gcc -c util.c

dbutil.o: dbutil.c dbutil.h
	gcc -c dbutil.c -lpq

clean_object:
	rm -f util.o server.o client.o

clean:
	rm -f util.o server.o client.o dbutil.o server client
