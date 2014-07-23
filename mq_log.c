/*
 * Copyright (C) 2014 zomoe
 *
 * zomoemq may be copied only under the terms of the GNU General Public License V3
 * mail  : xianszm007@gmail.com
 * Author: zomoe
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <time.h>

#include "mq_types.h"
#include "mq_log.h"
static pthread_mutex_t _locker = PTHREAD_MUTEX_INITIALIZER;
static int _fd = 2;  //2 as the systerm_error_ptr
static char _suffix[PATH_LEN];
char *format_datetime(const struct tm *datetime,
                         const size_t bufflen, char *buff){
    size_t len = DATETIME_LEN_NOSEPY < bufflen ?  DATETIME_LEN_NOSEPY : bufflen;

    if (NULL == datetime){
        time_t timep;
        time(&timep);
        struct tm *datetime_temp = localtime(&timep);

        snprintf(buff, len, "%04d%02d%02d%02d%02d%02d", datetime_temp->tm_year
            + 1900, datetime_temp->tm_mon + 1, datetime_temp->tm_mday,
            datetime_temp->tm_hour, datetime_temp->tm_min,
            datetime_temp->tm_sec);
    }else{
        snprintf(buff, len, "%04d%02d%02d%02d%02d%02d", datetime->tm_year
            + 1900, datetime->tm_mon + 1, datetime->tm_mday,
            datetime->tm_hour, datetime->tm_min,
            datetime->tm_sec);
    }

    *(buff + len) = '\0';
    return buff;
}

static int open_logfile(const char *path){
    int result = 0;
    char *log_filename;

    log_filename = (char*)malloc(PATH_LEN);
    if (NULL == log_filename){
        result = 0 == errno ? ENOMEM : errno;
        printf("Error!File:%s,Line:%d.malloc memory is error.Errno:%d,Info:%s.", __FILE__, __LINE__,
                result,strerror(result));
        return result;
    }

    char datetime[DATETIME_LEN_NOSEPY];
    format_datetime(NULL, DATETIME_LEN_NOSEPY, datetime);
    do{
        result = snprintf(log_filename, PATH_LEN, "%s%s", path, datetime);
        if (-1 == result || PATH_LEN <= result){
            result = 0 == errno ? ENOMEM : errno;
            printf("Error!File:%s,Line:%d.Errno:%d,Info:%s.", __FILE__,
                    __LINE__, result,strerror(result));
            break;
        }
        if (0 > (_fd = open(log_filename, O_RDWR | O_CREAT | O_APPEND
                        | O_SYNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
                        |S_IWOTH))){
            result = errno;
            _fd = STDERR_FILENO;
            printf("Error!File:%s,Line:%d.Errno:%d,Info:%s.", __FILE__,
                    __LINE__,result, strerror(result));
            break;
        }
        result = 0;
    } while (false);

    free(log_filename);
    log_filename=NULL;
    return result;
}

void logger_close(){
    fsync(_fd);
    close(_fd);
    _fd = 2;
}

static void do_log(const char *message){
    int result = 0;
    pthread_mutex_lock(&_locker);
    do{
        if (_linesize <= _lines){
            logger_close();
            if (0 != (result = open_logfile(_suffix))){
                _fd = STDERR_FILENO;
                //  result = 0 == errno ? EIO : errno;
                printf("Error!File:%s,Line:%d.Errno:%d,Info:%s.", __FILE__,
                        __LINE__, result,strerror(result));
                break;
            }
            _lines = 0;
        }
        int length = strlen(message);
        do{
            result += write(_fd, message + result, length - result);
        } while (length != result);

        _lines++;
    } while (false);
    pthread_mutex_unlock(&_locker);
    return;
}

int logger_init(const char *path, const char *prefix, const int linesize,const int level){

}

void log_info_recond(const char *format, ...){

}

void log_warn_recond(const char *format, ...){

}

void log_error_recond(const char *format, ...){

}

