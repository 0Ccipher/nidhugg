#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w=0,f=0;

void __VERIFIER_atomic_t1(){	
	//y = 1;
	x = 1;
	//printf(" y1 : %d " , y);
	printf(" x1:=%d " , x);
	//w = 1;
}

void __VERIFIER_atomic_t2(){
	printf(" y2:=%d " , y);
	printf(" x2:=%d " , x);
	//printf(" x2:=%d " , x);
	//printf(" y2:=%d " , y);
	//assert( y!=6 || x!= 1 );
   	y = 2;
}

/*void __VERIFIER_atomic_t7(){
	printf(" w7:=%d " , w);
   	y = 2;
}*/

void __VERIFIER_atomic_t3(){
	printf(" z3:=%d ", z);
}

void __VERIFIER_atomic_t4(){
	//printf(" w4:=%d", w);
	f = 4;
	printf(" f4:= %d", f);
	//printf(" x7_2 := %d", x);
}

void __VERIFIER_atomic_t5(){
	//printf(" x5 := %d", x);
	printf(" z5:=%d ", z);
}


void __VERIFIER_atomic_t6(){
	printf("f6:=%d ", f);
	y = 6;
	x = 6;
}

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	//__VERIFIER_atomic_t7();
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
	//__VERIFIER_atomic_t4();
	__VERIFIER_atomic_t6();
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
