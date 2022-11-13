#include "czmq.h"
#include<unistd.h>
#include<stdio.h>


int main(){
	zsock_t *socket = zsock_new(ZMQ_REP);
	zsock_bind(socket,"tcp://*:5555");

	while(1){
		char *message = zstr_recv(socket);
		printf("Received request: %s\n",message);
		sleep(1);
		zstr_send(socket,"World");
		zstr_free(&message);
	}	
	return 0;
}
