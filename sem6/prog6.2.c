#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(){
	
	key_t PET_key;

	char PET_pathname[] = "key.c";

	int PET_shmid;

	if((PET_key = ftok(PET_pathname,0)) < 0){ 
        printf("Can't generate key for PET_ARRAY\n");
        exit(-1);
    }   

	if((PET_shmid = shmget(PET_key, 3 * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

        if(errno != EEXIST){
            printf("Can't create PET_shm\n");
            exit(-1);

        }else {
            if((PET_shmid = shmget(PET_key, 3 * sizeof(int), 0)) < 0){
                printf("Can't find PET_shm\n");
                exit(-1);
            }
        }
    }

	int *PET_array = NULL;

	if((PET_array = (int *)shmat(PET_shmid, NULL, 0)) == (int *)(-1)){
        printf("Can't attach PET_shm\n");
        exit(-1);
    }


   int shmid;

   int new = 1;

   char pathname[] = "prog6.1.c";

   key_t key;

	if((key = ftok(pathname,0)) < 0){
		printf("Can\'t generate key\n");
		exit(-1);
	}

   	if((shmid = shmget(key, 3 * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      	if(errno != EEXIST){
         	printf("Can\'t create shared memory\n");
         	exit(-1);

      	}else {
        	if((shmid = shmget(key, 3 * sizeof(int), 0)) < 0){
           		printf("Can\'t find shared memory\n");
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


	/*Peterson's algorythm*/

	PET_array[1] = 1;

	PET_array[2] = 0;

	while(PET_array[0] && PET_array[2] == 0);

		/*critical section*/
	
   	if(new){
 		array[0] =  0;

		array[1] =  1;

		array[2] =  1;

	}else {

		array[1] += 1;

		for(long int i = 0; i < 10000000000L; i++); //10

		array[2] += 1;
	}

	printf("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n", 
			array[0], array[1], array[2]);

	/*end of critical section*/

	PET_array[1] = 0;

	/*end of Peterson algorythm*/

	if(shmdt(array) < 0){
		printf("Can't detach shared memory\n");
		exit(-1);
	}

	if(shmdt(PET_array) < 0){
		printf("Can't detach PET_shm\n");
		exit(-1);
	}

	return 0;
}
