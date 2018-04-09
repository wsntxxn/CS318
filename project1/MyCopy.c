/*
	Write a C program that makes a new copy of an existing file using system calls for file manipulation, such as open, read,
	write. The names of the two files, source and the destination are to be specified as command line arguments.
	Open the source file in read only mode and destination file in read/write mode. While the main function will carry out
	file opening and closing, a separate copy function needs to be written to do the actual copying. Copying can be done in
	blocks of suitable size. (MyCopy.c → MyCopy)
	An optional usage can be:
	>>./MyCopy srcFile desFile
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_LINE 8192

void copyFile(int srcFd, int desFd){//由源文件和目标文件的file descriptor进行copy操作的函数
  void *buff = malloc(MAX_LINE);
	int readwriteSize = read(srcFd, buff , MAX_LINE);
	if(readwriteSize < 0){
		perror("Read source file ");
		exit(1);
	}
  readwriteSize=write(desFd,buff,readwriteSize);
	if(readwriteSize<0){
		perror("Write ");
		exit(1);
	}
	if(buff){//释放内存
		free(buff);
		buff=0x0;
	}
	printf("Copying successfully executed.\n");
}

int main(int argc, char **argv){
  if(argc!=3) { //检测输入参数有效性
    printf("%s\n", "Invalid argument!");
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

  copyFile(srcFd, desFd);

	close(srcFd);
	close(desFd);
	return 0;
}
