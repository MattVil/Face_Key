#include "util.h"
#include "dbutil.h"

typedef struct node{
	char *mail;
	int account_id;
	struct node *next;
}*AccountList;

void serv_config(struct sockaddr_in* serv_addr, int* s_ecoute);
void database_connect(PGconn* conn);
int connect_to_client(int s_ecoute, struct sockaddr_in* cli_addr);
int authentification(char* login, char* password, PGconn *conn);
void verif_conn(PGconn* conn);
void idrequ(char *domain, int id_user, int s_dial, PGconn *conn, char *buf, AccountList *list);
char* extractPssw(int id, PGconn* conn);
int exist_mail(PGconn *conn, char *mail);
int exist_pseudo(PGconn *conn, char *pseudo);
//Account List Function
AccountList insert(char* mail, int id_account, AccountList list);
AccountList createNode(char* mail, int account_id);
void displayAccountList(AccountList list);
AccountList addToList(PGresult *result, AccountList list);
AccountList freeAccountList(AccountList list);
int extractID(AccountList list, char* mail);

int main(){

	/*Socket var*/
	char buf[BUF_SIZE];
	int s_ecoute, s_dial;
	struct sockaddr_in serv_addr, cli_addr;

	/*Database (postgres) var*/
	PGconn *conn;

	/*Timeout var*/
	fd_set readfds;
	struct timeval timeout;

	/*Telltale var*/
	int read_bytes, select_tt, read_tt;

	/*Util var*/
	char data[BUF_SIZE], temp_buf[BUF_SIZE], **split_data;
	int split_data_size;
	AccountList list = NULL;
	int code, account_id;
	int on = 1, user_id;

	//Creation var
	char *mail, *pseudo, *pssw, *name, *firstname, *lang;
	int gender;
	int login_ok = 0;
	char query[500];
	PGresult *result;
	int version = 20180108;
	char *tempon;
	int cont = 1;
	int id_user;
	char directory[100];

	serv_config(&serv_addr, &s_ecoute);
	//database_connect(conn); //DataBase connection block the timeout

	// timeout_config(s_dial, &readfds, &timeout);
	// select_tt = select(5, &readfds, NULL, NULL, &timeout);
	// if (!select_tt){
	// 	printf("Timeout\n");
	// }
	// else{
	// 	memset(buf, 0, BUF_SIZE);
	// 	read_bytes = read(s_dial, buf, BUF_SIZE);
	// 	if (read_bytes != -1)
	// 		printf("%s\n", buf);
	// }

	while (on){
		s_dial = connect_to_client(s_ecoute, &cli_addr);

		if (fork() == 0){
			char trace[20];
			sprintf(trace, "[%s:%d]", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
			read_tt = recv_data(s_dial, buf);
			if (read_tt != -1){
				printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
				strcpy(temp_buf, buf);
				code = getCode(temp_buf);
				strcpy(temp_buf, buf);
				getData(temp_buf, data);
				switch(code){
					/*case SHUTDOWN:
						if (strcmp(removechar(data,'\n'), SHUTD_PW) == 0){
							printf("%s Shuting down the server\n", trace);
							on = 0;
						}
						else{
							printf("%s %s tried to shutdown the server\n", trace, inet_ntoa(cli_addr.sin_addr));
							send_data(s_dial, GOD, "Don't tease with me", buf, sizeof(buf));
						}
						break;*/
					case AUTH:
						split_data = str_split(data, ',', &split_data_size);
						if (split_data_size != 2){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Too much or not enough data (%s)\n", trace, data);
							send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
							break;
						}
						split_data[1] = removechar(split_data[1], '\n');
						conn = PQconnectdb(CONN_INFO);
						verif_conn(conn);
						printf("%s/%s\n", *split_data, *(split_data+1));
						user_id = authentification(split_data[0], split_data[1], conn);
						PQfinish(conn);
						if (user_id == -1){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Wrong Login\n", trace);
							send_data(s_dial, WRG_LOGIN, "Wrong login", buf, sizeof(buf));
							break;
						}
						else if (user_id == -2){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Wrong Password\n", trace);
							send_data(s_dial, WRG_PSSW, "Wrong Password", buf, sizeof(buf));
							break;
						}
						send_data(s_dial, OK, "You're logged in", buf, sizeof(buf));
						break;


					case CONNEXION:
						send_data(s_dial, OK, "OK", buf, sizeof(buf));

						//AUTHENTIFICATION
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CONNEXION: Auth timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CONNEXION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code != AUTH){
							if (DEBUG)
								printf("%s CONNEXION: (AUTH) Code unrecognized at this point (%d)\n", trace, code);
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
							break;
						}
						split_data = str_split(data, ',', &split_data_size);
						if (split_data_size != 2){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Too much or not enough data (%s)\n", trace, data);
							send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
							break;
						}
						//database_connect(conn);
						split_data[1] = removechar(split_data[1], '\n');
						conn = PQconnectdb(CONN_INFO);
						verif_conn(conn);
						printf("%s/%s\n", *split_data, *(split_data+1));
						int user_id = authentification(split_data[0], split_data[1], conn);
						PQfinish(conn);
						if (user_id == -1){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Wrong Login\n", trace);
							send_data(s_dial, WRG_LOGIN, "Wrong login", buf, sizeof(buf));
							break;
						}
						else if (user_id == -2){
							if (DEBUG)
								printf("%s AUTHENTIFICATION: Wrong Password\n", trace);
							send_data(s_dial, WRG_PSSW, "Wrong Password", buf, sizeof(buf));
							break;
						}
						send_data(s_dial, OK, "You're logged in", buf, sizeof(buf));

						//DOMAIN
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CONNEXION: IDS_REQU timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CONNEXION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code != IDS_REQU){
							if (DEBUG)
								printf("%s CONNEXION: (IDS_REQU) Code unrecognized at this point (%d)\n", trace, code);
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
							break;
						}
						split_data = str_split(data, ',', &split_data_size);
						if (split_data_size != 1){
							if (DEBUG)
								printf("%s IDS_REQU: Too much or not enough data\n", trace);
							send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
							break;
						}
						split_data[0] = removechar(split_data[0], '\n');
						conn = PQconnectdb(CONN_INFO);
						verif_conn(conn);
						idrequ(split_data[0], user_id, s_dial, conn, buf, &list);
						//printf("%d\n", (list == NULL));
						if (DEBUG)
							displayAccountList(list);
						PQfinish(conn);

						//PSSW_RQ
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CONNEXION: PSSW_REQU timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CONNEXION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code != PSSW_REQU){
							if (DEBUG)
								printf("%s CONNEXION: (PSSW_REQU) Code unrecognized at this point (%d)\n", trace, code);
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
							break;
						}
						account_id = extractID(list, removechar(data, '\n'));
						if (account_id == -1){
							if (DEBUG)
								printf("%s CONNEXION: (PSSW_REQU) The Account List is empty\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						else if(account_id == -2){
							if (DEBUG)
								printf("%s CONNEXION: (PSSW_REQU) No ID matching with Login given (%s)\n", trace, data);
							send_data(s_dial, ERR_MATCH_ID, "The Login provided is not an option", buf, sizeof(buf));
							break;
						}
						conn = PQconnectdb(CONN_INFO);
						verif_conn(conn);
						send_data(s_dial, PSSW_SD, extractPssw(account_id, conn), buf, sizeof(buf));
						PQfinish(conn);

						//END
						list = freeAccountList(list);
						break;

					case CREATION:

						send_data(s_dial, OK, "OK", buf, sizeof(buf));

						//REC MAIL+PSEUDO
						while (!login_ok){
							timeout_config(s_dial, &readfds, &timeout);
							select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
							printf("%d - %d\n", select_tt, s_dial);
							if (!select_tt){
								if (DEBUG)
									printf("%s CREATION: Auth timeout\n", trace);
								send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
								break;
							}
							else if (select_tt == -1){
								printf("Erreur\n");
								break;
							}
							read_tt = recv_data(s_dial, buf);
							if (read_tt == -1){
								if (DEBUG)
									printf("%s CREATION: Read failed\n", trace);
								send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
								break;
							}
							printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
							if (split_message(&code, data, buf, s_dial))
								break;
							if (code != LOG_CREA){
								if (DEBUG)
									printf("%s CREATION: (LOG_CREA) Code unrecognized at this point (%d)\n", trace, code);
								send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
								break;
							}
							split_data = str_split(data, ',', &split_data_size);
							if (split_data_size != 2){
								if (DEBUG)
									printf("%s LOG_CREA: Too much or not enough data\n", trace);
								send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
								break;
							}
							conn = PQconnectdb(CONN_INFO);
							verif_conn(conn);
							if (exist_mail(conn, split_data[0])){
								if (DEBUG)
									printf("%s Mail provided (%s) is already used\n", trace, split_data[0]);
								send_data(s_dial, ERR_MAIL, "Mail provided is already used, please choose another one", buf, sizeof(buf));
							}
							else if (exist_pseudo(conn, split_data[1])){
								if (DEBUG)
									printf("%s Pseudo provided (%s) is already used\n", trace, split_data[1]);
								send_data(s_dial, ERR_MAIL, "Pseudo provided is already used, please choose another one", buf, sizeof(buf));
							}
							else{
								mail = split_data[0];
								pseudo = split_data[1];
								send_data(s_dial, OK, "OK", buf, sizeof(buf));
								login_ok = 1;
							}
							PQfinish(conn);
						}

						//MDP
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CREATION: Password timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CREATION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code != PSSW_CREA){
							if (DEBUG)
								printf("%s CREATION: (PSSW_CREA) Code unrecognized at this point (%d)\n", trace, code);
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
							break;
						}
						split_data = str_split(data, ',', &split_data_size);
						if (split_data_size != 1){
							if (DEBUG)
								printf("%s PSSW_CREA: Too much or not enough data\n", trace);
							send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
							break;
						}
						pssw = split_data[0];
						send_data(s_dial, OK, "OK", buf, sizeof(buf));

						//OTHER INFO
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CREATION: Info timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CREATION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code != ID_INFO){
							if (DEBUG)
								printf("%s CREATION: (ID_INFO) Code unrecognized at this point (%d)\n", trace, code);
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
							break;
						}
						split_data = str_split(data, ',', &split_data_size);
						if (split_data_size != 4){
							if (DEBUG)
								printf("%s ID_INFO: Too much or not enough data\n", trace);
							send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
							break;
						}
						gender = atoi(split_data[0]);
						name = split_data[1];
						firstname = split_data[2];
						lang = split_data[3];
						if (DEBUG){
							printf("%s Account will be created with:\n", trace);
							printf("\tMAIL: %s\n\tPSEUDO: %s\n\tPASSWORD: %s\n\tGENDER: %d\n\tNAME: %s\n\tFIRSTNAME: %s\n\tLANG: %s\n", mail, pseudo, pssw, gender, name, firstname, lang);
						}

						if (ONLINE){
							conn = PQconnectdb(CONN_INFO);
							verif_conn(conn);
							sprintf(query, "INSERT INTO Users (name, first_name, gender, pseudo, mail, password, language, creation_date) VALUES ('%s', '%s', '%d', '%s', '%s', '%s', '%s', '1970/01/01');", name, firstname, gender, pseudo, mail, pssw, lang);
							result = PQexec(conn, query);
							PQclear(result);
							PQfinish(conn);
						}

						send_data(s_dial, OK, "Account created", buf, sizeof(buf));

						//PHOTO
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CREATION: Photo timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CREATION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code == NO_PHOTO){
							if (DEBUG)
							printf("%s Client doesn't want to upload photos\n", trace);
						}
						else{
							if (ONLINE){
								sprintf(query, "SELECT id_user FROM Users WHERE mail='%s';", mail);
								PGresult *result;
								conn = PQconnectdb(CONN_INFO);
								verif_conn(conn);
								result = PQexec(conn, query);
								tempon = PQgetvalue(result, 0, 0);
								PQclear(result);
								PQfinish(conn);
							}
							else{
								tempon = "100";
							}
							sprintf(directory, "/face_key/usr/%s", tempon);
							printf("flag\n");
							while (cont){
								if (code == LAST_PHOTO){
									printf("Cas 1\n");
									receive_file(s_dial, directory);
									cont = 0;
								}
								else if(code == PHOTO){
									printf("Cas 2\n");
									receive_file(s_dial, directory);
								}
								else if (code != PHOTO || code != LAST_PHOTO){
									printf("Cas 3\n");
									if (DEBUG)
										printf("%s CREATION: (PHOTO) Code unrecognized at this point (%d)\n", trace, code);
									send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
									break;
								}
								timeout_config(s_dial, &readfds, &timeout);
								select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
								if (!select_tt){
									if (DEBUG)
										printf("%s CREATION: Photo timeout\n", trace);
									send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
									break;
								}
								read_tt = recv_data(s_dial, buf);
								if (read_tt == -1){
									if (DEBUG)
										printf("%s CREATION: Read failed\n", trace);
									send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
									break;
								}
								printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
								if (split_message(&code, data, buf, s_dial))
									break;
							}
						}

						//Version Réseau
						sprintf(query, "%d", version);
						send_data(s_dial, OK, query, buf, sizeof(buf));
						timeout_config(s_dial, &readfds, &timeout);
						select_tt = select(max(s_ecoute, s_dial)+1, &readfds, NULL, NULL, &timeout);
						if (!select_tt){
							if (DEBUG)
								printf("%s CREATION: Neuron timeout\n", trace);
							send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
							break;
						}
						read_tt = recv_data(s_dial, buf);
						if (read_tt == -1){
							if (DEBUG)
								printf("%s CREATION: Read failed\n", trace);
							send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
							break;
						}
						printf("%s MESSAGE RECEIVED: %s\n", trace, buf);
						if (split_message(&code, data, buf, s_dial))
							break;
						if (code == OK)
							send_file("neuron/network.npz", "network.npz", s_dial);
						break;

					case UPDATE:
						break;
					case -1:
						if (DEBUG)
							printf("%s The message is not well formed\n", trace);
						send_data(s_dial, MISSING, "The message is not well formed", buf, sizeof(buf));
						break;
					default:
						printf("%s Code unrecognized at this point (%d)\n", trace, code);
						break;
				}
			}
			else{
				printf("Read failed\n");
			}
			if (list != NULL)
			list = freeAccountList(list);
			if (PQstatus(conn) == CONNECTION_OK)
				PQfinish(conn);

			printf("End of Communication with %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

			return 1;
		}
		close (s_dial);
	}

	if (list != NULL)
		list = freeAccountList(list);
	if (PQstatus(conn) == CONNECTION_OK)
		PQfinish(conn);
	close (s_ecoute);
	return EXIT_SUCCESS;
}

