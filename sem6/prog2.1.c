/*creates shared memory, writes there a message, detache shm*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 3000

int main(){
	
	int shmid = 0;

	long int nsym = 0;

	FILE *file = fopen("prog2.1.c", "r");

	if(fseek(file, 0, SEEK_END)){
        printf("Can't fseek file!\n");
        fclose(file);
        exit(-1);
    }   

    nsym = ftell(file); //number of sym in file

    fseek(file, 0, SEEK_SET);

    char *message = calloc(MAX_LEN, sizeof(char));

    if(!message){ 
        printf("can't allocate buffer\n");
        fclose(file);
        exit(-1);
    }

    message[nsym] = '\n';
    
	if(nsym > MAX_LEN){
		printf("Can't write a message in buffer: too long message\n");
		fclose(file);
		exit(-1);
	}

    long int mes_size = fread((void*)message, sizeof(char), nsym, file);

	fclose(file);

	key_t key = ftok("prog2.1.c", 1);

	if(key < 0)	{
		printf("Can't generate a key!\n");
		exit(-1);
	}

	shmid = shmget(key, MAX_LEN * sizeof(char), 0666 | IPC_CREAT);

	if(shmid < 0){
		printf("Can't create shared memory!\n");
		exit(-1);
	}

	char *shm = shmat(shmid, NULL, 0);

	if(shm == (char *) (-1)){
		printf("Can't attach memory!\n");
		exit(-1);
	}
	
	strncpy(shm, message, mes_size);

	if(shmdt(shm) < 0){
		printf("Can't detached shared memory!\n");
		exit(-1);
	}

	printf("Prog2.1.c executed correctly\n");

	return 0;
}
