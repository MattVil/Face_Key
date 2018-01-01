#include "util.h"
#include "dbutil.h"

void serv_config(struct sockaddr_in* serv_addr, int* s_ecoute);
void database_connect(PGconn* conn);
int connect_to_client(int s_ecoute, struct sockaddr_in* cli_addr);
void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout);

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

	serv_config(&serv_addr, &s_ecoute);
	database_connect(conn);
	s_dial = connect_to_client(s_ecoute, &cli_addr);


	read_tt = recv_data(s_dial, buf);
	if (read_tt != -1){
		printf("MESSAGE RECEIVED: %s\n", buf);
		switch(getCode(buf)){
			case CONNEXION:
				break;
			case CREATION:
				break;
			case UPDATE:
				break;
			case -1:
				printf("The message is not well formed\n");
				break;
			default:
				printf("Code unrecognized at this point\n");
				break;
		}
	}
	else{
		printf("Read failed\n");
	}

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

	return 0;
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

void database_connect(PGconn* conn){
	conn = PQconnectdb(CONN_INFO);
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