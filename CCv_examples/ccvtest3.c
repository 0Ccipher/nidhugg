#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>

int x = 0, y = 0;

void __VERIFIER_atomic_t1(){
	x = 4;
	//printf("x1 = %d: ",x);
}

void __VERIFIER_atomic_t2(){
  x = -5;
 }

void __VERIFIER_atomic_t3(){
	y = 4;
	y = 4;
	y = 3;
	//printf("y3 = %d:",y);
}
void __VERIFIER_atomic_t4(){
	//y = 10;
	y = 4;
	return;
}

void __VERIFIER_atomic_t5(){
	x = -4;
	y = 5;
	//printf("y5 = %d:",y);
	return;
}

void __VERIFIER_atomic_t6(){
	x = 4;
	//y = 6;
	printf(" y := %d " , y);
	return;
}
void __VERIFIER_atomic_t7(){
	y = 7;
	printf(" x71 := %d ", x);
	printf(" y7 := %d " , y);
	printf(" x72 := %d ", x);
	return;
}

void *thr1(void *arg){
		//__VERIFIER_atomic_t0();
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	__VERIFIER_atomic_t3();
	return NULL;
}

void *thr2(void *arg){
		//__VERIFIER_atomic_t0();
	__VERIFIER_atomic_t4();
	return NULL;

}
void *thr3(void *arg){
		//__VERIFIER_atomic_t0();
	__VERIFIER_atomic_t5();
	__VERIFIER_atomic_t6();
	__VERIFIER_atomic_t7();
	return NULL;
}
int main(int argc, char *argv[]){
	pthread_t t1,t2,t3;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t3,NULL,thr3,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	printf("\n");
	return 0;
}
