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