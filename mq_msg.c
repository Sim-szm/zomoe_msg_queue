/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mq_msg.h"

int buffer_to_header_t(const char *buffer, header_t *header){
    assert(buffer!=NULL && header!=NULL);
    header->protocol = *buffer;
    header->state = convert_buffer_to_int(buffer+1);
    header->bodylen = convert_buffer_to_long(buffer+1+sizeof(int));
    return 0;
}

int header_t_to_buffer(const header_t *header, char *buffer){
    assert(buffer!=NULL && header!=NULL);
    *buffer = header->protocol;
    convert_int_to_buffer(header->state,buffer+1);
    convert_long_to_buffer(header->bodylen,buffer+1+sizeof(int));
    return 0;
}

//the follow functions got from internet ,thanks much !

void convert_int_to_buffer(const int n, char *buffer){
    unsigned char *p_convert;
    p_convert = (unsigned char *)buffer;
    *p_convert++ = (n >> 24) & 0xFF;
    *p_convert++ = (n >> 16) & 0xFF;
    *p_convert++ = (n >> 8) & 0xFF;
    *p_convert++ = n & 0xFF;
}


int convert_buffer_to_int(const char *buffer){
    int convert_v;
    convert_v=(
             ((unsigned char)(*buffer)) << 24) |
            (((unsigned char)(*(buffer + 1)))<< 16) |
            (((unsigned char)(*(buffer + 2))) << 8) |
             ((unsigned char)(*(buffer + 3))
                )
            ;
    return convert_v;
}

void convert_long_to_buffer(int64_t n, char *buffer){
    unsigned char *p_convert;
    p_convert = (unsigned char *)buffer;
    *p_convert++ = (n >> 56) & 0xFF;
    *p_convert++ = (n >> 48) & 0xFF;
    *p_convert++ = (n >> 40) & 0xFF;
    *p_convert++ = (n >> 32) & 0xFF;
    *p_convert++ = (n >> 24) & 0xFF;
    *p_convert++ = (n >> 16) & 0xFF;
    *p_convert++ = (n >> 8) & 0xFF;
    *p_convert++ = n & 0xFF;
}

int64_t convert_buffer_to_long(const char *buffer){
    unsigned char *p_convert;
    p_convert = (unsigned char *)buffer;
    int64_t convert_v;
    convert_v=(
             ((int64_t) (*p_convert)) << 56) |
            (((int64_t) (*(p_convert + 1))) << 48) |
            (((int64_t) (*(p_convert + 2))) << 40) |
            (((int64_t) (*(p_convert + 3))) << 32) |
            (((int64_t) (*(p_convert + 4))) << 24) |
            (((int64_t) (*(p_convert + 5))) << 16) |
            (((int64_t) (*(p_convert + 6))) << 8)  |
             ((int64_t) (*(p_convert + 7))
                )
             ;
}

