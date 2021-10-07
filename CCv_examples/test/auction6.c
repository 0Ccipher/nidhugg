#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define clients 6

int bids[clients] = {0,0,0,0,0};
int lbids[clients] = {0,0,0,0,0,0};
int bid = -1;
int winner = 0;
int status = 0;
int amount[clients] = {100,105,110,90,115,120};
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
void __VERIFIER_atomic_place_bid3(){	
	if(bids[3] == 0 && status == 1 && winner == 0){
		bids[3] = amount[3];
	}
}
void __VERIFIER_atomic_place_bid4(){	
	if(bids[4] == 0 && status == 1 && winner == 0){
		bids[4] = amount[4];
	}
}
void __VERIFIER_atomic_place_bid5(){	
	if(bids[5] == 0 && status == 1 && winner == 0){
		bids[5] = amount[5];
	}
}
int __VERIFIER_atomic_close_auction() {
	//lbids[0] = 0;
	//lbids[2] =bids[2];
	printf(" b1=%d b2=%d b3=%d " , bids[0],bids[1],bids[2]);
	printf(" b4=%d b5=%d b6=%d " , bids[3],bids[4],bids[5]);
	max = bids[0];
	/*if(bids[1] > max) {
		max = bids[1];
	}
	if(bids[2] > max) {
		max = bids[2];
	}*/
	for(int i = 1 ; i < 6 ; i++){
		if( bids[i] > max)
			max = bids[i];
	}
	return max;
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
void *thr4(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid3();
	return NULL;
}
void *thr5(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid4();
	return NULL;
}
void *thr6(void *arg){
	printf(" ");
 	__VERIFIER_atomic_place_bid5();
	return NULL;
}
void *thr(void *arg){
	printf(" ");
	__VERIFIER_atomic_start_auction();
	w = __VERIFIER_atomic_close_auction();
	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t t[clients];
	pthread_t t1;
	pthread_create(&t[0],NULL,thr1,NULL);
	pthread_create(&t[1],NULL,thr2,NULL);
	pthread_create(&t[2],NULL,thr3,NULL);
	pthread_create(&t[3],NULL,thr4,NULL);
	pthread_create(&t[4],NULL,thr5,NULL);
	pthread_create(&t[5],NULL,thr6,NULL);
	pthread_create(&t1 ,NULL,thr, NULL);
	pthread_join(t[0],NULL);
	pthread_join(t[1],NULL);
	pthread_join(t[2],NULL);
	pthread_join(t[3],NULL);
	pthread_join(t[4],NULL);
	pthread_join(t[5],NULL);
	pthread_join(t1,NULL);
	printf("winner =%d " , w);
	printf("\n");
	return 0;
}
