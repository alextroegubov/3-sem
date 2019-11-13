
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

	if(id == -1){
	;
	} else if (id == 0){
		a += 10;
		a += 4;
		printf("My pid = %d, my ppid = %d\n",(int)pid, (int)ppid);
		printf("a = %d\n", a);
	} else{
		b += 7;
		printf("My pid = %d, my ppid = %d\n",(int)pid, (int)ppid);
		printf("b = %d\n", b);

	}
	return 0;
}