void serv_config(struct sockaddr_in* serv_addr, int* s_ecoute){
	int bind_flag;

	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = INADDR_ANY;
	serv_addr->sin_port = htons(PORT);
	memset(serv_addr->sin_zero, 0, sizeof(serv_addr->sin_zero));

	*s_ecoute = socket(PF_INET, SOCK_STREAM, 0);
	bind_flag = bind(*s_ecoute, (struct sockaddr *) serv_addr, sizeof(*serv_addr));
	if (bind_flag == -1){
		bind_error();
		exit(1);
	}
	listen(*s_ecoute, QUEUE);
	if (DEBUG)
		printf("SERVER STARTED: %s:%d\n", inet_ntoa(serv_addr->sin_addr), ntohs(serv_addr->sin_port));
}

int connect_to_client(int s_ecoute, struct sockaddr_in* cli_addr){
	int s_dial;
	unsigned int cli_len = sizeof(*cli_addr);
	s_dial = accept(s_ecoute, (struct sockaddr *)cli_addr, &cli_len);
	if (DEBUG)
		printf("CLIENT CONNECTED: %s:%d\n", inet_ntoa (cli_addr->sin_addr), ntohs (cli_addr->sin_port));
	return s_dial;
}

void verif_conn(PGconn* conn){
	if (PQstatus(conn) != CONNECTION_OK){
				fprintf(stderr, "Connection to database failed: %s",
								PQerrorMessage(conn));
				db_exit_nicely(conn);
		}
		else
			printf("Connection to database successed !\n");
}

