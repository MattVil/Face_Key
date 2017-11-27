#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define DEBUG 1
#define BUF_SIZE 80

//Codes Client -> Serveur
#define IDS_REQU 100
#define PSSW_REQU 101
#define PICGPS_UPDT 102

//Codes Serveur -> Client
#define IDS_SD 200
#define PSSW_SD 201

//Codes d'erreurs
#define UKNWREQ 400

void send_requ(int socket, int code, char* info, char* buf, int bufsize);
char** str_split(char* str, char sep, int *size);
//int str_split(char* str, const char sep, char** splited_str);
void print_splt_str(char** splited_str, int size);