#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <errno.h>

#define DEBUG 1
#define FULL_DEBUG 0
#define BUF_SIZE 1000
#define PORT 5000
#define TIMEOUT 120
#define QUEUE 5

#define REQU_SEP 58
#define DATA_SEP 44

//Diagramme Codes
#define OK 000
#define CONNEXION 001
#define CREATION 002
#define UPDATE 003

//Codes Client -> Serveur
#define IDS_REQU 100
#define PSSW_REQU 101
#define PICGPS_UPDT 102
#define AUTH 103

//Codes Serveur -> Client
#define IDS_SD 200
#define PSSW_SD 201

//Codes d'erreurs
#define UKNWREQ 400
#define MISSING 401
#define WRG_LOGIN 402
#define WRG_PSSW 403
#define ERR_TIMEOUT 405
#define FORBIDDEN_REQU 406
#define NO_ACC 407
#define ERR_MATCH_ID 408

//Commande Serveur
#define SHUTDOWN 999
#define SHUTD_PW ((char *)"ketchup\n")
#define GOD 1000

void send_data(int socket, int code, char* info, char* buf, int bufsize);
char** str_split(char* str, char sep, int *size);
void print_splt_str(char** splited_req, int size);
int recv_data(int socket, char *buf);
char* removechar(char* string, char car);
int getCode(char *message);
int getData(char* message, char** data);