void database_connect(PGconn* conn){
	const char* conninfo = CONN_INFO;
	conn = PQconnectdb(conninfo);
	if (PQstatus(conn) != CONNECTION_OK){
				fprintf(stderr, "Connection to database failed: %s",
								PQerrorMessage(conn));
				db_exit_nicely(conn);
		}
		else
			printf("Connection to database successed !\n");
}

int authentification(char* login, char* password, PGconn *conn){
	char query[500];
	PGresult *result;

	if(DEBUG)
		printf("AUTHENTIFICATION: %s/%s\n", login, password);
	sprintf(query, "SELECT id_user, password FROM Users WHERE Users.mail = '%s';", login);
	result = PQexec(conn, query);
	if (FULL_DEBUG)
		db_display_result(result);
	if (PQntuples(result) == 0){
		// if (DEBUG)
		// 	printf("AUTHENTIFICATION: Wrong Login\n");
		//send_data(s_dial, WRG_LOGIN, "Wrong login", buf, sizeof(buf));
		return -1;
	}
	else{
		if (strcmp(PQgetvalue(result, 0, 1), password) != 0){
			// if (DEBUG)
			// 	printf("AUTHENTIFICATION: Wrong Password\n");
			return -2;
		}
		else
			return atoi(PQgetvalue(result, 0, 0));
	}
}

