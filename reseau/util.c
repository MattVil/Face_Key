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
