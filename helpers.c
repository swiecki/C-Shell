#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

	//allocate the array of char *'s with one additional
	char **array = malloc(sizeof(char*)*(words+1));
	int i = 0;
	int excluded = 0;
	for (word = strtok(temp2, sep); word; word = strtok(NULL, sep)) {
		char *tempword = strdup(word);
		removeWhitespace(tempword);
		if(!(tempword[0]=='\0'&&strlen(tempword)<1)){
			array[i] = strdup(word);
			i++;
		}
		else{
			excluded++;
		}
		free(tempword);
	}

	free(temp1);
	free(temp2);
	array[words-excluded] = NULL;
	return array;
}

char ***tokenify2(char **s, const char *sep)
{
	int f = 0;
	//count how big our array is
	while(s[f] != NULL){
		f++;
	}
	//allocate an array to hold all of our subarrays
	char ***toreturn = malloc((sizeof(char **))*(f+1));
	toreturn[f] = NULL;
	int i = 0;
	//tokenify the substrings into arrays that go into the new array
	while(s[i] != NULL){
		toreturn[i] = tokenify(s[i], sep);
		i++;
	}
	return toreturn;
}

void freeAll1(char **array){
//free all memory in a **char
	int i = 0;
	while(array[i]!=NULL){
		free(array[i]);
		i++;
	}
}

void freeAll2(char ***array){
//free all memory in a ***char
	int i = 0;
	while(array[i]!=NULL){
		int j = 0;
		while(array[i][j]!=NULL){
			free(array[i][j]);
			j++;
		}	
		free(array[i]);	
		i++;
	}
}
char **readFile(char *filename){
	FILE *f;
	char c;
	char s[1024];
	int lines = 0;
	//find out how big of an array to allocate
	f = fopen(filename, "r");

	if(f == NULL){
		return NULL;
	}
	while((c=fgetc(f)) != EOF){
		if(c == '\n'){
			lines++;
		}
	}
	fclose(f);

	//allocate a properly sized array
	char **toreturn = malloc(sizeof(char*)*(lines+1));
	f = fopen(filename, "r");
	int i = 0;
	int skipped = 0;
	if(f != NULL){
		while(fgets(s, 1024, f) != NULL){
			removeWhitespace(s);
			if(!(s[0]=='\0'&&strlen(s)<1)){
				toreturn[i] = strdup(s);
				i++;
			}
			else {
				skipped++;
			}
		}
		toreturn[lines-skipped] = NULL;
		fclose(f);
	}

	return toreturn;
}

void listInsert(pid_t pid, char *command, int state, struct node **head){
	struct node *newnode = malloc(sizeof(struct node));
	strncpy(newnode->command, command, 1024);
	newnode->pid = pid;
	newnode->state = state;
	newnode->next = *head;
	*head = newnode;
}

void listDelete(pid_t pid, struct node **list){
	//check if first node is the one to be deleted, adjust pointers accordingly if so, and free
	if ((*list)->pid == pid){
		struct node *tmp = *list;
		*list = (*list)->next;
		free(tmp);
		return;
	}
	struct node *tmp = *list;
	//check if any of the rest of the nodes is the one we're deleting
	while (tmp->next != NULL) {
		struct node *tmpnext = tmp->next;
		if(tmpnext->pid == pid){
			tmp->next = tmpnext->next;
			free(tmpnext);
			return;
		}
		tmp = tmp->next;
	}
}

void listClear(struct node *list) {
	while (list != NULL) {
		struct node *tmp = list;
		list = list->next;
		free(tmp);
	}
}
