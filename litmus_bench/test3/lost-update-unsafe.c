#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,w = 0,a1=0,a2=0;
int vars[4] = {0,0,0,0};

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
void __VERIFIER_atomic_t5(){
	int r1 = vars[0];
	int r2 = r1 + 1;
	vars[0] = r2;
}

void __VERIFIER_atomic_t6(){
	int r1 = vars[2];
	int r2 = r1 * 3 ;
	vars[1] = r2;
	vars[2] = r2 * r2;
}

void __VERIFIER_atomic_t7(){
	int r1 = vars[3];
	int r2 = vars[1];
	r2 = r2 + r1;
	vars[1] = r2;
}


void *thr1(void *arg){
	printf(" ");
	__VERIFIER_atomic_t6();
 	__VERIFIER_atomic_t7();
 	a1 = __VERIFIER_atomic_t1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	__VERIFIER_atomic_t7();
	a2 =__VERIFIER_atomic_t2();
	__VERIFIER_atomic_t7();
	return NULL;
}
void *thr4(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t5();
 	__VERIFIER_atomic_t7();
 	__VERIFIER_atomic_t6();
	return NULL;
}

void *thr5(void *arg){
	printf(" ");
	__VERIFIER_atomic_t7();
	__VERIFIER_atomic_t6();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t1,t2,t4,t5;
	pthread_create(&t4,NULL,thr4,NULL);
	pthread_create(&t5,NULL,thr5,NULL);
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t4,NULL);
	pthread_join(t5,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf("a1=%d , a2=%d \n", a1,a2);
	if( a1 & a2) assert(0);
	return 0;
}
