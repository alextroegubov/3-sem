#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LEN 100

//to create semaphore: sem_numb = 0 and op = 0; server return number of semaphore
struct Info{
	pid_t id;
	char sem_numb;
	int op; //S(sem): 0; A(S, N); D(S, N);
};
typedef struct Info Info;

struct mymsgbuf{
	long mtype;
	Info info;
};
typedef struct mymsgbuf mymsgbuf;

struct Proc{
	pid_t proc_id;
	char wait;
};
typedef struct Proc Proc;

//sem number - number in array;
struct Semaphore{
	unsigned int sem_val;
	int first_free;
	Proc queue[MAX_LEN]; //pid_t == 0 - empty cell
};
typedef struct Semaphore Semaphore;

int main(){ 
	int msqid = 0;
    char pathname[] = "server_sem.c";
    key_t key = 0;
	int sem_count = 0;
	Semaphore sem_array[11] = {0}; //10 in total
    mymsgbuf mybuf = {0};

    /* Create message queue  */
    key = ftok(pathname, 0);
    
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
       printf("Can\'t get msqid\n");
       exit(-1);
    }
	pid_t client_id = 0;
	int sem_numb = 0;
	int op = 0;

	/*waiting for client*/
	while(1){
		//getting message
		msgrcv(msqid, (struct msgbuf *)&mybuf, sizeof(Info), 1, 0);//reading mess only with type = 1	
		client_id = mybuf.info.id;
		sem_numb = mybuf.info.sem_numb;
		op = mybuf.info.op;
		printf("op = %d\n", op);
		if(sem_numb == 0 && op == 0){ //init newsemaphore
			sem_count++;
			mybuf.info.sem_numb = sem_count;
			mybuf.mtype = client_id; 
			msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(Info), 0);
			continue;
		}
		//counting semaphores from 1 to 10;
		if(op == 0){ //S(sem); 
			//placing process in the queue of an appropriate semaphore
			int proc_place = sem_array[sem_numb].first_free;
			sem_array[sem_numb].first_free++;
			sem_array[sem_numb].queue[proc_place].proc_id = client_id;
			sem_array[sem_numb].queue[proc_place].wait = op;
			printf("S(%d)\n", sem_numb);
		}
		else if(op > 0){ //A(sem, n)
			sem_array[sem_numb].sem_val += op;
			mybuf.mtype = client_id;
			msgsnd(msqid, (struct msgbuf *)&mybuf, sizeof(Info), 0);
			printf("A(%d,%d)\n", sem_numb, op);
		}
		else if(op < 0){//D(sem, n)
			int proc_place = sem_array[sem_numb].first_free;
			sem_array[sem_numb].first_free++;
			sem_array[sem_numb].queue[proc_place].proc_id = client_id;
			sem_array[sem_numb].queue[proc_place].wait = -op;
			printf("D(%d, %d)\n", sem_numb, op);
		}
		
		//checking only sem_n(only it could be changed)	
		for(int i = 0; i < MAX_LEN; i++){
			if(sem_array[sem_numb].queue[i].proc_id != 0){
				if(sem_array[sem_numb].queue[i].wait == 0 && sem_array[sem_numb].sem_val == 0){	
					mybuf.mtype = sem_array[sem_numb].queue[i].proc_id;
					sem_array[sem_numb].queue[i].proc_id = 0;
					msgsnd(msqid, (struct msgbuf *)&mybuf, sizeof(Info), 0);
				}
				else if(sem_array[sem_numb].queue[i].wait <= sem_array[sem_numb].sem_val){
					mybuf.mtype = sem_array[sem_numb].queue[i].proc_id;
					sem_array[sem_numb].sem_val -= sem_array[sem_numb].queue[i].wait;
					sem_array[sem_numb].queue[i].proc_id = 0;
					msgsnd(msqid, (struct msgbuf *)&mybuf, sizeof(Info), 0);
				}
			}
		}
		fflush(stdout);
	}

    return 0;    
}
