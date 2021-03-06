/**
* \file client.c
* \brief Client of FaceKey
* \author  Matthieu Vilain, Quentin Gerard, Louis L'Haridon 
* \version 2.0
* \date 20/03/2018
*/
#include "util.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"

int first_conn_routine(int s_cli, char buf[BUF_SIZE]);
int conn_to_website_routine(int s_cli, char buf[BUF_SIZE]);
int photo_transfer_routine(int s_cli, char buf[BUF_SIZE]);
int weight_update_routine(int s_cli, char buf[BUF_SIZE]);
void recognition(char buff[100], int user_class);
void neuralNetwork(IplImage* frame);


char IP_SERV[20] = "127.0.0.1";
int PORT_SERV = 5000;
int ID_CLIENT = 1;
double version = 0;
char login[100];
char pssw[100];

RSA *pubkey, *privkey;
int encrypt_enable = 0;
unsigned char *encrypt_buf;
int hash = 1;

//Sem & SharedMem Init
key_t semkey;
pid_t pidFork;
int semid, shmkey, *x, tube[2];
char tubeBuffer[100];

/**
* \fn int main(int argc, char const *argv[])
* \brief  Main function
* \param  argc An integer argument count of the command line arguments
* \param  argv An argument vector of the command line arguments
* \return an integer 0 upon exit success
*/
int main(int argc, char const *argv[]) {

	printf("Init SEM, SHM and PIPE\n");

	if ((semkey = ftok("sem.key", 1)) == -1){
		perror("Semkey creation failed");
		exit(-1);
	}
	if ((shmkey = ftok("shm.key", 1)) == -1){
		perror("Semkey creation failed");
		exit(-1);
	}

	if ((semid = semalloc(semkey, 0)) == -1){
		perror("Semaphore creation failed");
		exit(-1);
	}

	if (pipe(tube)) {
	    perror("erreur creation de tube");
	    return 1;
	}
	/**
	* 0 = Lecture
	* 1 = Ecriture
	**/
	printf("SHM, SEM, TUBE OK\n");

	x = shmalloc(shmkey, sizeof(int));
	*x = 0;

	printf("Lancement du programme de reconnaissance\n");
	pidFork = fork();

	//Client
	if(pidFork == 0){

		int quitFlag = 1;

		//Hash var
		char mdString[33];
		unsigned char digest[MD5_DIGEST_LENGTH];

		/*Socket var*/
		char buf[BUF_SIZE];
		memset (buf, 0, BUF_SIZE);
		int s_cli;
		struct sockaddr_in serv_addr;

		/*Timeout var*/
		fd_set readfds;
		struct timeval timeout;

		config("configClient.txt", &PORT_SERV, IP_SERV);

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

			if (encrypt_enable){
				//RSA_FLAG
				send_file2("client_x/keys/publickey.pem", "publickey.pem", s_cli);
				recv_data(s_cli, buf);
				receive_file2(s_cli, "client_x/keys");
				pubkey = loadKey("client_x/keys/server_pubkey.pem", PUBKEY);
				if (pubkey == NULL){
					encrypt_enable = 0;
					printf("No Encryption\n");
				}
				else{
					if (DEBUG)
						printf("Key Loaded\n");
					encrypt_buf = malloc(RSA_size(pubkey));
				}
			}

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
	    else if (choice == 2){
	      /*connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
	      //RSA_FLAG
			send_file2("client_x/keys/publickey.pem", "publickey.pem", s_cli);
			recv_data(s_cli, buf);
			receive_file2(s_cli, "client_x/keys");*/

	      flag = first_conn_routine(s_cli, buf);
	      if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction conn_to_website_routine\n");}
	      printf("Votre compte a été créé ! Utilisez le pour vous connecter\n");
	      close(s_cli);
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

				//ENCRYPT_RSA_FLAG
				if (encrypt_enable){
					int len;
					printf("Message: %s\n", buf);
					//pubkey = loadKey("client_x/keys/publickey.pem", PUBKEY);
					len = RSA_public_encrypt(strlen(buf)+1, (unsigned char*)buf, (unsigned char*)encrypt_buf, pubkey, RSA_PKCS1_OAEP_PADDING);
					printf("Encrypted message (%d): %s\n", len, encrypt_buf);
					printf("Sizeof: %lu\tstrlen: %lu\n", sizeof(encrypt_buf), strlen(encrypt_buf));
					/*privkey = loadKey("client_x/keys/privatekey.pem", PRIVKEY);
					len = RSA_private_decrypt(256, (unsigned char*)encrypt_buf, (unsigned char*)buf, privkey, RSA_PKCS1_OAEP_PADDING);
					printf("Decrypted message: %s\n", buf);*/

					unsigned char digest[MD5_DIGEST_LENGTH];
					MD5((unsigned char*)&encrypt_buf, strlen(encrypt_buf), (unsigned char*)&digest);
				    char mdString[33];
				    for(int i = 0; i < 16; i++)
				         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
				     if (DEBUG)
				     	printf("md5: %s\n", mdString);

					memset(buf, 0, BUF_SIZE);
					sprintf(buf, "%s", encrypt_buf);

					for (int i = 0; i<strlen(encrypt_buf)+20; i++)
						printf("%d(%c) ", encrypt_buf[i], encrypt_buf[i]);
					printf("\n");

				}

				/*if (hash){
					for (int i = 0; i<strlen(pssw); i++)
						printf("%d(%c) ", pssw[i], pssw[i]);
					printf("\n");
					printf("Hint: %d\n", strcmp(pssw, (const char*)"azertyuiop"));
					MD5((unsigned char*)&pssw, strlen(pssw), (unsigned char*)&digest);
				    for(int i = 0; i < 16; i++)
				         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
				     if (DEBUG)
				     	printf("md5 of '%s' : %s\n", pssw, mdString);
				}*/

				if(DEBUG)
					printf("\t### Message envoyé : %s\n", buf);
				if(ONLINE){
					if (encrypt_enable){
						int o = write(s_cli, encrypt_buf, strlen(buf));
					}
					else{
						int o = write(s_cli, buf, strlen(buf));
					}
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
	        exit(1);

				if(code != OK){
					if(DEBUG)
						printf("\t### Permission de connexion au serveur refusé.\n");
				}
				else{
					flag_co = 0;
				}
				close(s_cli);
			}
		}

		while(quitFlag){
			choise = getchar();

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
						if (encrypt_enable){
							//RSA_FLAG
							send_file2("client_x/keys/publickey.pem", "publickey.pem", s_cli);
							recv_data(s_cli, buf);
							receive_file2(s_cli, "client_x/keys");
						}

						flag = first_conn_routine(s_cli, buf);
					if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction first_conn_routine\n");}
					close(s_cli);
					break;
				case 'c': //connexion à un site
					/*connexion*/
					if(ONLINE)
						s_cli = socket(PF_INET, SOCK_STREAM, 0);
						connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
						if (encrypt_enable){
							//RSA_FLAG
							send_file2("client_x/keys/publickey.pem", "publickey.pem", s_cli);
							recv_data(s_cli, buf);
							receive_file2(s_cli, "client_x/keys");
						}

						flag = conn_to_website_routine(s_cli, buf);
					if(DEBUG && flag == 0){printf("\t### Erreur dans la fonction conn_to_website_routine\n");}
			        if (!flag){
			          close(s_cli);
			          printf("Server is down\n");
			          exit(1);
			        }
					close(s_cli);
					break;
				case 'u': //update quotidienne des poids du réseau
					if(ONLINE)
						s_cli = socket(PF_INET, SOCK_STREAM, 0);
						connect(s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr);
						if (encrypt_enable){
							//RSA_FLAG
							send_file2("client_x/keys/publickey.pem", "publickey.pem", s_cli);
							recv_data(s_cli, buf);
							receive_file2(s_cli, "client_x/keys");
						}

						flag = weight_update_routine(s_cli, buf);
					if(DEBUG && flag == 1){printf("\t### Erreur dans la fonction conn_to_website_routine\n");}
					if (flag){
						close(s_cli);
						printf("Server is down\n");
						exit(1);
					}
					close(s_cli);
					break;
				case 't': //transmission quotidienne des photos
					break;
				case 'q':
					quitFlag = 0;
					break;
				default :
					printf("Mauvais choix, veuillez entrer une nouvelle lettre\n");
					break;
			}

		}

		close(s_cli);
	}
	//Code de reconnaissance (père)
	else{
		char buffRecognition[100];
		printf("Loading Model...\n");
		sleep(20);
		printf("Model Loaded !\n");
		while(1){
			/*Traitement*/
			printf("Wait for Recognition !\n");
			recognition(buffRecognition, ID_CLIENT);
			if (waitpid(pidFork, 0, WNOHANG) != pidFork){
				P(semid);
				sprintf(tubeBuffer, "Autorisation");
				write(tube[1], tubeBuffer, sizeof(tubeBuffer));
				sprintf(tubeBuffer, "0.98");
				write(tube[1], tubeBuffer, sizeof(tubeBuffer));
				sprintf(tubeBuffer, "0.80 0.99 0.99 0.50 0.99");
				write(tube[1], tubeBuffer, sizeof(tubeBuffer));
			}
			else{
				break;
			}
		}

		printf("Ending Recognition process ...\n");

		printf("Recognition process ended !\nGoodbye !\n");
	}

	//Free Semid & SharedMem
	if (shmfree(shmkey) == -1){
		perror("Shared Memory destruction failed");
		exit(-1);
	}
	if (semfree(semid) == -1){
		perror("Semaphore destruction failed");
		exit(-1);
	}
	close(tube[0]);
	close(tube[1]);

	return 0;
}

