#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//server


struct Info{
	pid_t id;
	double number;
};
typedef struct Info Info;


int main(void)
{
    
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
	
	pid_t client_id = 0;

	double result = 0;

	/*waiting for client*/
	while(1){

		if(msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 1, 0) < 0){

			printf("Can\'t receive message from client\n");

			exit(-1);
		}

		if (mybuf.mtype == 1) {
			
			client_id = mybuf.info.id;

			result = sqrt(mybuf.info.number);


			mybuf.mtype = client_id;

			mybuf.info.number = result;

			mybuf.info.id = 1;

			if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){

				printf("Can\'t send message to queue\n");

        		msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

        		exit(-1);
       		}
			
		}
	}

	//	printf("message type = %ld, int_info = %d, double_info = %lf\n", mybuf.mtype, mybuf.info.int_info, mybuf.info.double_info);
    return 0;    
}
