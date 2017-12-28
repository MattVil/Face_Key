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
	int count = 1;
	int i;

	while (*temp){
		if (*temp == sep)
			count++;
		temp++;
	}

	result = malloc(count*sizeof(char*));
	result_temp = result;

	token = strtok(str, &sep);
	while(token != NULL) {
		*result_temp = strdup(token);
		token = strtok(NULL, &sep);
		result_temp++;
	}

	*size = count;
	if (DEBUG)
		printf("MESSAGE SPLITTED: %d PART(S)\n", count);

	return result;
}

void print_splt_str(char **tab, int size){
	int i;
	for(i=0; i<size; i++){
		printf("PART %d: %s\n", i+1, tab[i]);
	}
}