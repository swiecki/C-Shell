/*
 * project 1 (shell) main.c template 
 *
 * project partner names and other information up here, please
 *
 */

/* you probably won't need any other header files for this project */
#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>


int main(int argc, char **argv) {
	char *prompt = "term> ";
	printf("%s", prompt);
	fflush(stdout);
  
	char buffer[1024];
	while (fgets(buffer, 1024, stdin) != NULL) {
		//remove any comment at the end of the line
		removeComment(buffer);
		
		//Remove whitespace from buffer string
		// whitespace removal is currently too overzealous-- things like "e x i t" work...
		//removewhitespace(buffer);

		//Tokenize buffer by semicolons- each will be an executable command
		
		char **firststep = tokenify(buffer,";");
		char ***secondstep = tokenify2(firststep," \t\n");

		free(firststep);
		int j = 0;
		//execute commands in a loop:
		while(secondstep[j] != NULL){
			pid_t p = fork();
			if (p == 0){
				break;
			}
			//waitpid(p);
			j++;
		}

	  printf("%s",secondstep[j][0]);

		printf("\n%s\n", buffer);
		//char *cmd[] = { "/bin/ls", "-ltr", ".", NULL };

		//if (p == 0) {
			/* in child */
			// if (execv(cmd[0], cmd) < 0) {
			//     fprintf(stderr, "execv failed: %s\n", strerror(errno));
			// }
		//	exit(0);

		//} else if (p > 0) {
			/* in parent */
			//int rstatus = 0;
			//pid_t childp = wait(&rstatus);

			/* for this simple starter code, the only child process we should
			"wait" for is the one we just spun off, so check that we got the
			same process id */ 
			//assert(p == childp);

			//printf("Parent got carcass of child process %d, return val %d\n", childp, rstatus);
		///} else {
			/* fork had an error; bail out */
			// fprintf(stderr, "fork failed: %s\n", strerror(errno));
		//}

		printf("%s", prompt);
		fflush(stdout);
}
	return 0;
}

