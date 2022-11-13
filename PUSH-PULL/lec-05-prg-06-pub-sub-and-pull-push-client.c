#include <stdio.h>
#include <time.h>
#include <czmq.h>
#include <zmq.h>
#include <unistd.h>
#include <assert.h>

int main(){
	zsock_t * subscriber = zsock_new_sub("tcp://localhost:5557","");
	assert(subscriber);
	zsock_t * publisher = zsock_new_push("tcp://localhost:5558");
	assert(publisher);

	zpoller_t * poller = zpoller_new(subscriber,NULL);
		
	srand((unsigned int)time(NULL));

	while(1){
		if(zpoller_wait(poller,100) != NULL & ZMQ_POLLIN){
			int message;
			zsock_recv(subscriber,"i",&message);
			printf("I: received message %d\n",message);
		}
		else{
			int random = (rand() % 100) + 1;
			zsock_send(publisher,"i",random);
			printf("I: sending message %d\n",random);		
		}
	}

	return 0;
}
