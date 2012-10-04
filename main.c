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
	//Establish anything we'll need constantly.
	int mode = 1;//Start in sequential mode.
	int usepath = 0;//does our path file exist?
	int futuremode = mode;//This keeps track of mode changes until the end of the line.
	char *prompt = "s-term> ";//The prompt string.
	printf("%s", prompt);//Print the prompt.
	fflush(stdout);
 
	//do pathstuff
	char **paths = readFile("shell-config");
	if(paths != NULL){
		usepath = 1;
	}

	char **firststep = NULL;//The array of commands made by splitting the buffer along semicolons.
	char ***secondstep = NULL;//The array of commands, with each command split along whitespace into its arguments.
	
	char buffer[1024];//The buffer.
	while (fgets(buffer, 1024, stdin) != NULL) {//Get input from the buffer. If it's not null, continue.
		mode = futuremode;//Ensure that mode is up-to-date.

		//Remove any comments after a hash.
		removeComment(buffer);
		
		//Tokenize buffer by semicolons- each will be an executable command.
		
		firststep = tokenify(buffer,";");
		secondstep = tokenify2(firststep," \t\n");

		//Free firststep, as it is no longer needed. Free the sub-arrays first, then the array proper.
		freeAll1(firststep);
		free(firststep);
	
		int j = 0;
		int futureExit = 0;
		int status = 0;

		pid_t p = 1;	
		//Insert some sort of pid_t p[] here, sized to the number of commands in secondstep? May come in useful when switching over to waitpid.

		//Execute all commands in a loop.
		while(secondstep[j] !=  NULL && secondstep[j][0] != NULL){
			//check for the special commands mode or exit. If neither of these, fork and execv.
			if(!strcasecmp(secondstep[j][0],"exit")){
				futureExit = 1;//Will be checked at the end of the loop.
			}
			else if(!strcasecmp(secondstep[j][0],"MODE")){
				if(secondstep[j][1] == NULL){
					if(mode == 0){
						printf("\nCurrent mode is parallel\n");
					}
					else {
						printf("\nCurrent mode is sequential\n");
					}
				}
				else if(!strcasecmp(secondstep[j][1],"PARALLEL") || !strcasecmp(secondstep[j][1],"p")){
					futuremode = 0;
				}
				else if(!strcasecmp(secondstep[j][1],"SEQUENTIAL") || !strcasecmp(secondstep[j][1],"s")){
					futuremode = 1;
				}
				else {
					//Bullshit users with their bullshit commands - throw an error.
					printf("\nError: Your command was pretty awful.\n");
				}
			}
			else{
				//Fork and execute/wait depending on process id.
				p = fork();
				if (p == 0){
					break;//Child processes handled outside the while loop.
				}
				if(mode==1){//Sequential mode.
					wait(&status);
					//Do something with childp; error checking, probably
				}
			}
			j++;
		}
		
		if (p == 0){
			if(usepath==1){
				int k = 0;
				while(paths[k] != NULL){
					struct stat sr;
					char tempbuffer[1024];
					strcpy(tempbuffer, paths[k]);
					strcat(tempbuffer, "/");
					strcat(tempbuffer, secondstep[j][0]);
					int rv = stat(tempbuffer, &sr);
					if (rv < 0){
						k++;
					}	
					else{
						secondstep[j][0]=tempbuffer;
						if(execv(secondstep[j][0],secondstep[j])<0){
							exit(0);
						}
					}
				}
			}
			//Execv for an actual, non-hardcoded command.
			printf("\n%s\n",secondstep[j][0]);
			if(execv(secondstep[j][0],secondstep[j])<0){
				fprintf(stderr, "Your command failed, and here's why you're a bad person: %s\n", strerror(errno));
			}
			exit(0);//Close out the child process corpse.
		
		} 

		if (mode==0){//Parallel mode.
			//Wait for each child to die.
			int pnum = 0;
			while(secondstep[pnum]!=NULL && secondstep[pnum][0] != NULL){
				pid_t childp = wait(&status);//Do something with childp, probably error checking; also change this to waitpid (sequential doesn't need to be touched, only parallel)
				pnum++;
			}	
		}
		//check if there was an exit command earlier
		if(futureExit == 1){
			break;
		}

		//If we don't exit, free current buffer
		freeAll2(secondstep);	
		free(secondstep);

		printf("%s", prompt);
		fflush(stdout);
}
		//on a quit, flush our command array if it's not null already
		if(secondstep != NULL){
			freeAll2(secondstep);
			free(secondstep);
		}
		
		//Free the paths array as well.
		if(paths!=NULL){
			freeAll1(paths);
			free(paths);
		}
		
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
	//Ctrl+D will drop you down here; need to make sure any cleanup also happens here too.
	return 0;
}

