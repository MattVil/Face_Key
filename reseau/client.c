#include "util.h"

#define WORDY_DEBUG 0
#define ID_CLIENT 1

char* IP_SERV = "127.0.0.1";
int PORT_SERV = 5000;

int main(int argc, char const *argv[]) {

  /*Socket var*/
  char buf [BUF_SIZE];
	int s_cli;
	struct sockaddr_in serv_addr;

	s_cli = socket(PF_INET, SOCK_STREAM, 0);

  /*Config serveur*/
  serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP_SERV);
	serv_addr.sin_port = htons(PORT_SERV);
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));


  /*connection*/
  connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
  if(WORDY_DEBUG)
    printf("Connection au serveur OK\n");

  char choise;

  while(1){

    scanf("%c", &choise);

    switch(choise){
      case 'p': //premiere connection,creation du compte FK
        break;
      case 'c': //connection à un site
        break;
      case 'u': //update quotidienne des poids du réseau
        break;
      case 't': //transmission quotidienne des photos
        break;
      default :
        printf("Mauvais choix, veuillez entrer une nouvelle lettre\n");
    }

  }

  return 0;
}

int first_conn_routine(/*...*/){

}

int conn_to_website_routine(/*...*/){

}

int weight_update_routine(/*...*/){

}

int photo_transfer_routine(/*...*/){

}
