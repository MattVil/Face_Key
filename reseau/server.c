#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define DEBUG 1
#define BUF_SIZE 80

//Codes Client -> Serveur
#define IDS_REQU 100
#define PSSW_REQU 101
#define PICGPS_UPDT 102

//Codes Serveur -> Client
#define IDS_SD 200
#define PSSW_SD 201

//Codes d'erreurs
#define UKNWREQ 400

char** str_split(char* str, char sep);

int main(){

	char** splited_str;
	char buf [BUF_SIZE];
	int s_ecoute, s_dial, cli_len, i;
	struct sockaddr_in serv_addr, cli_addr;

	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = INADDR_ANY ;
	serv_addr.sin_port = htons(5000) ;
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

	s_ecoute = socket(PF_INET, SOCK_STREAM, 0);
	bind(s_ecoute, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(s_ecoute, 5);
	cli_len = sizeof(cli_addr);
	s_dial = accept(s_ecoute, (struct sockaddr *)&cli_addr, &cli_len);

	memset(buf, 0, BUF_SIZE);
	read(s_dial, buf, BUF_SIZE);
	printf("J'ai recu [%s] du client\n", buf) ;

	splited_str = str_split(buf, ';');
	switch(atoi(splited_str[0])){
		case IDS_REQU:
			printf("ID Request on: %s\n",splited_str[1]);
			break;

		case PSSW_REQU:
			printf("Password Request on: %s\n",splited_str[1]);
			break;

		case PICGPS_UPDT:
			printf("Picture and GPS Update on: %s\n",splited_str[1]);
			break;

		default:
			memset(buf, 0, 80);
        	strcpy(buf, "400;Request Unknown\n");
        	write(s_dial, buf, strlen(buf));
			break;
	}

	close (s_dial) ;
	close (s_ecoute) ;
}

char** str_split(char* str, const char sep){
	char **result, **result_temp;
	char *temp = str;
	char *token;
	int count = 1;
	int i;

	while (*temp){
		if (*temp == sep)
			count++;
		temp++;
	}

	result = malloc(count*sizeof(char*));
	result_temp = result;

	token = strtok(str, &sep);
	while(token != NULL) {
		*result_temp = strdup(token);
		token = strtok(NULL, &sep);
		result_temp++;
	}
	return result;
}