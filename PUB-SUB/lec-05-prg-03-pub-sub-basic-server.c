#include <stdio.h>
#include <stdlib.h>
#include "czmq.h"
#include <time.h>
#include <unistd.h>
#include <assert.h>

int main(){
	printf("Publishing updates at weather server...\n");
	zsock_t *socket = zsock_new_pub("tcp://*:5556");
	assert(socket);

	srand((unsigned int)time(NULL));
	//char weather[200];
	char s_zipcode[10];
	char s_temp[5];
	char s_relhum[3];
	while(1){
		int zipcode = (rand() % 100000) + 1;
		int temperature = (rand() % 215) - 80;
		int relhumidity = (rand() % 50) + 10;
		//sprintf(weather,"%d,%d,%d",zipcode,temperature,relhumidity);
		sprintf(s_zipcode,"%d",zipcode);
		sprintf(s_temp,"%d",temperature);
		sprintf(s_relhum,"%d",relhumidity);
		zsock_send(socket,"sss",s_zipcode,s_temp,s_relhum);
		
		sleep(0.01);
		//zstr_send(socket,weather);
		//
		//zstr_send로 보내면 하나의 string으로 보내기 때문에 client 측에서 일련의 split작업을 해주어야하지만
		//zsock_send를 사용하면 자기가 보내는 string의 정보를 명시할 수 있기 때문에 client 측에서 slice 없이
		//바로 사용가능!!
	}	
	
	zsock_destroy(&socket);	
	
	return 0;
}
