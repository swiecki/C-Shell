void removeWhitespace(char *string);
void removeComment(char *string);
char **tokenify (char *s, const char *sep);
char ***tokenify2 (char **s, const char *sep);
void freeAll1(char **array);
void freeAll2(char ***array);
char **readFile(char *filename);
void listInsert(pid_t pid, char *command, int state, struct node **head);
void listClear(struct node *list);
