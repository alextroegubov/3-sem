#include<stdio.h>
#include <unistd.h>
#include<string.h>
int main(int argc, char* argv[], char *envp[]){

	int fd = atoi(argv[1]);

	char string[] = "Hello!\n";
	printf("I am a parent. I am writing into pipe: Hello!\n");


	write(fd, string, 8);

	close(fd);
	return 0;
}
