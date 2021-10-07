/* Copyright (C)
* This benchmark is part of NIDHUGG-CCV*/

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <assert.h>

int vars[4]; 

void __VERIFIER_atomic_t0(){
  vars[0]=2;
  vars[1]=2;
}

void *t0(void *arg){
  __VERIFIER_atomic_t0();
return NULL;

}

void __VERIFIER_atomic_t1(){
  int r1 = vars[1];
  vars[3]= r1 + 1;
  int r2 = vars[0];
  printf("r11=%d , r12=%d ", r1,r2);
  vars[2]= r2 + 1;
}

void *t1(void *arg){
  __VERIFIER_atomic_t1();
  return NULL;
}

void __VERIFIER_atomic_t2(){
  int r1 = vars[3];
  int r2 = (r1 == 1);
  int r3 = vars[2];
  int r4 = (r3 == 0);
  printf("r1=%d , r3=%d ", r1,r3);
  if (r2 & r4) assert(0);

}

void *t2(void *arg){
__VERIFIER_atomic_t2();
return NULL;

}

int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 
  pthread_t thr2;
  pthread_create(&thr0 , NULL , t0, NULL);
  pthread_create(&thr1 , NULL , t1, NULL);
  pthread_create(&thr2 , NULL , t2, NULL);
  pthread_join(thr0,NULL);
  pthread_join(thr1,NULL);
  pthread_join(thr2,NULL);
  printf("\n");
  return 0;
}
