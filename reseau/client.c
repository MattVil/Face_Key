#include "util.h"

int first_conn_routine(int s_cli, char buf[BUF_SIZE]);
int conn_to_website_routine(int s_cli, char buf[BUF_SIZE]);


char* IP_SERV = "127.0.0.1";
int PORT_SERV = 5000;
int ID_CLIENT = 1;
char login[100];
char pssw[100];

int main(int argc, char const *argv[]) {

	/*Socket var*/
	char buf[BUF_SIZE];
	memset (buf, 0, BUF_SIZE);
	int s_cli;
	struct sockaddr_in serv_addr;

	/*Timeout var*/
	fd_set readfds;
	struct timeval timeout;

	config("configClient", PORT_SERV, IP_SERV);

	s_cli = socket(PF_INET, SOCK_STREAM, 0);

	/*Config serveur*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP_SERV);
	serv_addr.sin_port = htons(PORT_SERV);
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

	char choise;
	int flag;

	int flag_co = 1, choice, connect_flag;
	int splited_req_size;
	char **splited_req;

  int code;
  char data[BUF_SIZE];

	printf("------------------------------------------\n");
	printf("|                 Client                 |\n");
	printf("------------------------------------------\n");

	while (flag_co){
		s_cli = socket(PF_INET, SOCK_STREAM, 0);
		connect_flag = connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
		if (connect_flag == -1){
			connect_err();
			printf("An error occurred: you may need to reconfigure the conf file\n");
			return 1;
		}
		printf("\nVeuillez vous connecter à votre compte ou creer un compte\n");
		printf("1 - Connexion\n");
		printf("2 - Creer un compte\n");
		printf("3 - Quitter\n");
		printf("------------------------------------------\n");
		printf("Votre choix: ");
		scanf("%d", &choice);
		if (choice != 1 && choice != 2 && choice != 3){
			printf("Ce choix n'est pas disponible\n");
		}
		else if (choice == 3){
			printf("Arrêt...\n");
			exit(0);
		}
		else if (choice){
			printf("Veuillez entrer vos identifiants de connexion:\n");
			printf("\tLogin: ");
			scanf("%s", login);
			printf("\tPassword: ");
			scanf("%s", pssw);
			memset(buf, 0, BUF_SIZE);
			sprintf(buf, "%d;%s,%s", 103, login, pssw);
			printf("%s\n", buf);
			if(DEBUG)
				printf("\t### Message envoyé : %s\n", buf);
			if(ONLINE){
				int o = write(s_cli, buf, strlen(buf));
			}

			memset(buf, 0, BUF_SIZE);
			if(ONLINE){
				timeout_config(s_cli, &readfds, &timeout);
				int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
				if (!select_tt){
					if (DEBUG)
						printf("\t### CONNEXION Timeout\n");
					break;
				}
				read(s_cli, buf, BUF_SIZE);
			}
			else
				strcpy(buf, "000;0K");//exemple

			if(DEBUG)
				printf("\t### Message recu : %s\n", buf);

      if (split_message(&code, data, buf, s_cli))
        break;

			if(code != OK){
				if(DEBUG)
					printf("\t### Permission de connexion au serveur refusé.\n");
			}
			else{
				flag_co = 0;
			}
			close(s_cli);
		}
		else if (choice == 2){
			connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
			flag = conn_to_website_routine(s_cli, buf);
			if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction conn_to_website_routine\n");}
			printf("Votre compte a été créé ! Utilisez le pour vous connecter\n");
			close(s_cli);
		}
	}

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

    fflush(stdin);
		printf("\nVotre choix (p/c/u/t/q): ");

		scanf("%c", &choise);

		//system ("clear");

		switch(choise){
			case 'p': //premiere connexion,creation du compte FK
				/*connexion*/
				if(ONLINE)
					s_cli = socket(PF_INET, SOCK_STREAM, 0);
					connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
					flag = first_conn_routine(s_cli, buf);
				if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction first_conn_routine\n");}
				close(s_cli);
				break;
			case 'c': //connexion à un site
				/*connexion*/
				if(ONLINE)
					s_cli = socket(PF_INET, SOCK_STREAM, 0);
					connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
					flag = conn_to_website_routine(s_cli, buf);
				if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction conn_to_website_routine\n");}
				close(s_cli);
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

int first_conn_routine(int s_cli, char *buf){

	char mail[50], pseudo[50], mdp[50], mdp_confirmation[50], gender[10], name[50], first_name[50], lang[10];
	char **splited_req;
	int splited_req_size;
	int login_ok = 0, mdp_ok = 0;
	memset(mail, 0, 50);
	memset(pseudo, 0, 50);
	memset(buf, 0, BUF_SIZE);

	/*Timeout var*/
	fd_set readfds;
	struct timeval timeout;

	/*Demande de creation*/
	strcpy(buf, "002;creation");
	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);
	if(ONLINE)
		write(s_cli, buf, strlen(buf));

	memset(buf, 0, BUF_SIZE);
	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		read(s_cli, buf, BUF_SIZE);
	}
	else
		strcpy(buf, "000;0K");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);
	splited_req = str_split(buf, ';', &splited_req_size);
	if(atoi(splited_req[0]) != OK){
		if(DEBUG)
			printf("\t### Permission de connexion au serveur refusé.\n");
		return 0;
	}

	while(!login_ok){

		/*Demande utilisateur*/
		printf("Votre adresse mail ('quit' pour quitter) : ");
		scanf("%s", mail);
		if(strcmp(mail, "quit") == 0)
			return 2;
		printf("Votre pseudo : ");
		scanf("%s", pseudo);

		/*envoi*/
		memset(buf, 0, BUF_SIZE);
		strcpy(buf, "110;");
		strcat(buf, mail);
		strcat(buf, ",");
		strcat(buf, pseudo);

		if(ONLINE)
			write(s_cli, buf, strlen(buf));
		if(DEBUG)
			printf("\t### Message envoyé : %s\n", buf);

		/*reponse*/
		memset(buf, 0, BUF_SIZE);
		if(ONLINE){
			timeout_config(s_cli, &readfds, &timeout);
			int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
			if (!select_tt){
				if (DEBUG)
					printf("\t### CONNEXION Timeout\n");
				break;
			}
			read(s_cli, buf, BUF_SIZE);
		}
		else
			strcpy(buf, "000");//exemples

		if(DEBUG)
			printf("\t### Message recu : %s\n", buf);

		splited_req = str_split(buf, ';', &splited_req_size);
		switch(atoi(splited_req[0])){
			case OK :
				login_ok = 1;
				break;
			case ERR_MAIL :
				printf("Ce mail est déjà utilisé\n");
				break;
			case ERR_PSEUDO :
				printf("Ce pseudo est déjà utilisé\n");
				break;
			default :
				if(DEBUG)
					printf("\t### Erreur dans le code recu : %s\n", buf);
				printf("Une erreur est survenue :(\n");
				return 0;
				break;
		}
	}

	/*Demande et verif des MDPs*/
	while(!mdp_ok){
		printf("Votre mot de passe : ");
		scanf("%s", mdp);

		if(strcmp(mdp, "quit") == 0)
			return 2;

		printf("Confirmé votre mot de passe : ");
		scanf("%s", mdp_confirmation);

		if(strcmp(mdp, mdp_confirmation) == 0)
			mdp_ok = 1;
		else
			printf("Erreur dans un des mot de passe\n");

		if(FULL_DEBUG)
			printf("\t### %s = %s ? -> %d\n", mdp, mdp_confirmation, mdp_ok);

		if(strlen(mdp) < 10){
			printf("Votre mot de passe n'est pas assez complexe, 10 caractère minimum\n");
			mdp_ok = 0;
		}
	}

	memset(buf, 0, BUF_SIZE);
	strcpy(buf, "111;");
	strcat(buf, mdp);
	if(ONLINE)
		write(s_cli, buf, BUF_SIZE);

	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);

	memset(buf, 0, BUF_SIZE);
	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		read(s_cli, buf, BUF_SIZE);
	}
	else
		strcpy(buf, "000");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);

	splited_req = str_split(buf, ';', &splited_req_size);
	if(atoi(splited_req[0]) != OK){
		printf("Une erreur est survenue :(\n");
		return 0;
	}

	do{
		printf("Sexe (homme/femme) : ");
		memset(gender, 0, 10);
		scanf("%s", gender);
	}while(strcmp(gender, "homme") != 0 && strcmp(gender, "femme") != 0);
	if(strcmp(gender, "homme") == 0){
		memset(gender, 0, 10);
		strcpy(gender, "1");
	}
	else{
		memset(gender, 0, 10);
		strcpy(gender, "0");
	}

	printf("Votre nom : ");
	memset(name, 0, 50);
	scanf("%s", name);
	printf("Votre prenom : ");
	memset(first_name, 0, 50);
	scanf("%s", first_name);
	do{
		printf("Choisissez une langue :\n");
		printf("\tfr - Français\n");
		printf("\ten - Anglais\n");
		printf("\tde - Allemand\n");
		printf("Votre choix (fr/en/de) : ");
		memset(lang, 0, 10);
		scanf("%s", lang);
	}while(strcmp(lang, "en") != 0 && strcmp(lang, "fr") != 0 && strcmp(lang, "de") != 0);

	memset(buf, 0, BUF_SIZE);
	strcpy(buf, "112;");
	strcat(buf, gender);
	strcat(buf, ",");
	strcat(buf, name);
	strcat(buf, ",");
	strcat(buf, first_name);
	strcat(buf, ",");
	strcat(buf, lang);

	if(ONLINE)
		write(s_cli, buf, strlen(buf));

	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);

	memset(buf, 0, BUF_SIZE);
	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		read(s_cli, buf, BUF_SIZE);
	}
	else
		strcpy(buf, "000;34");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);

	splited_req = str_split(buf, ';', &splited_req_size);
	if(atoi(splited_req[0]) == OK){
		ID_CLIENT = atoi(splited_req[1]);
		printf("Bravo vous avez créé votre compte FaceKey !\n");
		if(FULL_DEBUG)
			printf("\t### New ID_CLIENT : %d\n", ID_CLIENT);
	}
	else{
		printf("Une erreur est survenue :(\n");
		if(DEBUG)
			printf("\t### Erreur dans le code recu : %s\n", buf);
		return 0;
	}

	printf("Pour faire fonctionner notre système de reconnaissance faciale nous avons besoin d'apprendre à vous reconnaitre :)\n");
	char choix[10];
	do{
		printf("Voulez vous commencer l'apprentissage maintenant ? (oui/non) : ");
		memset(choix, 0, 10);
		scanf("%s", choix);
	}while(strcmp(choix, "oui")!=0 && strcmp(choix, "non")!=0);

	if(strcmp(choix, "oui") == 0){
		printf("Très bien, commençons !\n");
	}
	else	{
		memset(buf, 0, BUF_SIZE);
		strcpy(buf, "115");
		if(ONLINE){
			write(s_cli, buf, strlen(buf));
		}
		if(DEBUG)
			printf("\t### Message envoyé : %s\n", buf);

		printf("Très bien, remettons ça à plus tard ! :)\n");
		printf("Vous pouvez tout de meme utiliser l'application de manière classique avec votre master mot de passe\n");
		return 1;
	}

	//ouverture du flux video + transmission des images
	printf("ouverture du flux video + transmission des images ...\n");

	return 1;

}

