#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

#define N 2

int userid =0;
int pwd = 0;
int r1 = 2, r2 = 2;
int a1 = 0, a2 = 0;
void __VERIFIER_atomic_register1(){
	r1 = userid;
	userid = 1;
	pwd = 1;
	printf("P1 userid=%d \t", r1);
}

void __VERIFIER_atomic_register2(){
	r2 = userid;
	userid = 1;
	pwd = 2;
	printf("P2 userid=%d \t", r2);
}

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_register1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
 	__VERIFIER_atomic_register2();
	return NULL;
}
void __VERIFIER_atomic_check(){
	if( r1 == 0 && r2 == 0){
		assert(0);
	}
}
int main(int argc, char *argv[]){
	pthread_t t[N];
	pthread_t t1;
	
	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);

	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);

	//__VERIFIER_atomic_check();
	printf("\n");
	return 0;
}
