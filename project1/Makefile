TARGETLIST: Sig MyCopy ForkCopy PipeCopy SockCopy MyShell MoreShell DupShell clean

all:${TARGETLIST}

Sig: Sig.o
	gcc -o Sig Sig.o

MyCopy: MyCopy.o
	gcc -o MyCopy MyCopy.o

ForkCopy: ForkCopy.o
	gcc -o ForkCopy ForkCopy.o
    
PipeCopy: PipeCopy.o
	gcc -o PipeCopy PipeCopy.o
    
SockCopy: SockCopy.o
	gcc -o SockCopy SockCopy.o
    
MyShell: MyShell.o
	gcc -o MyShell MyShell.o
    
MoreShell: MoreShell.o
	gcc -o MoreShell MoreShell.o
    
DupShell: DupShell.o
	gcc -o DupShell DupShell.o
    
clean:
	rm -f *.o
