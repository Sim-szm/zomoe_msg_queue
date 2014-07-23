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

//enable non_blocking

int set_nonblocking(int fd);
int sockfd_create(const char *ip_addr,int port, size_t timeout);
int client_socket_init(const char *ip_addr, int port, size_t timeout);
int server_socket_init(const char *ip_addr, int port, size_t timeout);
//int accept_callback(int sockfd,....);



#ifdef __cplusplus
}
#endif
#endif
