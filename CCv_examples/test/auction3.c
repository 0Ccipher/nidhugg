#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define clients 3

int bids[3] = {0,0,0};
int lbids[3] = {0,0,0};
int bid = -1;
int winner = 0;
int status = 1;
int amount[3] = {100,105,110};
int w=0;
int max=0;

void __VERIFIER_atomic_place_bid0(){	
	if(bids[0] == 0 && status == 1 && winner == 0){
		bids[0] = amount[0];
	}
}
void __VERIFIER_atomic_place_bid1(){	
	if(bids[1] == 0 && status == 1 && winner == 0){
		bids[1] = amount[1];
	}
}
void __VERIFIER_atomic_place_bid2(){	
	if(bids[2] == 0 && status == 1 && winner == 0){
		bids[2] = amount[2];
	}
}

int __VERIFIER_atomic_close_auction() {
	//lbids[0] = 0;
	//lbids[2] =bids[2];
	printf(" b1=%d b2=%d b3=%d " , bids[0],bids[1],bids[2]);
	max = bids[0];
	/*if(bids[1] > max) {
		max = bids[1];
	}
	if(bids[2] > max) {
		max = bids[2];
	}*/
	for(int i = 1 ; i < 3 ; i++){
		if( bids[i] > max)
			max = bids[i];
	}
	status = 2;
	winner = max;
	return winner;
}
void __VERIFIER_atomic_start_auction(){
	status = 1;
	winner = 0;
}
void *thr1(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid0();
	return NULL;
}
void *thr2(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid1();
	return NULL;
}
void *thr3(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid2();
	return NULL;
}
void *thr(void *arg){
	printf(" ");
	//__VERIFIER_atomic_start_auction();
	w = __VERIFIER_atomic_close_auction();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t[clients];
	pthread_t t1;
	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);
	pthread_create(&t[2],NULL,thr3,NULL);
	pthread_create(&t1 ,NULL,thr, NULL);
	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);
	pthread_join(t[2],NULL);
	pthread_join(t1,NULL);
	printf("winner =%d " , w);
	printf("\n");
	return 0;
}
