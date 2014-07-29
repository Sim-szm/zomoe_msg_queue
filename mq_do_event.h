/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_DO_EVENT_H_
#define MQ_DO_EVENT_H_

#ifdef __cplusplus
extern "C":
{
#endif

#include <stdio.h>
#include <stdlib.h>

#include "mq_task.h"

void client_req_callback(int sockfd, short event, void *arg);
void client_resp_callback(int sockfd, short event, void *arg);

void client_work(task_t *task);

void server_req_callback(int sockfd, short event, void *arg);
void server_resp_callback(int sockfd, short event, void *arg);

void server_work(task_t *task);

#ifdef __cplusplus
}
#endif
#endif
