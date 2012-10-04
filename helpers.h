void removeWhitespace(char *string);
void removeComment(char *string);
int commandSplit(char *string);
char **tokenify (char *s, const char *sep);
char ***tokenify2 (char **s, const char *sep);
void freeAll1(char **array);
void freeAll2(char ***array);
