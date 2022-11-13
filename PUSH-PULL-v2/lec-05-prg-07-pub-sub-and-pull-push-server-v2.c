#include <stdio.h>
#include <czmq.h>
#include <assert.h>

int main(){
	zsock_t * publisher = zsock_new_pub("tcp://*:5557");
	zsock_t * collector = zsock_new_pull("tcp://*:5558");
	
	char * message;
	while(1){
		zsock_recv(collector,"s",&message);
		printf("server: publishing update => %s\n",message);
		zsock_send(publisher,"s",message);
	}
}
