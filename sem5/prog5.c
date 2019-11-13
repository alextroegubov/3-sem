#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char *envp[]){
	int fd[2], result;

	size_t size;
	char  resstring[14];

	if(pipe(fd) < 0){
		printf("Can\'t open pipe\n");
		exit(-1);
	}
	
	printf("prog5: %p\n", fd);
	result = fork();

	if(result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);

	} else if(result > 0){

	 /* Parent process */
	 	close(fd[0]);
	 	char str1[10] = {'\0'};
		sprintf(str1, "%d", fd[1]);
		execle("for_parent", "for_parent", str1, NULL, envp);
		
		printf("Parent: can't open program\n");
		exit(1);

	} else{

		/* Child process */
		close(fd[1]);
		char str2[10] = {'\0'};;
		sprintf(str2, "%d", fd[0]);
		execle("for_child", "for_child", str2, NULL, envp);

		printf("Child: can't open program\n");
		exit(1);
	}

	return 0;
}
