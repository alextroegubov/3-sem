#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>

int main(){
	
	int semid;

    char sem_pathname[] = "key3.c";

    key_t sem_key;

    struct sembuf mybuf;

    sem_key = ftok(sem_pathname, 1);

    if((semid = semget(sem_key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }


	mybuf.sem_num = 0;
	mybuf.sem_op = 1;
	mybuf.sem_flg = 0;

	if(semop(semid, &mybuf, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }	
	
	return 0;
}
