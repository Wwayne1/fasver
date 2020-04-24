/*************************************************
 *Author*        :wayne
 *Description*   :优先队列小根堆
 *Created Time*  : Fri 20 Apr 2020 10:58:27 AM CST
 * Ended  Time*  : Fri 24 Apr 2020 11:00:05 AM CST
**************************************************/

#include <string.h>
#include "priority_queue.h"

void exch(pq_t *pq, size_t i, size_t j)
{
		void *temp = pq->pq[i];
		pq->pq[i] = pq->pq[j];
		pq->pq[j] = temp;
}

//由下至上的堆有序化
void swim(pq_t *pq, size_t k)
{
		while (k > 1 && pq->comp(pq->pq[k], pq->pq[k/2]))
		{
				exch(pq, k, k/2);
				k /= 2;
		}
}

//由上至下的堆有序化,下沉
int sink(pq_t *pq, size_t k)
{
		size_t j;
		size_t nalloc = pq->nalloc;
		while ((k << 1) <= nalloc)
		{
				j = k << 1;
				if ((j < nalloc) && (pq->comp(pq->pq[j+1], pq->pq[j])))
						j++;
				if (!pq->comp(pq->pq[j], pq->pq[k]))
						break;

				exch(pq, j, k);
				k = j;
		}
		return k;
}

/*int pq_sink(pq_t *pq, size_t i)*/
/*{*/
		/*return sink(pq, i);*/
/*}*/

int pq_init(pq_t *pq, pq_comparator_pt comp, size_t size)
{
		//为pq_t节点的pq分配(void *)指针
		pq->pq = (void **)malloc(sizeof(void *) * (size + 1));
		if (!pq->pq)
				return -1;

		pq->nalloc = 0;
		pq->size = size + 1;
		pq->comp = comp;

		return 0;
}

int pq_is_empty(pq_t * pq)
{
		//通过nalloc值款快速判断是否为空
		return (pq->nalloc == 0) ? 1 : 0;
}

//获取优先队列大小
size_t pq_size(pq_t *pq)
{
		return pq->nalloc;
}

void *pq_min(pq_t *pq)
{
		//优先队列最小堆直接返回第一个元素
		if (pq_is_empty(pq))
				return (void *)(-1);

		return pq->pq[1];
}

int resize(pq_t *pq, size_t new_size)
{
		if (new_size <= pq->nalloc)
				return -1;

		void **new_ptr = (void **)malloc(sizeof(void *) * new_size);
		if (!new_ptr)
				return -1;
		//将原本nalloc + 1个元素值拷贝到new_ptr指向的位置
		memcpy(new_ptr, pq->pq, sizeof(void *) * (pq->nalloc + 1));
		//释放旧元素
		free(pq->pq);
		//重新改写优先队列元素pq指针为new_ptr
		pq->pq = new_ptr;
		pq->size = new_size;
		return 0;
}

int pq_delmin(pq_t *pq)
{
		if (pq_is_empty(pq))
				return 0;

		exch(pq, 1, pq->nalloc);
		--pq->nalloc;
		sink(pq, 1);
		if ((pq->nalloc > 0) && (pq->nalloc <= (pq->size - 1)/4))
		{
				if (resize(pq, pq->size/2) < 0)
						return -1;
		}
		return 0;
}

int pq_insert(pq_t *pq, void *item)
{
		if (pq->nalloc + 1 == pq->size)
		{
				if (resize(pq, pq->size * 2) < 0)
				{
						return -1;
				}
		}
		pq->pq[++pq->nalloc] = item;
		swim(pq, pq->nalloc);
		return 0;
}
