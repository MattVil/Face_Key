#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <libpq-fe.h>
#endif

#ifdef __linux__
#include <postgresql/libpq-fe.h>
#endif

//#define CONN_INFO "host=10.40.128.23 port=5432 dbname=db2017l3i_mvilain user=y2017l3i_mvilain password=A123456# connect_timeout=10"
#define CONN_INFO "host=127.0.0.1 port=5432 dbname=face_key user=postgres password= connect_timeout=10"

void db_exit_nicely(PGconn *conn);
void db_display_result(PGresult *result);