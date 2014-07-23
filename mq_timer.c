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

#include "mq_types.h"
#include "mq_timer.h"

int timer(int sec,int usec,
        timerout_handler timer_ev,
                        void *arg){
    struct timeval timeSpan;
    int ret_flag;

    while(true){

        timeSpan.tv_sec = sec;
        timeSpan.tv_usec = usec;
        ret_flag = select(0,NULL,NULL,NULL,&timeSpan);

        if(ret_flag > 0){
            //log_recond,timer select error !
        }else if(ret_flag < 0){
            if(!(errno == EAGAIN || errno ==EWOULDBLOCK)){
                //log error recond
                return errno;
            }
        }else{
            timerout_handler(arg);
        }
    }

    return 0;
}