int conn_to_website_routine(int s_cli, char *buf){

	//char login[100], pssw[100];

	int buf_len = sizeof(buf);
	int i;
	char site[50], password[50];
	int splited_req_size, splited_mdp_size, splited_data_size;
	char **splited_req, **splited_mdp, **splited_data;
	memset(site, 0, 50);
	memset(buf, 0, buf_len);

	/*Timeout var*/
	fd_set readfds;
	struct timeval timeout;

	/*Demande de connexion*/
	strcpy(buf, "001;connexion");
	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);
	if(ONLINE)
		write(s_cli, buf, strlen(buf));

	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		memset(buf, 0, buf_len);
		read(s_cli, buf, sizeof(buf));
	}
	else
		strcpy(buf, "000;0K");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);
	splited_req = str_split(buf, ';', &splited_req_size);
	if(atoi(splited_req[0]) != OK){
		if(DEBUG)
			printf("\t### Permission de connexion au serveur refusé.\n");
		return 0;
	}

	/*authentification*/
	/*printf("Veuillez entrer vos identifiants de connexion:\n");
	printf("\tLogin: ");
	scanf("%s", login);
	printf("\tPassword: ");
	scanf("%s", pssw);*/
	memset(buf, 0, BUF_SIZE);
	sprintf(buf, "%d;%s,%s", 103, login, pssw);
	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);
	if(ONLINE)
		write(s_cli, buf, strlen(buf));

	memset(buf, 0, BUF_SIZE);
	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		read(s_cli, buf, BUF_SIZE);
	}
	else
		strcpy(buf, "000;0K");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);
	splited_req = str_split(buf, ';', &splited_req_size);

	if(atoi(splited_req[0]) != OK){
		if(DEBUG)
			printf("\t### Permission de connexion au serveur refusé.\n");
		return 0;
	}

	/*demande utilisateur*/
	printf("Veuillez choisir le site auquel se connecter : \t");
	scanf("%s", site);

	printf("Vérification de l'identité ... prise de la photo ... identification ... OK !\n");

	/*Envoie 100;site;ID_CLIENT*/
	memset(buf, 0, BUF_SIZE);
	strcpy(buf, "100");
	strcat(buf, ";");
	strcat(buf, site);

	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);
	if(ONLINE)
		write(s_cli, buf, strlen(buf));

	/*Reception de la liste des comptes pour ce sites*/
	memset(buf, 0, BUF_SIZE);
	if(ONLINE){
		timeout_config(s_cli, &readfds, &timeout);
		int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
		if (!select_tt){
			if (DEBUG)
				printf("\t### CONNEXION Timeout\n");
			return 0;
		}
		read(s_cli, buf, BUF_SIZE);
	}
	else
		strcpy(buf, "200;mattvil@gmail.com,jean@ucp.fr,jack@mit.com"); //exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);
	splited_req = str_split(buf, ';', &splited_req_size);
	splited_data = str_split(splited_req[1], ',', &splited_data_size);

	switch(atoi(splited_req[0])){
		/*Site il y a une lisye de site dispo*/
		case IDS_SD :
			printf("Voici les comptes %s auquel vous avez accès : \n", site);
			for(i=0; i<splited_data_size; i++)
				printf("\t%d - %s\n", i+1, splited_data[i]);
			printf("A quel compte voulez vous vous connecter ? (choisir un numero) : ");
			int num_choice;
			scanf("%d", &num_choice);

			if(num_choice<0 || num_choice>splited_data_size){
				if(DEBUG)
					printf("\t### Erreur dans la selection du compte\n");
				return 0;
			}

			/*construction de la chaine d'envoi*/
			memset(buf, 0, BUF_SIZE);
			strcpy(buf, "101;");
			strcat(buf, splited_data[num_choice-1]);
			if(DEBUG)
				printf("\t### Message envoyé : %s\n", buf);
			if(ONLINE)
				write(s_cli, buf, strlen(buf));

			memset(buf, 0, BUF_SIZE);
			if(ONLINE){
				timeout_config(s_cli, &readfds, &timeout);
				int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
				if (!select_tt){
					if (DEBUG)
						printf("\t### CONNEXION Timeout\n");
					return 0;
				}
				read(s_cli, buf, BUF_SIZE);
			}
			else
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
	return 0;
}

int photo_transfer_routine(/*...*/){
	return 0;
}
