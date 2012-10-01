#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"

void removeComment(char *s1){
	//remove comment by replacing '#' with '\0'
	if (s1 == NULL){
		return;
	}
	int i = 0;
	int newLength = strlen(s1);
	for (; i < strlen(s1);i++) {
		if (s1[i]=='#') {
			newLength = i;
		}
	}
	//check if replacement is necessary
	
	if (newLength != strlen(s1)){
		char buffer[newLength+1];
		int i = 0;
		int j = 0;
		for (; i < strlen(s1);i++) {
			if (s1[i]=='#'){
				buffer[j] = '\0';
				strcpy(s1,buffer);
				return;
			} else {
				buffer[j] = s1[i];
				j++;
			}
		}
	}
}
void removeWhitespace(char *s1){
	//remove whitespace from a string, in place
	if (s1 == NULL){
		return;
	}
	char buffer[strlen(s1)+1];
	int i = 0;
	int j = 0;
	for (; i < strlen(s1);i++) {
		if (!isspace(s1[i])) {
			buffer[j] = s1[i];
			j++;
		}
	}
	buffer[j] = '\0';
	strcpy(s1,buffer);
}
int commandSplit(char *s1){
	const char *sep = ";";
	char *command = NULL;
	char *temp1 = strdup(s1);
	int commands =0;
	for (command = strtok(temp1, sep);command;command = strtok(NULL, sep)){ commands++;}
	free(temp1);
	return commands;
}

char** tokenify(char *s, const char *sep)
{
	//this comment is new
	char *word = NULL;
	//copy string
	char *temp1 = strdup(s);
	char *temp2 = strdup(s);
	
	//find out how many tokens we have
	int words = 0;
	for (word = strtok(temp1, sep);word;word = strtok(NULL,sep)){words++;}

	//allocate teh array of char *'s with one additional
	char **array = malloc(sizeof(char*)*(words+1));
	int i = 0;
	for (word = strtok(temp2, sep); word; word = strtok(NULL, sep)) {
		printf("adding word %s to array pos %d\n", word, i);
		array[i] = strdup(word);
		i++;
	}

	free(temp1);
	free(temp2);
	array[words] = NULL;
	return array;
}
