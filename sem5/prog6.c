#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int fd_par_ch[2], fd_ch_par[2]; 
	int result;

	size_t size;
	char  resstring[14];

	if(pipe(fd_par_ch) < 0){
		printf("Can't open pipe\n");
		exit(-1);
	}

	if(pipe(fd_ch_par) < 0){
		printf("Can't open pipe\n");
		exit(-1);
	}


	result = fork();

	if(result < 0) {
		printf("Can't fork child\n");
		exit(-1);
	} else if (result > 0) {

	 /* Parent process */

		close(fd_par_ch[0]); //can't read
		close(fd_ch_par[1]); //can't write

		size = write(fd_par_ch[1], "from parent: Hello, child!", 27);

		if(size < 27){
			printf("Can't write all string to pipe\n");
			exit(-1);
		}
		
		char string1[30] = {'\0'};
		read(fd_ch_par[0], string1, 27);
		printf("parent received: %s\n", string1);
		close(fd_par_ch[1]);
		close(fd_ch_par[0]);
		printf("Parent exit\n");

	} else {

		/* Child process */

		close(fd_par_ch[1]); //can't write
		close(fd_ch_par[0]); //can't read
		write(fd_ch_par[1], "from child: Hello, parent!", 27);

		char string2[30] = {'\0'};

		size = read(fd_par_ch[0], string2, 27);

		if(size < 27){
			printf("Can\'t read string from pipe\n");
			exit(-1);
		}
		
		printf("Child received: %s\n", string2);

		close(fd_par_ch[0]);
		close(fd_ch_par[1]);
		printf("Child exit\n");
	}

	return 0;
}
