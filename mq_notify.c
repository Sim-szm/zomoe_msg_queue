/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */

#include <event.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <string.h>

#include "mq_types.h"
#include "mq_notify.h"
#include "mq_socket.h"

static int STACK_SIZE = 8;

int notify_pool_init(size_t m_size,notify_read_callback callbackfunc){
    int ret=0;
    assert(m_size!=0);
    p_notify_t=(notify_t*)calloc(m_size,sizeof(notify_t));
    assert(p_notify!=NULL);
    do{
        for(int i=0;i<=m_size;i++){
            (p_notify_t+i)->ev_base=event_base_new();
            (p_notify_t+i)->pid=0;
            (p_notify_t+i)->index=0;
            if(pipe((p_notify_t+i)->pipe_fd)==-1){
                ret=-1;
                break;
            }
            (p_notify_t+i)->func=callbackfunc;
        }
    }while(0);
    if(ret==-1){
        free(p_notify_t);
    }
    return ret;
}

int notify_pool_destroy(){
    free(p_notify_t);
    p_notify_t=NULL;
    return 0;
}

int notify_event_init(notify_t *p_notify){
    int ret=0;
    assert(p_notify!=NULL);
    set_nonblocking(p_notify->pipe_fd[0]);
    struct event event_notify;
    do{
        if(event_set(&event_notify,\
                    p_notify->pipe_fd[0],\
                    EV_READ|EV_PERSIST,\
                    p_notify->func,\
                    &(p_notify->index))!=0){
            ret=-1;
            break;
        }
        if(event_add(&event_notify,NULL)!=0){
            ret=-1;
            break;
        }
        while(isLoop){
            event_base_loop(p_notify->ev_base,0);
        }

    }while(0);
    return ret;
}

void notify_read_event(int sockfd,short event,void *arg){
    assert(arg!=NULL || *((int*)arg) >0);
    int index=*((int*)arg);
    int client_sockfd,ret;
    while(isLoop){
        if((ret=read(sockfd,&client_sockfd,\
                sizeof(client_sockfd))) > 0){
            if(!(EAGAIN ==errno || EWOULDBLOCK == errno)){
                //log_error
            }
            break;
        }else if(ret==0){
            break;
        }

        // here should be some_func to manage task
        //.....not end!
    }
}

static void *notify_thread_issue(void *arg)
{
    assert(arg!=NULL);
    notify_t *p_notify = (notify_t *)arg;
    if(notify_event_init(p_notify)!=0){
        return NULL;
    }
    return NULL;
}

int init_thread(notify_t* p_notify){
    int ret=0;
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    do{
        if(pthread_attr_setstacksize(&thread_attr,\
                         STACK_SIZE * 1024 * 1024)!=0){
            ret=-1;
            break;
        }
        if ((ret=pthread_create(&(p_notify->pid), &thread_attr,\
                         notify_thread_issue, p_notify))!=0){
            ret = -1;
            break;
        }
    }while(0);
    pthread_attr_destroy(&thread_attr);
    return ret;
}

int work_thread_init(size_t size,size_t stack_size){
    int ret=0;
    assert(size!=0 || stack_size!=0);
    STACK_SIZE=stack_size;
    for(int i=0;i<size;i++){
        notify_t *p_notify=p_notify_t+i;
        if(p_notify==NULL){
            ret=-1;
            break;
        }
        if(init_thread(p_notify)==-1){
            ret=-1;
            break;
        }

    }
    return ret;
}
