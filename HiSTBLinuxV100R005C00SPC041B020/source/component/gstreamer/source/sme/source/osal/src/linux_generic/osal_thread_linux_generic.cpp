/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_thread_android.c
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2012年9月11日
  最近修改   :
  功能描述   : android 平台vos 线程，互斥，条件同步，睡眠，时间实现
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

/*lint -e* */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>

#ifdef __LINUX__
#include <stdlib.h>
#include <sys/time.h>
#else
#include <cutils/log.h>
#endif

#include "osal_thread.h"
#include "osal_str.h"
#include "osal_mem.h"

#undef MOD_NAME
#define MOD_NAME "osal_thread"

#if defined __LINUX__
#define ICS_LOGD printf
#define ICS_LOGI printf
#define ICS_LOGW printf
#define ICS_LOGE printf
#elif defined __ANDROID__
#define ICS_LOGD(pccFmt,...) LOG_PRI(ANDROID_LOG_DEBUG, MOD_NAME, pccFmt, ##__VA_ARGS__)
#define ICS_LOGI(pccFmt,...) LOG_PRI(ANDROID_LOG_INFO, MOD_NAME, pccFmt, ##__VA_ARGS__)
#define ICS_LOGW(pccFmt,...) LOG_PRI(ANDROID_LOG_WARN, MOD_NAME, pccFmt, ##__VA_ARGS__)
#define ICS_LOGE(pccFmt,...) LOG_PRI(ANDROID_LOG_ERROR, MOD_NAME, pccFmt, ##__VA_ARGS__)
#else
#define ICS_LOGD(pccFmt,...) ((V_VOID)0)
#define ICS_LOGI(pccFmt,...) ((V_VOID)0)
#define ICS_LOGW(pccFmt,...) ((V_VOID)0)
#define ICS_LOGE(pccFmt,...) ((V_VOID)0)
#endif //__LINUX__

#define VOS_THD_RET_VAL_IF(Cond, Val, Log) \
    if(ICS_TRUE == (Cond))\
    {\
        ICS_LOGD Log;\
        return Val;\
    }

#define VOS_THD_RET_IF(Cond, Log) \
    if(ICS_TRUE == (Cond))\
    {\
        ICS_LOGD Log;\
        return ;\
    }

#define VOS_SAFEFREE(p) \
if(NULL != p)\
{\
	free(p);\
	p = NULL;\
}

#define ANDROID_PHTREAD_LOWEST_PRI 99
#define ANDROID_PHTREAD_HIGHEST_PRI 1

static V_INT32 ErrorMaps(V_INT32 i32SysErr)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_UNKOWN;

    switch(i32SysErr)
    {
        case 0:
            i32Ret = E_VOS_THD_ERR_NONE;
            break;
        case EAGAIN:
            i32Ret = E_VOS_THD_ERR_NOMORE_RES;
            break;
        case ENOTSUP://不支持
        case EINVAL:
            i32Ret = E_VOS_THD_ERR_ARGS;
            break;
        case EDEADLK:
            i32Ret = E_VOS_THD_ERR_THD_DEAD_LOCK;
            break;
        case ETIMEDOUT:
            i32Ret = E_VOS_THD_ERR_COND_TIMEOUT;
            break;
        default:
            fprintf(stdout, "vos pthread err:%#x\n", i32Ret);
            break;
    }

    return i32Ret;
}

//static V_INT32 AThreadPriToVOSPri(V_INT32 i32AThdPri)
//{
//    return (ANDROID_PHTREAD_LOWEST_PRI - i32AThdPri);
//}

static V_INT32 SetThdAttrStackSize(pthread_attr_t *pstAttr, V_INT32 i32StackSize)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    VOS_THD_RET_VAL_IF(0 > i32StackSize, E_VOS_THD_ERR_NONE, ("\n"));
    i32Ret = pthread_attr_setstacksize(pstAttr, i32StackSize);

    return i32Ret;
}

static V_VOID VOSSchedPriToAThds(V_INT32 i32Sched, V_INT32 i32Pri,
                                          V_INT32* pi32OutSched, V_INT32* pi32OutPri)
{
    switch(i32Sched)
    {
        case E_VOS_THD_SCHED_OTHER:
            *pi32OutSched = SCHED_OTHER;
            *pi32OutPri = 0;
            break;
        case E_VOS_THD_SCHED_FIFO:
            *pi32OutSched = SCHED_FIFO;
            if ( i32Pri > E_VOS_THD_PRI_INVALID && i32Pri < E_VOS_THD_PRI_BUTT)
            {
                *pi32OutPri = i32Pri;
            }
            else
            {
                *pi32OutPri =  E_VOS_THD_PRI_LOWEST;
            }
            break;
        case E_VOS_THD_SCHED_RR:
            *pi32OutSched = SCHED_RR;
            if ( i32Pri > E_VOS_THD_PRI_INVALID && i32Pri < E_VOS_THD_PRI_BUTT)
            {
                *pi32OutPri = i32Pri;
            }
            else
            {
                *pi32OutPri =  E_VOS_THD_PRI_LOWEST;
            }
            break;
        default :
            *pi32OutSched = SCHED_OTHER;
            *pi32OutPri = 0;
            break;
    }
}

