/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */
#ifndef MQ_LOG_H_
#define MQ_LOG_H_

#ifdef __cplusplus
extern "C":
{
#endif

int logger_init(const char *path, const char *prefix, const int linesize,const int level);
void log_info_recond(const char *format, ...); //Log_level  0
void log_warn_recond(const char *format, ...); //Log_level  1
void log_error_recond(const char *format, ...); //Log_level 2
void logger_close();

#ifdef __cplusplus
}
#endif
#endif
