all: server client take_picture

server: server.o util.o dbutil.o
	gcc -ggdb -Wall -Wextra server.o util2.o dbutil.o -o server -lpq 

client: client_with_chrome.o util.o
	gcc -ggdb -Wall -Wextra client_with_chrome.o util.o -o client 

take_picture:
	g++ take_picture.cpp -o take_picture `pkg-config --cflags opencv` `pkg-config --libs opencv`

server.o: server.c util2.h
	gcc -c server.c -lssl

client_with_chrome.o: client_with_chrome.c util2.h
	gcc -c client_with_chrome.c

util.o: util2.c util2.h
	gcc -c util2.c

dbutil.o: dbutil.c dbutil.h
	gcc -c dbutil.c -lpq

clean_object:
	rm -f util2.o server.o client_with_chrome.o

clean_take_picture:
	rm -f take_picture

clean:
	rm -f util2.o server.o client_with_chrome.o dbutil.o server client take_picture
