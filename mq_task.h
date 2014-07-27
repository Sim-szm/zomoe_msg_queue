/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_TASK_H_
#define MQ_TASK_H_

#ifdef __cplusplus
extern "C":
{
#endif

#include <event.h>
#include <pthread.h>
#include <stddef.h>
#include <time.h>
#include "mq_types.h"
#include "mq_msg.h"

typedef void (*header_handler)(void *);
typedef void (*body_handler)(void *);

typedef struct task{
	int sockfd;
	int64 offset;
	pthread_mutex_t mutex;
	struct event_base *event_base;
	struct event req;
	struct event resq;
	struct timeval timeout;
	header_t req_head;
	header_t reqs_head;
	session_t session_type;
	char req_head_buffer[HEADERLEN+1];
	char *req_body_buffer;
	char reqs_head_buffer[HEADERLEN+1];
	char *reqs_body_buffer;
	header_handler header_h;
	body_handler body_h;
	struct task *next;
}task_t;


#ifdef __cplusplus
}
#endif
#endif
