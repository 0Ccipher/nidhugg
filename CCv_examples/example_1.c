#include <pthread.h>
#include <assert.h>
#include <stdio.h>

//Sample example 1
int x=0,y=0,z=0,w=0;
void __VERIFIER_atomic_t1(){	
	printf("x1:=%d ",x);
	//assert(x != 3);
	printf("y1:=%d ",y);
	z = 1;
}

void __VERIFIER_atomic_t2(){
 	y = 2;
 	printf("y2 = %d ",y);
 }

void __VERIFIER_atomic_t3(){
	printf("z3:= %d ",z);
	w = 2;
}
void __VERIFIER_atomic_t4(){
	printf(" w4:=%d ",w);
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
	__VERIFIER_atomic_t3();
	return NULL;

}
void *thr3(void *arg){
	printf(" ");
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
