#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0;
void __VERIFIER_atomic_t1(){	
	//y = 1;
	x = 1;
}

void __VERIFIER_atomic_t2(){
   y = 2;
   z = 2;
   //assert( y < 2);
 }

void __VERIFIER_atomic_t3(){
	//y = 3;
	//z = 3;
}

void __VERIFIER_atomic_t4(){
	y = 4;
	z = 4;
	x = 4;
}

void __VERIFIER_atomic_t5(){
	printf(" x5:=%d", x);
	printf(" y5:=%d", y);
	printf(" z5:=%d", z);
	//assert( x != 0);
}


void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	 __VERIFIER_atomic_t3();
	//__VERIFIER_atomic_t4();
	__VERIFIER_atomic_t4();
	return NULL;

}
void *thr3(void *arg){
	printf(" ");
	__VERIFIER_atomic_t5();
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
