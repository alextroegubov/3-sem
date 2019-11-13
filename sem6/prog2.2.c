/*ataches shared memory, reads a message from it, delete shm*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_LEN 3000

int main(){
	
	int shmid = 0;

	key_t key = ftok("prog2.1.c", 1);

	char message[MAX_LEN] = {'\0'};

	if(key < 0)	{
		printf("Can't generate a key!\n");
		exit(-1);
	}

	shmid = shmget(key, MAX_LEN * sizeof(char), 0);

	if(shmid < 0){
		printf("Can't find shared memory!\n");
		exit(-1);
	}

	char *shm = shmat(shmid, NULL, 0);

	if(shm == (char *) (-1)){
		printf("Can't attach memory!\n");
		exit(-1);
	}
	
	printf("Read message:\n %s", shm);

	if(shmdt(shm) < 0){
		printf("Can't detached shared memory!\n");
		exit(-1);
	}
	
	if(shmctl(shmid, IPC_RMID, NULL) < 0){
		printf("Can't delete shared memory!\n");
		exit(-1);
	}

	printf("Prog2.2.c executed correctly\n");

	return 0;
}
