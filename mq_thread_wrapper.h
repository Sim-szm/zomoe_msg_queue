/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_THREAD_WRAPPER_H_
#define MQ_THREAD_WRAPPER_H_

#ifdef __cplusplus
extern "C":
{
#endif

int thread_kill(pthread_t pid);

int thread_detach(pthread_attr_t *thread_attr, bool isDetach);

int thread_mutex_init(pthread_mutex_t *mutex);

int thread_cond_init(pthread_cond_t *cond);

int thread_create(void *(*callback)(void *),void *arg);

#ifdef __cplusplus
}
#endif
#endif
