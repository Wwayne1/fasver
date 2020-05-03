/*************************************************
 *Author*        :wayne
 *Description*   :线程池以及任务头文件
 *Created Time*  : Sat 28 Mar 2020 02:42:27 PM CST
 * Ended  Time*  : Sat 28 Mar 2020 04:05:24 PM CST
**************************************************/

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

typedef struct task
{
		void (*func)(void*);
		void* arg;
		struct task* next;		//任务链表（下一节点指针）
}task_t;

typedef struct threadpool
{
		pthread_mutex_t lock;   //互斥锁
	  pthread_cond_t cond;		//条件变量
	  pthread_t *threads;			//线程
		task_t *head;						//任务链表
    int thread_count;				//线程数
		int queue_size;					//任务链表长
		int shutdown;						//关机模式
		int started;
}threadpool_t;

typedef enum
{
		tp_invalid = -1,
		tp_lock_fail = -2,
		tp_already_shutdown = -3,
		tp_cond_broadcast = -4,
		tp_thread_fail = -5,
}threadpool_error_t;

typedef enum
{
	immediate_shutdown = 1,			//立即关机
	graceful_shutdown = 2				//平滑关机
}thread_pool_sd_t;

threadpool_t* threadpool_init(int thread_num);
int threadpool_add(threadpool_t* pool, void (*func)(void *), void* arg);
int threadpool_destroy(threadpool_t* pool, int gracegul);

#endif
