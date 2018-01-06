#include "util.h"

void timeout_config(int file_desc, fd_set* readfds, struct timeval* timeout){
	FD_ZERO(readfds);//Put to zero
	FD_SET(file_desc, readfds);//Set the FileDescriptor

	if (FULL_DEBUG)
		printf("TIMEOUT_SETTING: FileDescriptor set\n");

	timeout->tv_sec = TIMEOUT; //Timeout seconds
	timeout->tv_usec = 0; //Timeout microseconds
	if (FULL_DEBUG)
		printf("TIMEOUT_SETTING: ");
	if (DEBUG)
		printf("Timeout set to: %ld sec %ld µsec\n", timeout->tv_sec, timeout->tv_usec);
}

void send_data(int socket, int code, char* info, char* buf, int bufsize){
	char* str_code;
	str_code = (char*) malloc(3*sizeof(char));
	if (DEBUG)
		printf("SENDING REQUEST %d (%s):\n", code, info);

	//itoa(code, str_code, 10);
	sprintf(str_code, "%d", code);
	if (FULL_DEBUG)
		printf("\tCODE CONVERTED IN STRING\n");
	memset(buf, 0, bufsize);
	if (FULL_DEBUG)
		printf("\tBUFFER CLEARED\n");
	/*strcat(str_code, ";");
	strcpy(buf, strcat(str_code, info));*/
	sprintf(buf, "%s;%s", str_code, info);
	if (FULL_DEBUG)
		printf("\tREQUEST BUILD\n");
	write(socket, buf, strlen(buf));
	if (DEBUG)
		printf("\tREQUEST SENT\n");

	free(str_code);
}

char** str_split(char *str, const char sep, int *size){
	char **result, **result_temp;
	char *temp = str;
	char *token;
	/*char *toksep = malloc(sizeof(char));
	*toksep = sep;*/
	char toksep[10]; sprintf(toksep, "%c%c%c%c", sep, sep, sep, sep);
	int count = 1;
	int i;

	while (*temp){
		if (*temp == sep)
			count++;
		temp++;
	}

	result = malloc(count*sizeof(char*));
	result_temp = result;

	if (FULL_DEBUG){
		printf("String: %s\n", str);
		//printf("Separator: %c -->%c(%s)\n", sep, *toksep, toksep);
		printf("Separator: %c --> %s\n", sep, toksep);
	}
	token = strtok(str, toksep);
	while(token != NULL) {
		//printf("%s\n", token);
		*result_temp = strdup(token);
		token = strtok(NULL, toksep);
		result_temp++;
	}

	*size = count;
	if (FULL_DEBUG)
		printf("MESSAGE SPLITTED: %d PART(S)\n", count);

	//free(toksep);

	return result;
}

void print_splt_str(char **tab, int size){
	int i;
	for(i=0; i<size; i++){
		printf("PART %d: %s\n", i+1, tab[i]);
	}
}

int recv_data(int socket, char *buf){
	memset(buf, 0, BUF_SIZE);
	return read(socket, buf, BUF_SIZE);
}

char* removechar(char* string, char car){
	char *result = calloc(strlen(string)+1, sizeof(char));
	int i, j=0;
	for (i=0; i<strlen(string); i++){
		if (string[i] != car){
			result[j] = string[i];
			j++;
		}
	}
	result[j] = '\0';
	return result;
}

int getCode(char *message){
	char **splited_message;
	int splited_message_size;
	splited_message = str_split(message, ';', &splited_message_size);
	if (FULL_DEBUG){
		int i;
		for (i=0; i< splited_message_size; i++)
			printf("%s/", splited_message[i]);
		printf("\n");
	}
	if (splited_message_size != 2)
		return -1;
	else
		return atoi(splited_message[0]);
}

int getData(char* message, char* data){
	char **splited_message;
	int splited_message_size;
	splited_message = str_split(message, ';', &splited_message_size);
	if (FULL_DEBUG){
		int i;
		for (i=0; i< splited_message_size; i++)
			printf("%s/", splited_message[i]);
		printf("\n");
	}
	if (splited_message_size != 2)
		return 1;
	else{
		sprintf(data, "%s", removechar(splited_message[1], '\n'));
		return 0;
	}
}

int read_file(char* file_name, char buf[BUF_SIZE]){
	FILE* file = fopen(file_name, "r");
	memset(buf, 0, BUF_SIZE);
	int MAX = 1000;
	char chaine[MAX];
	memset(chaine, 0, MAX);

	if(file != NULL){
		while(fgets(chaine, MAX, file) != NULL){
			strcat(buf, chaine);
		}
		fclose(file);
		return 1;
	}
	return 0;
}

int write_file(char* file_name, char* buf){
	FILE* file = fopen(file_name, "r");
	if(file != NULL){
		fprintf(file, "%s", buf);
		fclose(file);
		return 1;
	}
	return 0;
}

