#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0;
int a1 = 0, a2 =0;
void __VERIFIER_atomic_t1(){	
	z = 1;
	int r1 = z;
	x = 1;
}

void __VERIFIER_atomic_t2(){
	y = 2;
 }

int __VERIFIER_atomic_t3(){
	z =3;
	int r1 = x;
	int r2 = y;
	int r3 = 0;
	printf("x3=%d , y3=%d ", x,y);
	if(r1==1 && r2==0) r3 = 1;
	return r3;
}
int __VERIFIER_atomic_t4(){
	int r1 = x;
	int r2 = y;
	printf("x4=%d , y4=%d ", x,y);
	int r3 = 0;
	if(r1==0 && r2==2) r3 = 1;
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
	return NULL;
}

void *thr3(void *arg){
	printf(" ");
 	a1 = __VERIFIER_atomic_t3();
	return NULL;
}

void *thr4(void *arg){
	printf(" ");
	a2 = __VERIFIER_atomic_t4();
	return NULL;

}

int main(int argc, char *argv[]){
	pthread_t t1,t2,t3,t4;
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t3,NULL,thr3,NULL);
	pthread_create(&t4,NULL,thr4,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	pthread_join(t4,NULL);

	printf(" a1=%d , a2=%d \n", a1,a2);
	if(a1 & a2) assert(0);
	return 0;
}
