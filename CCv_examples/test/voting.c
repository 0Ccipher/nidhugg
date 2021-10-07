#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

#define N 3
// Unsafe SC --  (0 , 0)
int vid[N] = {0,1,2};
int voted[3] = {0,0,0};
int contestants[2] = {0,1};
int c = 0;
int v = 0;
//int voterToPhoneVotesCount[3] = {0,0,0};
int i=0;
void __VERIFIER_atomic_addvote1(){
	if(voted[0] == 0){
		if(voted[1] ==0 || voted[2] == 0){
			printf(" Vid:%d ",v);
			voted[0] = c;
			printf(" Voted:%d ",c);
		}

	}
}
void __VERIFIER_atomic_addvote2(){
	if(voted[1] == 0){
		if(voted[2] ==0 || voted[0] == 0){
			printf(" Vid:%d ",v);
			voted[1] = c;
			printf(" Voted:%d ",c);
		}

	}
}

void __VERIFIER_atomic_addvote3(){
	if(voted[2] == 0){
		if(voted[1] ==0 || voted[0] == 0){
			printf(" Vid:%d ",v);
			voted[2] = c;
			printf(" Voted:%d ",c);
		}

	}
}

void *thr1(void *arg){
	printf(" ");
	c = 1;
	v = 0;
 	__VERIFIER_atomic_addvote1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	c = 2;
	v = 1;
 	__VERIFIER_atomic_addvote2();
	return NULL;
}
void *thr3(void *arg){
	printf(" ");
	c = 2;
	v = 2;
 	__VERIFIER_atomic_addvote3();
	return NULL;
}
int main(int argc, char *argv[]){
	pthread_t t[N];	
	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);
	pthread_create(&t[2],NULL,thr3,NULL);

	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);
	pthread_join(t[2],NULL);

	printf("\n");
	return 0;
}
