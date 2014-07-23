/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_TIMER_H_
#define MQ_TIMER_H_

#ifdef __cplusplus
extern "C":
{
#endif

#include <stdio.h>
#include <stdlib.h>

typedef void (*timerout_handler)(void *arg);
int timer(int sec,int usec,timerout_handler timer_ev,void *arg);

#ifdef __cplusplus
}
#endif
#endif
