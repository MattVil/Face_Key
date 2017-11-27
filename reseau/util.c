#include "util.h"

void send_requ(int socket, int code, char* info, char* buf, int bufsize){
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

	if (DEBUG)
		printf("MESSAGE SPLITTED: %d PART(S)\n", count);
	*size = count;

	return result;
}

// int str_split(char *str, const char sep, char **splited_str){
// 	char **result, **result_temp;
// 	char *temp = str;
// 	char *token;
// 	int count = 1, i;
// 	size_t ind = 0;

// 	while (*temp){
// 		if (*temp == sep)
// 			count++;
// 		temp++;
// 	}

// 	splited_str = NULL;
// 	splited_str = malloc(count*sizeof(char*));

// 	/*if (splited_str == NULL)
// 		splited_str = malloc(count*sizeof(char*));
// 	else
// 		splited_str = realloc(splited_str, count*sizeof(char*));*/

// 	if (DEBUG)
// 		printf("%s\n", str);

// 	token = strtok(str, &sep);
// 	for (i=0; i<count; i++){
// 		splited_str[i] = strdup(token);
// 		token = strtok(NULL, &sep);
// 	}
// 	print_splt_str(splited_str, count);

// 	/*result = malloc(count*sizeof(char*));
// 	result_temp = result;
// 	token = strtok(str, &sep);
// 	while(token != NULL) {
// 		*result_temp = strdup(token);
// 		token = strtok(NULL, &sep);
// 		result_temp++;
// 	}
// 	print_splt_str(result, count);
// 	splited_str = result;*/

// 	if (DEBUG)
// 		printf("MESSAGE SPLITTED: %d PART(S)\n", count);

// 	return count;
// }

void print_splt_str(char **tab, int size){
	int i;
	for(i=0; i<size; i++){
		printf("PART %d: %s\n", i+1, tab[i]);
	}
}