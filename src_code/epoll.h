/*************************************************
 *Author*        :wayne
 *Description*   :epoll头文件
 *Created Time*  : Tue 14 Apr 2020 11:45:50 PM CST
 * Ended  Time*  : Fri 17 Apr 2020 09:53:44 PM CST
**************************************************/

#ifndef EPOLL_H
#define EPOLL_H


#include <sys/epoll.h>
#include "http.h"
#include "threadpool.h"

#define MAXEVENTS 1024

int epoll_create(int flags);
int epoll_add(int epoll_fd, int fd, http_request_t* request, int events);
int epoll_mod(int epoll_fd, int fd, http_request_t* request, int events);
int epoll_del(int epoll_fd ,int fd, http_request_t* request, int events);
int epoll_wait(int epoll_fd, struct epoll_event *events, int max_events, int timeout);
void handle_events(int epoll_fd, int listen_fd, struct epoll_event* events, int events_num, char* path, threadpool_t* tp);

#endif
