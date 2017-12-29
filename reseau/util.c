#include "util.h"

void send_data(int socket, int code, char* info, char* buf, int bufsize){
	char* str_code;
	str_code = (char*) malloc(3*sizeof(char));
	if (DEBUG)
		printf("SENDING REQUEST %d (%s):\n", code, info);

	//itoa(code, str_code, 10);
	sprintf(str_code, "%d", code);
	if (DEBUG)
		printf("\tCODE CONVERTED IN STRING\n");
	memset(buf, 0, bufsize);
	if (DEBUG)
		printf("\tBUFFER CLEARED\n");
	strcat(str_code, ";");
	strcpy(buf, strcat(str_code, info));
	if (DEBUG)
		printf("\tREQUEST BUILD\n");
	write(socket, buf, strlen(buf));
	if (DEBUG)
		printf("\tREQUEST SENT\n");

	free(str_code);
}

char** str_split(char* str, const char sep, int *size){
	char **result, **result_temp;
	char *temp = str;
	char *token;
	char *toksep = malloc(sizeof(char));
	*toksep = sep;
	int count = 1;
	int i;

	while (*temp){
		if (*temp == sep)
			count++;
		temp++;
	}

	result = malloc(count*sizeof(char*));
	result_temp = result;

	token = strtok(str, toksep);
	while(token != NULL) {
		*result_temp = strdup(token);
		token = strtok(NULL, toksep);
		result_temp++;
	}

	*size = count;
	if (DEBUG)
		printf("MESSAGE SPLITTED: %d PART(S)\n", count);

	free(toksep);

	return result;
}

void print_splt_str(char **tab, int size){
	int i;
	for(i=0; i<size; i++){
		printf("PART %d: %s\n", i+1, tab[i]);
	}
}

void recv_data(int socket, char *buf){
	memset(buf, 0, BUF_SIZE);
	read(socket, buf, BUF_SIZE);
}

char* removechar(char* string, char car){
	char *result = calloc(strlen(string), sizeof(char));
	int i, j=0;
	for (i=0; i<strlen(string); i++){
		if (string[i] != car){
			result[j] = string[i];
			j++;
		}
	}
	return result;
}