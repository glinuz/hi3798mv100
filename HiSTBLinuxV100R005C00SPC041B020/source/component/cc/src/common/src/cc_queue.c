
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#include <errno.h>

#include "hi_type.h"
#include "cc_queue.h"
#include "cc_debug.h"

#define EnterCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_lock(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_lock(QUEUE) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define LeaveCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_unlock(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_unlock(QUEUE) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define InitializeCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_init(x,NULL); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_init(QUEUE) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define DeleteCriticalSection(x)  \
    do{ \
        int ret = pthread_mutex_destroy(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_destroy(QUEUE) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

void CCQueue_Init(TRI_QUEUE *queue,HI_U32 queue_size,HI_U32 element_size)
{
    queue->Size = queue_size;

    queue->Element_size = element_size;
    queue->Element = (void *)malloc(element_size*queue_size);
    InitializeCriticalSection( &queue->mCriticalSection );
    EnterCriticalSection(&queue->mCriticalSection );
    queue->Head = 0;
    queue->Tail = 0;
    LeaveCriticalSection(&queue->mCriticalSection );
    pthread_cond_init(&queue->cond_insert,NULL);
    pthread_cond_init(&queue->cond_remove,NULL);
}

void CCQueue_Destroy(TRI_QUEUE *queue)
{
    if (queue->Element)
    {
        free(queue->Element);
        queue->Element = NULL;
    }
    pthread_cond_destroy(&queue->cond_insert);
    pthread_cond_destroy(&queue->cond_remove);
    DeleteCriticalSection(&queue->mCriticalSection );
}

HI_BOOL CCQueue_InsertTimeout(TRI_QUEUE *queue,void *element,HI_S32 ms,HI_U32*reason)
{
    HI_S32 n=0,t_sec,t_ms,ret;
    struct timespec ts;
    struct timeval tv;
    ret=HI_TRUE;
    EnterCriticalSection( &queue->mCriticalSection );
    if (((queue->Head + 1)%queue->Size) == queue->Tail ) //Queue is full
    {
        if (ms<0)  //no timeout needed, wait forever.
        {
            ret=pthread_cond_wait(&queue->cond_insert,&queue->mCriticalSection);
            *reason=TIMEOUT_RET_REASON_WAIT_FAIL;//failed
        }
        else
        {
            if(-1 == gettimeofday(&tv,NULL))
            {
                /*system can't support timeout*/
                ret=HI_FALSE;
                *reason=TIMEOUT_RET_REASON_NO_TIME_FUNCTION;//time function not work
            }
            else
            {
                t_sec = ms/1000;
                t_ms = ms%1000;

                ts.tv_sec = t_sec + tv.tv_sec;
                ts.tv_nsec = (tv.tv_usec*1000) + (t_ms*1000000);
                t_sec = ts.tv_nsec/1000000000;
                ts.tv_nsec %= 1000000000;
                ts.tv_sec += t_sec;

                //waiting untill timeout or queue has room.
                ret=pthread_cond_timedwait(&queue->cond_insert,&queue->mCriticalSection,&ts);
                *reason=TIMEOUT_RET_REASON_WAIT_FAIL;//wait fail
            }
        }
        if (ret!=0) //timeout or wait failed.
        {
            LeaveCriticalSection(&queue->mCriticalSection);
            if (ret==ETIMEDOUT)
                *reason=TIMEOUT_RET_REASON_TIMEOUT;//timeout
            return HI_FALSE;
        }
        else if (((queue->Head + 1)%queue->Size) == queue->Tail)
        {
            LeaveCriticalSection(&queue->mCriticalSection);
            *reason=TIMEOUT_RET_REASON_QUEUE_FULL;//wake up, but queue has been empty.
            return HI_FALSE;
        }
    }
    //have to check it again, many threads can be wake up by queue available signal
    //only one can access the internal data.
    n=queue->Head++;
    memcpy((HI_U8 *)queue->Element+n*queue->Element_size,(HI_U8 *)element,queue->Element_size);
    queue->Head%=queue->Size;
    LeaveCriticalSection(&queue->mCriticalSection);
    (HI_VOID)pthread_cond_signal(&queue->cond_remove);
    *reason=TIMEOUT_RET_REASON_SUCCESS;//success
    return HI_TRUE;
}

HI_BOOL CCQueue_Insert(TRI_QUEUE *queue, HI_VOID *element)
{
    HI_U32 n = 0;
    EnterCriticalSection( &queue->mCriticalSection );

    if (((queue->Head + 1) % queue->Size) == queue->Tail )
    {
        LeaveCriticalSection( &queue->mCriticalSection );
        return HI_FALSE;
    }
    n = queue->Head++;
    memcpy((HI_U8 *)queue->Element+n*queue->Element_size,(HI_U8 *)element,queue->Element_size);
    queue->Head %= queue->Size;
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    (HI_VOID)pthread_cond_signal(&queue->cond_remove);
    return HI_TRUE;
}

HI_BOOL CCQueue_RemoveTimeout(TRI_QUEUE *queue, void *element,HI_S32 ms,HI_U32 *reason)
{
    HI_U32 n=0,t_sec,t_ms;
    HI_S32 ret;
    struct timespec ts;
    struct timeval tv;
    EnterCriticalSection( &queue->mCriticalSection );
    if (queue->Tail == queue->Head)
    {
        if (ms<0)
        {
            ret=pthread_cond_wait(&queue->cond_remove,&queue->mCriticalSection);
            *reason=TIMEOUT_RET_REASON_WAIT_FAIL;//failed
        }
        else
        {
            if(-1 == gettimeofday(&tv,NULL))
            {
                /*system can't support timeout*/
                ret=HI_FALSE;
                *reason=TIMEOUT_RET_REASON_NO_TIME_FUNCTION;//time function not work
            }
            else
            {
                t_sec=ms/1000;
                t_ms=ms%1000;

                ts.tv_sec = tv.tv_sec + t_sec;
                ts.tv_nsec = (tv.tv_usec * 1000) + (t_ms * 1000000);
                t_sec=ts.tv_nsec/1000000000;
                ts.tv_nsec %= 1000000000;
                ts.tv_sec+=t_sec;
                ret=pthread_cond_timedwait(&queue->cond_remove,&queue->mCriticalSection,&ts);
                *reason=TIMEOUT_RET_REASON_WAIT_FAIL;//failed
            }
        }
        if (ret!=0)
        {
            LeaveCriticalSection(&queue->mCriticalSection);
            if (ret == ETIMEDOUT)
                *reason=TIMEOUT_RET_REASON_TIMEOUT;//timeout
            return HI_FALSE;
        } else if (queue->Tail == queue->Head)
        {
            LeaveCriticalSection(&queue->mCriticalSection);
            *reason=TIMEOUT_RET_REASON_QUEUE_EMPTY;//wake up, but queue_empty
            return HI_FALSE;
        }
    }
    n = queue->Tail++;
    memcpy((HI_U8 *)element,(HI_U8 *)queue->Element+n*queue->Element_size,queue->Element_size);
    queue->Tail%=queue->Size;
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    (HI_VOID)pthread_cond_signal(&queue->cond_insert);
    *reason=TIMEOUT_RET_REASON_SUCCESS;//success
    return HI_TRUE;
}

HI_BOOL CCQueue_Remove(TRI_QUEUE *queue, HI_VOID *element)
{
    HI_U32 n=0;
    EnterCriticalSection( &queue->mCriticalSection );
    if ( queue->Tail == queue->Head)
    {
        LeaveCriticalSection( &queue->mCriticalSection );
        return HI_FALSE;
    }

    n = queue->Tail++;
    memcpy((HI_U8 *)element,(HI_U8 *)queue->Element+n*queue->Element_size,queue->Element_size);
    queue->Tail%=queue->Size;
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    (HI_VOID)pthread_cond_signal(&queue->cond_insert);
    return HI_TRUE;
}

HI_BOOL CCQueue_Get(TRI_QUEUE *queue,HI_VOID *element)
{
    HI_U32 n;
    HI_BOOL ret;
    EnterCriticalSection( &queue->mCriticalSection );
    if ( queue->Tail == queue->Head )
    {
        ret = HI_FALSE;
    }
    else
    {
        ret = HI_TRUE;
        n=queue->Tail;
        memcpy((HI_U8 *)element,(HI_U8 *)queue->Element+n*queue->Element_size,queue->Element_size);
    }
    LeaveCriticalSection( &queue->mCriticalSection );
    return ret;
}

HI_BOOL CCQueue_IsFull(TRI_QUEUE *queue)
{
    HI_BOOL ret = HI_FALSE;
    // enter the critical section
    EnterCriticalSection( &queue->mCriticalSection );
    ret = (HI_BOOL)(((queue->Head+1)%queue->Size) == queue->Tail);
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    return ret;
}

HI_BOOL CCQueue_IsEmpty(TRI_QUEUE *queue)
{
    HI_BOOL ret = HI_FALSE;

    // enter the critical section
    EnterCriticalSection( &queue->mCriticalSection );

    ret = (HI_BOOL)(queue->Tail == queue->Head);

    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );

    return ret;

}

HI_U32 CCQueue_Availability(TRI_QUEUE *queue)
{
    HI_U32 ret = 0;
    // enter the critical section
    EnterCriticalSection( &queue->mCriticalSection );
    if (queue->Head<queue->Tail)
        ret=queue->Tail-queue->Head;
    else
        ret = queue->Size - queue->Head + queue->Tail;
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    return ret;

}

HI_U32 CCQueue_GetHeadPos(TRI_QUEUE *queue)
{
    return queue->Head;
}

HI_U32 CCQueue_TailPos(TRI_QUEUE *queue)
{
    HI_U32 n ;
    n = queue->Tail;
    return n;
}

void CCQueue_Flush(TRI_QUEUE *queue)
{
    // enter the critical section
    EnterCriticalSection( &queue->mCriticalSection );
    queue->Head = queue->Tail = 0;
    LeaveCriticalSection( &queue->mCriticalSection );
    (HI_VOID)pthread_cond_signal(&queue->cond_insert); //wake up the thread want to insert elements.
}

HI_U32 CCQueue_GetItemNum(TRI_QUEUE *queue)
{
    HI_U32 n;
    // enter the critical section
    EnterCriticalSection( &queue->mCriticalSection );
    if (queue->Head>=queue->Tail)
        n =  (queue->Head - queue->Tail);
    else
        n = queue->Size + queue->Head - queue->Tail;
    // leave the critical section
    LeaveCriticalSection( &queue->mCriticalSection );
    return n;
}

