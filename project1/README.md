project 1 of CS318. Names of C source code and corresponding executable file are the same.(*.c->*)

Sig: Using sleep() and signal() system call to change the handler of singal SIGINT. 
	 Usage:./Sig
	 It will print "sleep" per second and when it receives SIGINT, it will print "hello" per every two seconds while still print "sleep" per second. The program will end after 10 seconds.

MyCopy: Using open(), read() and write() system call to copy a file. 
		Usage:./MyCopy ./test/test.txt ./test/new.txt

ForkCopy: Using fork() to create child process. Parent process wait for child process to call MyCopy to copy file.
		  Usage:./ForkCopy ./test/test.txt ./test/new.txt

PipeCopy: Using pipe() to create pipe. Parent and child process communicate by pipe to copy file.
		  Usage:./PipeCopy ./test/test.txt ./test/new.txt

SockCopy: Using socket(), bind(), conect(), send() and recv(). Parent and child process communicate by socket to copy file.
		  Usage:./SockCopy ./test/test.txt ./test/new.txt

MyShell: A mini shell. Using fork() to create child process. Command is executed in child process and parent process wait for child and wait for the user's next input. It can only handle one argument.
		 Usage:./MyShell
		 >ls

MoreShell: Extend MyShell by parsing command line with multiple strings.
		   Usage: ./MoreShell
		   >ls -l

DupShell: Extend MoreShell by using dup2() to redirect IO stream.
		  Usage:./DupShell
		  > ls -l | more
