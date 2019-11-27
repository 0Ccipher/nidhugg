/* Copyright (C) 2018 Magnus Lång and Tuan Phong Ngo
 * This benchmark is part of SWSC */

#include <assert.h>
#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

atomic_int vars[2]; 
atomic_int atom_0_r5_2; 
atomic_int atom_1_r3_3; 
atomic_int atom_1_r4_4; 

void *t0(void *arg){
label_1:;
  atomic_store_explicit(&vars[0], 2, memory_order_seq_cst);

  atomic_store_explicit(&vars[1], 1, memory_order_seq_cst);

  int v2_r5 = atomic_load_explicit(&vars[1], memory_order_seq_cst);
  int v17 = (v2_r5 == 2);
  atomic_store_explicit(&atom_0_r5_2, v17, memory_order_seq_cst);
  return NULL;
}

void *t1(void *arg){
label_2:;
  atomic_store_explicit(&vars[1], 2, memory_order_seq_cst);
  int v4_r3 = atomic_load_explicit(&vars[1], memory_order_seq_cst);
  int v6_r4 = atomic_load_explicit(&vars[1], memory_order_seq_cst);
  int v7_r5 = v6_r4 ^ v6_r4;
  int v8_r5 = v7_r5 + 1;
  atomic_store_explicit(&vars[0], v8_r5, memory_order_seq_cst);
  int v18 = (v4_r3 == 3);
  atomic_store_explicit(&atom_1_r3_3, v18, memory_order_seq_cst);
  int v19 = (v6_r4 == 4);
  atomic_store_explicit(&atom_1_r4_4, v19, memory_order_seq_cst);
  return NULL;
}

void *t2(void *arg){
label_3:;
  atomic_store_explicit(&vars[1], 3, memory_order_seq_cst);
  return NULL;
}

void *t3(void *arg){
label_4:;
  atomic_store_explicit(&vars[1], 4, memory_order_seq_cst);
  return NULL;
}

int main(int argc, char *argv[]){
  pthread_t thr0; 
  pthread_t thr1; 
  pthread_t thr2; 
  pthread_t thr3; 

  atomic_init(&vars[0], 0);
  atomic_init(&vars[1], 0);
  atomic_init(&atom_0_r5_2, 0);
  atomic_init(&atom_1_r3_3, 0);
  atomic_init(&atom_1_r4_4, 0);

  pthread_create(&thr0, NULL, t0, NULL);
  pthread_create(&thr1, NULL, t1, NULL);
  pthread_create(&thr2, NULL, t2, NULL);
  pthread_create(&thr3, NULL, t3, NULL);

  pthread_join(thr0, NULL);
  pthread_join(thr1, NULL);
  pthread_join(thr2, NULL);
  pthread_join(thr3, NULL);

  int v9 = atomic_load_explicit(&atom_0_r5_2, memory_order_seq_cst);
  int v10 = atomic_load_explicit(&vars[0], memory_order_seq_cst);
  int v11 = (v10 == 2);
  int v12 = atomic_load_explicit(&atom_1_r3_3, memory_order_seq_cst);
  int v13 = atomic_load_explicit(&atom_1_r4_4, memory_order_seq_cst);
  int v14_conj = v12 & v13;
  int v15_conj = v11 & v14_conj;
  int v16_conj = v9 & v15_conj;
  if (v16_conj == 1) assert(0);
  return 0;
}