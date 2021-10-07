#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define TIMEOUT 2
int inputs[2] = {7 , 10};
int bets[2]  = { 0, 0};
int t = 0;
int winner = 0;
int w = 0;
int b=0;
void __VERIFIER_atomic_bet1(){
	if( t < TIMEOUT){
		bets[0] = inputs[0];
		printf(" t1=%d " , t);
		t = t + 1;
	}
}

void __VERIFIER_atomic_bet2(){
	if( t < TIMEOUT){
		bets[1] = inputs[1];
		printf(" t2=%d " , t);
		t = t + 1;
	}
}

int __VERIFIER_atomic_settle(){
	printf(" t3=%d " , t);
	if( t >= TIMEOUT){
		if( bets[b] != 0){
			winner = b;
			printf(" winner=%d\n", winner);
		}
	}
	return winner;
}
void * thr1(void *arg){
	printf(" ");
	__VERIFIER_atomic_bet1();
	return NULL;
}

void * thr2(void *arg){
	printf(" ");
	__VERIFIER_atomic_bet2();
	return NULL;
}
void * thr3(void *arg){
	printf(" ");
	w = __VERIFIER_atomic_settle();
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_t t1 , t2 , t3;

	pthread_create(&t1, NULL, thr1 , NULL);
	pthread_create(&t2, NULL, thr2 , NULL);
	pthread_create(&t3, NULL, thr3 , NULL);

	pthread_join(t1 , NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	printf("\n");
	return 0;
}