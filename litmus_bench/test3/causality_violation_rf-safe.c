#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w = 0,a1=0,a2=0,a3=0;
int vars[4] = {0,0,0,0};

int __VERIFIER_atomic_t1(){
	x = 1;
	int r1 = y;
	printf("y1=%d ", y);
	int r2 = 0;
	r2 = (r1 == 0);
	return r2;
}

int __VERIFIER_atomic_t2(){
	int r1 = x;
	printf("x2=%d ", x);
	w = 1;
	y = 2;
	int r3 = 0;
	r3 = (r1 == 1);
	return r3;
 }

void __VERIFIER_atomic_t5(){
	int r1 = vars[0];
	int r2 = r1 + 1;
	vars[0] = r2;
}

void __VERIFIER_atomic_t6(){
	int r1 = vars[2];
	int r2 = r1 * 3 ;
	vars[1] = r2;
	vars[2] = r2 * r2;
}

void __VERIFIER_atomic_t7(){
	int r1 = vars[3];
	int r2 = vars[1];
	r2 = r2 + r1;
	vars[1] = r2;
}

int __VERIFIER_atomic_t3(){	
	z = 1;
	int r1 = x;
	int r2 = y;
	printf("x3=%d ", x);
	printf("y3=%d ", y);
	int r3 = 0;
	if( x == 0 && y == 2) r3 = 1;
	return r3;
}


void *thr1(void *arg){
	printf(" ");
 	a1 = __VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t6();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a2 = __VERIFIER_atomic_t2();
	__VERIFIER_atomic_t7();
	__VERIFIER_atomic_t6();
	return NULL;
}
void *thr3(void *arg){
	//printf(" ");
	a3 = __VERIFIER_atomic_t3();
	__VERIFIER_atomic_t5();
	__VERIFIER_atomic_t7();
	return NULL;
}

void *thr4(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t5();
 	__VERIFIER_atomic_t7();
	return NULL;
}

void *thr5(void *arg){
	printf(" ");
	__VERIFIER_atomic_t7();
	__VERIFIER_atomic_t6();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t1,t2,t3,t4,t5;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t3,NULL,thr3,NULL);
	pthread_create(&t4,NULL,thr4,NULL);
	pthread_create(&t5,NULL,thr5,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);
	pthread_join(t5,NULL);
	//printf("a1=%d a2=%d a3=%d \n", a1 , a2 , a3);
	if( a1 & a2 & a3) assert(0);
	printf("\n");
	return 0;
}
