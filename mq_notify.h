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
    pthread_t pid;
    int pipe_fd[2];
    int index;
    notify_read_callback func;
}notify_t;

notify_t *p_notify_t;

int notify_pool_init(size_t m_size,notify_read_callback callbackfunc);
int notify_pool_destroy();

int notify_event_init(notify_t *p_notify);
void notify_read_event(int sockfd,short event,void *arg);

int work_thread_init(size_t size,size_t stack_size);

#ifdef __cplusplus
}
#endif
#endif
