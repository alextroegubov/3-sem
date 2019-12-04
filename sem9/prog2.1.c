#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LAST_MESSAGE 255

struct Info{
	int int_info;
	double double_info;
};
typedef struct Info Info;

int main(void)
{
    
	int msqid;
    char pathname[] = "prog2.1.c";
    key_t  key;

    struct mymsgbuf{
       	long mtype;
		Info info;
    } mybuf;



    /* Create or attach message queue  */
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }


    /* Send information */
    
    for (int i = 1; i <= 5; i++){
       
		mybuf.mtype = 1;
		mybuf.info.int_info = i;
		mybuf.info.double_info = sqrt(i);
       
		if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0) < 0){
			printf("Can\'t send message to queue\n");
        	msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        	exit(-1);
       }
    }    
       
    /* Send the last message */   
       
    mybuf.mtype = LAST_MESSAGE;      
       
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, 0, 0) < 0){
       printf("Can\'t send message to queue\n");
       msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
       exit(-1);
    }

    return 0;    
}
