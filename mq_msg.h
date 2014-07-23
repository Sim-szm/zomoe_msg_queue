/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */

#ifndef MQ_MSG_H_
#define MQ_MSG_H_

#ifdef __cplusplus
extern "C":
{
#endif

#include <stdint.h>
#include "mq_types.h"

typedef enum session{
    unspecified, header, body,
}session_t;

typedef struct header{
    char protocol;
    int state;
    int64_t bodylen;
}header_t;

#define HEADERLEN (sizeof(char) + sizeof(int) + sizeof(int64_t))

#ifdef __cplusplus
}
#endif
#endif
