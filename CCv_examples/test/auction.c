#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>


#define clients 2
int bids[clients] = {0,0};
atomic_int winner;
atomic_int status;
int amount[clients] = {100,105};
int b1 = 0, b2 = 0, l1 = 0,l2 =0, s = 0, w=0;

int __VERIFIER_atomic_place_bid1(){	
	if(bids[0] == 0 && status == 1 && winner == 0){
		bids[0] = amount[0];
	}
	return bids[0];
}

int __VERIFIER_atomic_place_bid2(){	
	if(bids[1] == 0 && status == 1 && winner == 0){
		bids[1] = amount[1];
	}
	return bids[1];
}



int __VERIFIER_atomic_close_auction() {
	if(status == 1 && winner == 0){
		if(bids[0] > 0 || bids [1] > 0){
			if(bids[0] > bids[1]){
				atomic_store_explicit(&status, 2 , memory_order_seq_cst);
				atomic_store_explicit(&winner, 1 , memory_order_seq_cst);
			}
			else{
				atomic_store_explicit(&status, 2 , memory_order_seq_cst);
				atomic_store_explicit(&winner, 2 , memory_order_seq_cst);
			}
		}
	}
	return winner;
}

void *thr1(void *arg){
	printf(" ");
 	b1 = __VERIFIER_atomic_place_bid1();
	return NULL;
}

void *thr2(void *arg){
	printf(" ");
 	b2 = __VERIFIER_atomic_place_bid2();
	return NULL;
}

void *thr(void *arg){
	printf(" ");
	w = __VERIFIER_atomic_close_auction();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t[clients];
	pthread_t t1;
	
	atomic_init(&winner,0);
	atomic_init(&status,1); //auction started

	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);
	pthread_create(&t1 ,NULL,thr, NULL);

	
	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);
	pthread_join(t1,NULL);

	printf("bid1=%d , bid2=%d , winner=%d \n", b1,b2,w);

	/*if( w > 0){
		if( w == 1){
			assert( b1 > b2);
		}
		if( w == 2 ){
			assert( b2 > b1);
		}
	}*/
	printf("\n");
	return 0;
}