/**
* \fn int first_conn_routine(int s_cli, char *buf)
* \brief function of the first connexion to server
* \param s_cli ref to server
* \param *buf buffer to write and read
*/
int first_conn_routine(int s_cli, char *buf){

	char mail[50], pseudo[50], mdp[100], mdp_confirmation[100], gender[10], name[50], first_name[50], lang[10];
	char **splited_req;
	int splited_req_size;
	int login_ok = 0, mdp_ok = 0;
  int code;
  char data[BUF_SIZE];
	memset(mail, 0, 50);
	memset(pseudo, 0, 50);
	memset(buf, 0, BUF_SIZE);

	/*Timeout var*/
	fd_set readfds;
	struct timeval timeout;

	/*Demande de creation*/
	strcpy(buf, "002;creation");
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
		strcpy(buf, "000;0K");//exemple

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);
	if (split_message(&code, data, buf, s_cli))
    	return 1;
	if(code != OK){
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

		if (split_message(&code, data, buf, s_cli))
    		return 1;
		switch(code){
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

	if (split_message(&code, data, buf, s_cli))
    	return 1;
	if(code != OK){
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

	if (split_message(&code, data, buf, s_cli))
    	return 1;
	if(code == OK){
		//ID_CLIENT = atoi(splited_req[1]);
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
		strcpy(buf, "115;No_photo");
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
	photo_transfer_routine(s_cli, buf);

	memset(buf, 0, BUF_SIZE);
	timeout_config(s_cli, &readfds, &timeout);
	int select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
	if (!select_tt){
		if (DEBUG)
			printf("\t### CONNEXION Timeout\n");
		return 0;
	}
	read(s_cli, buf, BUF_SIZE);

	if(DEBUG)
		printf("\t### Message recu : %s\n", buf);

	if (split_message(&code, data, buf, s_cli))
    	return 1;
	if(code == OK){
		if(data != NULL){
			version = atof(data);
			if(DEBUG)
				printf("\t### Nouvelle version actuelle du reseau de neurones : %lf \n", atof(data));
		}
	}
	else{
		if(DEBUG)
			printf("\t### Erreur - code attendu : %d recu : %d\n", OK, atoi(splited_req[0]));
		return 0;
	}

	memset(buf, 0, BUF_SIZE);
	strcpy(buf, "000;Version_recu");
	write(s_cli, buf, strlen(buf));
	if(DEBUG)
		printf("\t### Message envoyé : %s\n", buf);

	receive_file2(s_cli, "client_x/");


	return 1;
}

/**
* \fn int conn_to_website_routine(int s_cli, char *buf)
* \brief function of the connect to a website
* \param s_cli ref to server
* \param *buf buffer to write and read
*/
int conn_to_website_routine(int s_cli, char *buf){

	//char login[100], pssw[100];

	int buf_len = sizeof(buf);
	int i;
	char site[50], password[50], mail[100];
	int splited_req_size, splited_mdp_size, splited_data_size;
	char **splited_req, **splited_mdp, **splited_data;
	memset(site, 0, 50);
	memset(buf, 0, buf_len);
	int code;
	char data[BUF_SIZE];

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
	if (split_message(&code, data, buf, s_cli))
    	return 1;
	if(code != OK){
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

	if (split_message(&code, data, buf, s_cli))
    	return 1;

	if(code != OK){
		if(DEBUG)
			printf("\t### Permission de connexion au serveur refusé.\n");
		return 0;
	}

	/*demande utilisateur*/
	printf("Veuillez choisir le site auquel se connecter : \t");
	scanf("%s", site);

	//printf("Vérification de l'identité ... prise de la photo ... identification ... OK !\n");

	printf("Info reconnaissance:\n");
	V(semid);
	read(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("%s\n", tubeBuffer);
	read(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("%s\n", tubeBuffer);
	read(tube[0], tubeBuffer, sizeof(tubeBuffer));
	printf("%s\n", tubeBuffer);

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
	if (split_message(&code, data, buf, s_cli))
    	return 1;
	splited_data = str_split(data, ',', &splited_data_size);

	switch(code){
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
				printf("Le mot de passe du compte %s pour le site %s est %s\n", splited_data[num_choice - 1], site, password);
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

/**
* \fn int weight_update_routine(int s_cli, char buf[BUF_SIZE])
* \brief function to update weight
* \param s_cli ref to server
* \param buf[BUF_SIZE] buffer to write and read
*/
int weight_update_routine(int s_cli, char buf[BUF_SIZE]){
	int select_tt, read_tt;
	fd_set readfds;
	struct timeval timeout;
	int code;
	char data[BUF_SIZE];
	send_data(s_cli, UPDATE, "update", buf, BUF_SIZE);
	timeout_config(s_cli, &readfds, &timeout);
	select_tt = select(s_cli+1, &readfds, NULL, NULL, &timeout);
	if (!select_tt){
		if (DEBUG)
			printf("UPDATE: Neuron timeout\n");
		send_data(s_cli, ERR_TIMEOUT, "Timeout Reached", buf, BUF_SIZE);
		return 1;
	}
	read_tt = recv_data(s_cli, buf);
	if (read_tt == -1){
		if (DEBUG)
			printf("UPDATE: Read failed\n");
		send_data(s_cli, 1000, "Internal error", buf, BUF_SIZE);
		return 1;
	}
	printf("MESSAGE RECEIVED: %s\n", buf);
	if (split_message(&code, data, buf, s_cli))
		return 1;
	if (code != OK){
		printf("Code unrecognized at this point (%d)\n", code);
		send_data(s_cli, UKNWREQ, "Code unrecognized at this point", buf, BUF_SIZE);
		return 1;
	}
	send_data(s_cli, UP, "OK", buf, BUF_SIZE);

	receive_file2(s_cli, "client_x");

	return 0;
}

/**
* \fn int photo_transfer_routine(int s_cli, char buf[BUF_SIZE])
* \brief function to transfer photo to server
* \param s_cli ref to server
* \param buf[BUF_SIZE] buffer to write and read
*/
int photo_transfer_routine(int s_cli, char buf[BUF_SIZE]){

	int nb_photo = 29;
	//int nb_photo = system("./take_picture");
	int flag;
	int i;
	char data[BUF_SIZE];
	int code;
	char path[50];
	char filename[50];
	FILE* file;
	for(i=1; i<=nb_photo; i++){
		sprintf(path, "./client_x/image/%d.jpg", i);
		sprintf(filename, "%d.jpg", i);
		file = fopen(path, "r");
		if(file != NULL){
			fclose(file);

			memset(buf, 0, BUF_SIZE);
			if(i == nb_photo)
				strcpy(buf, "114;Last_picture_in_coming");
			else
				strcpy(buf, "113;Picture_in_coming");

			write(s_cli, buf, strlen(buf));
			if(DEBUG)
				printf("\t### Message envoyé : %s\n", buf);


			printf("Transfer du fichier : %s ... ", path);
			flag = send_file2(path, filename, s_cli);
			//remove(path);
			if(flag == 0)
				printf("OK\n");
			else{
				printf("Erreur\n");
			}

			memset(buf, 0, BUF_FILE);
			read(s_cli, buf, BUF_SIZE);
			if(DEBUG)
				printf("\t### Message recu : %s\n", buf);

			if(split_message(&code, data, buf, s_cli))
		    	return 1;
			if(code != OK)
				return 1;

		}
		else{
			if (DEBUG)
				printf("File: %s is null\n", filename);
		}

	}

	return 0;
}

/**
* \fn void recognition(char buff[100], int user_class)
* \brief function to recognize
* \param buff[100] buffer to write and read
* \param user_class user class to recognize
*/
void recognition(char buff[100], int user_class){
	printf("Ouai ca reconnais avec Opencv mdr\n");

	//open the video stream
	CvCapture* cap=cvCaptureFromCAM(0);
	IplImage* frame;

	//ignore the few first image bc they can be co
	int i=0;
	for(i=0; i<5; i++){
		frame = cvQueryFrame(cap);
	}

	//pass somme image of the video stream on the neural network for recognition
	for(i=0; i<10; i++){
		frame = cvQueryFrame(cap);
		neuralNetwork(frame);
	}
	//fake result of the recognition

}

/**
* \fn void neuralNetwork(IplImage* frame)
* \brief NeuralNetwork function
* \param frame image to put into neural network

	This function is just a simulation for the OS course.
	The real neural network is implemented in python and can be found at :
	/face_recognition/client_detect.py
*/
void neuralNetwork(IplImage* frame){
	/*
	This function is just a simulation for the OS course.
	The real neural network is implemented in python and can be found at :
	/face_recognition/client_detect.py
	*/
}
