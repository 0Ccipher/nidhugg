#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define clients 6

int bids[clients] = {0,0,0,0,0};
int lbids[clients] = {0,0,0,0,0,0};
int token[clients] = {0,0,0,0,0,0};
int bid = -1;
int winner = 0;
int status = 0;
int amount[clients] = {100,105,110,90,115,120};
int w=0;
int max=0;

void __VERIFIER_atomic_place_bid0(){	
	if(bids[0] == 0 && status == 1 && winner == 0){
		bids[0] = amount[0];
		token[0] = 1;
	}
}
void __VERIFIER_atomic_place_bid1(){	
	if(bids[1] == 0 && status == 1 && winner == 0){
		bids[1] = amount[1];
		token[1] = 1;
	}
}
void __VERIFIER_atomic_place_bid2(){	
	if(bids[2] == 0 && status == 1 && winner == 0){
		bids[2] = amount[2];
		token[2] = 1;
	}
}
void __VERIFIER_atomic_place_bid3(){	
	if(bids[3] == 0 && status == 1 && winner == 0){
		bids[3] = amount[3];
		token[3] = 1;
	}
}
void __VERIFIER_atomic_place_bid4(){	
	if(bids[4] == 0 && status == 1 && winner == 0){
		bids[4] = amount[4];
		token[4] = 1;
	}
}
void __VERIFIER_atomic_place_bid5(){	
	if(bids[5] == 0 && status == 1 && winner == 0){
		bids[5] = amount[5];
		token[5] = 1;
	}
}
int __VERIFIER_atomic_close_auction() {
	for(int i=0 ; i < 6 ; i++){
		printf(" b[%d]=%d , t[%d]=%d ",i+1,bids[i],i+1,token[i]);
	}
	int count=0;
	for (int i = 0; i < 6; ++i) {
		if(token[i] == 1) count++;
	}
	max = bids[0];
	for(int i = 1 ; i < 6 ; i++){
		if( bids[i] > max)
			max = bids[i];
	}
	if(count < 6) max = 0;
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
