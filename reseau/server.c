#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int main(){
	int s_ecoute, s_dial, cli_len;
	struct sockaddr_in serv_addr, cli_addr;

	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = INADDR_ANY ;
	serv_addr.sin_port = htons(5000) ;
	memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

	s_ecoute = socket(PF_INET, SOCK_STREAM, 0);
	bind (s_ecoute, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen (s_ecoute, 5);
	cli_len = sizeof (cli_addr);
	s_dial = accept (s_ecoute, (struct sockaddr *)&cli_addr, &cli_len);

	memset (buf, 0, 80);
	read (s_dial, buf, 80);
}

