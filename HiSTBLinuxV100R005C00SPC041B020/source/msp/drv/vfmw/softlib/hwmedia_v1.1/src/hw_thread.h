
/*******************************************************************************
Copyright (C), 2009-2012, Huawei Tech. Co., Ltd.
File name: iMedia_common.h
Author & ID: 宋小刚+00133955
Version: 1.00
Date:  2010-3-4
Description: 本文件包括了跨平台线程创建等相关函数定义
Function List:
History:
1、2010-3-4 编码完成 宋小刚+00133955
2、xxxx
3、xxxx
*******************************************************************************/
#ifndef __HW_THREAD_H__
#define __HW_THREAD_H__

#if defined(_MSC_VER)
#include <windows.h>
#include <process.h>
#elif defined(__GNUC__)
#include <pthread.h>
#elif defined(_WIN32_WCE)
#include <Kfuncs.h>
#endif
#include <assert.h>
#include <time.h>

#define THREAD_INVALID 0
#define THREAD_WAITING 1
#define THREAD_RUNNING 2
#define THREAD_STOPPED 3

typedef void (*hw_thread_fun)(void *args);

typedef struct hw_thread
{
#if defined( _MSC_VER)
    HANDLE handle;
#elif defined(__GNUC__)
    pthread_t handle;
#endif
    int  status;
    hw_thread_fun fun;
    void *args;
} hw_thread_t;

#if defined(_MSC_VER)
static unsigned int WINAPI hw_thread_entry(LPVOID param)
#elif defined(__GNUC__)
static void *hw_thread_entry(void *param)
#endif
{
    struct hw_thread *thread = (struct hw_thread *)param;

    thread->status = THREAD_RUNNING;

    if (thread->fun)
    {
        thread->fun(thread->args);
    }

    thread->status = THREAD_STOPPED;

    return 0;
}

static /*inline*/ int hw_get_current_thread_id()
{
#if defined(_MSC_VER)
    return (int)GetCurrentThreadId();
#elif defined(__GNUC__)
    return pthread_self();
#endif
}

static /*inline*/ void hw_thread_create(struct hw_thread *thread)
{
#if defined(_MSC_VER)
    unsigned int theId = 0; /* We don't care about the identifier*/
    thread->handle = (HANDLE)_beginthreadex(NULL, /* Inherit security*/
                                            0, /* Inherit stack size*/
                                            hw_thread_entry, /* Entry function*/
                                            (void *)thread,	 /* Entry arg*/
                                            0,	/* Begin executing immediately*/
                                            &theId);
#elif defined(__GNUC__)
    pthread_attr_t *p_attr = 0;
    pthread_create((pthread_t *)&thread->handle, p_attr, hw_thread_entry, (void *)thread);
#endif

    thread->status = THREAD_WAITING;
}

static /*inline*/ void hw_thread_wait_destroy(struct hw_thread *thread)
{
    if (thread->handle)
    {
#if defined(_MSC_VER)
        DWORD ret = WaitForSingleObject(thread->handle, INFINITE);

        if (ret != 0)
        { return; }

#elif defined(__GNUC__)
        void *ret;
        pthread_join((pthread_t)thread->handle, &ret);
#endif
    }
}

static /*inline*/ void hw_thread_sleep(unsigned int ms)
{
    if (ms == 0)
    { return; }

    /* >10 s*/
    if (ms > 10000)
    { ms = 10000; }

#if defined( _MSC_VER)
    Sleep(ms);
#elif defined(__GNUC__)
    {
        struct timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = ms * 1000000;
        nanosleep(&ts, NULL);
    }
    /*usleep(ms * 1000); // for linux*/
    /*taskDelay(sysClkRateGet()*ms/1000);*/
#endif
}

#endif /* __HW_THREAD_H__ */
