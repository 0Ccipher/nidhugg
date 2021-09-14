#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0;
void __VERIFIER_atomic_t1(){	
	x = 1;
	printf(" x1:=%d ",x);
}

void __VERIFIER_atomic_t2(){
 	printf(" x2:=%d: ",x);
 }

void __VERIFIER_atomic_t3(){
	//x = 3;
	//printf(" x3 = %d: ",x);
}
void __VERIFIER_atomic_t4(){
	printf(" x4:=%d ",x);
}
void __VERIFIER_atomic_t5(){
	x = 5;
	printf(" x5:=%d ",x);
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
	__VERIFIER_atomic_t4();
	return NULL;

}
void *thr3(void *arg){
	printf(" ");
	__VERIFIER_atomic_t5();
	return NULL;

}
int main(int argc, char *argv[]){
	pthread_t t1,t2,t3,t[9];
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t3,NULL,thr3,NULL);
	for(int i=0 ; i < 9;i++){
		pthread_create(&t[i], NULL, thr2, NULL);
	}
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	for(int i=0 ; i < 9; i++){
		pthread_join(t[i],NULL);
	}
	printf("\n");
	return 0;
}
