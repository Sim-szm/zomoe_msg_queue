/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */

#include <sys/poll.h>
#include <sys/select.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <string.h>
#include <pthread.h>
#include <stddef.h>

#include "mq_types.h"
#include "mq_thread_wrapper.h"

int thread_kill(pthread_t pid){
    return pthread_kill(pid,SIGINT);
}

int thread_detach(pthread_attr_t *thread_attr, int isDetach){
    assert(thread_attr!=NULL);
    if(isDetach){
        return pthread_attr_setdetachstate(thread_attr, \
                                PTHREAD_CREATE_DETACHED);
    }else{
        return pthread_attr_setdetachstate(thread_attr, \
                                PTHREAD_CREATE_JOINABLE);
    }
    return 0;
}

int thread_mutex_init(pthread_mutex_t *mutex){
    int ret=0;
    pthread_mutexattr_t mutex_attr;
    if((ret=pthread_mutexattr_init(&mutex_attr))!=0){
        return ret;
    }
    do{
        if((ret=pthread_mutexattr_setpshared(&mutex_attr,\
                            PTHREAD_PROCESS_PRIVATE))!=0){  //设置mutex是否进程间共享
            break;
        }
        if((ret=pthread_mutex_init(mutex,&mutex_attr))!=0){
            break;
        }
    }while(0);
    ret=pthread_mutexattr_destroy(&mutex_attr);
    return ret;
}

int thread_cond_init(pthread_cond_t *cond){
    int ret=0;
    pthread_condattr_t cond_attr;
    if((ret=pthread_condattr_init(&cond_attr))!=0){
        return ret;
    }
    do{
        if((ret=pthread_condattr_setpshared(&cond_attr,\
                            PTHREAD_PROCESS_PRIVATE))!=0){  //设置mutex是否进程间共享
            break;
        }
        if((ret=pthread_cond_init(cond,&cond_attr))!=0){
            break;
        }
    }while(0);
    ret=pthread_condattr_destroy(&cond_attr);
    return ret;
}

int thread_create(void *(*callback)(void *),void *arg){
    pthread_t pid;
    if(pthread_create(&pid,NULL,callback,arg)<0){
        return -1;
    }
    return 0;
}
