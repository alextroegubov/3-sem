#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define NUMBER 1000
#define LEN_FOR_CHILD 4
#define LEN_FOR_PARENT 5
#define PARENT_SEND "123"
#define CHILD_SEND "4567"

int main()
{
	int fd[2], result;

	size_t size = 0;

	char  resstring1[LEN_FOR_CHILD] = {'\0'}; //for child

	char resstring2[LEN_FOR_PARENT] = {'\0'}; //for parent
	
	int semid;
	char pathname[] = "prog4.c";
	key_t key = ftok(pathname, 0);

	struct sembuf mybuf;

	if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
		printf("Can\'t create semaphore set\n");
		exit(-1);
     }
	//sem = 0;

	if(pipe(fd) < 0){
		printf("Can\'t open pipe\n");
		exit(-1);
	}

	result = fork();
	
	if(result < 0) {
		printf("Can\'t fork child\n");
		exit(-1);
	} else if (result > 0) {

	 /* Parent process */

		for(int i = 0; i < NUMBER; i++){

//			size = write(fd[1], "123", 4);

			if(write(fd[1], PARENT_SEND, LEN_FOR_CHILD) < LEN_FOR_CHILD){
				printf("Can't write all string to pipe\n");
				exit(-1);
			}

			mybuf.sem_num = 0;
			mybuf.sem_op = 1;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);

			mybuf.sem_num = 0;
			mybuf.sem_op = -2;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);

			if(read(fd[0], resstring2, LEN_FOR_PARENT) != LEN_FOR_PARENT){
				printf("Can't read all string from pipe\n");
				exit(-1);
			}
			printf("Parent: read: %s\n", resstring2);

			mybuf.sem_num = 0;
			mybuf.sem_op = 3;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);

			mybuf.sem_num = 0;
			mybuf.sem_op = 0;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);
		}

		close(fd[0]);

		close(fd[1]);

		printf("Parent exit\n");

	} else {

		/* Child process */
		for(int i = 0; i < NUMBER; i++){

			mybuf.sem_num = 0;
			mybuf.sem_op = -1;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);

			if(read(fd[0], resstring1, LEN_FOR_CHILD) < LEN_FOR_CHILD){
				printf("Can't read string from pipe\n");
				exit(-1);
			}
			printf("Child: read: %s\n", resstring1);

			if(write(fd[1], CHILD_SEND, LEN_FOR_PARENT) < LEN_FOR_PARENT){
				printf("Can't write string to pipe\n");
				exit(-1);
			}

			mybuf.sem_num = 0;
			mybuf.sem_op = 2;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);

			mybuf.sem_num = 0;
			mybuf.sem_op = -3;
			mybuf.sem_flg = 0;
			semop(semid, &mybuf, 1);
		}

		printf("Child exit\n");

		close(fd[1]);
		close(fd[0]);
	}

	return 0;
}
