#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>

atomic_int x = 0, y = 0;
void __VERIFIER_atomic_t1(){
	x = 4;
	//printf("x1 = %d: ",x);
}

void __VERIFIER_atomic_t2(){
	x = -3;
  x = 90;
	if( x <= 0){
			y = 2;
			//printf("y2 = %d: ",y);  
	}
	else{
			assert(x <= 0);
			//y = 7;
			//y = 1;
			printf("x2 = %d: ",x);
	}

}

void __VERIFIER_atomic_t3(){
	y = 4;
	y = 4;
	y = 4;
	//printf("y3 = %d:",y);
}
void __VERIFIER_atomic_t4(){
	//y = 10;
	y = 4;
	return;
}
void *thr1(void *arg){
 	__VERIFIER_atomic_t1();
 	/* __VERIFIER_atomic_t2(); */
        __VERIFIER_atomic_t3();
	return NULL;
}

void *thr2(void *arg){
	/* __VERIFIER_atomic_t3(); */
	__VERIFIER_atomic_t2();
	return NULL;

}
void *thr3(void *arg){
	__VERIFIER_atomic_t4();
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