/* BEGIN: Modified by l00180035 for DTS2013051706485 , 2013/6/4 */
static V_INT32 SetThdAttrPri(pthread_attr_t *pstAttr, V_INT32 i32Sched, V_INT32 i32Pri)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    V_INT32 i32AndroidThdPri = 99;
    V_INT32 i32AThdSched = SCHED_OTHER;
    struct sched_param stSchedParam;
    E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;

    VOSSchedPriToAThds(i32Sched, i32Pri, &i32AThdSched, &i32AndroidThdPri);
    if((eErr = VOS_Memset_S(&stSchedParam, sizeof(stSchedParam), 0, sizeof(stSchedParam))) != E_SME_MEM_OK)
    {
        printf("VOS_Mmeset_S failed(%d)!\n", eErr);
        return E_VOS_THD_ERR_FAIL;
    }
    stSchedParam.sched_priority = i32AndroidThdPri;
    i32Ret = pthread_attr_setschedpolicy(pstAttr, i32AThdSched);
    i32Ret |= pthread_attr_setschedparam(pstAttr,(const struct sched_param*)&stSchedParam);

    return i32Ret;
}

V_UINT32 VOS_CreateThread(OUT VOSTHDHDL* pThdHdl,
                        IN const ST_VOS_THDATTR* pstAttr,
                        IN const PFNVOSTHD pfnThd,
                        INOUT V_VOID* pvCtx)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    pthread_attr_t stThdAttr;
    pthread_attr_t* pstThdAttr = NULL;
    pthread_t stThdId = -1;
    VOS_THD_RET_VAL_IF((NULL == pfnThd) || (NULL == pThdHdl), E_VOS_THD_ERR_ARGS,
        ("thread runtime is null, or pThdHdl is null\n"));

    /*是否需要设置属性*/
    if ( NULL !=  pstAttr)
    {
        i32Ret = pthread_attr_init(&stThdAttr);
        if ( 0 == i32Ret )
        {
            SetThdAttrStackSize(&stThdAttr, pstAttr->i32StackSize);
            SetThdAttrPri(&stThdAttr, pstAttr->i32Sched, pstAttr->i32Prioty);
            pstThdAttr = &stThdAttr;
        }
    }

    i32Ret = pthread_create(&stThdId,
                            pstThdAttr,
                            (V_VOID* (*)(V_VOID*))pfnThd,
                            pvCtx);
    i32Ret = ErrorMaps(i32Ret);
    /* BEGIN: Modified by l00180035 for DTS2013052005258, 2013/5/24 */
    if (E_VOS_THD_ERR_NONE != i32Ret)
    {
        *pThdHdl = (VOSTHDHDL)VOS_INVALIDE_THD_HANDLE;
    }
    else
    {
        *pThdHdl = (VOSTHDHDL)stThdId;
        ICS_LOGD("[VOS_CreateThread] thread id:%ld", stThdId);
    }
    /* END:   Modified by l00180035 for DTS2013052005258, 2013/5/24 */

    if(NULL != pstThdAttr)
    {
        pthread_attr_destroy(&stThdAttr);
    }

    return (V_UINT32)i32Ret;
}

V_VOID VOS_DetachThread(IN VOSTHDHDL hThdHdl)
{
    /*参数不符合，返回成功*/
    VOS_THD_RET_IF(NULL == hThdHdl, ("detach hThdHdl is NULL."));

    /*该函数有EINVAL(已经分离)，ESRCH(进程中找不到该线程ID)，这两个都可以忽略*/
    pthread_detach((pthread_t)hThdHdl);

    return;
}

V_UINT32 VOS_JoinThread(INOUT VOSTHDHDL hThdHdl)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;

    /*参数不符合，返回成功*/
    VOS_THD_RET_VAL_IF(NULL == hThdHdl, E_VOS_THD_ERR_NONE, ("join hThdHdl is %p\n", hThdHdl));

    i32Ret = pthread_join((pthread_t)hThdHdl, NULL);

    /*这两个错误值分别对应 没找到线程 线程已经分离，这两种情况直接返回成功*/
    VOS_THD_RET_VAL_IF(ESRCH == i32Ret|| EINVAL == i32Ret, E_VOS_THD_ERR_NONE,
                        ("join i32Ret:%#x\n", i32Ret));

    i32Ret = ErrorMaps(i32Ret);

    return (V_UINT32)i32Ret;
}

