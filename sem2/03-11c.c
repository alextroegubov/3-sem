/* Программа 03-1.с – пример создания нового процесса
с одинаковой работой процессов ребенка и родителя */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[], char *envp[]){
	pid_t pid, ppid, id;
	int a = 0;
	int b = 0;

	id = fork();
	pid = getpid();
	ppid = getppid();

	if(id == -1){
	;
	} else if (id == 0){
		printf("I am a child process, look at me:\n");
		(void) execle("/home/alex/inf_less_2/ppid", "./ppid", NULL, envp);
	
	} else{
		b += 7;
		printf("I am a parent process! \n");
		printf("b = %d\n", b);
	}
	
	return 0;
}
