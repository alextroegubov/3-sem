#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

	int semid;

	char pathname[] = "key3.c";

	key_t key;

	struct sembuf mybuf;

	key = ftok(pathname, 1); 
    
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){ 
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }   
    
	/* A(s, 1)*/
    mybuf.sem_num = 0;
    mybuf.sem_op  = 1; 
    mybuf.sem_flg = 0;

	if(semop(semid, &mybuf, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

	printf("Semaphor created\n");

    return 0;
}