V_UINT32 VOS_SetThreadPriority(IN VOSTHDHDL hThdHdl,
                                    IN V_INT32 i32Sched,
                                    IN V_INT32 i32Priority)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    V_INT32 i32AndroidThdPri = 99;
    V_INT32 i32AThdSched = SCHED_OTHER;
    struct sched_param stSchedParam;
    E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;

    VOS_THD_RET_VAL_IF(NULL == hThdHdl, E_VOS_THD_ERR_ARGS, ("Set pri hThdHdl is %p\n", hThdHdl));

    VOSSchedPriToAThds(i32Sched, i32Priority, &i32AThdSched, &i32AndroidThdPri);

    if((eErr = VOS_Memset_S(&stSchedParam, sizeof(stSchedParam), 0, sizeof(stSchedParam))) != E_SME_MEM_OK)
    {
        printf("VOS_Mmeset_S failed(%d)!\n", eErr);
        return (V_UINT32)E_VOS_THD_ERR_FAIL;
    }

    stSchedParam.sched_priority = i32AndroidThdPri;
    i32Ret = pthread_setschedparam((pthread_t)hThdHdl, i32AThdSched,
                                    (const struct sched_param *)&stSchedParam);

    i32Ret = ErrorMaps(i32Ret);

    return (V_UINT32)i32Ret;
}

V_UINT32 VOS_GetThreadPriority(IN VOSTHDHDL hThdHdl,
                                    OUT V_INT32* pi32Sched,
                                    OUT V_INT32* pi32Priority)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    V_INT32 i32VOSThdPri = 0;
    V_INT32 i32Policy = 0;
    struct sched_param stSchedParam;
    E_SME_MEM_SECURE_RET eErr = E_SME_MEM_OK;

    VOS_THD_RET_VAL_IF(NULL == hThdHdl || (NULL == pi32Priority && NULL == pi32Sched),
        E_VOS_THD_ERR_ARGS,
        ("get pri hThdHdl is %p, pi32Priority is :%p\n", hThdHdl, pi32Priority));

    if((eErr = VOS_Memset_S(&stSchedParam, sizeof(stSchedParam), 0, sizeof(stSchedParam))) != E_SME_MEM_OK)
    {
        printf("VOS_Mmeset_S failed(%d)!\n", eErr);
        return (V_UINT32)E_VOS_THD_ERR_FAIL;
    }
    i32Ret = pthread_getschedparam((pthread_t)hThdHdl, &i32Policy,
                                    (struct sched_param *)&stSchedParam);
    i32Ret = ErrorMaps(i32Ret);

    /* BEGIN: Added by l00180035 for DTS2013051706485, 2013/5/18 */
    VOS_THD_RET_VAL_IF(E_VOS_THD_ERR_NONE != i32Ret, (V_UINT32)i32Ret, ("get pri fail\n"));
    /* END:   Added by l00180035 for DTS2013051706485, 2013/5/18 */

    if (SCHED_OTHER == i32Policy )
    {
        i32VOSThdPri = E_VOS_THD_PRI_LOWEST;
        i32Policy = E_VOS_THD_SCHED_OTHER;
    }
    else
    {
        i32VOSThdPri = stSchedParam.sched_priority;
        if ( SCHED_FIFO == i32Policy )
        {
            i32Policy = E_VOS_THD_SCHED_FIFO;
        }
        else
        {
            i32Policy = E_VOS_THD_SCHED_RR;
        }
    }

    if ( NULL != pi32Sched )
    {
        *pi32Sched = i32Policy;
    }

    if ( NULL != pi32Priority )
    {
        *pi32Priority = i32VOSThdPri;
    }

    return (V_UINT32)i32Ret;
}

VOS_TID_T VOS_GetSelfThreadId()
{
    return (VOS_TID_T)pthread_self();
}

VOSTHDHDL VOS_GetSelfThdHdl()
{
    return (VOSTHDHDL)pthread_self();
}

V_UINT32 VOS_GetThdIdByHandle(IN VOSTHDHDL hThdHdl, OUT VOS_TID_T* pThdId)
{
    VOS_THD_RET_VAL_IF(NULL == hThdHdl || NULL == pThdId, E_VOS_THD_ERR_ARGS,
        ("get pri hThdHdl is %p, pi32Priority is :%p\n", hThdHdl, pThdId));

    *pThdId = (VOS_TID_T)hThdHdl;

    return (V_UINT32)E_VOS_THD_ERR_NONE;
}

