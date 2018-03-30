/**
* \file util2.h
* \brief tools for database use 
* \author  Matthieu Vilain, Quentin Gerard
* \version 2.0
* \date 20/03/2018
*/

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
#include <fcntl.h>
#include <sys/stat.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

#define DEBUG 1
#define FULL_DEBUG 0
#define BUF_SIZE 1000
#define ONLINE 1
#define PORT 5000
#define TIMEOUT 120
#define QUEUE 5
#define BUF_FILE 1


#define REQU_SEP 58
#define DATA_SEP 44

//Diagramme Codes
#define OK 000
#define NO 999
#define CONNEXION 001
#define CREATION 002
#define UPDATE 003
#define CLOSE_REQ 808

//Codes Client -> Serveur
#define IDS_REQU 100
#define PSSW_REQU 101
#define PICGPS_UPDT 102
#define AUTH 103
#define LOG_CREA 110
#define PSSW_CREA 111
#define ID_INFO 112
#define NO_PHOTO 115
#define PHOTO 113
#define LAST_PHOTO 114
#define UP 136

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
#define ERR_MAIL 409
#define ERR_PSEUDO 410

//Commande Serveur
#define SHUTDOWN 999
#define SHUTD_PW ((char *)"ketchup")
#define GOD 1000

//RSA
#define PUBKEY 1
#define PRIVKEY 0

/**
 *  \brief Gestion du timeout
 *
 *  Méthode qui permet de définir un timeout
 *
 *  \param file_desc : file descriptor
 *  \param readfds : read value
 *  \param timeout : timeout value
 */
void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout);

/**
 *  \brief configuration fo db 
 *  \param filename : file name
 *  \param port : port to connect
 * 	\param ip : ip to connect
 *	\return socket
 */
int config(char* filename, int* port, char* ip);

/**
 *  \brief send data
 *  \param socket : socket
 *  \param code : code of data to send
 * 	\param info : info of data
 * 	\param buf : buffer
 * 	\param bufsize : buffer size
 */
void send_data(int socket, int code, char* info, char* buf, int bufsize);

/**
 *  \brief split string
 *  \param str : string to seperate
 *  \param sep : separator
 * 	\param *size : size
 * 	\return splitted char
 */
char** str_split(char* str, char sep, int *size);

/**
 *  \brief print splitted string
 *  \param splited_req : strings to disp
 * 	\param size : size
 */
void print_splt_str(char** splited_req, int size);

/**
 *  \brief receive data
 *  \param socket : socket
 *  \param *buf : buffer
 * 	\return 0 if failed 1 if success
 */
int recv_data(int socket, char *buf);

/**
 *  \brief remove a char
 *  \param string : string to remove a char
 *  \param car : char to reove
 * 	\return string without char
 */
char* removechar(char* string, char car);

/**
 *  \brief get code from message
 *  \param *message : message to get code
 * 	\return code
 */
int getCode(char *message);

/**
 *  \brief get data from message
 *  \param *message : message to get data
 * 	\return code
 */
int getData(char* message, char* data);

/**
 *  \brief split message
 *  \param *code : code of message
 *  \param data : data of message
 *  \param *buf : buffer
 *  \param s_dial : dialer
 * 	\return 0 if failed 1 if success
 */
int split_message(int *code, char* data, char *buf, int s_dial);

/**
 *  \brief read a file
 *  \param file_name : file name
 *  \param buf[BUF_SIZE] : buffer
 * 	\return 0 if failed 1 if success
 */
int read_file(char* file_name, char buf[BUF_SIZE]);

/**
 *  \brief write on a file
 *  \param file_name : file name
 *  \param buf[BUF_SIZE] : buffer
 * 	\return 0 if failed 1 if success
 */
int write_file(char* file_name, char buf[BUF_SIZE]);

/**
 *  \brief connection error
 * 	\return 0 if false 1 if true
 */
int connect_err();

/**
 *  \brief Binding error
 * 	\return 0 if false 1 if true
 */
int bind_error();

/**
 * 	\deprecated deprecated function
 *  \brief send a file
 *  \param *filename : file name
 *  \param *send_filename : name of the file to send
 * 	\param s_dial : dialer
 * 	\return 0 if false 1 if true
 */
int send_file(char *filename, char *send_filename, int s_dial);


/**
* 	\deprecated deprecated function
 *  \brief receive a file
 *  \param *filename : file name
 *  \param *send_filename : name of the file to send
 * 	\param s_dial : dialer
 * 	\return 0 if false 1 if true
 */
int receive_file(int s_dial, char* directory);

/**
 *  \brief receive a file v2
 * 	\param s_dial : dialer
 * 	\param *directory : directory of file
 * 	\return 0 if false 1 if true
 */
int receive_file2(int s_dial, char *directory);

/**
 *  \brief send a file v2
 * 	\param *filename : file name to send
 * 	\param *send_filename : file name's name to send
 * 	\param *directory : directory of file
 * 	\param s_dial : dialer
 * 	\return 0 if false 1 if true
 */
int send_file2(char *filename, char *send_filename, int s_dial);
