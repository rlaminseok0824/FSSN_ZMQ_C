#include <stdio.h>
#include <czmq.h>
#include <time.h>
#include <pthread.h>

typedef struct rcvip{
	zsock_t * _sock;
	char * _id;
}rcvip;

typedef struct ClientTask{
	void (*start)(struct ClientTask * this,char * id);
}ClientTask;

void * rcv(void * arg){
	rcvip * data = (rcvip *)arg;
	zsock_t * sock = data->_sock;
	char * id = data->_id;
	zpoller_t * poller = zpoller_new(sock,NULL);
	zpoller_set_nonstop(poller,true);
	while(1){
		if(zpoller_wait(poller,1000) != NULL){
			char * str = zstr_recv(sock);
			printf("%s received: %s\n",id,str);	
		}
	}

	pthread_exit(0);
	return NULL;
}

void client_task(zsock_t * pipe,void * args){
	char * id = (char *)args;
	zsock_t * socket = zsock_new(ZMQ_DEALER);
	zsock_set_identity(socket,id);
	zsock_connect(socket,"tcp://localhost:5570");
	printf("Client %s started\n",id);
	
	rcvip ip;
	ip._sock = socket;
	ip._id = id;

	pthread_t rcv_thread;
	pthread_create(&rcv_thread,NULL,rcv,(void *)&ip);

	int reqs = 0;
	while(1){
		reqs++;
		printf("Req #%d sent..\n",reqs);
		
		zstr_sendf(socket,"request #%d",reqs);

		sleep(1);

	}

}


void ClientTask_start(struct ClientTask * this,char * id){
	zactor_t * client = zactor_new(client_task,(void *)id);
	zactor_destroy(&client);
}

void ClientTask_init(struct ClientTask * this){
	this->start = ClientTask_start;
}


int main(int argc,char ** argv){
	ClientTask client;
	ClientTask_init(&client);
	client.start(&client,argv[1]);
	return 0;
}