VOS_API VOS_PID_T VOS_GetSelfPid()
{
    return (VOS_PID_T)getpid();
}

/*线程互斥*/
/*stAttr保留*/
V_UINT32 VOS_InitThdMutex(OUT PST_VOS_THD_MUTEX pstMutex,
                               IN const ST_VOS_THD_MUTEX_ATTR* pstAttr)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    pthread_mutexattr_t stAttr;

    VOS_THD_RET_VAL_IF(NULL == pstMutex, E_VOS_THD_ERR_ARGS,
        ("init mutex failed,invalid args\n"));

    /*设置为递归锁*/
    /*不需要判断其返回值，因为其错误值在这里不会出现。*/
    if((NULL != pstAttr) && (0 != pstAttr->u32Reserve))
    {
        i32Ret = pthread_mutex_init((pthread_mutex_t*)pstMutex, NULL);
    }
    else
    {
        pthread_mutexattr_init(&stAttr);
        pthread_mutexattr_settype(&stAttr , PTHREAD_MUTEX_RECURSIVE);
        i32Ret = pthread_mutex_init((pthread_mutex_t*)pstMutex, &stAttr);
        /* BEGIN: Added by l00180035 for DTS2013051706485, 2013/5/18 */
        pthread_mutexattr_destroy(&stAttr);
        /* END:   Added by l00180035 for DTS2013051706485, 2013/5/18 */
    }
    if(0 == i32Ret || EBUSY == i32Ret)
    {
       i32Ret = E_VOS_THD_ERR_NONE;
    }
    else
    {
        i32Ret = ErrorMaps(i32Ret);
    }

    return (V_UINT32)i32Ret;
}

V_VOID VOS_DeInitThdMutex(INOUT PST_VOS_THD_MUTEX pstMutex)
{
    VOS_THD_RET_IF(NULL == pstMutex, ("deinit mutex: arg is null\n"));
    pthread_mutex_destroy((pthread_mutex_t*)pstMutex);
}

V_VOID VOS_ThdMutexLock(IN PST_VOS_THD_MUTEX pstMutex)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;

    VOS_THD_RET_IF(NULL == pstMutex, ("mutex unlock failed,invalid args\n"));
    i32Ret = pthread_mutex_lock((pthread_mutex_t*)pstMutex);
    i32Ret = ErrorMaps(i32Ret);
    if(E_VOS_THD_ERR_NONE != i32Ret)
    {
        ICS_LOGW("[mutex unlock] failed:%d", i32Ret);
    }

    return;
}

V_VOID VOS_ThdMutexUnLock(IN PST_VOS_THD_MUTEX pstMutex)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;

    VOS_THD_RET_IF(NULL == pstMutex, ("mutex unlock failed,invalid args\n"));
    i32Ret = pthread_mutex_unlock((pthread_mutex_t*)pstMutex);

    //该锁未没拥有，返回成功
    VOS_THD_RET_IF(EPERM == i32Ret, ("not locked\n"));
    i32Ret = ErrorMaps(i32Ret);
    if(E_VOS_THD_ERR_NONE != i32Ret)
    {
        ICS_LOGW("[mutex unlock] failed:%d", i32Ret);
    }

    return;
}

/*条件同步， pstAttr保留*/
VOSCONDHDL VOS_CreateThdCond(IN const ST_VOS_THD_COND_ATTR* pstAttr)
{
    V_INT32 i32Err = E_VOS_THD_ERR_NONE;
    pthread_cond_t* pstRet = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
#ifdef __LINUX__
    pthread_condattr_t cattr;
#endif
    pstAttr = pstAttr;

    VOS_THD_RET_VAL_IF(NULL == pstRet, NULL, ("create cond, malloc failed\n"));
#ifdef __LINUX__
    i32Err = pthread_condattr_init(&cattr);
    i32Err |= pthread_condattr_setclock(&cattr, CLOCK_MONOTONIC);
    i32Err |= pthread_cond_init(pstRet, &cattr);
    i32Err |= pthread_condattr_destroy(&cattr);
#else
    i32Err = pthread_cond_init(pstRet, NULL);
#endif
    if(0 != i32Err && EBUSY != i32Err)
    {
        free(pstRet);
        pstRet = NULL;
    }

    return pstRet;
}

V_VOID VOS_DestroyThdCond(INOUT V_VOID* pvCond)
{
    VOS_THD_RET_IF(NULL == pvCond, ("destroy cond, cond is null\n"));
    pthread_cond_destroy((pthread_cond_t*)pvCond);
    free(pvCond);
    pvCond = NULL;

    return;
}

