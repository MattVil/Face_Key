all: server client take_picture

server: server.o util.o dbutil.o
	gcc -ggdb -Wall -Wextra server.o util.o dbutil.o -o server -lpq -lcrypto -lssl

client: client.o util.o ipcTools.o
	gcc -ggdb -Wall -Wextra client.o util.o ipcTools.o -o client -lcrypto `pkg-config --cflags opencv` `pkg-config --libs opencv`

take_picture:
	g++ take_picture.cpp -o take_picture `pkg-config --cflags opencv` `pkg-config --libs opencv`

server.o: server.c util.h
	gcc -c server.c -lssl

client.o: client.c util.h
	gcc -c client.c

ipcTools.o: ipcTools.c ipcTools.h
	gcc -c ipcTools.c

util.o: util.c util.h
	gcc -c util.c

dbutil.o: dbutil.c dbutil.h
	gcc -c dbutil.c -lpq

clean_object:
	rm -f util.o server.o client.o

clean_take_picture:
	rm -f take_picture

clean:
	rm -f util.o server.o client.o dbutil.o server client take_picture
