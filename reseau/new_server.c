#include "util.h"
#include "dbutil.h"

void serv_config(struct sockaddr_in* serv_addr, int* s_ecoute);
void database_connect(PGconn* conn);
int connect_to_client(int s_ecoute, struct sockaddr_in* cli_addr);
void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout);
int authentification(char* login, char* password, PGconn *conn);
void verif_conn(PGconn* conn);

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
				timeout_config(s_dial, &readfds, &timeout);
				select_tt = select(5, &readfds, NULL, NULL, &timeout);
				if (!select_tt){
					if (DEBUG)
						printf("CONNEXION: Auth timeout\n");
					send_data(s_dial, ERR_TIMEOUT, "Timeout Reached", buf, sizeof(buf));
				}
				else{
					read_tt = recv_data(s_dial, buf);
					if (read_tt != -1){
						printf("MESSAGE RECEIVED: %s\n", buf);
						strcpy(temp_buf, buf);
						if (getCode(temp_buf) == 103){
							strcpy(temp_buf, buf);
							getData(temp_buf, &data);
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
						}
						else{
							if (DEBUG)
								printf("CONNEXION: (AUTH) Code unrecognized at this point (%d)\n", getCode(buf));
							send_data(s_dial, FORBIDDEN_REQU, "Code unrecognized at this point", buf, sizeof(buf));
						}
					}
				}
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