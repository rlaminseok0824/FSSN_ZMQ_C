#include "czmq.h"
#include <stdio.h>

int main(){
	printf("Conneccting to hello world server...\n");
	zsock_t *socket = zsock_new(ZMQ_REQ);
	zsock_connect (socket,"tcp://localhost:5555");

	for(int request = 0; request != 10;request++){
		printf("Sending Hello %d ...\n",request);
		zstr_send(socket,"Hello");
		char *message = zstr_recv(socket);
		printf("Received reply %d [ %s ]\n",request,message);
		zstr_free(&message);
	}
	zsock_destroy(&socket);

	return 0;
}
