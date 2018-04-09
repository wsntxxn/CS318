/*
	Make the mini-shell (from the previous part) a little more powerful by allowing arguments to the commands.
	For example, it should be able to execute commands such as more filename and ls –l ~/tmp etc.
	(MoreShell.c → MoreShell) 
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
	int bg;
	pid_t pid;

	strcpy(buf, cmdline);
	bg = parseline(buf, argv); //Parse the command line and build the argv array

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
	if(!bg) {
		int status;
		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid error");
		}
	}
	else
		printf("%d %s", pid, cmdline);
	}
	return;
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

	//Build the argv list, may be more than one args
	argc = 0;
	while((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while(*buf && (*buf == ' ')) //Ignore spaces
			buf++;
	}
	argv[argc] = NULL;

	if(argc == 0)
		return 1;

	if((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}
