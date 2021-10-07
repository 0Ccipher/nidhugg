#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w = 0,a1=0 , a2=0;
void __VERIFIER_atomic_t1(){
	x = 1;
	y = 1;
}

int __VERIFIER_atomic_t2(){
	int r1 = x;
	printf("x2=%d ", x);
	w = 1;
	y = 2;
	int r3 = 0;
	x = 2;
	if( x == 1) r3 = 1;
	return r3;
 }

int __VERIFIER_atomic_t3(){	
	z = 1;
	int r1 = x;
	int r2 = y;
	printf("x3=%d ", x);
	printf("y3=%d ", y);
	int r3 = 0;
	if( x == 1 && y == 1) r3 = 1;
	return r3;
}


void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a1 = __VERIFIER_atomic_t2();
	a2 = __VERIFIER_atomic_t3();
	return NULL;
}


int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("a1=%d a2=%d \n", a1 , a2);
	if( a1 & a2) assert(0);
	return 0;
}
