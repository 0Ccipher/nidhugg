#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w = 0,r1 = 0,r2=0,r3 = 0,a1=0;
void __VERIFIER_atomic_t1(){
	x = 1;
	y = 1;
}

void __VERIFIER_atomic_t2(){
	w = 1;
	r2 = y;
	r3 = w;
 }

void __VERIFIER_atomic_t3(){	
	z = 3;
	x = 3;
}



int __VERIFIER_atomic_t4(){
	r1 = x;
	r2 = x;
	r3 = 0;
	r1 = (r1 == 1);
	r2 = (r2 == 3);
	if(r1 & r2)
		r3 = 1;
	return r3;
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
	a1 = __VERIFIER_atomic_t4();
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
	printf("a1=%d \n", a1);
	//if( a1) assert(0);
	return 0;
}
