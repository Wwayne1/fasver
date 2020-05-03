/*************************************************
 *Author*        :wayne
 *Description*   :时间堆定时器
 *Created Time*  : Thu 23 Apr 2020 11:09:48 PM CST
 * Ended  Time*  : Fri 24 Apr 2020 11:00:43 AM CST
**************************************************/

#include <sys/time.h>
#include "timer.h"

pq_t timer;
size_t current_msec;

int timer_comp(void* ti, void* tj)
{
		timer_tp* timeri = (timer_tp*)ti;
		timer_tp* timerj = (timer_tp*)tj;
		return (timeri->key < timerj->key) ? 1 : 0;
}

void time_update()
{
		//获取当前时间
		struct timeval tv;
		int rc = gettimeofday(&tv, NULL);
		current_msec = ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int timer_init()
{
		//建立连接后立即初始化
		//初始优先队列大小PQ_DEFAULT_SIZE
		int rc = pq_init(&timer, timer_comp, PQ_DEFAULT_SIZE);

		//更新当前时间
		time_update();
		return 0;
}

int find_timer()
{
		int time;

		//返回队列中最早时间和当前时间之差
		while (!pq_is_empty(&timer))
		{
				//更新当前时间
				time_update();
				//timer_node指向最小的时间
				timer_tp* timer_node = (timer_tp*)pq_min(&timer);
				//如果已删则释放此节点（tk_del_timer只置位不删除）
				if (timer_node->deleted)
				{
						int rc = pq_delmin(&timer);
						free(timer_node);
						continue;
				}
				//此时timer_node为时间最小节点，time为优先队列里最小时间减去当前时间
				time = (int)(timer_node->key - current_msec);
				time = (time > 0) ? time : 0;
				break;
		}
		return time;
}

void handle_expire_timers()
{
		while (!pq_is_empty(&timer))
		{
				//更新当前时间
				time_update();
				timer_tp* timer_node = (timer_tp*)pq_min(&timer);

				//如果已删则释放此节点
				if (timer_node->deleted)
				{
						int rc = pq_delmin(&timer);
						free(timer_node);
						continue;
				}

				//最早入队列节点超时时间大于当前时间（未超时）
				//结束超时检查，顺带删了下标记为删除的节点
				if (timer_node->key > current_msec)
				{
						return;
				}

				//出现了没被删但是超时的情况，调用handler处理
				if (timer_node->handler)
				{
						timer_node->handler(timer_node->request);
				}
				int rc = pq_delmin(&timer);
				free(timer_node);
		}
}

void add_timer(http_request_t* request, size_t timeout, timer_handler_pt handler)
{
		time_update();
		//申请新的timer_t节点，并加入到http_request_t的timer下
		timer_tp* timer_node = (timer_tp*)malloc(sizeof(timer_tp));
		request->timer = timer_node;

		//加入时设置超时阈值，删除信息等
		timer_node->key = current_msec + timeout;
		timer_node->deleted = 0;
		timer_node->handler = handler;

		//注意需要在timer_t节点中反向设置指向对应resquest的指针
		timer_node->request = request;

		//将新节点插入优先队列
		int rc = pq_insert(&timer, timer_node);
}

void del_timer(http_request_t* request)
{
		time_update();
		timer_tp* timer_node = request->timer;

		//惰性删除，标记为已删，在find_timer和handle_expire_timers检查队列时会删除
		timer_node->deleted = 1;
}
