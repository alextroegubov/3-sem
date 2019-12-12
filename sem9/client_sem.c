#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define MAX_LEN 100

//client


struct Info{
	pid_t id;
	char sem_numb;
	int op;
};
typedef struct Info Info;


int main(){

//	srand(time(NULL));

	int msqid;
    char pathname[] = "server_sem.c";
    key_t key;

    struct mymsgbuf{
       	long mtype;
		Info info;
    } mybuf;


    /* Create message queue  */
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
	
	pid_t my_id = getpid();

	mybuf.mtype = 1;

//init semaphore
	mybuf.info.sem_numb = 0;
	mybuf.info.op = 0;
	mybuf.info.id = my_id;
	mybuf.mtype = 1;
	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){
				printf("Can\'t send message to server\n");
        		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        		exit(-1);
	}
	if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(Info), my_id, 0) < 0){
		printf("Can\'t receive message from server\n");
		exit(-1);
	}

	int my_sem_numb = mybuf.info.sem_numb;
	printf("inited sem: mysem %d\n", my_sem_numb);

	mybuf.info.sem_numb = my_sem_numb;
	mybuf.info.op = 4;
	mybuf.info.id = my_id;
	mybuf.mtype = 1;
	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){
				printf("Can\'t send message to server\n");
        		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        		exit(-1);
	}
	if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(Info), my_id, 0) < 0){
		printf("Can\'t receive message from server\n");
		exit(-1);
	}
	printf("A(sem, 4)\n");	

	mybuf.info.sem_numb = my_sem_numb;
	mybuf.info.op = -4;
	mybuf.info.id = my_id;
	mybuf.mtype = 1;
	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){
				printf("Can\'t send message to server\n");
        		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        		exit(-1);
	}
	if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(Info), my_id, 0) < 0){
		printf("Can\'t receive message from server\n");
		exit(-1);
	}
	printf("D(sem, 4)\n");

	printf("it works\n");
    return 0;    
}