V_VOID VOS_ThdCondSignal(IN V_VOID* pvCond)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    VOS_THD_RET_IF(NULL == pvCond,("cond signal, cond is null\n"));
    i32Ret = pthread_cond_signal((pthread_cond_t*)pvCond);
    i32Ret = ErrorMaps(i32Ret);
    if(E_VOS_THD_ERR_NONE != i32Ret)
    {
        ICS_LOGW("VOS_ThdCondSignal pthread_cond_signal failed(%d)", i32Ret);
    }

    return;
}

V_UINT32 VOS_ThdCondWait(IN V_VOID* pvCond, IN PST_VOS_THD_MUTEX pstMutex)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    VOS_THD_RET_VAL_IF(NULL == pvCond, E_VOS_THD_ERR_ARGS,("cond wait, cond is null\n"));
    i32Ret = pthread_cond_wait((pthread_cond_t*)pvCond, (pthread_mutex_t*)pstMutex);
    i32Ret = ErrorMaps(i32Ret);

    return (V_UINT32)i32Ret;
}

V_UINT32 VOS_ThdCondTimeWait(IN V_VOID* pvCond,
                                    IN PST_VOS_THD_MUTEX pstMutex,
                                    IN V_UINT32 u32MiliSecTimeOut)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    struct timespec stTimeSpec;

    VOS_THD_RET_VAL_IF(NULL == pvCond, E_VOS_THD_ERR_ARGS,("cond timewait, cond is null\n"));

    i32Ret = clock_gettime (CLOCK_MONOTONIC, &stTimeSpec);
    stTimeSpec.tv_sec += u32MiliSecTimeOut / 1000;
    stTimeSpec.tv_nsec += (u32MiliSecTimeOut % 1000) * 1000000;

    /* BEGIN: Added by liurongliang(l00180035), 2015/3/10 */
    stTimeSpec.tv_sec += stTimeSpec.tv_nsec / 1000000000;
    stTimeSpec.tv_nsec %= 1000000000;
    /* END:   Added by liurongliang(l00180035), 2015/3/10 */

#ifdef __LINUX__
    i32Ret = pthread_cond_timedwait((pthread_cond_t*)pvCond,
                                    (pthread_mutex_t*)pstMutex,
                                    (const struct timespec*)&stTimeSpec);
#else
    i32Ret = pthread_cond_timedwait_monotonic((pthread_cond_t*)pvCond,
                                    (pthread_mutex_t*)pstMutex,
                                    (const struct timespec*)&stTimeSpec);
#endif
    i32Ret = ErrorMaps(i32Ret);

    return (V_UINT32)i32Ret;
}

V_VOID VOS_ThdCondBrodcast(IN VOSCONDHDL pvCond)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    VOS_THD_RET_IF(NULL == pvCond,("cond broadcast, cond is null\n"));
    i32Ret = pthread_cond_broadcast((pthread_cond_t*)pvCond);
    i32Ret = ErrorMaps(i32Ret);
    if(E_VOS_THD_ERR_NONE != i32Ret)
    {
        ICS_LOGW("VOS_ThdCondBrodcast pthread_cond_broadcast failed(%d)", i32Ret);
    }

    return;
}

V_VOID VOS_Sleep(IN V_UINT32 u32Sec)
{
    sleep(u32Sec);

    return;
}

V_VOID VOS_MSleep(IN V_UINT32 u32MilliSec)
{
    /*usleep 超过1s之后就不准,会影响调度*/
    //usleep(u32MilliSec * 1000 );
    struct timespec stTmspec = {0,0};
    stTmspec.tv_sec = u32MilliSec / 1000;
    stTmspec.tv_nsec = (u32MilliSec % 1000) * 1000 * 1000;
    if(-1 == nanosleep(&stTmspec, NULL))
    {
        printf("nanosleep failed !\n");
    }

    return;
}
/* END:   Modified by l00180035 for DTS2013051706485, 2013/6/4 */

V_UINT32 VOS_GetTimeTicks()
{
    struct timespec stTv;
    V_UINT32 u32CurTime;
    int result;

    result = clock_gettime (CLOCK_MONOTONIC, &stTv);
    if (result != 0)
        return 0;
    u32CurTime = stTv.tv_sec * 1000 + (stTv.tv_nsec + 500000) / 1000000;

    return u32CurTime;
}

