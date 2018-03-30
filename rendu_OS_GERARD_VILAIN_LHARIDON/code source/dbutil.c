/**
* \file dbutil.c
* \brief tools for database use
* \author  Matthieu Vilain, Quentin Gerard
* \version 1.0
* \date 20/03/2018
*/
#include "dbutil.h"

/**
* \fn void db_exit_nicely(PGconn *conn)
* \brief exit database
* \param *conn connexion to exit
*/
void db_exit_nicely(PGconn *conn){
  PQfinish(conn);
  exit(1);
}

/**
* \fn db_display_result(PGresult *result)
* \brief display db result
* \param *result result of database
*/
void db_display_result(PGresult *result){
	int i, j, nFields = PQnfields(result);
	for (i = 0; i < PQntuples(result); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(result, i, j));
        printf("\n");
    }
}

/**
* \fn build_id_data(PGresult *result)
* \brief build data
* \param *result result of database
*/
char* build_id_data(PGresult *result){
	char *data = NULL;
	char temp[100];
	int bytes_count = 0;
	int i, j;

	for (i=0;i<PQntuples(result);i++)
		for (j=0; j<PQnfields(result);j++)
			bytes_count += strlen(PQgetvalue(result, i, j));

	data = calloc(bytes_count, sizeof(char));
	memset(data, 0, sizeof(data));

	if (PQntuples(result) == 0){
		return "";
	}
	else{
		for (i=0;i<PQntuples(result);i++){
			if (i == 0){
				sprintf(data, "%s", PQgetvalue(result, i, 1));
			}
			else{
				sprintf(temp, ",%s", PQgetvalue(result, i, 1));
				strcat(data, temp);
			}
		}
		return data;
	}
}