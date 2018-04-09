/*
    Write a C program that creates a new process to copy the files using the MyCopy. This program should spawn a new 
    process using fork system call. Then use execl to execute MyCopy program. The source and destination file names 
    presented as command-line arguments should be passed to execl as system call arguments. The main process waits for 
    completion of copy operation using wait system call. (ForkCopy.c → ForkCopy)
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc != 3) {
		printf("%s\n", "Invalid argument!");
		exit(1);
	}

	pid_t cpid;
	int status;
	cpid = fork();
	if (cpid < 0) {
		perror("Create child process");
	}
	else if(cpid == 0) {//child process
		if(execl("./MyCopy", "MyCopy", argv[1], argv[2], (char *) 0) == -1){
			perror("Executing MyCopy");
			exit(1);
		}
		exit(0);
	}
	else { //parent process
		cpid = wait(&status);
		printf("Child process %d exit with status %d\n", cpid, status);
	}
	return 0;
}
