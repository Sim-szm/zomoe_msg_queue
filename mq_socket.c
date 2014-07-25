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

int sockfd_create(const char *ip_addr,int port, struct sockaddr_in *socket_addr,size_t timeout){
    int sockfd;
    int opt=1,flag=0;
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

    if(flag==1 || -1==set_socket_opt(sockfd,timeout) ){
        return -1;
    }
    return sockfd;
}

int client_socket_init(const char *ip_addr, \
                        int port, size_t timeout){
    struct sockaddr_in server_t;
    sockfd=sockfd_create(ip_addr,port,&server_t,timeout);
    if(-1==connect(sockfd,&server_t,sizeof(server_t))){
        return -1;
    }
    set_nonblocking(sockfd); //enable nonblocking!
}

int server_socket_init(const char *ip_addr, \
                        int port, size_t timeout){
    struct sockaddr_in server_t;
    int flag=0;
    do{
        sockfd=sockfd_create(ip_addr,port,\
                            &server_t,timeout);
        if(sockfd<0){
            flag=1;
            break;
        }
        if(-1==bind(sockfd, (struct sockaddr *) &ip_addr,\
                            sizeof(struct sockaddr_in))){
            close(sockfd);
            flag=1;
            break;

        }
        if(-1==listen(sockfd, BACKLOG)){
            close(sockfd);
            flag=1;
            break;
        }
    }while(0);

    if(flag == 1){
        return -1;
    }
    return sockfd;
}

int do_accept_issue(int listenfd){
    while(true){
        if(!isLoop)
            break;
        do{
            struct sockaddr_in client_addr;
            memset(&client_addr,0,sizeof(client_addr));
            socklen_t addrlen=sizeof(client_addr);

#if NOT_USE_NONBLOCK

            int connfd=accept(m_sockfd,\
                (struct sockaddr *)&client_addr,&addrlen);
            set_nonblocking(connfd);
#else

            int connfd=accept4(m_sockfd,(struct sockaddr*)&client_addr,\
                        &addrlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
#endif

            if (connfd < 0){
                //log_error ...
            }else if(connfd > 0){
                if(EINTR == errno || EAGAIN == errno){
                        continue;
                }
                    continue;
            }else{
                //log_error ...
                    continue;
            }
            char client[IP_LEN + 1 + 10];
            memset(client,'\0',IP_LEN + 1 + 10);
            snprintf(client,IP_LEN + 1 + 10,"%s 's port: %d",\
            inet_ntoa(client_addr.sin_addr),\
            ntohs(client_addr.sin_port));

            //   here needs to notify a process or thread !
            /*
             *   needs a struct to manage processes or threads,not finish yet !
             *   if error return -1
             */
        }while(false);
    }
    return 0;
}
