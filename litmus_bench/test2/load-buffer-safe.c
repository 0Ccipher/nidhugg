/* Copyright (C)
* This benchmark is part of NIDHUGG-CCV*/

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <assert.h>

int x=0,y=0;
int a1=0,a2=0;
int vars[4] = {0,0,0,0};

int __VERIFIER_atomic_t0(){
  int r1 = x;
  y = 1;
  printf("x=%d ",x);
  int r2 = 0;
  r2 = (r1 == 1);
  return r2;
}


int __VERIFIER_atomic_t1(){
  int r1 = y;
  x = 1;
  printf("y=%d ",y);
  int r2 = 0;
  r2 = (r1 == 1);
  return r2;
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
void *t0(void *arg){
  a1 = __VERIFIER_atomic_t0();
  __VERIFIER_atomic_t6();
  __VERIFIER_atomic_t6();
  __VERIFIER_atomic_t5();
  __VERIFIER_atomic_t7();
return NULL;

}

void *t1(void *arg){
  a2 =__VERIFIER_atomic_t1();
  __VERIFIER_atomic_t5();
  __VERIFIER_atomic_t6();
  __VERIFIER_atomic_t7();
  __VERIFIER_atomic_t7();
  __VERIFIER_atomic_t6();
  __VERIFIER_atomic_t5();
  __VERIFIER_atomic_t7();
return NULL;

}


int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 

  pthread_create(&thr0 , NULL , t0, NULL);
  pthread_create(&thr1 , NULL , t1, NULL);

  pthread_join(thr0,NULL);
  pthread_join(thr1,NULL);
  printf("a1=%d a2=%d \n",a1,a2);
  if( a1 & a2 )assert(0);
  return 0;
}
