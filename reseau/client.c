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

  /*connexion*/
  //connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
  if(WORDY_DEBUG)
    printf("connexion au serveur OK\n");

  char choise;

  printf("-----------------------------------------\n");
  printf("|\t\t\t\t\t|\n");
  printf("|\tVous êtes le client %d\t\t|\n", ID_CLIENT);
  printf("|\t\t\t\t\t|\n");
  printf("-----------------------------------------\n");

  printf("\n\nChoix :\n");
  printf("\tp - Simuler une premiere connexion\n");
  printf("\tc - Vous connecter à un site\n");
  printf("\tu - Mettre à jour la version de votre réseau de neurones\n");
  printf("\tt - Transmettre les photos de vous pour amélioration du réseau\n");
  printf("\tq - Quitter\n");

  while(1){

    printf("\nVotre choix (p/c/u/t/q): ");

    scanf("%c", &choise);

    //system ("clear");

    switch(choise){
      case 'p': //premiere connexion,creation du compte FK
        break;
      case 'c': //connexion à un site
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

  int i;
  char site[50], password[50];
  int splited_req_size, splited_mdp_size;
  char **splited_req, **splited_mdp;
  memset(site, 0, 50);
  memset(buf, 0, BUF_SIZE);

  /*demande utilisateur*/
  printf("Veuillez choisir le site auquel se connecter : \t");
  scanf("%s", site);

  printf("Vérification de l'identité ... prise de la photo ... identification ... OK !\n");

  /*Envoie 100;site;ID_CLIENT*/
  strcpy(buf, "100");
  strcat(buf, ";");
  strcat(buf, site);
  strcat(buf, ";");
  char tmp_id[20];
  sprintf(tmp_id, "%d", ID_CLIENT);
  strcat(buf, tmp_id);
  if(DEBUG)
    printf("\t### Message envoyé : %s\n", buf);
  //write(s_cli, buf, strlen(buf));

  /*Reception de la liste des comptes pour ce sites*/
  memset(buf, 0, BUF_SIZE);
  //read(buf, 0, BUF_SIZE);
  strcpy(buf, "200;mattvil@gmail.com;jean@ucp.fr;jack@mit.com"); //exemple
  if(DEBUG)
    printf("\t### Message recu : %s\n", buf);
  splited_req = str_split(buf, ';', &splited_req_size);

  switch(atoi(splited_req[0])){
    /*Site il y a une lisye de site dispo*/
    case IDS_SD :
      printf("Voici les comptes %s auquel vous avez accès : \n", site);
      for(i=1; i<splited_req_size; i++)
        printf("\t%d - %s\n", i, splited_req[i]);
      printf("A quel compte voulez vous vous connecter ? (choisir un numero) : \t");
      int num_choice;
      scanf("%d", &num_choice);

      if(num_choice<0 || num_choice>splited_req_size)
        return 0;

      /*construction de la chaine d'envoi*/
      memset(buf, 0, BUF_SIZE);
      strcpy(buf, "101");
      strcat(buf, ";");
      strcat(buf, splited_req[num_choice]);
      if(DEBUG)
        printf("\t### Message envoyé : %s\n", buf);
      //write(s_cli, buf, strlen(buf));

      memset(buf, 0, BUF_SIZE);
      //read(buf, 0, BUF_SIZE);
      strcpy(buf, "201;c38e<5fe{5e#ec5^}{ec2#ec5"); //exemple
      if(DEBUG)
        printf("\t### Message recu : %s\n", buf);
      splited_mdp = str_split(buf, ';', &splited_mdp_size);
      if(atoi(splited_mdp[0]) == PSSW_SD){
        memset(password, 0, 50);
        strcpy(password, splited_mdp[1]);
        printf("Le mot de passe du compte %s pour le site %s est %s\n", splited_req[num_choice], site, password);
      }
      else{
        printf("Une erreur est survenue\n");
        if(DEBUG)
          printf("\t### Erreur dans le code reçu %d dans : %s\n", atoi(splited_mdp[0]), buf);
        return 0;
      }

      break;
    /*si il n'y à pas de site dispo pour ce site*/
    case NO_ACC :
      printf("Vous n'avez pas de compte enregistré pour le site %s\n", site);
      break;
    default :
      printf("Une erreur est survenue :(\n");
      if(DEBUG)
        printf("\t### Erreur dans le code reçu %d dans : %s\n", atoi(splited_req[0]), buf);
      return 0;
  }

  return 1;
}

int weight_update_routine(/*...*/){

}

int photo_transfer_routine(/*...*/){

}
