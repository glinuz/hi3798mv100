/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/*
 * Copyright (c) 2005 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/** OMX_OSAL_Linux.c
 *  Linux implemenation of the OpenMax IL OS abstraction layer including memory, threading,
 *  mutex, event, time query and tracing functionality.
 */

#include "OMX_OSAL_Interfaces.h"

#define _XOPEN_SOURCE 600   /* version 6.0 of XOpen source (for recursive locks) */
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern OMX_U32 g_OMX_OSAL_TraceFlags;

/**********************************************************************
 * MEMORY
 **********************************************************************/
OMX_PTR OMX_OSAL_Malloc( OMX_IN OMX_U32 size )
{
    return (OMX_PTR)malloc(size);
}

void OMX_OSAL_Free( OMX_IN OMX_PTR pData )
{
    if (pData)
    { free(pData); }
}

/* JVW-NVIDIA: Added for http://cvs.khronos.org/bugzilla/show_bug.cgi?id=217 */

OMX_PTR OMX_OSAL_Memset( OMX_IN OMX_PTR pDest, OMX_IN OMX_U32 cChar, OMX_IN OMX_U32 nCount)
{
    return memset(pDest, cChar, nCount);
}

/* JVW-NVIDIA: Added for http://cvs.khronos.org/bugzilla/show_bug.cgi?id=217 */

OMX_PTR OMX_OSAL_Memcpy( OMX_IN OMX_PTR pDest, OMX_IN OMX_PTR pSrc, OMX_IN OMX_U32 nCount)
{
    return memcpy(pDest, pSrc, nCount);
}

/**********************************************************************
 * THREADS
 **********************************************************************/

typedef struct OMX_OSAL_THREADDATATYPE
{
    pthread_t oPosixThread;
    pthread_attr_t oThreadAttr;
    OMX_U32 (*pFunc)(OMX_PTR pParam);
    OMX_PTR pParam;
    OMX_U32 uReturn;
} OMX_OSAL_THREADDATATYPE;

static void* threadFunc(void* pParameter)
{
    OMX_OSAL_THREADDATATYPE* pThreadData;
    pThreadData = (OMX_OSAL_THREADDATATYPE*)pParameter;
    pThreadData->uReturn = pThreadData->pFunc(pThreadData->pParam);
    return pThreadData;
}

/** Create a thread given the thread function, a data parameter to pass
 *  to the thread, and the thread priority (higher values=higher priority).
 *  The thread is created and executed immediately calling the thread
 *  function with the supplied data parameter. */
OMX_ERRORTYPE OMX_OSAL_ThreadCreate( OMX_IN OMX_U32 (*pFunc)(OMX_PTR pParam),
                                     OMX_IN OMX_PTR pParam,
                                     OMX_IN OMX_U32 nPriority,
                                     OMX_OUT OMX_HANDLETYPE* phThread )
{
    OMX_OSAL_THREADDATATYPE* pThreadData;
    struct sched_param sched;

    pThreadData = (OMX_OSAL_THREADDATATYPE*)OMX_OSAL_Malloc(sizeof(OMX_OSAL_THREADDATATYPE));
    if (pThreadData == NULL)
    { return OMX_ErrorInsufficientResources; }

    pThreadData->pFunc = pFunc;
    pThreadData->pParam = pParam;
    pThreadData->uReturn = 0;

    pthread_attr_init(&pThreadData->oThreadAttr);

    pthread_attr_getschedparam(&pThreadData->oThreadAttr, &sched);
    sched.sched_priority += nPriority; /* relative to the default priority */
    pthread_attr_setschedparam(&pThreadData->oThreadAttr, &sched);


    if (pthread_create(&pThreadData->oPosixThread,
                       &pThreadData->oThreadAttr,
                       threadFunc,
                       pThreadData))
    {
        OMX_OSAL_Free(pThreadData);
        return OMX_ErrorInsufficientResources;
    }

    *phThread = (OMX_HANDLETYPE)pThreadData;
    return OMX_ErrorNone;
}

/* JVW-NVIDIA: Moved local variable declarations up for http://cvs.khronos.org/bugzilla/show_bug.cgi?id=224 */

OMX_ERRORTYPE OMX_OSAL_ThreadDestroy( OMX_IN OMX_HANDLETYPE hThread )
{
    OMX_OSAL_THREADDATATYPE* pThreadData = (OMX_OSAL_THREADDATATYPE*)hThread;
    void* retVal;

    if (pThreadData == NULL || pthread_join(pThreadData->oPosixThread, &retVal))
    {
        return OMX_ErrorBadParameter;
    }

    OMX_OSAL_Free(pThreadData);
    return OMX_ErrorNone;
}

