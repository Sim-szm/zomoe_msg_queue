/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_TYPES_H_
#define MQ_TYPES_H_

#ifdef __cplusplus
extern "C":
{
#endif

#ifndef int64
typedef long long int64;
#endif

#ifndef ubyte
typedef unsigned char ubyte;
#endif

#ifndef byte
typedef signed char byte;
#endif

#define true  1
#define false 0
#define IP_LEN 16
#define LINE_LEN 1024
#define PATH_LEN 256
#define DATETIME_LEN_NOSEPY 15
#define DATETIME_LEN_SEPY 20
#define CONFKEY_LEN 64
#define CONFVALUE_LEN LINE_LEN-CONFKEY_LEN-1
#define INT_LEN 32
#define isLoop  1

#ifdef __cplusplus
}
#endif
#endif
