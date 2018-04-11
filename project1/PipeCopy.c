/*
    Write a C program that forks two processes one for reading from a file (source file) and the other
    for writing (destination file) into. These two programs communicate using pipe system call.
    Once again the program accomplishes copying files, the names of which are specified as command-line
    arguments. (PipeCopy.c → PipeCopy)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define MAX_LINE 8192

int main(int argc,char **argv){
  if(argc != 3){
    printf("Invalid arguments!\n");
    exit(1);
  }


  int srcFd=open(argv[1], O_RDONLY);
	if(srcFd < 0){
		perror("Source file open ");
		exit(1);
	}

	int desFd = open(argv[2],O_RDWR|O_CREAT, 0644);//若不存在则创建文件
	if(desFd==-1){
		perror("Destination file open");
		exit(1);
	}

	void *buffer = malloc(MAX_LINE);
	int myPipe[2];
	int status;
	char *OutPipe;
	char InPipe[MAX_LINE];

	if(pipe(myPipe)==0){

    pid_t cpid=fork();
		if(cpid < 0){
			perror("Create child process");
		}
		else if(cpid == 0){ // child process, read from file and write to pipe
			close(myPipe[0]);
			read(srcFd, buffer, MAX_LINE);//read source file to buffer
			OutPipe = (char *)buffer;
			if(write(myPipe[1], OutPipe, strlen(OutPipe)+1) == -1)//write content from buffer to write-end pipe
				perror("Write pipe");
			close(myPipe[1]);
			exit(0);

		}
		else{ //parent process, read from pipe and write to destination file
			cpid=wait(&status);
    	printf("Child process %d exit with status %d\n", cpid, status);
			close(myPipe[1]);

			if(read(myPipe[0], InPipe, MAX_LINE ) == -1)//read content from read-end pipe to Inpipe
				perror("Read pipe");
        write(desFd, InPipe, strlen(InPipe));
        close(myPipe[0]);
        if(buffer){//释放内存
				free(buffer);
				buffer=0x0;
			}
		}
	}
	else {
		perror("Create pipe");
	}
    return 0;
}
