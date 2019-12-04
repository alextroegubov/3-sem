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
	double number;
};
typedef struct Info Info;


int main(void)
{

	srand(time(NULL));

	int msqid;
    char pathname[] = "server.c";
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

	double number = rand() % 100;

	mybuf.mtype = 1;

	mybuf.info.id = my_id;

	mybuf.info.number = number;

	if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){

				printf("Can\'t send message to server\n");

        		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

        		exit(-1);
	}
	
	printf("my id = %d, asked server for sqrt(%lf)\n", my_id, number);

	if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(Info), my_id, 0) < 0){

		printf("Can\'t receive message from server\n");

		exit(-1);
	}

	printf("my id = %d, received: sqrt(%lf) = %lf\n",my_id, number, mybuf.info.number); 

    return 0;    
}
