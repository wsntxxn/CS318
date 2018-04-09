/*
	Write a shell-like program that illustrates how Linux spawns processes. This simple program will provide its own prompt
	to the user, read the command from the input and execute the command. It is sufficient to handle just "argument-less"
	commands, such as ls and date. (MyShell.c → MyShell)
*/

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 8192
#define MAXARGS 128

void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv);

char *environ[] = {NULL};

int main(){
	char cmdline[MAXLINE];
	while(1) {
		/*read*/
		printf(">");
		fgets(cmdline, MAXLINE, stdin);
		if(feof(stdin))
			exit(0);
		/*evaluate*/
		eval(cmdline);
	}
}

void eval(char *cmdline){
	char *argv[MAXARGS]; //argument value array
	char buf[MAXLINE];

	pid_t pid;

	strcpy(buf, cmdline);
	parseline(cmdline, argv); //Parse the command line and build the argv array
	if(argv[0] == NULL)
		return;

	if(!builtin_command(argv)){
		if((pid = fork()) == 0) { //Child process runs user job
			if(execvp(argv[0], argv) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}

		//Parent process waits for foreground job to terminate
		int status;
		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid error");
		}
		return;
	}
}

int builtin_command(char **argv) {
	if(!strcmp(argv[0], "quit")||!strcmp(argv[0], "exit"))
		exit(0);
	if(!strcmp(argv[0], "&"))
		return 1;
	return 0;
}

int parseline(char *buf, char **argv){
	char *delim;//points to the first space delimiter
	int argc;
	int bg;

	buf[strlen(buf)-1] = ' ';//Replace trailing '\n' with space
	while(*buf && (*buf == ' '))//Ignore leading spaces
		buf++;

	//Build argv list, only one argument
	if((delim = strchr(buf, ' '))) {
		argv[0] = buf;
		*delim = '\0';
	}
	argv[1] = NULL;

}
