#include <stdio.h>
#include <time.h>
#include <czmq.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(int argc,char ** argv){
	zsock_t * subscriber = zsock_new_sub("tcp://localhost:5557","");
	assert(subscriber);
	zsock_t * publisher = zsock_new_push("tcp://localhost:5558");
	assert(publisher);
	zpoller_t * poller = zpoller_new(subscriber,NULL);

	char * clientID = argv[1];

	char msg[100];
	srand((unsigned int)time(NULL));
	while(1){
		if(zpoller_wait(poller,100)){
			char * message;
			zsock_recv(subscriber,"s",&message);
			printf("%s: receive status => %s\n",clientID,message);
			free(message);
		}
		else{
			int random = (rand() % 100) + 1;
			if(random < 10){
				sleep(1);
				sprintf(msg,"(%s:ON)",clientID);
				zsock_send(publisher,"s",msg);
				printf("%s: send status - activated\n",clientID);
			}
			else if(random > 90){
				sleep(1);
				sprintf(msg,"(%s:OFF)",clientID);
				zsock_send(publisher,"s",msg);
				printf("%s: send status - deactivated\n",clientID);
			}
		}
	}

	return 0;
}
