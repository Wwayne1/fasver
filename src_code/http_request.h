/*************************************************
 *Author*        :wayne
 *Description*   :http相关报文结构
 *Created Time*  : Thu 16 Apr 2020 09:28:58 AM CST

**************************************************/

#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <stdio.h>

typedef struct http_request
{
		char* root;
		int fd;
		int epoll_fd;
}http_request_t;

int init_request_t(http_request_t* request, int fd, int epoll_fd, char* path);

#endif
