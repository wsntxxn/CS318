/*
	Write a C program that forks two processes one for reading from a file (source file) and the other for writing
	(destination file) into. These two programs communicate using Socket. Once again the program accomplishes copying
	files, the names of which are specified as command-line arguments. (SockCopy.c → SockCopy)
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


#define MAX_LINE 8192
#define SERVERHOST 10004

int main(int argc, char **argv){
	if(argc != 3) {
		printf("Usage: %s <Source file path> <Destination file path>\n", argv[0]);
		exit(1);
	}

	int socket_fd;
	int client_fd;
	struct sockaddr_in sock_addr;
	struct sockaddr_in remote_addr;

	char *contentSend;
	char contentReceive[MAX_LINE];
	int readwriteSize;

	pid_t cpid;
	int status;
	cpid = fork();
	if (cpid < 0) {
		perror("Create child process");
	}
	else if(cpid == 0) {//child process, read from the source file, client 

		sleep(2);

		//Open file and read from file to buff
		int srcFd=open(argv[1], O_RDONLY);
		if(srcFd < 0){
			perror("Source file open ");
			exit(1);
		}

		void *buff = malloc(MAX_LINE);
		readwriteSize = read(srcFd, buff, MAX_LINE);
		if(readwriteSize < 0){
			perror("Read source file ");
			exit(1);
		}

  		contentSend = (char *) buff;


  		//Send content with socket

  		char *servInetAddr = "127.0.0.1";

    	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

   		memset(&sock_addr, 0, sizeof(sock_addr));

    	sock_addr.sin_family=AF_INET;
		sock_addr.sin_port=htons(SERVERHOST);
		inet_pton(AF_INET, servInetAddr, &sock_addr.sin_addr);

    	if((connect(socket_fd,(struct sockaddr *) &sock_addr, sizeof(sock_addr))) < 0){
        	perror("connect");
        	exit(1);
    	}

    	if((send(socket_fd, contentSend, strlen(contentSend), 0)) < 0){
        	perror("send content error");
        	exit(1);
    	}

    	close(socket_fd);

  		if(buff){
			free(buff);
			buff=0x0;
		}

		exit(0);

	}
	else { //parent process, read from client and write to the file, server

		int desFd = open(argv[2],O_RDWR|O_CREAT, 0644);
		if(desFd==-1){
			perror("Destination file open");
			exit(1);
		}

		int sin_size;

		memset(&sock_addr, 0, sizeof(sock_addr));

		//Create socket
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if(socket_fd == -1){
			perror("connect creat communication socket");
			exit(1);
		}


		sock_addr.sin_family=AF_INET;
		sock_addr.sin_port=htons(SERVERHOST);
		sock_addr.sin_addr.s_addr=htonl(INADDR_ANY);


		//Bind socketfd&&addr
		if(bind(socket_fd,(struct sockaddr *) &sock_addr, sizeof(sock_addr))==-1){
			perror("bind");
			exit(1);
		}


		//Listen
		if(listen(socket_fd, 1024) == -1){
			perror("listen");
			exit(1);
		}


		sin_size = sizeof(struct sockaddr_in);
		if((client_fd = accept(socket_fd, (struct sockaddr *) &remote_addr, &sin_size)) == -1) {
			perror("accept");
		}

		//receive from child
		readwriteSize = recv(client_fd, contentReceive, MAX_LINE, 0);
		contentReceive[readwriteSize] = '\0';
		close(client_fd);
		close(socket_fd);


		//write to the destination file
		write(desFd, contentReceive, strlen(contentReceive));
    	printf("Copying successfully executed.\n");

    	cpid = wait(&status);
		printf("Child process %d exit with status %d\n", cpid, status);
    	return 0;
	}
}
