/*************************************************
 *Author*        :wayne
 *Description*   :http协议相关函数
 *Created Time*  : Thu 16 Apr 2020 09:26:19 AM CST

**************************************************/

#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "http_request.h"
#include "util.h"
#include "epoll.h"
#include "timer.h"
#include "rio.h"

#define MAXLINE 8192
#define SHORTLINE 512

//用key-value表示mime_type_t
typedef struct mime_type
{
		const char *type;
		const char *value;
}mime_type_t;

void do_request(void *ptr);

#endif
