#include <stdio.h>
#include <czmq.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int g_port_nameserver = 9001;
int g_port_chat_publisher = 9002;
int g_port_chat_collector = 9003;
int g_port_subscribe = 9004;

typedef struct p2pdb{
	char ip_address[32];
	char user_name[32];
}p2pdb;

int kbhit(void)
        {
          struct termios oldt, newt;
          int ch;
          int oldf;

          tcgetattr(STDIN_FILENO, &oldt);
          newt = oldt;
          newt.c_lflag &= ~(ICANON | ECHO);
          tcsetattr(STDIN_FILENO, TCSANOW, &newt);
          oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
          fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

          ch = getchar();

          tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
          fcntl(STDIN_FILENO, F_SETFL, oldf);

          if(ch != EOF)
          {
            ungetc(ch, stdin);
            return 1;
          }

          return 0;
	}

void relay_server_nameserver(zsock_t * pipe, void * local_ip_addr){
	zsock_signal(pipe, 0);

	char * local_ip = (char *) local_ip_addr;
	char serverip[32],serverip2[32];
	sprintf(serverip,"tcp://%s:%d",local_ip,g_port_chat_publisher);
	zsock_t * publisher = zsock_new_pub(serverip);
	sprintf(serverip2,"tcp://%s:%d",local_ip,g_port_chat_collector);
	zsock_t * collector = zsock_new_pull(serverip2);
	printf("local p2p relay server activated at tcp://%s:%d & %d.\n",local_ip,g_port_chat_publisher,g_port_chat_collector);
	
	while(1){
		if(kbhit())
			break;
		char * message = zstr_recv(collector);
		printf("p2p-relay:<==>%s\n",message);
		zstr_sendx(publisher,"RELAY",message,NULL);
		zstr_free(&message);
	}

	zsock_destroy(&publisher);
	zsock_destroy(&collector);
}


void user_manager_nameserver(zsock_t * pipe, void * local_ip_addr){
	zsock_signal(pipe,0);

	p2pdb user_db[10];
	int usr_cnt = 0;
	char * local_ip = (char *) local_ip_addr;
	char serverip[32];
	sprintf(serverip,"tcp://%s:%d",local_ip,g_port_subscribe);
	zsock_t * socket_rep = zsock_new_rep(serverip);
	printf("local p2p db server activated at %s\n",serverip);
	while(1){
		if(kbhit())
			break;
		char * rcv = zstr_recv(socket_rep);
		if(rcv != NULL){
			char * temp = strtok(rcv," ");
			for(int i = 0; i < strlen(temp);i++){
				user_db[usr_cnt].ip_address[i] = *(temp + i);
			}
			temp = strtok(NULL," ");
			for(int i = 0; i < strlen(temp);i++){
				user_db[usr_cnt].user_name[i] = *(temp + i);
			}
			printf("user registration '%s' from '%s'\n",user_db[usr_cnt].user_name,user_db[usr_cnt].ip_address);
			usr_cnt++;
			zstr_send(socket_rep,"ok");
		}
	}

	zsock_destroy(&socket_rep);
}

void beacon_nameserver(zsock_t * pipe,void * args){
	zsock_signal(pipe,0);

	char * local_ip_addr = (char *) args;
	char serverip[32];
	sprintf(serverip,"tcp://%s:%d",local_ip_addr,g_port_nameserver);
	zsock_t * socket = zsock_new_pub(serverip);

	printf("local p2p name server bind to %s.\n",serverip);
	while(1){
		if(kbhit())
			break;
		sleep(1);	
		char msg[32];
		sprintf(msg,"NAMESERVER:%s",local_ip_addr);
		zstr_sendx(socket,"NAMESERVER",local_ip_addr,NULL);
	}

	zsock_destroy(&socket);
}

char * search_nameserver(char * ip_mask,char * local_ip_addr,int port_nameserver){
	zsock_t * req = zsock_new(ZMQ_SUB);
	char target_ip_addr[32];
	for(int last = 1; last < 255;last++){
		sprintf(target_ip_addr,"tcp://%s.%d:%d",ip_mask,last,port_nameserver);
		if(target_ip_addr != local_ip_addr || target_ip_addr == local_ip_addr){
			int rc = zsock_connect(req,target_ip_addr);
		}
		zsock_set_rcvtimeo(req,2000);
		zsock_set_subscribe(req,"NAMESERVER");
	}
	
	char * filter;
	char * ip;
	if(zstr_recvx(req,&filter,&ip,NULL) != -1){
		if(strcmp(filter,"NAMESERVER") == 0){
			zsock_destroy(&req);
			return ip;
		}
	}

	zsock_destroy(&req);
	return NULL;
}

