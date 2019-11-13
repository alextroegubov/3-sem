//find out the size of the pipe:

#include <unistd.h>
#include <fcntl.h>

int main(){
	
	int fd[2] = {0};
	char str[30] = {'a'};
	pipe(fd);

	for(int i = 0; i < 70000;i += 1){
		printf("i = %d\n", i);
		write(fd[1], str, 1);
	}
	close(fd[0]);
	close(fd[1]);
}
