#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255
#define MAX_LEN 100

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
	int len = 0;

    struct mymsgbuf
    {
       long mtype;
	   Info info;
    } mybuf;
    
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
    
    while (1) {
       
       if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, MAX_LEN, 0, 0)) < 0){
         printf("Can\'t receive message from queue\n");
         exit(-1);
       }

       if (mybuf.mtype == LAST_MESSAGE) {
          msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
          exit(0);
       }
       
       printf("message type = %ld, int_info = %d, double_info = %lf\n", mybuf.mtype, mybuf.info.int_info, mybuf.info.double_info);
    }    

    return 0;       
}
