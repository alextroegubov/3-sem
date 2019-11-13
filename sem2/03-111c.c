
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
	pid_t pid, ppid, id;
	int a = 0;
	int b = 0;

	id = fork();
	pid = getpid();
	ppid = getppid();
	switch(id) {
	case -1:
		printf("Error!\n");
		break;
	case 0:
		a += 10;
		a += 4;
		printf("Child process: My pid = %d, my ppid = %d\n",(int)pid, (int)ppid);
		printf("a = %d\n", a);
		break;
	default:
		b += 7;
		printf("Parent process: My pid = %d, my ppid = %d\n",(int)pid, (int)ppid);
		printf("b = %d\n", b);

	}
	return 0;
}
