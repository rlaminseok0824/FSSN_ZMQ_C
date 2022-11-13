#include <stdio.h>
#include <czmq.h>
#include <time.h>

typedef struct ClientTask{
	void (*start)(struct ClientTask * this,char * id);
}ClientTask;

void client_task(zsock_t * pipe,void * args){
//zsock_t * socket = zsock_new_dealer("tcp://localhost:5570");

	char * id = (char *)args;
	zsock_t * socket = zsock_new(ZMQ_DEALER);
	zsock_set_identity(socket,id);
	zsock_connect(socket,"tcp://localhost:5570");
	printf("Client %s started\n",id);

	zpoller_t * poller = zpoller_new(socket,NULL); 
	zpoller_set_nonstop(poller, true);
	
	int reqs = 0;
	while(1){
		reqs++;
		printf("Req #%d sent..\n",reqs);
		
		zstr_sendf(socket,"request #%d",reqs);

		sleep(1);
			
		if(zpoller_wait(poller,1000) != NULL){
			char * str = zstr_recv(socket);
			printf("%s received: %s\n",id,str);	
		}


		//sprintf(msg,"request #%d",reqs);

		//zmsg_t * message = zmsg_new();
		
		//zmsg_addmem(message,msg,15);
		//zmsg_addmem(message,id,10);
		
		//zmsg_send(&message,socket);


		//zsock_t * ready = zpoller_wait(poller,-1);

//		if(poller != NULL){
//			zmsg_t * recvmsg = zmsg_recv (socket);
//			zmsg_print(recvmsg);
//            		//zframe_t * contents = zmsg_first(recvmsg);
//			//zframe_t * identity = zmsg_last(recvmsg);
//			//char * ident = zframe_strdup(identity);
//			//char * content = zframe_strdup(contents);
//			//printf("%s received: %s\n",ident,content);
//		}
	}

}



void ClientTask_start(struct ClientTask * this,char * id){
	zactor_t * client = zactor_new(client_task,(void *)id);
	//zactor_destroy(&client);
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
