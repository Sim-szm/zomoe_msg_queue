/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_NOTIFY_H_
#define MQ_NOTIFY_H_

#ifdef __cplusplus
extern "C":
{
#endif

#include <stdlib.h>
#include <stdio.h>
#include <event.h>
#include <unistd.h>
#include <pthread.h>


typedef void (*notify_read_callback)(int sockfd,short event,void *arg);

typedef struct notify{
    struct event_base *ev_base;
    pthread_t tid;
    int pipe_fd[2];
    int index;
    notify_read_callback func;
}notify_t;

notify_t *g_notify_elts;

int notifypool_init(size_t max_size,notify_read_callback func);
int notifypool_destroy();

#ifdef __cplusplus
}
#endif
#endif