V_UINT64 VOS_GetTimeTicks64()
{
    struct timespec stTv;
    V_UINT64 u64CurTime;
    int result;

    result = clock_gettime (CLOCK_MONOTONIC, &stTv);
    if (result != 0)
        return 0ULL;
    u64CurTime = (V_UINT64)(stTv.tv_sec) * 1000 + (V_UINT64)(stTv.tv_nsec + 500000) / 1000000;

    return u64CurTime;
}

V_UINT32 VOS_GetLocalTime(OUT ST_VOS_SYSTIME* pstSysTime,
                                OUT V_CHAR* pszTime)
{
    time_t stCurTt;
    struct tm* pstLocalTime;
    struct timeval stTv;

    (V_VOID)(pszTime);

    gettimeofday(&stTv, 0);
    stCurTt = (time_t)stTv.tv_sec;
    pstLocalTime = localtime((const time_t*)&stCurTt);

    VOS_THD_RET_VAL_IF(NULL == pstLocalTime, E_VOS_THD_ERR_FAIL,("GMT TIME FAILED\n"));

    if(NULL != pstSysTime)
    {
        pstSysTime->u32Year = pstLocalTime->tm_year + 1900;
        pstSysTime->u32Mon = pstLocalTime->tm_mon + 1;
        pstSysTime->u32Day = pstLocalTime->tm_mday;
        pstSysTime->u32Hour = pstLocalTime->tm_hour;
        pstSysTime->u32Min = pstLocalTime->tm_min;
        pstSysTime->u32Sec = pstLocalTime->tm_sec;
        pstSysTime->u32MilliSec = (stTv.tv_usec + 500) / 1000;
        if(pstSysTime->u32MilliSec >= 1000)
        {
            pstSysTime->u32Sec += pstSysTime->u32MilliSec / 1000;
            pstSysTime->u32MilliSec %= pstSysTime->u32MilliSec;
        }
    }

/*  if ( NULL != pszTime)
    {
        VOS_Sprintf(pszTime,"%04d:%02d:%02d:%02d:%02d:%02d:%03ld", pstLocalTime->tm_year + 1900,
            pstLocalTime->tm_mon + 1, pstLocalTime->tm_mday, pstLocalTime->tm_hour,
            pstLocalTime->tm_min, pstLocalTime->tm_sec, (stTv.tv_usec + 500) / 1000);
    }
*/
    return E_VOS_THD_ERR_NONE;
}

/* Event */
typedef enum _tagEventFlgs
{
    E_THD_EVENT_FLGS_NONE = 0,
    E_THD_EVENT_FLGS_MUTEX_INITED = 1 << 0,
    E_THD_EVENT_FLGS_COND_INITED = 1 << 1,
}E_THD_EVENT_FLGS;

typedef struct __tagVOSEvent
{
    V_UINT32 u32Count;       //唤醒标记，只取0[表示为唤醒], 1[已换醒];
    V_UINT32 u32InitFlgs;     //初始化标记
    pthread_mutex_t stMutex;
    pthread_cond_t stCond;
}ST_VOSEVENT, *PST_VOSEVENT;

VOSEVENTHDL VOS_CreateThdEvent(IN const ST_VOS_THD_EVENT_ATTR* pstAttr)
{
    ST_VOSEVENT* pstEvent = (ST_VOSEVENT*)malloc(sizeof(ST_VOSEVENT));
    V_INT32 i32Err = ICS_SUCCESS;
    pstAttr = pstAttr;

    do
    {
        if ( NULL == pstEvent )
        {
            pstEvent = (ST_VOSEVENT*)VOS_INVALIDE_EVENT_HANDLE;
            break;
        }
        pstEvent->u32InitFlgs = E_THD_EVENT_FLGS_NONE;

        /*初始化互斥*/
        i32Err = pthread_mutex_init(&pstEvent->stMutex, NULL);
        if ( ICS_SUCCESS != i32Err )
        {
            break;
        }
        pstEvent->u32InitFlgs |= E_THD_EVENT_FLGS_MUTEX_INITED;

        /*初始化cond*/
        i32Err = pthread_cond_init(&pstEvent->stCond, NULL);
        if ( ICS_SUCCESS != i32Err )
        {
            break;
        }
        pstEvent->u32InitFlgs |= E_THD_EVENT_FLGS_COND_INITED;
        pstEvent->u32Count = 0;
    } while ( ICS_FALSE );

    if ( NULL != pstEvent && ICS_SUCCESS != i32Err )
    {
        VOS_DestroyThdEvent(pstEvent);
        pstEvent = (ST_VOSEVENT*)VOS_INVALIDE_EVENT_HANDLE;
    }

    return (VOSEVENTHDL)pstEvent;
}

