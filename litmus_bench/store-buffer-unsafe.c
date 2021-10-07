#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w = 0,a1=0,a2;
void __VERIFIER_atomic_t1(){
	x = 1;
	y = 1;
}

int __VERIFIER_atomic_t2(){	
	int r1 = z;
	y = 3;
	int r2 = 0;
	r2 = (r1 == 0);
	return r2;
}

void __VERIFIER_atomic_t3(){
	int r1 = w;
	w = 2;
	z = 1;
 }

int __VERIFIER_atomic_t4(){
	int r1 = x;
	int r2 = x;
	int r3 = 0;
	r3 = (x==0);
	return r3;
}


void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	a1 = __VERIFIER_atomic_t2();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	__VERIFIER_atomic_t3();
	a2 =__VERIFIER_atomic_t4();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("a1=%d a2=%d \n", a1,a2);
	if( a1 & a2) assert(0);
	return 0;
}