/*************************************************
 *Author*        :wayne
 *Description*   :定时器
 *Created Time*  : Thu 23 Apr 2020 10:53:57 PM CST
 * Ended  Time*  : Fri 24 Apr 2020 11:01:46 AM CST
**************************************************/

#ifndef TIMER_H
#define TIMER_H

#include "priority_queue.h"
#include "http_request.h"

#define TIMEOUT_DEFAULT 500       //ms

//函数指针，负责超时处理，add_timer时指定处理函数
typedef int (*timer_handler_pt)(http_request_t* request);

typedef struct timer
{
		size_t key;			//标记超时时间
		int deleted;		//标记是否被删除
		timer_handler_pt handler;			//超时处理，add时指定
		http_request_t* request;			//指向对应的request请求
}timer_tp;

extern pq_t timer;
extern size_t current_msec;

int timer_init();
int find_timer();
void handle_expire_timers();
void add_timer(http_request_t* request, size_t timeout, timer_handler_pt handler);
void del_timer(http_request_t* request);
int timer_comp(void *ti, void *tj);

#endif