V_VOID VOS_DestroyThdEvent(OUT VOSEVENTHDL pvEvent)
{
    ST_VOSEVENT* pstEvent = (ST_VOSEVENT*)pvEvent;
    V_INT32 i32Err = ICS_SUCCESS;

    if ( VOS_INVALIDE_EVENT_HANDLE != pvEvent)
    {
        /*销毁mutex*/
        if ( E_THD_EVENT_FLGS_MUTEX_INITED == (pstEvent->u32InitFlgs
                & E_THD_EVENT_FLGS_MUTEX_INITED))
        {
            /* BEGIN: Modified by l00180035 for DTS2013051706485, 2013/6/4 */
            i32Err = pthread_mutex_lock(&pstEvent->stMutex);
            i32Err |= pthread_mutex_unlock(&pstEvent->stMutex);
            if(ICS_SUCCESS != i32Err)
            {
                ICS_LOGW("[VOS_DestroyThdEvent] mutex lock err:%d", i32Err);
            }
            /* END:   Modified by l00180035 for DTS2013051706485, 2013/6/4 */
            i32Err = pthread_mutex_destroy(&pstEvent->stMutex);
            if ( ICS_SUCCESS != i32Err )
            {
                ;// TODO
            }
            pstEvent->u32InitFlgs &= (~E_THD_EVENT_FLGS_MUTEX_INITED);
        }

        /*销毁cond*/
        if ( E_THD_EVENT_FLGS_COND_INITED != (pstEvent->u32InitFlgs
                & E_THD_EVENT_FLGS_COND_INITED))
        {
            i32Err = pthread_cond_destroy(&pstEvent->stCond);
            if ( ICS_SUCCESS != i32Err )
            {
                ;// TODO
            }
            pstEvent->u32InitFlgs &= (~E_THD_EVENT_FLGS_COND_INITED);
        }

        /*释放semaphore结构*/
        free(pstEvent);
        pstEvent = (ST_VOSEVENT*)VOS_INVALIDE_EVENT_HANDLE;
    }

    return;
}

V_VOID VOS_ThdEventSignal(INOUT VOSEVENTHDL pvEvent)
{
    ST_VOSEVENT* pstEvent = (ST_VOSEVENT*)pvEvent;
    V_INT32 i32Ret = ICS_SUCCESS;


    if ( VOS_INVALIDE_EVENT_HANDLE == pvEvent
        || (E_THD_EVENT_FLGS_MUTEX_INITED | E_THD_EVENT_FLGS_COND_INITED)
            != pstEvent->u32InitFlgs)
    {
        return;
    }

    i32Ret = pthread_mutex_lock(&pstEvent->stMutex);
    pstEvent->u32Count = 1;
    i32Ret |= pthread_cond_signal(&pstEvent->stCond);
    i32Ret |= pthread_mutex_unlock(&pstEvent->stMutex);
    if(ICS_SUCCESS != i32Ret)
    {
        ICS_LOGW("[VOS_ThdEventSignal] failed:%d", i32Ret);
    }

    return;
}

V_UINT32 VOS_ThdEventTimeWait(INOUT VOSEVENTHDL pvEvent, IN V_UINT32 u32TimeOutMs)
{
    ST_VOSEVENT* pstEvent = (ST_VOSEVENT*)pvEvent;
    V_UINT32 u32Ret = ICS_SUCCESS;
    V_INT32 i32Err = ICS_SUCCESS;
    struct timespec stTimeSpec;
    struct timeval stTv;

    if ( VOS_INVALIDE_EVENT_HANDLE == pvEvent
        || (E_THD_EVENT_FLGS_MUTEX_INITED | E_THD_EVENT_FLGS_COND_INITED)
            != pstEvent->u32InitFlgs)
    {
        return E_VOS_THD_ERR_ARGS;
    }

    gettimeofday(&stTv, 0);
    stTimeSpec.tv_sec = stTv.tv_sec + u32TimeOutMs / 1000;
    stTimeSpec.tv_nsec = stTv.tv_usec * 1000 + (u32TimeOutMs % 1000) * 1000000;

    /* BEGIN: Added by liurongliang(l00180035), 2015/3/10 */
    stTimeSpec.tv_sec += stTimeSpec.tv_nsec / 1000000000;
    stTimeSpec.tv_nsec %= 1000000000;
    /* END:   Added by liurongliang(l00180035), 2015/3/10 */

    i32Err = pthread_mutex_lock(&pstEvent->stMutex);
    if(ICS_SUCCESS != i32Err)
    {
        ICS_LOGD("[VOS_ThdEventTimeWait] lock failed:%d", i32Err);
    }
    if(0 == pstEvent->u32Count)
    {
        u32Ret = pthread_cond_timedwait(&pstEvent->stCond,
                                    &pstEvent->stMutex,
                                    (const struct timespec*)&stTimeSpec);
        u32Ret = ErrorMaps(u32Ret);
    }
    pstEvent->u32Count = 0;

    i32Err = pthread_mutex_unlock(&pstEvent->stMutex);
    if(ICS_SUCCESS != i32Err)
    {
        ICS_LOGD("[VOS_ThdEventTimeWait] unlock failed:%d", i32Err);
    }

    return u32Ret;
}

