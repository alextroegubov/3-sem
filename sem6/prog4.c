#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int a = 0;

int b = 5;

void *mythread(void *dummy){

	pthread_t mythid;

	mythid = pthread_self();
	
//	a = a + 1;
	
	printf("Thread %d, a = %d\n", mythid, ++a);

	return NULL;
}

void *mythread2(void *dummy){
	pthread_t mythid;

	mythid = pthread_self();

	printf("Tread %d, b = %d\n", mythid, ++b);
	
	return NULL;
}

int main(){

	pthread_t thid1, thid2, mythid;

	int result;

	result = pthread_create( &thid1,(pthread_attr_t *)NULL, mythread, NULL);

	if(result != 0){
		printf ("Error on thread create, return value = %d\n", result);

		exit(-1);
	}

	printf("Thread 1 created: thid1 = %d\n", thid1);

	result = pthread_create( &thid2,(pthread_attr_t *)NULL, mythread2, NULL);

	if(result != 0){
		printf ("Error on thread create, return value = %d\n", result);

		exit(-1);
	}

	printf("Thread 2 created, thid2 = %d\n", thid2);

	mythid = pthread_self();

	/*a = a + 1;*/

	printf("Main thread %d, a = %d\n", mythid, ++a);

	pthread_join(thid1, (void **)NULL);

	pthread_join(thid2, (void **)NULL);

	return 0;
}
