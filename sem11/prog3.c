#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


struct A{
	double f;
	double f2;
};

int main(void)
{

	int    fd = 0;
	size_t length = 0;
	int i = 0;

	struct A *ptr, *tmpptr;

	fd = open("mapped.dat", O_RDWR | O_CREAT, 0666);

	if( fd == -1){
		printf("File open failed!\n");
		exit(1);
	}

        length = 100000*sizeof(struct A);

        ftruncate(fd,length);

		ptr = (struct A *)mmap(NULL, length, 
			PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

        close(fd);

	if( ptr == MAP_FAILED ){
		printf("Mapping failed!\n");
		exit(2);
	}

        tmpptr = ptr;

	for(i = 1; i <=100000; i++){
	    tmpptr->f = i;
	    tmpptr->f2 = tmpptr->f*tmpptr->f;
	    tmpptr++;
	}

	munmap((void *)ptr, length);

	return 0;
}