V_UINT32 VOS_ThdEventWait(INOUT VOSEVENTHDL pvEvent)
{
    ST_VOSEVENT* pstEvent = (ST_VOSEVENT*)pvEvent;
    V_INT32 i32Ret = ICS_SUCCESS;

    if ( VOS_INVALIDE_EVENT_HANDLE == pvEvent
        || (E_THD_EVENT_FLGS_MUTEX_INITED | E_THD_EVENT_FLGS_COND_INITED)
            != pstEvent->u32InitFlgs)
    {
        return E_VOS_THD_ERR_ARGS;
    }

    i32Ret = pthread_mutex_lock(&pstEvent->stMutex);

    if(0 == pstEvent->u32Count)
    {
        i32Ret |= pthread_cond_wait(&pstEvent->stCond, &pstEvent->stMutex);
    }
    pstEvent->u32Count = 0;

    i32Ret |= pthread_mutex_unlock(&pstEvent->stMutex);
    if(ICS_SUCCESS != i32Ret)
    {
        ICS_LOGW("[VOS_ThdEventWait] failed:%d", i32Ret);
    }

    return E_VOS_THD_ERR_NONE;
}
/* END:   Modified by l00180035 for DTS2013051706485, 2013/6/4 */

VOS_API VOSSEMHDL VOS_CreateThdSem(IN V_UINT32 u32InitValue)
{
    sem_t* pSem = (sem_t*)malloc(sizeof(sem_t));
    VOS_THD_RET_VAL_IF(NULL == pSem, NULL, ("malloc sem_t fail"));

    if (ICS_SUCCESS != sem_init(pSem, 0, u32InitValue))
    {
        VOS_SAFEFREE(pSem);
        ICS_LOGW("sem_init fail");
        return NULL;
    }

    return pSem;
}

VOS_API V_VOID VOS_DestroyThdSem(IN VOSSEMHDL hSem)
{
    VOS_THD_RET_IF(NULL == hSem, ("invalid arg"));
    sem_destroy((sem_t*)hSem);
    VOS_SAFEFREE(hSem);

    return;
}

VOS_API V_VOID VOS_ThdSemPost(IN VOSSEMHDL hSem)
{
    VOS_THD_RET_IF(NULL == hSem, ("invalid arg"));
    sem_post((sem_t*)hSem);

    return;
}

VOS_API V_UINT32 VOS_ThdSemWait(IN VOSSEMHDL hSem)
{
    VOS_THD_RET_VAL_IF(NULL == hSem, E_VOS_THD_ERR_FAIL, ("invalid arg"));
    if (0 != sem_wait((sem_t*)hSem))
    {
        ICS_LOGW("sem_wait fail");
        return E_VOS_THD_ERR_FAIL;
    }

    return E_VOS_THD_ERR_NONE;
}

VOS_API V_UINT32 VOS_ThdSemTimedWait(IN VOSSEMHDL hSem, IN V_UINT32 u32TimeOutMs)
{
    V_INT32 i32Ret = E_VOS_THD_ERR_NONE;
    struct timespec stTimeSpec;
    struct timeval stTv;

    VOS_THD_RET_VAL_IF(NULL == hSem, E_VOS_THD_ERR_FAIL, ("invalid arg"));
    gettimeofday(&stTv, 0);
    stTimeSpec.tv_sec = stTv.tv_sec + u32TimeOutMs / 1000;
    stTimeSpec.tv_nsec = stTv.tv_usec * 1000 + (u32TimeOutMs % 1000) * 1000000;

    /* BEGIN: Added by liurongliang(l00180035), 2015/3/10 */
    stTimeSpec.tv_sec += stTimeSpec.tv_nsec / 1000000000;
    stTimeSpec.tv_nsec %= 1000000000;
    /* END:   Added by liurongliang(l00180035), 2015/3/10 */

    i32Ret = sem_timedwait((sem_t*)hSem, (const struct timespec*)&stTimeSpec);
    if (ICS_SUCCESS != i32Ret)
    {
        i32Ret = ErrorMaps(errno);
    }

    return (V_UINT32)i32Ret;
}

