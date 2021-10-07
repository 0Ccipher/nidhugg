/* Copyright (C)
* This benchmark is part of NIDHUGG-CCV*/

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <assert.h>

int vars[3] = {0,0,0}; 
int t=0,a1=0;
void __VERIFIER_atomic_t0(){
t=1;
vars[0]=2;
}
void __VERIFIER_atomic_t1(){
t=1;
vars[1]=1;
}

void *t0(void *arg){
__VERIFIER_atomic_t0();
__VERIFIER_atomic_t1();
return NULL;

}

void __VERIFIER_atomic_t2(){
t=2;
vars[1]=2;

}
void __VERIFIER_atomic_t3(){
t=2;
vars[2]=1;
}
void *t1(void *arg){
__VERIFIER_atomic_t2();
__VERIFIER_atomic_t3();
return NULL;

}

void __VERIFIER_atomic_t4(){
t=3;
vars[2]=2;

}
void __VERIFIER_atomic_t5(){
t=3;
vars[0]=1;

}
void *t2(void *arg){
__VERIFIER_atomic_t4();
__VERIFIER_atomic_t5();
return NULL;

}

int __VERIFIER_atomic_t6(){
int r1 = vars[0];
int r2 = vars[1];
int r3 = vars[2];
int r4 = 0;
if(r1==2 && r2==2 && r2==2) r4 = 1;
return r4;
}

void *t3(void *arg){
a1 = __VERIFIER_atomic_t6();
return NULL;

}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 
  pthread_t thr2; 
  pthread_t thr3;
  pthread_create(&thr0 , NULL , t0, NULL);
  pthread_create(&thr1 , NULL , t1, NULL);
  pthread_create(&thr2 , NULL , t2, NULL);
  pthread_create(&thr2 , NULL , t3, NULL);
  pthread_join(thr0,NULL);
  pthread_join(thr1,NULL);
  pthread_join(thr2,NULL);
  pthread_join(thr3,NULL);
  if( a1) assert(0);
  return 0;
}
