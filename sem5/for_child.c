#include<stdio.h>
#include <unistd.h>
int main(int argc, char* argv[], char *envp[]){

	int fd = atoi(argv[1]);

	printf("I am a child. I am reading from pipe\n");
	
	char string[8]= {'\0'};

	read(fd, string, 8);

	printf("string: %s", string);
	close(fd);
	return 0;
}
