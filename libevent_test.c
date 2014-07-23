/*
 * =====================================================================================
 *
 *       Filename:  libevent_test.c
 *        Version:  1.0
 *        Created:  2014年07月21日 19时26分35秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  zomoe, xianszm007@gmail.com
 *        Company:  Class 1107 of Computer Science and Technology
 *
 * ====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <event.h>

#define LISTEN_PORT 9527
#define BACKLOG 20

typedef struct client{
	int sockfd;
	struct bufferevent *buffer_ev;
}client_t;

struct event_base *base;
int set_nonblocking(int fd){
	if(fcntl(fd,F_SETFL,
		fcntl(fd,F_GETFD,0)|O_NONBLOCK) == -1){
		return 1;
	}
	return 0;
}

void buf_read_callback(struct bufferevent *come_in,
			short what,
			void *arg){
	struct evbuffer *ev_ret;
	char *req;
	req=evbuffer_readline(come_in->input);
	if(req==NULL){
		return;
	}
	ev_ret=evbuffer_new();
	evbuffer_add_printf(ev_ret,"echo back : %s \n",req);
	bufferevent_write_buffer(come_in,ev_ret);
	evbuffer_free(ev_ret);
	free(req);
	
}

void buf_write_callbaack(){
	//...
}

void buf_error_callback(struct bufferevent *bev,
			short what,
			void *arg){
	client_t *p_client=(client_t*)arg;
	bufferevent_free(p_client->buffer_ev);
	close(p_client->sockfd);
	free(p_client);
}

void accept_callback(int sockfd,
			short ev,
			void *arg){
	int client_fd;
	struct sockaddr_in client_addr;
	socklen_t len=sizeof(client_addr);
	client_t *p_client;
	client_fd=accept(sockfd,
			(struct sockaddr*)&client_addr,
			&len);
	if(client_fd < 0){
		return ;
	}
	set_nonblocking(client_fd);
	p_client=(client_t*)malloc(sizeof(client_t));
	p_client->sockfd=client_fd;
	p_client->buffer_ev=bufferevent_new(client_fd,
						(void*)buf_read_callback,
						(void*)buf_write_callbaack,
						(void*)buf_error_callback,
						p_client);
	bufferevent_enable(p_client->buffer_ev,EV_READ);
}

int init_listen_socket(){
	int listenfd;
	int opt=1,flag=0;
	struct sockaddr_in local_addr;

	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family=AF_INET;
	local_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	local_addr.sin_port=htons(LISTEN_PORT);
	do{
		if((listenfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
			flag=1;
			break;
		}
		setsockopt(listenfd,
			SOL_SOCKET,
			SO_REUSEADDR,
			&opt,sizeof(opt)
			);
		if((bind(listenfd,(struct sockaddr*)&local_addr,sizeof(local_addr)) < 0)){
			flag=1;
			break;
		}
		if(listen(listenfd,BACKLOG) < 0){
			flag=1;
			break;
		}
	}while(0);
	if(flag==1){
		return -1;
	}
	listenfd=set_nonblocking(listenfd);
	return listenfd;

}

int main(int argc, char *argv[]){
	int listenfd=init_listen_socket();
	struct event accept_event;
	base=event_base_new();
	event_set(&accept_event,listenfd,
				EV_READ|EV_PERSIST,
				accept_callback,
				NULL
				);
	event_base_set(base,&accept_event);
	event_add(&accept_event,NULL);
	event_base_dispatch(base);
	close(listenfd);
	return 0;
}

