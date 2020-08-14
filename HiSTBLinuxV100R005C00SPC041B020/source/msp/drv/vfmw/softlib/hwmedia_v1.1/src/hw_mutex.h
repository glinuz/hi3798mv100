
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: iMedia_common.h
Author & ID: 宋小刚+00133955
Version: 1.00
Date:  2010-3-4
Description: 本文件包括了跨平台线程同步结构定义
Function List:
History:
1、2010-3-4 编码完成 宋小刚+00133955
2、xxxx
3、xxxx
*******************************************************************************/
#ifndef __HW_MUTEX_H__
#define __HW_MUTEX_H__

#if defined(_MSC_VER)
#include <windows.h>
//extern BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
#elif defined(__GNUC__)
#include <pthread.h>
#endif

#include "hw_thread.h"

typedef struct hw_mutex
{
#if defined(_MSC_VER)
    CRITICAL_SECTION id;
#elif defined(__GNUC__)
    pthread_mutex_t id;
#endif
    int owner;
    int recursive;
} hw_mutex_t;

static /*inline*/ void hw_mutex_init(struct hw_mutex *mutex)
{
#if defined(_MSC_VER)
    InitializeCriticalSection(&mutex->id);
#elif defined(__GNUC__)
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    /*pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);*/
    /*pthread_mutexattr_setkind_np(&attr, PTHREAD_MUTEX_RECURSIVE_NP);*/
    pthread_mutex_init(&mutex->id, &attr);
#endif

    mutex->owner = 0;
    mutex->recursive = 0;
}

static /*inline*/ void hw_mutex_destroy(struct hw_mutex *mutex)
{
#if defined(_MSC_VER)
    DeleteCriticalSection(&mutex->id);
#elif defined(__GNUC__)
    pthread_mutex_destroy(&mutex->id);
#endif
}

static /*inline*/ void hw_mutex_lock(struct hw_mutex *mutex)
{
    /* 2010/08/25 11:30:00 liuxw+00139685 [AZ1D02270] */
    /* 注释掉用于优化的代码，直接调用系统的互锁函数 */
    //	int this_thread = hw_get_current_thread_id();
    //	if (this_thread == mutex->owner)
    //	{
    //		++mutex->recursive;
    //		return;
    //	}

#if defined(_MSC_VER)
    EnterCriticalSection(&mutex->id);
#elif defined(__GNUC__)
    pthread_mutex_lock(&mutex->id);
#endif
    /* 2010/08/25 11:30:00 liuxw+00139685 [AZ1D02270] */
    /* 注释掉用于优化的代码，直接调用系统的互锁函数 */
    //	mutex->owner = this_thread;
    //	++mutex->recursive;
}

static /*inline*/ void hw_mutex_unlock(struct hw_mutex *mutex)
{
    /* 2010/08/25 11:30:00 liuxw+00139685 [AZ1D02270] */
    /* 注释掉用于优化的代码，直接调用系统的互锁函数 */
    //	int this_thread = hw_get_current_thread_id();
    //	if (this_thread != mutex->owner)
    //		return;

    //	if (--mutex->recursive <= 0)
    //	{
    //		mutex->owner = 0;
#if defined(_MSC_VER)
    LeaveCriticalSection(&mutex->id);
#elif defined(__GNUC__)
    pthread_mutex_unlock(&mutex->id);
#endif
    //	}
}
/*
static  int hw_mutex_trylock(struct hw_mutex *mutex)
{
	int this_thread = hw_get_current_thread_id();
	if (this_thread == mutex->owner)
	{
		++mutex->recursive;
		return 0;
	}

#if defined(_MSC_VER)
	if (!TryEnterCriticalSection(&mutex->id))
		return -1;
#elif defined(__GNUC__)
	if (pthread_mutex_trylock(&mutex->id) != 0)
		return -1;
#endif

	mutex->owner = this_thread;
	++mutex->recursive;
	return 0;
}*/

#endif /* __HW_MUTEX_H__ */
