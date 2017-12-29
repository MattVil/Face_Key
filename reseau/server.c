#include "util.h"
#include "dbutil.h"

void idrequ(char *datarcv, int s_dial, PGconn *conn, char *buf);
void psswrequ(char *datarcv, int s_dial, PGconn *conn, char *buf);

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
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(5000);
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
	splited_req[splited_req_size-1] = removechar(splited_req[splited_req_size-1], '\n');
	if (DEBUG)
		print_splt_str(splited_req, splited_req_size);

	char *login, *pssw;

	switch(atoi(splited_req[0])){
		case AUTH:
			if (DEBUG)
				printf("Auth request on: %s\n", splited_req[1]);
			splited_data = str_split(splited_req[1], 44/* = ','*/, &splited_data_size);
			if (DEBUG)
				print_splt_str(splited_data, splited_data_size);
			if (splited_data_size == 2){
				login = splited_data[0]; pssw = splited_data[1];
				if (DEBUG)
					printf("%s/%s\n", login, pssw);
				sprintf(query, "SELECT password FROM Users WHERE Users.mail = '%s';", login);
				if (DEBUG)
					printf("Query: %s\n", query);
				result = PQexec(conn, query);
				if (DEBUG)
					printf("Number of results: %d\n", PQntuples(result));
				if (PQntuples(result) == 0)
					send_data(s_dial, WRG_LOGIN, "Wrong login", buf, sizeof(buf));
				else{
					if (strcmp(PQgetvalue(result, 0, 0), pssw) != 0)
						send_data(s_dial, WRG_PSSW, "Wrong password", buf, sizeof(buf));
					else 
						send_data(s_dial, GOD, "Access granted", buf, sizeof(buf));
				}
				db_display_result(result);
				PQclear(result);
			}
			else
				send_data(s_dial, MISSING, "Missing data", buf, sizeof(buf));
			break;

		case IDS_REQU:
			if (DEBUG)
				printf("ID Request on: %s\n",splited_req[1]);
			idrequ(splited_req[1], s_dial, conn, buf);
			break;

		case PSSW_REQU:
			if (DEBUG)
				printf("Password Request on: %s\n",splited_req[1]);
			psswrequ(splited_req[1], s_dial, conn, buf);
			break;

		case PICGPS_UPDT:
			if (DEBUG)
				printf("Picture and GPS Update on: %s\n",splited_req[1]);
			break;

		default:
			send_data(s_dial, UKNWREQ, "Unknown Request", buf, sizeof(buf));
			break;
	}

	if (DEBUG)
		printf("------------END OF MESSAGE------------\n\n");

	close (s_dial) ;
	close (s_ecoute) ;
}

void idrequ(char *datarcv, int s_dial, PGconn *conn, char *buf){
	char **splited_data;
	int splited_data_size;

	PGresult *result;
	char query[500];

	char *domain;
	char *userid;
	char *accessibleAccountPartOne;
	char *accessibleAccountPartTwo;
	char *data; int data_size;

	splited_data = str_split(datarcv, ',', &splited_data_size);
	if ((splited_data_size) != 2){
		send_data(s_dial, 401, "Missing Data", buf, sizeof(buf));
	}
	else{
		if (DEBUG)
			print_splt_str(splited_data, splited_data_size);

		domain = splited_data[0];
		userid = splited_data[1];

		sprintf(query, "SELECT id_account, login FROM Account INNER JOIN Sites ON account.id_site = sites.id_site WHERE account.id_user = '%s' AND domain = '%s';", userid, domain);
		if(DEBUG)
			printf("Query: %s\n", query);
		result = PQexec(conn, query);
		if (DEBUG)
			db_display_result(result);
		accessibleAccountPartOne = build_id_data(result);
		PQclear(result);
		sprintf(query, "SELECT Account.id_account, Account.login FROM SharedAccount INNER JOIN Account ON sharedAccount.id_account = account.id_account INNER JOIN Sites ON account.id_site = sites.id_site INNER JOIN Users ON account.id_user = users.id_user WHERE sharedAccount.id_receiver = '%s' AND domain = '%s';", userid, domain);
		if(DEBUG)
			printf("Query: %s\n", query);
		result = PQexec(conn, query);
		if (DEBUG)
			db_display_result(result);
		accessibleAccountPartTwo = build_id_data(result);
		PQclear(result);

		data_size = strlen(accessibleAccountPartOne) + strlen(accessibleAccountPartTwo);
		data = malloc((data_size + 1)*sizeof(char));
		printf("%s\n", accessibleAccountPartOne);
		printf("%s\n", accessibleAccountPartTwo);
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
			data = "0";
			send_data(s_dial, IDS_SD, data, buf, sizeof(buf));
		}
	}
}

void psswrequ(char *datarcv, int s_dial, PGconn *conn, char *buf){
	char **splited_data;
	int splited_data_size;

	PGresult *result;
	char query[500];

	splited_data = str_split(datarcv, ',', &splited_data_size);
	if (DEBUG)
		print_splt_str(splited_data, splited_data_size);

	sprintf(query, "SELECT password FROM Account WHERE id_account='%s';", splited_data[0]);
	if(DEBUG)
		printf("Query: %s\n", query);

	result = PQexec(conn, query);
	if (DEBUG)
		db_display_result(result);
	send_data(s_dial, PSSW_SD, PQgetvalue(result, 0, 0), buf, sizeof(buf));
	PQclear(result);
}