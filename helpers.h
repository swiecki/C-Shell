#include <sys/types.h>
void removeWhitespace(char *string);
void removeComment(char *string);
char **tokenify (char *s, const char *sep);
char ***tokenify2 (char **s, const char *sep);
void freeAll1(char **array);
void freeAll2(char ***array);
char **readFile(char *filename);


struct node {
	pid_t pid;
	char command[1024];
	int state;
	struct node *next;
};

void listInsert(pid_t pid, char *command, int state, struct node **head);
void listClear(struct node *list);
void listDelete(pid_t pid, struct node **list);
