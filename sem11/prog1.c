#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	
	DIR *dir = NULL;

	if(argc == 2)
		dir = opendir(argv[1]);
	else
		dir = opendir(".");

	if(!dir){
		printf("Can't open dir\n");

		exit(-1);
	}

	struct dirent *mydir = NULL;
	
	int n_dir = 0;

	while((mydir = readdir(dir)) != NULL){
		
		n_dir++;
		printf("%s\n", mydir->d_name);
	}

	closedir(dir);

	return 0;	
}
