/* Программа 03-2.с, изменяющая пользовательский
контекст процесса (запускающая другую программу) */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[], char *envp[]){
	(void) execle("/bin/ls", "/bin/ls", "-la", NULL, envp);
	printf("Error on program start\n");
	exit(-1);
	return 0; 
}
