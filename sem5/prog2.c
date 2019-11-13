#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
	int     fd;
	size_t  size;
	char *string = NULL;

	(void)umask(0);

	if((fd = open("myfile",  O_RDONLY)) < 0){
		printf("Can\'t open file\n");
		exit(-1);
	}
	
	long int sym_count = 0;
	if (!(sym_count = lseek(fd, 0, SEEK_END))){
		printf("Can't lseek file!\n");
		exit(-1);
	}
	
	lseek(fd, 0, SEEK_SET);
	printf("found %ld symbols\n", sym_count);

	string = calloc(sym_count + 1, sizeof(char));

	string[sym_count] = '\0';

	if(size != sym_count){
		printf("Can\'t read all string\n");
		exit(-1);
	}
	
	if(close(fd) < 0){
		printf("Can't close file\n");
	}

	printf("From file:\n");
	printf("%s", string);
	free(string);
	
	return 0;
}
