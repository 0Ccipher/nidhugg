/* Copyright (C)
* This benchmark is part of NIDHUGG-CCV*/

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <assert.h>

int var[4]; 
int vars[4] = {0,0,0,0};
void __VERIFIER_atomic_t0(){
  var[0]=2;
  var[1]=2;
}



void __VERIFIER_atomic_t1(){
  int r1 = var[1];
  var[3]= r1 + 1;
  int r2 = var[0];
  printf("r11=%d , r12=%d ", r1,r2);
  var[2]= r2 + 1;
}


void __VERIFIER_atomic_t2(){
  int r1 = var[3];
  int r2 = (r1 == 1);
  int r3 = var[2];
  int r4 = (r3 == 0);
  printf("r1=%d , r3=%d ", r1,r3);
  if (r2 & r4) assert(0);

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
  __VERIFIER_atomic_t0();
  __VERIFIER_atomic_t5();
return NULL;

}
void *t1(void *arg){
  __VERIFIER_atomic_t1();
  __VERIFIER_atomic_t6();
  __VERIFIER_atomic_t7();
  return NULL;
}

void *t2(void *arg){
__VERIFIER_atomic_t2();
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
int main(int argc, char **argv){
  pthread_t thr0; 
  pthread_t thr1; 
  pthread_t thr2,t4,t5;
  pthread_create(&thr0 , NULL , t0, NULL);
  pthread_create(&thr1 , NULL , t1, NULL);
  pthread_create(&thr2 , NULL , t2, NULL);
  pthread_create(&t4,NULL,thr4,NULL);
  pthread_create(&t5,NULL,thr5,NULL);
  pthread_join(t4,NULL);
  pthread_join(t5,NULL);
  pthread_join(thr0,NULL);
  pthread_join(thr1,NULL);
  pthread_join(thr2,NULL);
  printf("\n");
  return 0;
}
