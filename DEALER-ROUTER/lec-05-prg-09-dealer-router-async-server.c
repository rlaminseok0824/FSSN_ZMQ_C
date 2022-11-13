#include <stdio.h>
#include <czmq.h>
#include <zmq.h>
#include <stdlib.h>
#include <assert.h>

typedef struct ServerTask{
	int num_server;
	void (*start)(struct ServerTask *this,int num_server);
}ServerTask;

void ServerWorker(zsock_t *pipe,void * args);
void server_task(zsock_t *pipe,void * args);

void ServerTask_start(struct ServerTask * this,int num_server){
	zactor_t * server = zactor_new(server_task,(void *) num_server);
	zactor_destroy(&server);	
}
	

void server_task(zsock_t * pipe, void * args){
	 zsock_signal(pipe, 0);
	int num = (intptr_t)args;
	zactor_t * workers[num];
	for(int i = 0; i < num;i++){
		workers[i] = zactor_new(ServerWorker,(void *)i);
	}

	zactor_t * proxy = zactor_new(zproxy,NULL);
	zstr_sendx(proxy,"FRONTEND","ROUTER","tcp://*:5570",NULL);
	zsock_wait(proxy);
	zstr_sendx(proxy,"BACKEND","DEALER","inproc://backend",NULL);
	zsock_wait(proxy);

	zsock_wait(pipe);
	zactor_destroy(&proxy);
}

void ServerWorker(zsock_t *pipe,void * args){
 	zsock_signal(pipe, 0);	
	zsock_t * worker = zsock_new_dealer("inproc://backend");
	
	int id = (intptr_t)args;
	printf("Worker#%d started\n",id);
	
	zpoller_t * poller = zpoller_new(pipe,worker,NULL);
	zpoller_set_nonstop(poller,true);

	char * ident;
	char * content;
	while(1){
		zsock_t *ready = zpoller_wait (poller, -1);
        	if (ready == NULL) continue;
        	else if (ready == pipe) break;
        	else assert (ready == worker);

		zmsg_t * msg = zmsg_recv(worker);
		zframe_t * identity = zmsg_pop(msg);
		zframe_t * contents = zmsg_pop(msg);

		ident = zframe_strdup(identity);
		content = zframe_strdup(contents);
		printf("Worker#%d received %s from %s\n",id,content,ident);
		
		zmsg_destroy(&msg);
		
		zstr_sendm(worker,ident);
		zstr_send(worker,content);

		//zframe_send (&identity, worker, ZFRAME_REUSE + ZFRAME_MORE + ZFRAME_DONTWAIT);
            	//zframe_send (&contents, worker, ZFRAME_REUSE + ZFRAME_DONTWAIT);
		
		zframe_destroy(&identity);
		zframe_destroy(&contents);
	}
}

void ServerTask_init(struct ServerTask * this){
	this->start = ServerTask_start;
}

int main(int argc,char ** argv){
	ServerTask server;
	ServerTask_init(&server);
	int cnt = atoi(argv[1]);
	server.start(&server, cnt);

	return 0;
}
