#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,r1 = 0,r2=0,r3=0;
int a1 =0, a2=0;
int __VERIFIER_atomic_t1(){
	z = 2;
	r1 = x;
	y = 1;
	r2 = 0;
	r2 = ( r1 == 0);
	return r2;
}
int __VERIFIER_atomic_t2(){
	r1 = y;
	x = 2;
	r2 = x;
	r3 = 0;
	r3 = (r1 == 0);
	return r3;
}


void *thr1(void *arg){
	printf(" ");
 	a1 = __VERIFIER_atomic_t1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	a2 = __VERIFIER_atomic_t2();
	return NULL;

}
int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf(" a1=%d , a2=%d \n", a1,a2);
	if(a1 & a2) assert(0);
	return 0;
}
