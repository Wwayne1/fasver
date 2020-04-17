/*************************************************
 *Author*        :wayne
 *Description*   :http协议相关函数
 *Created Time*  : Thu 16 Apr 2020 09:26:19 AM CST

**************************************************/

#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include "http_request.h"
#include "util.h"
#include "epoll.h"

void do_request(void *ptr);

#endif
