#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>

int x=0,y=0;
void __VERIFIER_atomic_t1(){	
	y = 1;
	x = 1;
	printf("y1 = %d: ",y);
}

void __VERIFIER_atomic_t2(){
	printf("x : %d", x);
  x = 2;
 }

void __VERIFIER_atomic_t3(){
	y = 4;
	y = 4;
	y = 3;
	x = 3;
}
void __VERIFIER_atomic_t4(){
	//y = 10;
	y = 4;
	printf("x4 = %d: ",x);
	//assert( y < 0);
}

void __VERIFIER_atomic_t5(){
	y = 4;
	y = 4;
	y = 3;
	//y = x;
	//assert(x <= 0);
	x = 3;
}

void *thr1(void *arg){
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	
	return NULL;
}

void *thr2(void *arg){
	 __VERIFIER_atomic_t3();
	__VERIFIER_atomic_t4();
	__VERIFIER_atomic_t5();
	return NULL;

}
int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("\n");
	return 0;
}
