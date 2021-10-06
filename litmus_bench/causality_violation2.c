#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w = 0,r1 = 0,r2=0,r3 = 0,a1=0,a2=0,a3=0;
void __VERIFIER_atomic_t1(){
	x = 1;
	w = 2;
	r1 = x;
}

int __VERIFIER_atomic_t2(){
	r1 = x;
	printf("x2=%d ", x);
	w = 1;
	y = 1;
	r3 = 0;
	r3 = (r1 == 2);
	return r3;
 }

int __VERIFIER_atomic_t3(){	
	z = 1;
	r1 = x;
	printf("x3=%d ", x);
	r3 = 0;
	r3 = (r1 == 1);
	x = 2;
	return r3;
}
int __VERIFIER_atomic_t4(){	
	r1 = x;
	printf("x4=%d ", x);
	r2 = y;
	printf("y4=%d ", y);
	r3 = 0;
	if( x== 1 && y == 1) r3 = 1;
	return r3;
}

void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	a1 = __VERIFIER_atomic_t2();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a2 = __VERIFIER_atomic_t3();
	a3 = __VERIFIER_atomic_t4();
	return NULL;
}


int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);

	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("a1=%d a2=%d a3=%d \n", a1 , a2 , a3);
	if( a1 & a2 & a3) assert(0);
	return 0;
}
