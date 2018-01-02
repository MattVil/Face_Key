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
void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout);
int authentification(char* login, char* password, PGconn *conn);
void verif_conn(PGconn* conn);
void idrequ(char *domain, int id_user, int s_dial, PGconn *conn, char *buf, AccountList *list);
AccountList insert(char* mail, int id_account, AccountList list);
AccountList createNode(char* mail, int account_id);
void displayAccountList(AccountList list);
AccountList addToList(PGresult *result, AccountList list);
AccountList freeAccountList(AccountList list);

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
	char *data, temp_buf[BUF_SIZE], **split_data;
	int split_data_size;
	AccountList list = NULL;
	int code;

	serv_config(&serv_addr, &s_ecoute);
	//database_connect(conn); //DataBase connection block the timeout
	s_dial = connect_to_client(s_ecoute, &cli_addr);

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


	read_tt = recv_data(s_dial, buf);
	if (read_tt != -1){
		printf("MESSAGE RECEIVED: %s\n", buf);
		switch(getCode(buf)){
			case CONNEXION:
				send_data(s_dial, OK, "OK", buf, sizeof(buf));

				//AUTHENTIFICATION
				timeout_config(s_dial, &readfds, &timeout);
				select_tt = select(5, &readfds, NULL, NULL, &timeout);
				if (!select_tt){
					if (DEBUG)
						printf("CONNEXION: Auth timeout\n");
					send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
					break;
				}
				read_tt = recv_data(s_dial, buf);
				if (read_tt == -1){
					if (DEBUG)
						printf("CONNEXION: Read failed\n");
					send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
					break;
				}
				printf("MESSAGE RECEIVED: %s\n", buf);
				strcpy(temp_buf, buf);
				code = getCode(temp_buf);
				strcpy(temp_buf, buf);
				getData(temp_buf, &data);
				if (code != AUTH){
					if (DEBUG)
						printf("CONNEXION: (AUTH) Code unrecognized at this point (%d)\n", code);
					send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
					break;
				}
				split_data = str_split(data, ',', &split_data_size);
				if (split_data_size != 2){
					if (DEBUG)
						printf("AUTHENTIFICATION: Too much or not enough data\n");
					send_data(s_dial, MISSING, "Too much or not enough data", buf, sizeof(buf));
					break;
				}
				//database_connect(conn);
				split_data[1] = removechar(split_data[1], '\n');
				conn = PQconnectdb(CONN_INFO);
				verif_conn(conn);
				int user_id = authentification(split_data[0], split_data[1], conn);
				PQfinish(conn);
				if (user_id == -1){
					if (DEBUG)
						printf("AUTHENTIFICATION: Wrong Login\n");
					send_data(s_dial, WRG_LOGIN, "Wrong login", buf, sizeof(buf));
					break;
				}
				else if (user_id == -2){
					if (DEBUG)
						printf("AUTHENTIFICATION: Wrong Password\n");
					send_data(s_dial, WRG_PSSW, "Wrong Password", buf, sizeof(buf));
					break;
				}
				send_data(s_dial, OK, "You're logged in", buf, sizeof(buf));

				//DOMAIN
				timeout_config(s_dial, &readfds, &timeout);
				select_tt = select(5, &readfds, NULL, NULL, &timeout);
				if (!select_tt){
					if (DEBUG)
						printf("CONNEXION: IDS_REQU timeout\n");
					send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
					break;
				}
				read_tt = recv_data(s_dial, buf);
				if (read_tt == -1){
					if (DEBUG)
						printf("CONNEXION: Read failed\n");
					send_data(s_dial, 1000, "Internal error", buf, sizeof(buf));
					break;
				}
				printf("MESSAGE RECEIVED: %s\n", buf);
				strcpy(temp_buf, buf);
				code = getCode(temp_buf);
				strcpy(temp_buf, buf);
				getData(temp_buf, &data);
				if (code != IDS_REQU){
					printf("%d\n", IDS_REQU);
					if (DEBUG)
						printf("CONNEXION: (IDS_REQU) Code unrecognized at this point (%d)\n", code);
					send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
					break;
				}
				split_data = str_split(data, ',', &split_data_size);
				if (split_data_size != 1){
					if (DEBUG)
						printf("IDS_REQU: Too much or not enough data\n");
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

				//END
				list = freeAccountList(list);
				break;

			case CREATION:
				break;
			case UPDATE:
				break;
			case -1:
				if (DEBUG)
					printf("The message is not well formed\n");
				send_data(s_dial, MISSING, "The message is not well formed", buf, sizeof(buf));
				break;
			default:
				printf("Code unrecognized at this point\n");
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
	close (s_dial);
	close (s_ecoute);
	return EXIT_SUCCESS;
}

void serv_config(struct sockaddr_in* serv_addr, int* s_ecoute){
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = INADDR_ANY;
	serv_addr->sin_port = htons(PORT);
	memset(serv_addr->sin_zero, 0, sizeof(serv_addr->sin_zero));

	*s_ecoute = socket(PF_INET, SOCK_STREAM, 0);
	bind(*s_ecoute, (struct sockaddr *) serv_addr, sizeof(*serv_addr));
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

void verif_read(int read_tt){

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

void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout){
	FD_ZERO(readfds);//Put to zero
	FD_SET(file_desc, readfds);//Set the FileDescriptor

	timeout->tv_sec = TIMEOUT; //Timeout seconds
	timeout->tv_usec = 0; //Timeout microseconds
	if (DEBUG)
		printf("Timeout set to: %ld sec %d µsec\n", timeout->tv_sec, timeout->tv_usec);
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

	if ((strlen(accessibleAccountPartOne) != 0) && (strlen(accessibleAccountPartTwo) != 0)){
		sprintf(data, "%s,%s", accessibleAccountPartOne, accessibleAccountPartTwo);
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else if (strlen(accessibleAccountPartTwo) == 0){
		data = accessibleAccountPartOne;
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else if (strlen(accessibleAccountPartOne) == 0){
		data = accessibleAccountPartTwo;
		send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
	}
	else{
		data = "";
		send_data(s_dial, NO_ACCESS_ACCOUNT, "There is no accessible account for this website", buf, sizeof(buf));
	}
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
		printf("No account in the AccountList");
	else{
		printf("Account List:\n");
		while(temp != NULL){
			printf("\tMail: %s\tID:%d\n", temp->mail, temp->account_id);
			temp = temp->next;
		}
	}
}

AccountList addToList(PGresult *result, AccountList list){
	int i;
	for (i=0;i<PQntuples(result);i++){
		list = insert(PQgetvalue(result, i, 1), atoi(PQgetvalue(result, i, 0)), list);
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