void idrequ(char *domain, int id_user, int s_dial, PGconn *conn, char *buf, AccountList *list){
	char query[500];
	PGresult *result;

	char *accessibleAccountPartOne, *accessibleAccountPartTwo, *data;
	int data_size;

	if (DEBUG)
		printf("IDS_REQU: %s\n", domain);

	sprintf(query, "SELECT id_account, login FROM Account INNER JOIN Sites ON account.id_site = sites.id_site WHERE account.id_user = '%d' AND domain = '%s';", id_user, domain);
	result = PQexec(conn, query);
	accessibleAccountPartOne = build_id_data(result);
	*list = addToList(result, *list);
	PQclear(result);

	sprintf(query, "SELECT Account.id_account, Account.login FROM SharedAccount INNER JOIN Account ON sharedAccount.id_account = account.id_account INNER JOIN Sites ON account.id_site = sites.id_site INNER JOIN Users ON account.id_user = users.id_user WHERE sharedAccount.id_receiver = '%d' AND domain = '%s';", id_user, domain);
	result = PQexec(conn, query);
	accessibleAccountPartTwo = build_id_data(result);
	*list = addToList(result, *list);
	PQclear(result);

	data_size = strlen(accessibleAccountPartOne) + strlen(accessibleAccountPartTwo);
	data = malloc((data_size + 1)*sizeof(char));

	if ((strcmp(accessibleAccountPartOne, "") > 0) && (strcmp(accessibleAccountPartTwo, "") > 0)){
		sprintf(data, "%s,%s", accessibleAccountPartOne, accessibleAccountPartTwo);
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else if (strcmp(accessibleAccountPartOne, "") > 0){
		data = accessibleAccountPartOne;
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else if (strcmp(accessibleAccountPartTwo, "") > 0){
		data = accessibleAccountPartTwo;
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else{
		data = "";
		send_data(s_dial, NO_ACC, "There is no accessible account for this website", buf, sizeof(buf));
	}

	if (FULL_DEBUG)
		printf("IDS_REQU: End\n");
}

char* extractPssw(int id, PGconn* conn){
	char query[500];
	PGresult *result;
	char* pssw;

	if(DEBUG)
		printf("Extracting Password ...\n");

	sprintf(query, "SELECT password FROM Account WHERE id_account='%d'", id);
	result = PQexec(conn, query);
	pssw = PQgetvalue(result, 0, 0);
	PQclear(result);

	if(FULL_DEBUG)
		printf("Password Extracted !\n");

	return pssw;
}

int exist_mail(PGconn *conn, char *mail){
	char query[500];
	PGresult *result;

	sprintf(query, "SELECT * FROM Users WHERE mail='%s'", mail);
	result = PQexec(conn, query);
	if (PQntuples(result) >= 1)
		return 1;
	else
		return 0;
}

int exist_pseudo(PGconn *conn, char *pseudo){
	char query[500];
	PGresult *result;

	sprintf(query, "SELECT * FROM Users WHERE pseudo='%s'", pseudo);
	result = PQexec(conn, query);
	if (PQntuples(result) >= 1)
		return 1;
	else
		return 0;
}

AccountList insert(char* mail, int id_account, AccountList list){
	AccountList temp = list;
	if (list == NULL){
		list = createNode(mail, id_account);
		return list;
	}
	else{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = createNode(mail, id_account);
		return list;
	}
}

AccountList createNode(char* mail, int account_id){
	AccountList node = (AccountList) malloc(sizeof(struct node));
	node->mail = mail;
	node->account_id = account_id;
	node->next = NULL;
	return node;
}

void displayAccountList(AccountList list){
	AccountList temp = list;
	if (temp == NULL)
		printf("No account in the AccountList\n");
	else{
		printf("Account List:\n");
		while(temp != NULL){
			printf("\tMail: %s\tID:%d\n", temp->mail, temp->account_id);
			temp = temp->next;
		}
		if(FULL_DEBUG)
			printf("End of Account List\n");
	}
}

AccountList addToList(PGresult *result, AccountList list){
	int i;
	if (PQntuples(result) > 0){
		for (i=0;i<PQntuples(result);i++){
			list = insert(PQgetvalue(result, i, 1), atoi(PQgetvalue(result, i, 0)), list);
		}
	}
	return list;
}

AccountList freeAccountList(AccountList list){
	if (list->next != NULL)
		freeAccountList(list->next);
	if (list != NULL)
		free(list);
	return NULL;
}

int extractID(AccountList list, char* mail){
	if (DEBUG)
		printf("Extracting ID ...\n");
	AccountList temp = list;
	if (temp == NULL)
		return -1;
	else{
		while (temp != NULL){
			if (strcmp(temp->mail, mail) == 0){
				if(FULL_DEBUG)
					printf("ID Extracted !\n");
				return temp->account_id;
			}
			temp = temp->next;
		}
		return -2;
	}
}
