#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w=0,f=0;

// Scedules init and ..
void __VERIFIER_atomic_t1(){	
	//y = 1;
	x = 1;
	//printf(" y1 : %d " , y);
	printf(" x1:=%d " , x);
	w = 1;
}

void __VERIFIER_atomic_t2(){
	//printf(" x2 :=%d " , x);
    printf(" z2:=%d " , z);
	printf(" y2:=%d ", y);
   	y = 2;
}

void __VERIFIER_atomic_t4(){
	z = 4;
	printf(" z4:=%d " , z);
} 

void __VERIFIER_atomic_t5(){
	//printf("f5 :=%d ", f);
	y = 5;
	printf("y5:=%d ", y);
	//z = 6;
	//x = 3;
}


void __VERIFIER_atomic_t3(){
	//printf(" w4 :=%d", w);
	f = 4;
	printf(" f3:=%d", f);
	x = 4;
	//z = 4;
	//printf(" x7_1 := %d", x);
	//printf(" x7_2 := %d", x);
}

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	
	return NULL;
}
void *thr2(void *arg){
	printf(" ");
	 //__VERIFIER_atomic_t3();
	__VERIFIER_atomic_t3();
	return NULL;

}
void *thr3(void *arg){
	printf(" ");
	__VERIFIER_atomic_t4();
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