//SC unsafe

#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

#define N 2
int tickets[2] = {3,3};
int events[2] = {0,0};

void __VERIFIER_atomic_createvent1(){
	events[0] = tickets[0];
}

void __VERIFIER_atomic_createvent2(){
	events[1] = tickets[1];
}

void __VERIFIER_atomic_totaltickets1(){
	printf(" total tickets P1 = %d \t" , events[0]+events[1]);
}

void __VERIFIER_atomic_totaltickets2(){
	printf(" total tickets P2 = %d \t" , events[0] + events[1]);
}


void *thr1(void *arg){
	printf(" ");
	__VERIFIER_atomic_createvent1();
 	__VERIFIER_atomic_totaltickets1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	__VERIFIER_atomic_createvent2();
 	__VERIFIER_atomic_totaltickets2();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t[N];
	pthread_t t1;
	
	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);

	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);

	printf("\n");
	return 0;
}
