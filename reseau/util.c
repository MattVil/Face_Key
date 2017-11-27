#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

void send(int socket, int code, char* info, char* buf, int bufsize){
	char* str_code;
	itoa(code, str_code, 10)
	memset(buf, 0, bufsize);
	strcpy(buf, strcat(str_code, info));
	write(s_dial, buf, strlen(buf));
}