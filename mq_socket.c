/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <pthread.h>

#include "mq_types.h"
#include "mq_socket.h"

// socket issue !

#define BACKLOG     20
#define KEEPALIVE   1
#define LINGER      1
#define NODELAY     1
#define TIMEOUT     1


static inline int set_nonblocking(int fd){
    if(fcntl(fd,F_SETFL,fcntl(fd,F_GETFD,0)|O_NONBLOCK)==-1){
        return -1;
    }
    return 0;
}

static int set_socket_opt(int sockfd, unsigned int timeout){
    int result=0,flag=0;
    do{
        if (KEEPALIVE){
            //set tcp_keepAlive
            result = setsockopt(sockfd, SOL_SOCKET,\
                             SO_KEEPALIVE, &optval,\
                                (socklen_t) sizeof(int));
            if (-1 == result){
                flag=1;
                break;
            }
            result = setsockopt(sockfd, SOL_TCP, \
                                TCP_KEEPIDLE,\
                                 &timeout,\
                                 sizeof(timeout));
            if (-1 == result){
                flag=1;
                break;
            }
            int keepcnt = 3;
            result = setsockopt(sockfd, SOL_TCP, \
                                TCP_KEEPCNT, \
                                &keepcnt, \
                                sizeof(keepcnt));
            if (-1 == result){
                flag=1;
                break;
            }
            int keepintvl = 10;
            result = setsockopt(sockfd, SOL_TCP, \
                                TCP_KEEPINTVL, \
                                &keepintvl,\
                                sizeof(keepintvl));
            if (-1 == result){
                flag=1;
                break;
            }
        }
        if (LINGER){
            struct linger optval;
            optval.l_onoff = true;
            optval.l_linger = timeout;
            result = setsockopt(sockfd, SOL_SOCKET,\
                                 SO_LINGER, &optval,
                                (socklen_t) sizeof(struct linger)));
            if (-1 == result){
                flag=1;
                break;
            }
        }
        if (NODELAY){
            result = setsockopt(sockfd, SOL_TCP, \
                                TCP_NODELAY, &optval,\
                                 sizeof(optval));
            if (-1 == result){
                flag=1;
                break;
            }
            result = setsockopt(sock, SOL_SOCKET, \
                                SO_RCVTIMEO, &waittime,\
                                    sizeof(struct timeval));
            if (-1 == result){
                flag=1;
                break;
            }
        }
        if(TIMEOUT){
            struct timeval waittime;
            waittime.tv_sec = timeout;
            waittime.tv_usec = 0;
            result = setsockopt(sockfd, SOL_SOCKET,\
                                 SO_SNDTIMEO, &waittime,\
                                    sizeof(struct timeval));
            if (-1 == result){
                flag=1;
                break;
            }
        }
    }while(0);

    if (flag == 1){
        return -1;
    }
    return 0;
}

int sockfd_create(const char *ip_addr,int port, size_t timeout){
    int sockfd;
    int opt=1,flag=0;
    struct sockaddr_in socket_addr;
    do{
        memset(&socket_addr,0,sizeof(socket_addr));
        socket_addr.sin_family=AF_INET;
        if (NULL == ip || '\0' == *ip \
            || 0 == strlen(ip)){
            socket_addr->sin_addr.s_addr = htonl(INADDR_ANY);
        }else{
            if(0 == inet_aton(ip_addr, &(socket_addr->sin_addr))){
                close(sockfd);
                flag=1;
                break;
            }
        }
        socket_addr.sin_addr.s_addr=htonl(INADDR_ANY);
        socket_addr.sin_port=htons(port);
        if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0){
            flag=1;
            break;
        }
        if(setsockopt(sockfd,
                        SOL_SOCKET,
                        SO_REUSEADDR,
                        &opt,sizeof(opt)
                            )==-1){
            flag=1;
            break;
        }

    }while(0);

    if(flag==1){
        return -1;
    }
    return sockfd;
}

int client_socket_init(const char *ip_addr, int port, size_t timeout){

}

int server_socket_init(const char *ip_addr, int port, size_t timeout){

}
