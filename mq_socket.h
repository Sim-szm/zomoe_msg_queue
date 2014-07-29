/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_SOCKET_H_
#define MQ_SOCKET_H_

#ifdef __cplusplus
extern "C":
{
#endif
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>
#include "mq_types.h"

// socket issue !

#define BACKLOG     20
#define KEEPALIVE   1
#define LINGER      1
#define NODELAY     1
#define TIMEOUT     0
#define NOT_USE_NONBLOCK 0

//enable non_blocking as default !

int set_nonblocking(int fd);
int sockfd_create(const char *ip_addr,int port,struct sockaddr_in *socket_addr, size_t timeout);
int client_socket_init(const char *ip_addr, int port, size_t timeout);
int server_socket_init(const char *ip_addr, int port, size_t timeout);
int do_accept_issue(int listenfd);



#ifdef __cplusplus
}
#endif
#endif
