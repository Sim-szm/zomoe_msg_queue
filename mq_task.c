/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <string.h>

#include "mq_task.h"
#include "mq_thread_wrapper.h"
int task_pool_init(const int pool_size,const size_t timeout,\
			header_handler header_h,body_handler body_h){
	int ret=0,i;
	struct timeval time;
	time.tv_sec=timeout;
	time.tv_usec=0;
	assert(pool_size!=0);
	p_task=(task_pool_t *)malloc(sizeof(task_pool_t));
	assert(p_task!=NULL);
	do{
		if((thread_mutex_init(&(p_task->mutex)))!=0){
			ret=-1;
			break;
		}
		p_task->p_list=(task_t *)malloc(sizeof(task_t));
		assert(p_task->p_list != NULL);
		do{
			task_t *cur=NULL;
			for(i=0;i<pool_size;i++){
				(p_task->p_list+i)->index=i;
				(p_task->p_list+i)->event_base=event_base_new();
				(p_task->p_list+i)->timeout=time;
				(p_task->p_list+i)->offset=0;
				(p_task->p_list+i)->session_type=unspecified;
				(p_task->p_list+i)->header_h=header_h;
				(p_task->p_list+i)->body_h=body_h;
				if((thread_mutex_init(&(p_task->p_list+i)->mutex))!=0){
					ret=-1;
					break;
				}
				memset((p_task->p_list+i)->req_head_buffer,'\0',HEADERLEN+1);
				memset((p_task->p_list+i)->reqs_head_buffer,'\0',HEADERLEN+1);
				p_task->p_list->next=cur;
				cur=p_task->p_list+i;
			}
			p_task->d_flag=0;
			return 0;
		}while(0);
		pthread_mutex_destroy(&(p_task->mutex));
	}while(0);
	return ret;
}

int task_pool_destory(){
	free(p_task);
	p_task=NULL;
	return 0;
}

int task_list_push(task_t *elt){
	pthread_mutex_lock(&(p_task->mutex));
	elment_clear(elt);
	pthread_mutex_unlock(&(p_task->mutex));
	return 0;
}

task_t *task_list_pop(int index){
	task_t *elt;
	pthread_mutex_lock(&(p_task->mutex));
	do{
		if(p_task->d_flag || isLoop){
			elt=NULL;
			break;
		}
		elt=p_task->p_list+index;
	}while(0);
	pthread_mutex_unlock(&(p_task->mutex));
	return elt;
}

static void elment_clear(task_t *elt){
	elt->offset=0;
	elt->sockfd=0;
	elt->session_type=unspecified;
	memset(&(elt->req_head),0,sizeof(header_t));
	memset(&(elt->reqs_head),0,sizeof(header_t));
	memset(elt->req_head_buffer,'\0',HEADERLEN+1);
	memset(elt->reqs_head_buffer,'\0',HEADERLEN+1);
	free(elt->req_body_buffer);
	free(elt->reqs_body_buffer);
	return ;

}

static void elment_destory(task_t *elt){
	free(elt->req_body_buffer);
	free(elt->reqs_body_buffer);
	free(elt->event_base);
	elt->event_base=NULL;
}
