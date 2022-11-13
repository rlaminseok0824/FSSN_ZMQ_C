#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "czmq.h"

int main(int argc,char ** argv){
	printf("Collecting updates from weather server...\n");	
	char* zip_filter;
	if(argc > 1){
		zip_filter = argv[1];
	}
	else{
		zip_filter = "10001";
	}
	zsock_t *socket = zsock_new_sub("tcp://localhost:5556",zip_filter);
	assert(socket);

	int total_temp = 0;
	
	//char *string;
	//char *stringAfSlice = NULL;
	char * zipcode;
	char * temp;
	char * relhum;
	int update_nbr;
	for(update_nbr = 0; update_nbr < 20; update_nbr++){
		//string = zstr_recv(socket);
		zsock_recv(socket,"sss",&zipcode,&temp,&relhum);
		//char* temp = strtok_r(string,",",&stringAfSlice);
		//temp = strtok_r(NULL,",",&stringAfSlice);
		int temperature = atoi(temp);
		total_temp += temperature;
		printf("Receive temperature for zipcode '%s' was %d F\n",zip_filter,temperature);
		//zstr_free(&string);
		free(zipcode);
		free(temp);
		free(relhum);
	}

	printf("Average temperature for zipcode '%s' was %d F\n",zip_filter,total_temp / (update_nbr + 1));
	zsock_destroy(&socket);
	return 0;
}
