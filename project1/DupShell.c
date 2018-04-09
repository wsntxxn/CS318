/*
	Add to the mini-shell ability to execute command lines with commands connected by pipes. Use 
	dup system call to redirect IO. Example: ls -l | wc. (DupShell.c → DupShell). 
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
void split_pipe(char *buf, char **argv, int *pipe_num);
void eval_pipe(char **argv);
void execute(char *buf, char **argv);


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

	strcpy(buf, cmdline);

	if(strchr(buf, '|') == NULL){ //no pipes
		execute(buf, argv);
	}
	else { //dealing with pipe(only consider redirect once)
		int pipe_num = 0;

		/*split cmd argument buf by '|', the resulting list is argv, 
		number of single command is pipe_num */
		split_pipe(buf, argv, &pipe_num);

		eval_pipe(argv);
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

	//Build the argv list
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

void eval_pipe(char **argv){
	char buf[MAXLINE];
	char *pipe_argv[MAXARGS];
	pid_t pid;
	int status;

	if((pid = fork()) == 0) {
		int fd[2];

		/* 创建管道 */
	  	if(pipe(fd) < 0){
			printf("Pipe create failed.\n");
			return;
		}

		if((pid = fork()) == 0){//执行第一个命令的子进程
			//管道输出重定向到stdout
			close(1);
			dup2(fd[1], 1);
			close(fd[0]);

			//处理第一个参数最后不是' '的情况(加一个'\n')
			if(argv[0][strlen(argv[0])-1] != ' '){
				char tmp[strlen(argv[0])+1];
				strcpy(tmp, argv[0]);
				tmp[strlen(argv[0])] = '\n';
				strcpy(argv[0], tmp);
			}
			
			parseline(argv[0], pipe_argv);
			if(pipe_argv[0] == NULL)
				return;

			if(!builtin_command(pipe_argv)){
				if(execvp(pipe_argv[0], pipe_argv) < 0) {
					printf("%s: Command not found.\n", pipe_argv[0]);
					exit(0);
				}
			}

	  	}

		//执行第二个命令的进程
		if (waitpid(pid, &status, 0) < 0) {
			perror("waitpid error");
		}

		//管道输入重定向到stdin
		close(0);
	  	dup2(fd[0], 0);
		close(fd[1]);

		parseline(argv[1], pipe_argv);
		if(pipe_argv[0] == NULL)
			return;

		if(!builtin_command(pipe_argv)){
			if(execvp(pipe_argv[0], pipe_argv) < 0) {
				printf("%s: Command not found.\n", pipe_argv[0]);
				exit(0);
			}
		}

	}

	if (waitpid(pid, &status, 0) < 0) { //父进程准备回收
		perror("waitpid error");
	}

}

void execute(char *buf, char **argv){
	int bg;
	pid_t pid;

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
		printf("%d %s", pid, buf);
	}
	return;
}

void split_pipe(char *buf, char **argv, int *pipe_num){
	buf[strlen(buf)-1] = ' ';
  	char *save = NULL;
  	while ((argv[*pipe_num] = strtok_r(buf, "|", &save)) != NULL){
		(*pipe_num)++;
		buf = NULL;
	}
}