/**********************************************************************
 * MUTEX
 **********************************************************************/

OMX_ERRORTYPE OMX_OSAL_MutexCreate(OMX_OUT OMX_HANDLETYPE* phMutex)
{
    pthread_mutex_t* pMutex = (pthread_mutex_t*)OMX_OSAL_Malloc(sizeof(pthread_mutex_t));
    static pthread_mutexattr_t oAttr;
    static pthread_mutexattr_t* pAttr = NULL;
    if (pAttr == NULL
        && !pthread_mutexattr_init(&oAttr)
        && !pthread_mutexattr_settype(&oAttr, PTHREAD_MUTEX_RECURSIVE))
    {
        pAttr = &oAttr;
    }

    if (pMutex == NULL || pthread_mutex_init(pMutex, pAttr))
    {
        OMX_OSAL_Free(pMutex);
        return OMX_ErrorInsufficientResources;
    }
    *phMutex = (void*)pMutex;
    return OMX_ErrorNone;
}


OMX_ERRORTYPE OMX_OSAL_MutexDestroy(OMX_IN OMX_HANDLETYPE hMutex)
{
    pthread_mutex_t* pMutex = (pthread_mutex_t*)hMutex;
    if (pMutex == NULL || pthread_mutex_destroy(pMutex))
    {
        return OMX_ErrorBadParameter;
    }
    OMX_OSAL_Free(pMutex);
    return OMX_ErrorNone;
}

/* JVW-NVIDIA: Moved local variable declarations up for http://cvs.khronos.org/bugzilla/show_bug.cgi?id=224 */

OMX_ERRORTYPE OMX_OSAL_MutexLock(OMX_IN OMX_HANDLETYPE hMutex)
{
    pthread_mutex_t* pMutex = (pthread_mutex_t*)hMutex;
    int err;
    if (pMutex == NULL)
    { return OMX_ErrorBadParameter; }
    err = pthread_mutex_lock(pMutex);
    switch (err)
    {
        case 0:
            return OMX_ErrorNone;
        case EINVAL:
            return OMX_ErrorBadParameter;
        case EDEADLK:
            return OMX_ErrorNotReady;

        default:
            return OMX_ErrorUndefined;
    }
    return OMX_ErrorNone;
}

/* JVW-NVIDIA: Moved local variable declarations up for http://cvs.khronos.org/bugzilla/show_bug.cgi?id=224 */

OMX_ERRORTYPE OMX_OSAL_MutexUnlock(OMX_IN OMX_HANDLETYPE hMutex)
{
    pthread_mutex_t* pMutex = (pthread_mutex_t*)hMutex;
    int err;
    if (pMutex == NULL)
    { return OMX_ErrorBadParameter; }
    err = pthread_mutex_unlock(pMutex);
    switch (err)
    {
        case 0:
            return OMX_ErrorNone;
        case EINVAL:
            return OMX_ErrorBadParameter;
        case EPERM:
            return OMX_ErrorNotReady;

        default:
            return OMX_ErrorUndefined;
    }
}

/**********************************************************************
 * EVENTS
 **********************************************************************/

typedef struct
{
    OMX_BOOL bSignaled;
    pthread_mutex_t mutex;
    pthread_cond_t  condition;
} OMX_OSAL_THREAD_EVENT;

OMX_ERRORTYPE OMX_OSAL_EventCreate(OMX_OUT OMX_HANDLETYPE* phEvent)
{
    /* Emulate a win32 event that
        - has default security
        - must be manualy reset
        - starts as nonsignaled
        - is unnamed
    */
    OMX_OSAL_THREAD_EVENT* pEvent = OMX_OSAL_Malloc(sizeof(OMX_OSAL_THREAD_EVENT));
    pEvent->bSignaled = OMX_FALSE;

    if (pEvent == NULL)
    {
        return OMX_ErrorInsufficientResources;
    }

    if (pthread_mutex_init(&pEvent->mutex, NULL))
    {
        OMX_OSAL_Free(pEvent);
        return OMX_ErrorInsufficientResources;
    }
    if (pthread_cond_init(&pEvent->condition, NULL))
    {
        pthread_mutex_destroy(&pEvent->mutex);
        OMX_OSAL_Free(pEvent);
        return OMX_ErrorInsufficientResources;
    }

    *phEvent = (OMX_HANDLETYPE)pEvent;
    return OMX_ErrorNone;
}

