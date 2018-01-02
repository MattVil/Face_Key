#include "dbutil.h"

void db_exit_nicely(PGconn *conn){
  PQfinish(conn);
  exit(1);
}

void db_display_result(PGresult *result){
	int i, j, nFields = PQnfields(result);
	for (i = 0; i < PQntuples(result); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(result, i, j));
        printf("\n");
    }
}

char* build_id_data(PGresult *result){
	char *data = NULL;
	char temp[100];
	int bytes_count = 0;
	int i, j;

	for (i=0;i<PQntuples(result);i++)
		for (j=0; j<PQnfields(result);j++)
			bytes_count += strlen(PQgetvalue(result, i, j));

	data = calloc(bytes_count, sizeof(char));

	for (i=0;i<PQntuples(result);i++){
		for (j=/*0*/1; j<PQnfields(result);/*j++*/j+=2){
			if ((i == 0) && (j == /*0*/1)){
				sprintf(data, "%s", PQgetvalue(result, i, j));
			}
			else{
				sprintf(temp, ",%s", PQgetvalue(result, i, j));
				strcat(data, temp);
			}
		}
	}

	return data;
}