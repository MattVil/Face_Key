#include "util.h"

int main(){

	char** splited_str;
	char buf [BUF_SIZE];
	int s_ecoute, s_dial, i, splited_str_size;
	unsigned int cli_len;
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
	if (DEBUG)
		printf("MESSAGE RECEIVED: %s\n", buf) ;

	splited_str = str_split(buf, ';', &splited_str_size);
	//splited_str_size = str_split(buf, ';', splited_str);
	if (DEBUG)
		print_splt_str(splited_str, splited_str_size);


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