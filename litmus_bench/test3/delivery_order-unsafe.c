#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0;
int a1=0,a2=0;
int vars[4] = {0,0,0,0};
void __VERIFIER_atomic_t1(){	
	z = 1;
	x = 1;
}

void __VERIFIER_atomic_t2(){
	y = 1;
 }

int __VERIFIER_atomic_t3(){
	x = 2;
	int r1 = z ;
	int l3 = ( z == 0);
	return l3;
}
int __VERIFIER_atomic_t4(){
	int r2 = y;
	int r3 = x;
	int r4 = 0;
	if( y == 1 && x == 2)
		r4 = 1;
	return r4;
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

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
 	__VERIFIER_atomic_t7();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a1 = __VERIFIER_atomic_t3();
	a2 = __VERIFIER_atomic_t4();
	__VERIFIER_atomic_t5();
	__VERIFIER_atomic_t7();
	return NULL;
}
void *thr4(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t5();
 	__VERIFIER_atomic_t6();
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
	pthread_t t1,t2,t4,t5;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t4,NULL,thr4,NULL);
	pthread_create(&t5,NULL,thr5,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t4,NULL);
	pthread_join(t5,NULL);
	printf(" a1=%d , a2=%d \n", a1,a2);
	if(a1 & a2) assert(0);
	return 0;
}
