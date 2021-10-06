#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,r1 = 0,r2=0,r3=0;
int l3=0,l4=0;
void __VERIFIER_atomic_t1(){	
	z = 1;
	x = 1;
}

void __VERIFIER_atomic_t2(){
	y = 1;
 }

int __VERIFIER_atomic_t3(){
	x = 2;
	r1 = z ;
	l3 = ( z == 0);
	return l3;
}
int __VERIFIER_atomic_t4(){
	r2 = y;
	r3 = x;
	if( y == 1 && x == 2)
		l4 = 1;
	return l4;
}


void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_t1();
 	__VERIFIER_atomic_t2();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
	l3 = __VERIFIER_atomic_t3();
	l4 = __VERIFIER_atomic_t4();
	return NULL;

}
int main(int argc, char *argv[]){
	pthread_t t1,t2;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf(" l3=%d , l4=%d \n", l3,l4);
	//if(l3 & l4) assert(0);
	return 0;
}
