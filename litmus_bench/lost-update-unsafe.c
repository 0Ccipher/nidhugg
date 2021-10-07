#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,w = 0,a1=0,a2=0;
int __VERIFIER_atomic_t1(){
	int r1 = x;//
	int r2 = 0;
	w = 0;
	x = 1;//
	if( r1 == 0) r2 =1;
	return r2;
}

int __VERIFIER_atomic_t2(){
	int r1 = x;//
	int r2 = 0;
	w = 1;
	x = 2;//
	if( r1 == 0) r2 =1;
	return r2;
 }


void *thr1(void *arg){
	printf(" ");
 	a1 = __VERIFIER_atomic_t1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a2 =__VERIFIER_atomic_t2();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t1,t2,t3;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("a1=%d , a2=%d \n", a1,a2);
	if( a1 & a2) assert(0);
	return 0;
}
