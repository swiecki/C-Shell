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
	int mode = 1;//paralell
	int futuremode = mode;
	char *prompt = "s-term> ";
	printf("%s", prompt);
	fflush(stdout);
  
	char buffer[1024];
	while (fgets(buffer, 1024, stdin) != NULL) {
		mode = futuremode;

		//remove any comment at the end of the line
		removeComment(buffer);
		
		//Tokenize buffer by semicolons- each will be an executable command
		
		char **firststep = tokenify(buffer,";");
		char ***secondstep = tokenify2(firststep," \t\n");

//		freeAll1(firststep);
		int j = 0;

		int status = 0;

		pid_t p = 1;
		//execute commands in a loop:
		while(secondstep[j] !=  NULL && secondstep[j][0] != NULL){
			//check for exit or mode, else fork
			if(!strcasecmp(secondstep[j][0],"exit")){
				//freeAll2(secondstep);
				
				//Check time spent in user mode and kernel mode. Right now I've got it separated by shell and processes, but we can add it together later.
				int idParent = RUSAGE_SELF;
				int idChild = RUSAGE_CHILDREN;
				int statParent = 0;
				int statChildren = 0;
				struct rusage dataParent;
				struct rusage dataChildren;
				statParent = getrusage(idParent, &dataParent);
				statChildren = getrusage(idChild, &dataChildren);
				if(!statParent){//If the getrvalue operation was a success
					printf("Shell time in user mode: %ld.%06ld seconds.\n", dataParent.ru_utime.tv_sec, dataParent.ru_utime.tv_usec);
					printf("Shell time in kernel mode: %ld.%06ld seconds. \n", dataParent.ru_stime.tv_sec, dataParent.ru_stime.tv_usec);
				}
				if(!statChildren){
					printf("Process time in user mode: %ld.%06ld seconds. \n", dataChildren.ru_utime.tv_sec, dataChildren.ru_utime.tv_usec);
					printf("Process time in kernel mode: %ld.%06ld seconds. \n", dataChildren.ru_stime.tv_sec, dataChildren.ru_stime.tv_usec);
				}
				exit(0);
			}
			else if(!strcasecmp(secondstep[j][0],"MODE")){
				if(secondstep[j][1] == NULL){
					printf("\nCurrent mode is %i\n", mode);
				}
				else if(!strcasecmp(secondstep[j][1],"PARALLEL") || !strcasecmp(secondstep[j][1],"p")){
					futuremode = 0;
				}
				else if(!strcasecmp(secondstep[j][1],"SEQUENTIAL") || !strcasecmp(secondstep[j][1],"s")){
					futuremode = 1;
				}
				else {
					//bullshit users with their bullshit commands- throw error
					printf("\nError: Your command was pretty awful.\n");
				}
			}
			else{
				//wasn't built in command, better use execv
				p = fork();
				if (p == 0){
					break;
				}
				if(mode==1){//Sequential mode
					pid_t childp = wait(&status);
					//Do something with childp; error checking, probably
				}
			}
			j++;
		}
		
		if (p == 0){
			//Execv for an actual, non-hardcoded command.
			printf("\n%s\n",secondstep[j][0]);
			if(execv(secondstep[j][0],secondstep[j])<0){
				fprintf(stderr, "Your command failed, and here's why you're a bad person: %s\n", strerror(errno));
			}
			exit(0);
		} 

		if (mode==0){
			//For each command, wait
			int pnum = 0;
			while(secondstep[pnum]!=NULL && secondstep[pnum][0] != NULL){
				pid_t childp = wait(&status);//Do something with childp, probably error checking
				pnum++;
			}	
		}

		printf("%s", prompt);
		fflush(stdout);
}
	//Ctrl+D will drop you down here; need to make sure any cleanup also happens here too. 
	return 0;
}

