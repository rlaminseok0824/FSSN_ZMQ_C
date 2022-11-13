#include <stdio.h>
#include <czmq.h>

int main(){
	zsock_t * publisher = zsock_new_pub("tcp://*:5557");
	zsock_t * collector = zsock_new_pull("tcp://*:5558");
	
	int recvData;
	while(1){
		zsock_recv(collector,"i",&recvData);
		printf("I: publishing update %d\n", recvData);
		zsock_send(publisher,"i",recvData);
		//free(recvData);
	}

	return 0;
}
