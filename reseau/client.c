#include "util.h"

#define WORDY_DEBUG 0
#define ID_CLIENT 1


int conn_to_website_routine(char buf[BUF_SIZE]);


char* IP_SERV = "127.0.0.1";
int PORT_SERV = 5000;

int main(int argc, char const *argv[]) {

  /*Socket var*/
  char buf[BUF_SIZE];
  memset (buf, 0, BUF_SIZE);
	int s_cli;
	struct sockaddr_in serv_addr;

	s_cli = socket(PF_INET, SOCK_STREAM, 0);

  /*Config serveur*/
  serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP_SERV);
	serv_addr.sin_port = htons(PORT_SERV);
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

  /*connection*/
  //connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
  if(WORDY_DEBUG)
    printf("Connection au serveur OK\n");

  char choise;

  printf("-----------------------------------------\n");
  printf("|\t\t\t\t\t|\n");
  printf("|\tVous êtes le client %d\t\t|\n", ID_CLIENT);
  printf("|\t\t\t\t\t|\n");
  printf("-----------------------------------------\n");

  printf("\n\nChoix :\n");
  printf("\tp - Simuler une premiere connection\n");
  printf("\tc - Vous connecter à un site\n");
  printf("\tu - Mettre à jour la version de votre réseau de neurones\n");
  printf("\tt - Transmettre les photos de vous pour amélioration du réseau\n");
  printf("\tq - Quitter\n");

  while(1){

    printf("\nVotre choix (p/c/u/t/q): ");

    scanf("%c", &choise);

    //system ("clear");

    switch(choise){
      case 'p': //premiere connection,creation du compte FK
        break;
      case 'c': //connection à un site
        conn_to_website_routine(buf);
        break;
      case 'u': //update quotidienne des poids du réseau
        break;
      case 't': //transmission quotidienne des photos
        break;
      case 'q':
        exit(0);
        break;
      default :
        //printf("Mauvais choix, veuillez entrer une nouvelle lettre\n");
        break;
    }

  }

  close(s_cli);

  return 0;
}

int first_conn_routine(/*...*/){

}

int conn_to_website_routine(char buf[BUF_SIZE]){

  char site[50];
  memset(site, 0, 50);
  memset(buf, 0, BUF_SIZE);

  /*demande utilisateur*/
  printf("Veuillez choisir le site auquel se connecter : \t");
  scanf("%s", site);

  /*Envoie 100;site;ID_CLIENT*/
  strcpy(buf, "100");
  strcat(buf, ";");
  strcat(buf, site);
  strcat(buf, ";");
  char tmp_id[20];
  sprintf(tmp_id, "%d", ID_CLIENT);
  strcat(buf, tmp_id);
  if(DEBUG)
    printf("\tMessage envoyé : %s\n", buf);
  write(s_cli, buf, strlen(buf));

  /*Reception du code de bonne existance du compte pour cet utilisateur*/
  memset(buf, 0, BUF_SIZE);
  read(buf, 0, BUF_SIZE);
  if(DEBUG)
    printf("Le compte %s existe-t-il pour l'utilisateur %d : %s\n", site, ID_CLIENT, buf);


  if(strcmp(buf, "True") == 0){
    //on envoie la photo
    printf("coucou\n");
  }
  else{
    printf("Vous ne posséder pas de compte %s connecter avec FaceKey :(\n", site);
  }


}

int weight_update_routine(/*...*/){

}

int photo_transfer_routine(/*...*/){

}
