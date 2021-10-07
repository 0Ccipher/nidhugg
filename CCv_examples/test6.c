#include <pthread.h>
#include <assert.h>
#include <stdio.h>

int x=0,y=0,z=0,w =0;
void __VERIFIER_atomic_t1(){	
	x = 1;
	z = 1;
	printf("x1:=%d ",x);
}

void __VERIFIER_atomic_t2(){
	y = 2;
 	printf("x2:=%d ",x);
 	//assert( x != 5);
 }

void __VERIFIER_atomic_t3(){
	x = 3;
	printf("y3:=%d ",y);
	printf("z3:=%d ",z);
}
void __VERIFIER_atomic_t4(){
	printf("x4:=%d ",x);
	printf("z4:=%d ",z);
}
void __VERIFIER_atomic_t5(){
	x = 5;
	z = 5;
	printf("z5:=%d ",z);
	printf("x5:=%d ",x);
}
void __VERIFIER_atomic_t6(){
	y = 6;
	x = 6;
	//w = 6;
	printf("y6:=%d ",y);
}
void __VERIFIER_atomic_t7(){
	x = 7;
	printf("x7:=%d ",x);
	printf("y7:=%d ",y);
	printf("z7:=%d ",z);
}
void __VERIFIER_atomic_t8(){
	printf("x8:=%d ",x);
	printf("z8:=%d ",z);
}
void __VERIFIER_atomic_t9(){
	x = 9;
	printf("x9:=%d ",x);
	printf("y9:=%d ",y);
	printf("z9:=%d ",z);
}
void __VERIFIER_atomic_t10(){
	printf("x10:=%d ",x);
	printf("z10:=%d ",z);
	//assert( x != 5);
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
	__VERIFIER_atomic_t6();
	return NULL;

}

void *thr4(void *arg){
	printf(" ");
	__VERIFIER_atomic_t7();
	//__VERIFIER_atomic_t8();
	return NULL;

}
void *thr5(void *arg){
	printf(" ");
	__VERIFIER_atomic_t9();
	__VERIFIER_atomic_t10();
	return NULL;

}
int main(int argc, char *argv[]){
	pthread_t t1,t2,t3,t4[4];
	pthread_create(&t1,NULL,thr1,NULL);
	pthread_create(&t2,NULL,thr2,NULL);
	pthread_create(&t3,NULL,thr3,NULL);
	//pthread_create(&t4[0],NULL,thr4,NULL);
	//pthread_create(&t4[1],NULL,thr5,NULL);
	for(int i=0 ; i < 4;i++){
		pthread_create(&t4[i], NULL, thr4, NULL);
	}
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	//pthread_join(t4[0],NULL);
	//pthread_join(t4[1],NULL);
	for(int i=0 ; i < 4; i++){
		pthread_join(t4[i],NULL);
	}
	
	printf("\n");
	return 0;
}
