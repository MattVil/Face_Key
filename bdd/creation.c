#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>

static void exit_nicely(PGconn *conn){
  PQfinish(conn);
  exit(1);
}

int main(int argc, char const *argv[]) {

  const char *conninfo;
  PGconn     *conn;
  PGresult   *res;
  int         nFields;
  int         i,
              j;

  /*
    Connexion
  */
  if (argc > 1)
      conninfo = argv[1];
  else
      conninfo = "host=10.40.128.23 port=5432 dbname=db2017l3i_qgerard user=y2017l3i_qgerard password=A123456# connect_timeout=10";

  conn = PQconnectdb(conninfo);

  if (PQstatus(conn) != CONNECTION_OK)
  {
      fprintf(stderr, "Connection to database failed: %s",
              PQerrorMessage(conn));
      exit_nicely(conn);
  }

  res = PQexec(conn, "INSERT INTO Users (id_user, pseudo, mail_adr, password) VALUES ('000000001','freez','mattvilain@gmail.com', 'A123456*');");

  return 0;
}