OMX_ERRORTYPE OMX_OSAL_EventDestroy(OMX_IN OMX_HANDLETYPE hEvent)
{
    OMX_OSAL_THREAD_EVENT* pEvent = (OMX_OSAL_THREAD_EVENT*)hEvent;
    if (pEvent == NULL)
    { return OMX_ErrorBadParameter; }

    if (pthread_mutex_lock(&pEvent->mutex))
    { return OMX_ErrorBadParameter; }

    pthread_cond_destroy(&pEvent->condition);
    pthread_mutex_unlock(&pEvent->mutex);
    pthread_mutex_destroy(&pEvent->mutex);

    OMX_OSAL_Free(pEvent);
    return OMX_ErrorNone;
}

OMX_ERRORTYPE OMX_OSAL_EventReset(OMX_IN OMX_HANDLETYPE hEvent)
{
    OMX_OSAL_THREAD_EVENT* pEvent = (OMX_OSAL_THREAD_EVENT*)hEvent;
    if (pEvent == NULL)
    { return OMX_ErrorBadParameter; }

    if (pthread_mutex_lock(&pEvent->mutex))
    { return OMX_ErrorBadParameter; }
    pEvent->bSignaled = OMX_FALSE;
    pthread_mutex_unlock(&pEvent->mutex);

    return OMX_ErrorNone;
}

OMX_ERRORTYPE OMX_OSAL_EventSet(OMX_IN OMX_HANDLETYPE hEvent)
{
    OMX_OSAL_THREAD_EVENT* pEvent = (OMX_OSAL_THREAD_EVENT*)hEvent;
    if (pEvent == NULL)
    { return OMX_ErrorBadParameter; }

    if (pthread_mutex_lock(&pEvent->mutex))
    { return OMX_ErrorBadParameter; }
    pEvent->bSignaled = OMX_TRUE;
    pthread_cond_signal(&pEvent->condition);
    pthread_mutex_unlock(&pEvent->mutex);

    return OMX_ErrorNone;
}

OMX_ERRORTYPE OMX_OSAL_EventWait(OMX_IN OMX_HANDLETYPE hEvent, OMX_IN OMX_U32 uMSec, OMX_OUT OMX_BOOL* pbTimedOut)
{
    OMX_OSAL_THREAD_EVENT* pEvent = (OMX_OSAL_THREAD_EVENT*)hEvent;
    OMX_U32         timeout_us;
    struct timespec timeout;
    int             retcode;

    struct timeval now;
    gettimeofday(&now, NULL);

    *pbTimedOut = OMX_FALSE;

    timeout_us = now.tv_usec + 1000 * uMSec;
    timeout.tv_sec = now.tv_sec + timeout_us / 1000000;
    timeout.tv_nsec = (timeout_us % 1000000) * 1000;

    if (pEvent == NULL)
    { return OMX_ErrorBadParameter; }
    if (pthread_mutex_lock(&pEvent->mutex))
    { return OMX_ErrorBadParameter; }

    if (uMSec == 0)
    {
        if (!pEvent->bSignaled)
        {
            *pbTimedOut = OMX_TRUE;
        }
    }
    else if (uMSec == INFINITE_WAIT)
    {
        while (!pEvent->bSignaled)
        {
            pthread_cond_wait(&pEvent->condition, &pEvent->mutex);
        }
    }
    else
    {
        while (!pEvent->bSignaled)
        {
            retcode = pthread_cond_timedwait(&pEvent->condition, &pEvent->mutex, &timeout);
            if (retcode == ETIMEDOUT && !pEvent->bSignaled)
            {
                *pbTimedOut = OMX_TRUE;
                break;
            }
        }
    }

    pthread_mutex_unlock(&pEvent->mutex);
    return OMX_ErrorNone;
}

/**********************************************************************
 * TIME
 **********************************************************************/

/** Returns a time value in milliseconds based on a clock starting at
 *  some arbitrary base. Given a call to GetTime that returns a value
 *  of n a subsequent call to GetTime made m milliseconds later should
 *  return a value of (approximately) (n+m). This method is used, for
 *  instance, to compute the duration of call. */
OMX_U32 OMX_OSAL_GetTime()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return ((OMX_U32)now.tv_sec) * 1000 + ((OMX_U32)now.tv_usec) / 1000;
}


