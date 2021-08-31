#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0;
void __VERIFIER_atomic_t1(){	
	y = 1;
	x = 1;
}

void __VERIFIER_atomic_t2(){
   y = 2;
   //assert( y < 2);
 }

void __VERIFIER_atomic_t3(){
	y = 3;
	//x = 3;
}

void __VERIFIER_atomic_t5(){
	printf(" x5 := %d", x);
	printf(" z5 := %d \n", z);
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
