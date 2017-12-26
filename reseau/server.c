#include "util.h"
#include "dbutil.h"

int main(){

	int splited_req_size, splited_data_size, i;
	char **splited_req, **splited_data;

	/*Socket var*/
	char buf[BUF_SIZE];
	int s_ecoute, s_dial;
	unsigned int cli_len;
	struct sockaddr_in serv_addr, cli_addr;

	/*Database (postgres) var*/
	const char *conninfo = CONN_INFO;
	PGconn *conn;
	PGresult *result;
	char query[500];

	/*Database connexion*/
	conn = PQconnectdb(conninfo);
	if (PQstatus(conn) != CONNECTION_OK){
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        db_exit_nicely(conn);
    }
    else
    	printf("Connection to database successed !\n");

	/*Socket configuration*/
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
	if (DEBUG){
		printf("------------MESSAGE------------\n");
		printf("MESSAGE RECEIVED: %s\n", buf);
	}

	splited_req = str_split(buf, ';', &splited_req_size);
	//splited_req_size = str_split(buf, ';', splited_req);
	if (DEBUG)
		print_splt_str(splited_req, splited_req_size);


	switch(atoi(splited_req[0])){
		case IDS_REQU:
			if (DEBUG)
				printf("ID Request on: %s\n",splited_req[1]);
			break;

		case PSSW_REQU:
			if (DEBUG)
				printf("Password Request on: %s\n",splited_req[1]);
			splited_data = str_split(splited_req[1], ',', &splited_data_size);
			if (DEBUG)
				print_splt_str(splited_data, splited_data_size);
			sprintf(query, "SELECT password FROM Account WHERE id_account='%s'", splited_data[0]);
			if(DEBUG)
				printf("Query: %s\n", query);
			result = PQexec(conn, query);
			db_display_result(result);
			PQclear(result);
			break;

		case PICGPS_UPDT:
			if (DEBUG)
				printf("Picture and GPS Update on: %s\n",splited_req[1]);
			break;

		default:
			send_data(s_dial, 400, "Unknown Request", buf, sizeof(buf));
			break;
	}

	if (DEBUG)
		printf("------------END OF MESSAGE------------\n\n");

	close (s_dial) ;
	close (s_ecoute) ;
}