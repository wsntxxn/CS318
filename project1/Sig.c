/*
  Requirements:
    Write a C program, deploy the signal. Let the program print “sleep\n” per second.
    When the progress receives the SIGINT signal, it starts to print “hello\n” every two seconds, while it still prints
    “sleep\n” at the original frequency. The progress will exit after a certain time, e.g. 10s. (Sig.c → Sig)
*/

#include <stdio.h>
#include <sys/signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

pid_t cpid;
int status;
int time = 0;

void handler(int sig) {
  if(cpid = fork() == 0){
    while(time < 10){
        printf("hello\n");
        sleep(2);
        time+=2;
    }
    exit(0);
  }
  else return;
}

int main(){
  signal(SIGINT, handler);//指定接收到SIGINT信号时的handler
  while(time < 10){
      printf("sleep\n");
      sleep(1);
      ++time;
  }
  if(cpid > 0) {//回收子进程
    cpid = wait(&status);
    printf("Child process %d exit with status %d\n", cpid, status);
  }
  
  exit(0);
}
