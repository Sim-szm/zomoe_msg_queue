/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#include <pthread.h>
#include <event.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>

#include "mq_msg.h"
#include "mq_socket.h"
#include "mq_do_event.h"
#include "mq_types.h"

void client_req_callback(int sockfd, short event, void *arg){
    assert(arg!=NULL);

    task_t *task = (task_t *)arg;
    if(event == EV_TIMEOUT)
        return ;

    int result = 0;

    while(header == type_t){
        if (-1 == (result = send(task->sockfd,\
             (task->req_head_buffer + task->offset), \
                    (HEADERLEN - task->offset), 0))){
            if (EAGAIN == errno || EWOULDBLOCK == errno){
                if (0 != event_add(&(task->req),\
                                     &(task->timeout))){
                    return;
                }
            }else{
                return;
            }
            return;
        }

        task->offset += result;

        if (HEADERLEN > task->offset){
            if (0 != event_add(&(task->req), \
                                &(task->timeout))){
                //log_error
                return;
            }
        }else if (HEADERLEN == task->offset){
            if (task->req_head.bodylen ==0 || \
                    NULL == task->req_body_buffer || \
                    strlen(task->req_body_buffer) ==0){
                return;
            }
            task->session_type = body;
            task->offset = 0;
            break;
        }
    }
    while(body == type_t){
        if (0 == task->req_head.bodylen || \
                NULL == task->req_body_buffer || \
                0 == strlen(task->req_body_buffer)){
            return;
        }

        if (-1 == (result = send(task->sockfd,\
                (task->req_body_buffer + task->offset), \
                    (task->req_head.bodylen - task->offset), 0))){
            if (EAGAIN == errno || EWOULDBLOCK == errno){
                if (0 != event_add(&(task->req), &(task->timeout))){
                    return;
                }
            } else{
                return;
            }
        }

        task->offset += result;
        if (task->req_head.bodylen > task->offset){
            if (0 != event_add(&(task->req), &(task->timeout))){
                return;
            }
        }
        if (task->req_head.bodylen == task->offset){
            return;
        }
    }

}

void client_resp_callback(int sockfd, short event, void *arg){
    assert(arg != NULL);
    task_t *task = (task_t *) arg;

    if (EV_TIMEOUT == event && 0 != task->sockfd){
        if (0 != event_add(&(task->resp), &(task->timeout))){
            //log_error
        }
        return;
    }

    int bytes = 0;
    while (header == task->session_type){
        bytes = recv(task->sockfd, task->resp_head_buffer + task->offset,
                            HEADERLEN - task->offset, 0);
        if (0 > bytes){
            if (errno == EAGAIN || errno == EWOULDBLOCK){
                if (0 != event_add(&(task->resp), &(task->timeout))){
                    //log_error
                    return;
                }
            }else{
                //log_error
                return;
            }
            return;
        }

        if (HEADERLEN > bytes + task->offset){
            task->offset += bytes;
            if (0 != event_add(&(task->resp), &(task->timeout))){
                //log_error
                return;
            }
        }else{
            task->session_type = body;
            task->header_h(task);

            if(0 != task->resp_head.bodylen){
                task->resp_body_buffer = (char *)malloc(task->resp_head.bodylen);
                if (NULL == task->resp_body_buffer){
                    //log_error
                    return;
                }
                task->offset = 0;//set recv body buffer offset to 0
            }
            break;
        }
    }

    while (task->resp_head.bodylen !=0 && \
                                    body == task->session_type){
        bytes = recv(task->sockfd, task->resp_body_buffer + task->offset,
                        task->resp_head.bodylen - task->offset, 0);
        if (0 > bytes){
            if (errno == EAGAIN || errno == EWOULDBLOCK){
                if (0 != event_add(&(task->resp), &(task->timeout))){
                    //log_error
                    return;
                }
            } else{
                //log_error
                return;
            }
            return;
        }

        if (task->resp_head.bodylen - task->offset > bytes){
            task->offset += bytes;
            if (0 != event_add(&(task->resp), &(task->timeout))){
                //log_error
                return;
            }
        }else{
            task->session_type = unspecified;
            *(task->resp_body_buffer + task->resp_head.bodylen) = '\0';
            break;
        }
    }
    task->body_h(task);
    return;
}

void client_work(task_t *task){
    assert(task != NULL);
    assert(task->sockfd > 0);

    set_nonblocking(task->sockfd);

    do{
        task->session_type = header;
        event_set(&(task->req_handler), task->sockfd, \
                    EV_WRITE, client_req_callback,(void *) task);

        if (event_base_set(task->event_base, &(task->req))==-1){
            task->session_type = unspecified;
            break;
        }

        event_set(&(task->resp), task->sockfd, \
                    EV_READ, client_resp_callback,(void *)task);

        if (event_base_set(task->event_base, &(task->resp))!=0){
            task->session_type = unspecified;
            break;
        }

        if (event_add(&(task->req), &(task->timeout))!=0){
            task->session_type = unspecified;
            break;
        }

        event_base_loop(task->event_base, 0);

    } while (0);

    return;
}

void server_req_callback(int sockfd, short event, void *arg){

}

void server_resp_callback(int sockfd, short event, void *arg){

}

void server_work(task_t *task){

}


