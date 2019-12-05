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

	int begin = 1;

    sem_key = ftok(sem_pathname, 1);

    if((semid = semget(sem_key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t create semaphore set\n");
        exit(-1);
    }


   	int shmid;

   	int new = 1;

   	char pathname[] = "prog3.1.c";

   	key_t key;
	
	if((key = ftok(pathname,0)) < 0){
		printf("Can't generate key\n");
		exit(-1);
	}

   	if((shmid = shmget(key, 3 * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      	if(errno != EEXIST){
         	printf("Can't create shared memory\n");
         	exit(-1);

      	}else {
        	if((shmid = shmget(key, 3 * sizeof(int), 0)) < 0){
           		printf("Can't find shared memory\n");
            	exit(-1);
	 		}

			new = 0;
   		}
   	}

	int *array = NULL;

   	if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
      	printf("Can't attach shared memory\n");
	  	exit(-1);
   	}
	/*critical section*/

	mybuf.sem_num = 0;
    mybuf.sem_op  = -1;
    mybuf.sem_flg = 0;

   	if(new){
 		array[0] =  1;

		array[1] =  0;

		array[2] =  1;

	}else {

	if(semop(semid, &mybuf, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }

		array[0] += 1;

		for(long long int i = 0; i < 5000000000L; i++); //9

		array[2] += 1;
	}

	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", 
			array[0], array[1], array[2]);

	/*end of critical section*/

    mybuf.sem_num = 0;
    mybuf.sem_op  = 1; 
    mybuf.sem_flg = 0;
    
    if(semop(semid, &mybuf, 1) < 0){ 
        printf("Can\'t wait for condition\n");
        exit(-1);
    }   

	if(shmdt(array) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
	}

	return 0;
}
