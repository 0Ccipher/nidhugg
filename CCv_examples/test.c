#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>

int x = 0, y = 0;

void __VERIFIER_atomic_t3(){
	printf("x3 = %d:",x);
	//printf("x1 = %d: ",x);
}

void __VERIFIER_atomic_t2(){
  printf("x2 = %d:",x);
 }

void __VERIFIER_atomic_t1(){
	printf("x1 = %d:",x);
	x = 3;
}

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	
	return NULL;
}

void *thr2(void *arg){
	 printf(" ");
	__VERIFIER_atomic_t2();
	return NULL;

}
void *thr3(void *arg){
	printf(" ");
	__VERIFIER_atomic_t3();
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