char * get_local_ip(){
	char hostbuffer[256];
	int hostname = gethostname(hostbuffer,sizeof(hostbuffer));
	struct hostent * host_entry = gethostbyname(hostbuffer);
	char * ipbuffer = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));

	return ipbuffer;
}

int main(int argc,char ** argv){
	if(argc == 1){
		printf("usage is './dechat _user-name'.\n");
		return 0;
	}
	
	printf("starting p2p chatting program.\n");
	char ip_addr_p2p_server[32];
	char * user_name = argv[1];
	char * ip_addr = get_local_ip();
	char * temp = rindex(ip_addr,'.');
	char ip_mask[32];

	srand((unsigned int)time(NULL));

	for(int i = 0; i < strlen(ip_addr) - strlen(temp);i++){
		ip_mask[i] = ip_addr[i];
	}	

	printf("searching for p2p server.\n");
	char * name_server_ip_addr = search_nameserver(ip_mask,ip_addr,g_port_nameserver);

	zactor_t * beacon_thread;
	zactor_t * db_thread;
	zactor_t * relay_thread;
	if(name_server_ip_addr == NULL){
		strcpy(ip_addr_p2p_server,ip_addr);
		printf("p2p server is not found, and p2p server mode is activated.\n");
		beacon_thread = zactor_new(beacon_nameserver,(void *)ip_addr);
		printf("p2p beacon server is actiivated.\n");
		db_thread = zactor_new(user_manager_nameserver,(void *)ip_addr);
		printf("p2p subscriber database server is activated.\n");
		relay_thread = zactor_new(relay_server_nameserver,(void *)ip_addr);
		printf("p2p message relay server is activated.\n");
	}
	else{
		strcpy(ip_addr_p2p_server,name_server_ip_addr);
		printf("p2p server found at %s, and p2p client mode is activated.\n",ip_addr_p2p_server);
	}
	
	printf("starting user registration procedure.\n");
	
	char ip_req[50];
	sprintf(ip_req,"tcp://%s:%d",ip_addr_p2p_server,g_port_subscribe);
	
	zsock_t * db_client_socket = zsock_new_req(ip_req);
	zstr_sendf(db_client_socket,"%s %s ",ip_addr, user_name);
	char * status = zstr_recv(db_client_socket);
	if(strcmp(status,"ok") == 0){
		printf("user registration to p2p server completed.\n");
	}
	else{
		printf("user registration to p2p server failed.\n");
	}
	printf("starting message transfer procedure.\n");
	
	char ip_sub[50];
	sprintf(ip_sub,"tcp://%s:%d",ip_addr_p2p_server,g_port_chat_publisher);
	zsock_t * p2p_rx = zsock_new_sub(ip_sub,"RELAY");
	
	char ip_push[50];
	sprintf(ip_push,"tcp://%s:%d",ip_addr_p2p_server,g_port_chat_collector);
	zsock_t * p2p_tx = zsock_new_push(ip_push);

	printf("starting autonomous message transmit and receive scenario.\n");
	
	zpoller_t * poller = zpoller_new(p2p_rx,NULL);

	while(1){
		if(kbhit())
			break;
		if(zpoller_wait(poller,100) == p2p_rx){
			char * filter;
			char * msg;
			zstr_recvx(p2p_rx,&filter,&msg,NULL);
			printf("p2p-recv::<<== %s\n",msg);
		}
		else{
			int random = (rand() % 100) + 1;
			if(random < 10){
				sleep(3);
				zstr_sendf(p2p_tx,"(%s,%s:ON)",user_name,ip_addr);
				printf("p2p-send::==>>(%s,%s:ON)\n",user_name,ip_addr);
			}
			else if(random > 90){
				sleep(3);
				zstr_sendf(p2p_tx,"(%s,%s:OFF)",user_name,ip_addr);
				printf("p2p-send::==>>(%s,%s:OFF)\n",user_name,ip_addr);
			}
		}
	}

	printf("closing p2p chatting program\n");

	zactor_destroy(&beacon_thread);
	zactor_destroy(&db_thread);
	zactor_destroy(&relay_thread);

	zsock_destroy(&db_client_socket);
	zsock_destroy(&p2p_rx);
	zsock_destroy(&p2p_tx);

	return 0;
}