int connect_err(){
	//perror("Error ");
	printf("Error: ");
	switch(errno){
		case EACCES:
			printf("You're trying to connect via broadcast or your firewall doesn't allow you to connect\n");
			return 1;
		case EPERM:
			printf("You're trying to connect via broadcast or your firewall doesn't allow you to connect\n");
			return 1;
		case EADDRINUSE:
			printf("Local address is already used\n");
			return 1;
		case EAFNOSUPPORT:
			printf("sa_family field is incorrect\n");
			return 1;
		case EAGAIN:
			printf("No local port available\n");
			return 1;
		case EALREADY:
			printf("The socket is nonblocking and a previous connection attempt has not yet been completed.\n");
			return 1;
		case EBADF:
			printf("Bad filedesc\n");
			return 1;
		case ECONNREFUSED:
			printf("Connexion refused\n");
			return 1;
		case EFAULT:
			printf("The socket structure address is outside the user's address space.\n");
			return 1;
		case EINPROGRESS:
			printf("The socket is nonblocking and the connection cannot be completed immediately.\n");
			return 1;
		case EINTR:
			printf("The system call was interrupted by a signal that was caught\n");
			return 1;
		case EISCONN:
			printf("The socket is already connected.\n");
			return 1;
		case ENETUNREACH:
			printf("Network is unreachable.\n");
			return 1;
		case ENOTSOCK:
			printf("The file descriptor sockfd does not refer to a socket\n");
			return 1;
		case EPROTOTYPE:
			printf("The socket type does not support the requested communications protocol\n");
			return 1;
		case ETIMEDOUT:
			printf("Timeout while attempting connection\n");
			return 1;
		default:
			printf("\n");
			return 0;
	}
}


int send_file(char *filename, char *send_filename, int s_dial){
	int file, file_size, bytes_sent, total_bytes_sent = 0, bytes_read;
	struct stat file_info;
	char buf[BUF_SIZE];
	if ((file = open(filename ,O_RDONLY)) == -1){
		perror("File can't be open");
		send_data(s_dial, NO, "Abort file transfer", buf, BUF_SIZE);
		return 1;
	}
	memset(buf, 0, BUF_SIZE);
	sprintf(buf, "%s", send_filename);
	write(s_dial, buf, sizeof(buf));

	memset(buf, 0, BUF_SIZE);
	read(s_dial, buf, BUF_SIZE);
	if (strcmp(buf, "0;Continue") != 0){
		printf("Receiver stop transfer\n");
		return 1;
	}

	fstat(file, &file_info);
	file_size = file_info.st_size;
	if (DEBUG)
		printf("Taille du fichier %s: %d\n", filename, file_size);
	memset(buf, 0, BUF_SIZE);
	sprintf(buf, "%d", file_size);
	write(s_dial, buf, sizeof(buf));

	while((bytes_read = read(file, buf, BUF_SIZE)) > 0){
		if((bytes_sent = write(s_dial, buf, BUF_SIZE)) < 0){
			perror("Error during transfer");
			return 1;
		}
		memset(buf, 0, BUF_SIZE);
		total_bytes_sent+=bytes_sent;
	}
	if (bytes_read < 0){
		perror("Read file error");
		return 1;
	}
	if (total_bytes_sent != file_size){
		printf("Curious thing that: File size is %d and sent size is %d\n", file_size, total_bytes_sent);
	}

	close(file);

	return 0;
}

int receive_file(int s_dial, char* directory){
	char buf[BUF_SIZE];
	char file_name[1000];
	int file, file_size, total_size_receive = 0;
	int read_flag;

	memset(buf, 0, BUF_SIZE);
	read_flag = read(s_dial, buf, BUF_SIZE);
	sprintf(file_name, "%s/%s", directory, buf);

	read(s_dial, buf, BUF_SIZE);
	if (strcmp(buf, "999;Abort file transfer") == 0){
		printf("Sender stop transfer\n");
		return 1;
	}

	if ((file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC,0666)) == -1){
		perror("Can't open file");
		send_data(s_dial, NO, "Abort file transfer", buf, BUF_SIZE);
		return 1;
	}
	send_data(s_dial, OK, "Continue", buf, BUF_SIZE);

	memset(buf, 0, BUF_SIZE);
	read_flag = read(s_dial, buf, BUF_SIZE);
	printf("Taille du fichier en arrivé: %s bytes\n", buf);
	file_size = atoi(buf);
	
	while((read_flag = read(s_dial, buf, BUF_SIZE)) > 0){
		write(file, buf, sizeof(buf));
		total_size_receive+=read_flag;
	}
	if (read_flag < 0){
		perror("Read file error");
		return 1;
	}

	close(file);
	return 0;
}