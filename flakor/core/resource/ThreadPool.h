#ifndef _FK_THREADPOOL_H_
#define _FK_THREADPOOL_H_

#include "targetMacros.h"
#include <pthread.h>

FLAKOR_NS_BEGIN

#ifdef __cplusplus
extern "C" {
#endif

/* 要执行的任务链表 */
typedef struct tpool_work {
    void*               (*routine)(void*);       /* 任务函数 */
    void                *arg;                    /* 传入任务函数的参数 */
    struct tpool_work   *next;
}tpool_work_t;

typedef struct tpool {
    int             shutdown;                    /* 线程池是否销毁 */
    int             max_thr_num;                /* 最大线程数 */
    pthread_t       *thr_id;                    /* 线程ID数组 */
    tpool_work_t    *queue_head;                /* 线程链表 */
    pthread_mutex_t queue_lock;
    pthread_cond_t  queue_ready;
}tpool_t;

/*
* @brief     创建线程池
* @param     max_thr_num 最大线程数
* @return     0: 成功 其他: 失败
*/
int
tpool_create(int max_thr_num);

/*
* @brief     销毁线程池
*/
void
tpool_destroy();

/*
* @brief     向线程池中添加任务
* @param    routine 任务函数指针
* @param     arg 任务函数参数
* @return     0: 成功 其他:失败
*/
int
tpool_add_work(void*(*routine)(void*), void *arg);

FLAKOR_NS_END

#ifdef __cplusplus
}
#endif

#endif
