#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define MESSAGE
#define COUNTS

#ifdef MESSAGE

#define MAP_SZ 1000

#endif
int main(){
	int fd = 0;

	void *ptr = NULL;

	#ifdef MESSAGE
	int len = MAT_SZ;
	#endif

	#ifdef COUNTS
	int len = 3 * sizeof(int);
	#endif

	char new = 0;

	if((fd = open("map_prog5.dat", O_RDWR, 0666)) == -1){
		//create file
		fd = open("map_prog5.dat", O_RDWR | O_CREAT, 0666);
		if(fd == -1){
			printf("Can't open file\n");
			exit(1);
		}
		printf("file created\n");
		ftruncate(fd, len);
		new = 1;
	}
	ptr = mmap(NULL, len, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

	close(fd);

	if(ptr == MAP_FAILED){
		printf("Mapping failed!\n");
		perror("2");
		exit(2);
	}

	if(new)
		memset((char*)ptr, '\0', len);

	#ifdef MESSAGE
	strcpy((char*)ptr, "hello, world!");
	#endif 

	#ifdef COUNTS
	((int*)ptr)[0] += 1;
	((int*)ptr)[2] += 1;

	printf("prog5.1: %d, prog5.2: %d, total: %d\n", ((int*)ptr)[0], ((int*)ptr)[1], ((int*)ptr)[2]);
	#endif

	munmap(ptr, len);

	return 0;
}
