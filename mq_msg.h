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
#include <sys/types.h>
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

void convert_int_to_buffer(const int n, char *buff);

int convert_buffer_to_int(const char *buff);

void convert_long_to_buffer(int64_t n, char *buff);

int64_t convert_buffer_to_long(const char *buff);

int buffer_to_header_t(const char *buffer, header_t *header);

int header_t_to_buffer(const header_t *header, char *buffer);

#ifdef __cplusplus
}
#endif
#endif
