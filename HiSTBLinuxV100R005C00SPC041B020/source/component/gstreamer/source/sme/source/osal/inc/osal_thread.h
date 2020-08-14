/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : vos_thread.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2012年9月11日
  最近修改   :
  功能描述   : vos 线程，互斥，条件同步，睡眠，系统时间接口
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __OSAL_THREAD__
#define __OSAL_THREAD__
#include <stdio.h>
#include "osal_type.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#ifdef VOSDLL_EXPORTS
#define VOS_API /*__declspec(dllexport)*/
#else
#define VOS_API /*__declspec(dllimport)*/
#endif
#else
#define VOS_API
#endif

/* 无效句柄 */
#define VOS_INVALIDE_THD_HANDLE (VOSTHDHDL)NULL

/* 线程句柄类型 */
typedef V_VOID* VOSTHDHDL;

/* 条件变量句柄 */
typedef V_VOID* VOSCONDHDL;

/* 线程ID */
typedef unsigned long VOS_TID_T;

/* 进程ID */
typedef unsigned long VOS_PID_T;

/* 线程属性 */
typedef struct _tag_VOSThdAttr
{
    V_INT32 i32StackSize;  //栈大小，默认为系统自动分配,不关注时设置成小于0
    V_INT32 i32Sched;      //调度策略，具体见E_VOS_THD_SCHED
    V_INT32 i32Prioty;     //优先级，具体见E_VOS_THD_PRI
}ST_VOS_THDATTR,PST_VOS_THDATTR;

/* INOUT V_VOID* pvCtx:输入，为主线程上下文 */
typedef V_UINT32 (*PFNVOSTHD)(INOUT V_VOID* pvCtx);

/* 线程调度策略 */
typedef enum _tag_VosThdSched
{
    E_VOS_THD_SCHED_INVALID = -1,  // 无效
    E_VOS_THD_SCHED_OTHER = 0,     // 默认调度
    E_VOS_THD_SCHED_FIFO,          // 实时，先进先出
    E_VOS_THD_SCHED_RR,            // 实时，时间轮询式
}E_VOS_THD_SCHED;

/* 线程优先级等级,范围在E_VOS_THD_PRI_INVALID ~ E_VOS_THD_PRI_BUTT之间，即1~99
 * 且优先级依次降低
 * 最好用E_VOS_THD_PRI_HIGHEST 到 E_VOS_THD_PRI_LOWEST之间的宏定义的等级 */
typedef enum _tag_VosThdPriority
{
    E_VOS_THD_PRI_INVALID = 0,  //不关注时设置成该值
    E_VOS_THD_PRI_HIGH = 95,
    E_VOS_THD_PRI_ABOVE_NORMAL,
    E_VOS_THD_PRI_NORMAL,
    E_VOS_THD_PRI_BELOW_NORMAL,
    E_VOS_THD_PRI_LOWEST,
    E_VOS_THD_PRI_BUTT,
}E_VOS_THD_PRI;

/* 返回值 */
typedef enum _tag_VOSThdERR
{
    E_VOS_THD_ERR_NONE = ICS_SUCCESS,   //成功
    E_VOS_THD_ERR_ARGS = ICS_FAIL + 1,  //参数错误
    E_VOS_THD_ERR_NOMORE_RES,           //没资源了,创建的该类资源超过系统最大,可重试
    E_VOS_THD_ERR_THD_DEAD_LOCK,        //线程死锁，或自己等自己。
    E_VOS_THD_ERR_COND_TIMEOUT,         //条件同步等待超时
    E_VOS_THD_ERR_UNKOWN,               //未知错误
    E_VOS_THD_ERR_NOTSUPPORT,           //不支持
    E_VOS_THD_ERR_LOCK,
    E_VOS_THD_ERR_UNLOCK,
    E_VOS_THD_ERR_COND_SIGNAL,
    E_VOS_THD_ERR_COND_WAIT,
    E_VOS_THD_ERR_FAIL,                 //失败
}E_VOS_THD_ERR;

/* 互斥属性 */
typedef struct _tag_VOSthread_mutexattr
{
     V_UINT32 u32Reserve;  //保留，用于扩展.//android平台u32Reserve为非0时非递归锁
}ST_VOS_THD_MUTEX_ATTR;

/* 互斥结构 */
typedef struct _tag_VOSThread_Mutex
{
    V_UINT32 u32Res[16];
}ST_VOS_THD_MUTEX, *PST_VOS_THD_MUTEX;

/* 条件同步属性 */
typedef struct _tag_VOSthread_condattr
{
     V_UINT32 u32Reserve;  //保留
}ST_VOS_THD_COND_ATTR;

/* 本地时间 */
typedef struct _tagVOS_SysTime
{
    V_UINT32 u32MilliSec;  //毫秒
    V_UINT32 u32Sec;       //秒
    V_UINT32 u32Min;       //分
    V_UINT32 u32Hour;      //时
    V_UINT32 u32Day;       //日
    V_UINT32 u32Mon;       //月
    V_UINT32 u32Year;      //年
}ST_VOS_SYSTIME, *PST_VOS_SYSTIME;

/* EVENT */
typedef V_VOID* VOSEVENTHDL;

/* 无效句柄 */
#define VOS_INVALIDE_EVENT_HANDLE (VOSEVENTHDL)NULL

/* Semaphore属性 */
typedef struct _tag_VOSEventAttr
{
     V_UINT32 u32Reserve;  //保留
}ST_VOS_THD_EVENT_ATTR;

/* 信号量 */
typedef V_VOID* VOSSEMHDL;


/*****************************************************************************
 函 数 名  : VOS_CreateThread
 功能描述  : 创建线程
 输入参数  : IN const ST_VOS_THDATTR* stAttr:线程属性，结构见结构体定义，
                                             可选参数，不关注可传入NULL
             IN const PFNVOSTHD pfnThd:线程体函数指针，必须传入否则失败
             INOUT V_VOID* pvCtx:线程体函数上下文，线程体函数的参数，可选，
                                 不关注可传入NUL
 输出参数  : OUT VOSTHDHDL* pThdHdl:线程句柄指针，线程返回成功则非零，其他为0
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_NOMORE_RES:创建的线程数过多，本次创建不成功，请重试.
                      E_VOS_THD_ERR_ARGS: 传入属性错误或者线程体函数指针为NULL
                      E_VOS_THD_ERR_UNKOWN:未知错误
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_CreateThread(OUT VOSTHDHDL* pThdHdl,
                        IN const ST_VOS_THDATTR* pstAttr,
                        IN const PFNVOSTHD pfnThd,
                        INOUT V_VOID* pvCtx);

/*****************************************************************************
 函 数 名  : VOS_DetachThread
 功能描述  : 线程体和主线程分离，分离后，子线程退出时该线程完全销毁，
             但子线程退出不能精确控制
 输入参数  : IN VOSTHDHDL hThdHdl:线程句柄，VOS_CreateThread第一个参数输出值
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_FAIL: 失败
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_DetachThread(IN VOSTHDHDL hThdHdl);

/*****************************************************************************
 函 数 名  : VOS_JoinThread
 功能描述  : 同步等待线程退出
 输入参数  : INOUT VOSTHDHDL hThdHdl:线程句柄，VOS_CreateThread第一个参数输出值
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_FAIL: 失败
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_JoinThread(INOUT VOSTHDHDL hThdHdl);

/*****************************************************************************
 函 数 名  : VOS_SetThreadPriority
 功能描述  : 动态设置线程优先级
 输入参数  : IN VOSTHDHDL hThdHdl:要设置的线程句柄，通过VOS_CreateThread获得的
             IN V_INT32 i32Sched:要设置的调度策略，见E_VOS_THD_SCHED
             IN V_INT32 i32Priority:要设置的优先级，见E_VOS_THD_PRI
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_THD_DEAD_LOCK:线程被死锁了，等不出来了
                      E_VOS_THD_ERR_UNKOWN:未知错误
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_SetThreadPriority(IN VOSTHDHDL hThdHdl,
                                       IN V_INT32 i32Sched,
                                       IN V_INT32 i32Priority);

/*****************************************************************************
 函 数 名  : VOS_GetThreadPriority
 功能描述  : 获取线程优先级
 输入参数  : IN VOSTHDHDL hThdHdl:要设置的线程句柄，通过VOS_CreateThread获得的
 输出参数  : OUT V_INT32* pi32Sched:要获取的调度策略
             OUT V_INT32* pi32Priority:要获取的优先级指针，见E_VOS_THD_PRI
             两个输出参数可选择输出哪个，即不关注的可传入NULL，但必须有一个不为NULL
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 传入参数不正确
                      E_VOS_THD_ERR_UNKOWN:未知错误
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_GetThreadPriority(IN VOSTHDHDL hThdHdl,
                                       OUT V_INT32* pi32Sched,
                                       OUT V_INT32* pi32Priority);

/*****************************************************************************
 函 数 名  : VOS_GetSelfThreadId
 功能描述  : 获取自身线程ID
 输出参数  : 无
 返 回 值  : VOS_TID_T:返回线程ID
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOS_TID_T VOS_GetSelfThreadId();

/*****************************************************************************
 函 数 名  : VOS_GetSelfThdHdl
 功能描述  : 返回自身线程句柄,必须谨慎使用该接口，该接口返回的句柄，使用完后必须使用
             VOS_DetachThread()或者VOS_JoinThread来释放该句柄
 输出参数  : 无
 返 回 值  : VOSTHDHDL:返回自身线程句柄
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOSTHDHDL VOS_GetSelfThdHdl();

/*****************************************************************************
 函 数 名  : VOS_GetThdIdByHandle
 功能描述  : 由线程句柄获取其对应的线程ID
 输入参数  : IN VOSTHDHDL hHdl:线程句柄，通过VOS_CreateThread获得的
 输出参数  : OUT V_UINT32 *pu32ThdId:线程ID指针
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 传入参数不正确
                      其他:失败
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_GetThdIdByHandle(IN VOSTHDHDL hThdHdl, OUT VOS_TID_T *pThdId);

/*****************************************************************************
 函 数 名  : VOS_GetSelfPid
 功能描述  : 获取本进程ID
 输出参数  : 无
 返 回 值  : VOS_API VOS_PID_T:返回进程号
 修改历史      :
  1.日    期   : 2012年9月14日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOS_PID_T VOS_GetSelfPid();

/* 互斥 */
/* 默认初始化互斥锁，建议最好只用在全局变量 */
#ifdef WIN32
#define VOS_PTHREAD_MUTEX_INITIALIZER  (VOS_InitThdMutex_Def())
VOS_API ST_VOS_THD_MUTEX VOS_InitThdMutex_Def();
#else
#define VOS_PTHREAD_MUTEX_INITIALIZER {{0,}}
#endif

/*****************************************************************************
 函 数 名  : VOS_InitThdMutex
 功能描述  : 初始化线程间互斥对象
 输入参数  : IN const ST_THD_MUTEX_ATTR* stAttr:互斥属性，具体见其结构定义处，
                                                该参数为可选，不关注传入NULL，
                                                这里暂时保留
 输出参数  : OUT PST_VOS_THD_MUTEX pstMutex:Mutex对象指针
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 传入参数不正确
                      其他:失败
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_InitThdMutex(OUT PST_VOS_THD_MUTEX pstMutex,
                                  IN const ST_VOS_THD_MUTEX_ATTR* pstAttr);

/*****************************************************************************
 函 数 名  : VOS_DestroyThdMutex
 功能描述  : 去初始化线程互斥对象
 输入参数  : INOUT PST_VOS_THD_MUTEX pstMutex:Mutex对象指针，当传入为NULL时，
                                              直接返回E_VOS_THD_ERR_ARGS
 输出参数  : 无
 返 回 值  : V_VOID:无
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_DeInitThdMutex(INOUT PST_VOS_THD_MUTEX pstMutex);

/*****************************************************************************
 函 数 名  : VOS_ThdMutexLock
 功能描述  : 获得线程互斥锁
 输入参数  : IN PST_VOS_THD_MUTEX pstMutex:Mutex对象指针，当传入为NULL时，返回错误
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdMutexLock(IN PST_VOS_THD_MUTEX pstMutex);

/*****************************************************************************
 函 数 名  : VOS_ThdMutexUnLock
 功能描述  : 释放线程互斥锁
 输入参数  : IN PST_VOS_THD_MUTEX pstMutex:Mutex对象指针，当传入为NULL时，返回错误
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdMutexUnLock(IN PST_VOS_THD_MUTEX pstMutex);

//线程条件同步
/*****************************************************************************
 函 数 名  : VOS_CreateThdCond
 功能描述  : 创建线程同步对象
 输入参数  : IN const ST_THD_COND_ATTR* pstAttr:条件同步属性，具体见其结构定义处，
                                                该参数为可选，不关注传入NULL，这里暂时保留
 输出参数  : 无
 返 回 值  : V_VOID*:NULL失败，其他成功
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOSCONDHDL VOS_CreateThdCond(IN const ST_VOS_THD_COND_ATTR* pstAttr);

/*****************************************************************************
 函 数 名  : VOS_DestroyThdCond
 功能描述  : 销毁线程同步对象
 输入参数  : INOUT V_VOID* pvCond:VOS_CreateThdCond接口创建的对象，当传入为NULL时，
                                  直接返回
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_DestroyThdCond(INOUT VOSCONDHDL hCond);

/*****************************************************************************
 函 数 名  : VOS_ThdCondSignal
 功能描述  : 条件同步标记信号
 输入参数  : IN V_VOID* pvCond:VOS_CreateThdCond接口创建的对象，当传入为NULL时，
                               直接返回
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdCondSignal(IN VOSCONDHDL hCond);

/*****************************************************************************
 函 数 名  : VOS_ThdCondBrodcast
 功能描述  : 广播标记条件同步信号
 输入参数  : IN V_VOID* pvCond:VOS_CreateThdCond接口创建的对象，当传入为NULL时，
                               直接返回
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdCondBrodcast(IN VOSCONDHDL hCond);

/*****************************************************************************
 函 数 名  : VOS_ThdCondWait
 功能描述  : 无限等待线程条件同步信号
 输入参数  : IN V_VOID* pvCond:VOS_CreateThdCond接口创建的对象，当传入为NULL时，
                               直接返回
             IN PST_VOS_THD_MUTEX pvMutex:互斥锁对象，当其不为空时会先释放锁。
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 参数错误
                      其他:错误码
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdCondWait(IN VOSCONDHDL hCond, IN PST_VOS_THD_MUTEX pstMutex);

/*****************************************************************************
 函 数 名  : VOS_ThdCondTimeWait
 功能描述  : 超时等待线程条件同步信号
 输入参数  : IN V_VOID* pvCond:VOS_CreateThdCond接口创建的对象，当传入为NULL时，
                               直接返回错误
             IN PST_VOS_THD_MUTEX pstMutex:互斥锁对象，当其不为NULL时会先释放锁，
                                           为NULL时忽略。
             IN V_UINT32 u32MiliSecTimeOut://超时时间，单位ms
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 参数错误
                      E_VOS_THD_ERR_COND_TIMEOUT:超时
                      其他:错误码
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdCondTimeWait(IN VOSCONDHDL hCond,
                                    IN PST_VOS_THD_MUTEX pstMutex,
                                    IN V_UINT32 u32MiliSecTimeOut);

//睡眠
/*****************************************************************************
 函 数 名  : VOS_Sleep
 功能描述  : 秒级睡眠
 输入参数  : IN V_UINT32 u32Sec:睡眠时间，单位s
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_Sleep(IN V_UINT32 u32Sec);

/*****************************************************************************
 函 数 名  : VOS_MSleep
 功能描述  : 毫秒级睡眠
 输入参数  : IN V_UINT32 u32MilliSec:睡眠时间，单位ms
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_MSleep(IN V_UINT32 u32MilliSec);

//时间
/*****************************************************************************
 函 数 名  : VOS_GetTimeTicks
 功能描述  : 获取系统时间戳
 输出参数  : 无
 返 回 值  : V_UINT32:失败:0XFFFFFFFF
                      其他：系统时间戳
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_GetTimeTicks();

VOS_API V_UINT64 VOS_GetTimeTicks64();

/*****************************************************************************
 函 数 名  : VOS_GetLocalTime
 功能描述  : 获取系统本地时间
 输入参数  : 无
 输出参数  : OUT ST_VOS_SYSTIME* pstSysTime: 本地时间结构指针
             OUT V_CHAR* pszTime:本地时间字符串，格式按照YYYY:MM:DD:HH:MM:SS:MMM
                                 其长度>=24个字符
            两个参数都可选，不关注时传入NULL即可
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      其他:错误码
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_GetLocalTime(OUT ST_VOS_SYSTIME* pstSysTime,
                                  OUT V_CHAR* pszTime);


/* Event:仅支持，一时刻单个目标等待，及唤醒，同一时刻同时等待时不保证唤醒顺序 */
/*****************************************************************************
 函 数 名  : VOS_CreateThdEvent
 功能描述  : 创建事件对象
 输入参数  : IN const ST_VOS_THD_EVENT_ATTR* pstAttr:Event属性，具体见其结构
                                                   定义处，该参数为可选，不关注
                                                   传入NULL，这里暂时保留
 输出参数  : 无
 返 回 值  : V_VOID*:VOS_INVALIDE_EVENT_HANDLE失败，其他成功
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOSEVENTHDL VOS_CreateThdEvent(IN const ST_VOS_THD_EVENT_ATTR* pstAttr);

/*****************************************************************************
 函 数 名  : VOS_DestroyThdEvent
 功能描述  : 销毁事件对象
 输入参数  : INOUT VOSEVENTHDL pvCond: IN VOSCONDHDL hCondVOS_CreateThdSemaphore
                                   接口创建的对象，当传入为VOS_INVALIDE_THDSEM_HANDLE
                                   时，直接返回
 输出参数  : 无
 返 回 值  : V_VOID
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_DestroyThdEvent(IN VOSEVENTHDL pvEvent);

/*****************************************************************************
 函 数 名  : VOS_ThdEventSignal
 功能描述  : 事件触发
 输入参数  : IN VOSEVENTHDL pvCond:VOS_CreateThdEvent接口创建的对象，
                                       当传入为VOS_INVALIDE_EVENT_HANDLE时，
                                       直接返回
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdEventSignal(IN VOSEVENTHDL pvEvent);

/*****************************************************************************
 函 数 名  : VOS_ThdEventTimeWait
 功能描述  : 超时等待事件
 输入参数  : IN VOSEVENTHDL pvCond:VOS_CreateThdEvent接口创建的对象，
                                       当传入为VOS_INVALIDE_EVENT_HANDLE时，
                                       直接返回错误
             IN V_UINT32 u32TimeOutMs://超时时间，单位ms
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 参数错误
                      E_VOS_THD_ERR_COND_TIMEOUT:超时
                      其他:错误码
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdEventTimeWait(IN VOSEVENTHDL pvEvent,
                                      IN V_UINT32 u32TimeOutMs);

/*****************************************************************************
 函 数 名  : VOS_ThdEventWait
 功能描述  : 无限等待线程semaphore信号
 输入参数  : IN VOSEVENTHDL pvCond:VOS_CreateThdEvent接口创建的对象，
                                       当传入为VOS_INVALIDE_EVENT_HANDLE时，
                                       直接返回
 输出参数  : 无
 返 回 值  : V_UINT32:E_VOS_THD_ERR_NONE：成功.
                      E_VOS_THD_ERR_ARGS: 参数错误
                      其他:错误码
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdEventWait(IN VOSEVENTHDL pvEvent);

/* 信号量抽象 */
/*****************************************************************************
 函 数 名  : VOS_CreateThdSem
 功能描述  : 创建并初始化信号量
 输入参数  : V_UINT32 u32InitValue  : 信号量初始值
 输出参数  : 无
 返 回 值  : VOS_API VOSSEMHDL NULL 失败
 修改历史      :
  1.日    期   : 2013年5月3日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API VOSSEMHDL VOS_CreateThdSem(IN V_UINT32 u32InitValue);

/*****************************************************************************
 函 数 名  : VOS_DestroyThdSem
 功能描述  : 销毁信号量
 输入参数  : IN VOSSEMHDL hSem  : 信号量句柄
 输出参数  : 无
 返 回 值  : VOS_API V_VOID
 修改历史      :
  1.日    期   : 2013年5月3日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_DestroyThdSem(IN VOSSEMHDL hSem);

/*****************************************************************************
 函 数 名  : VOS_TmdSemPost
 功能描述  : 解锁一个信号量
 输入参数  : IN VOSSEMHDL hSem  : 信号量句柄
 输出参数  : 无
 返 回 值  : VOS_API V_VOID
 修改历史      :
  1.日    期   : 2013年5月3日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_VOID VOS_ThdSemPost(IN VOSSEMHDL hSem);

/*****************************************************************************
 函 数 名  : VOS_ThdSemWait
 功能描述  : 等待信号量
 输入参数  : IN VOSSEMHDL hSem  : 信号量句柄
 输出参数  : 无
 返 回 值  : VOS_API V_UINT32 : E_VOS_THD_ERR_NONE：成功
                              : E_VOS_THD_ERR_FAIL: 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月3日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdSemWait(IN VOSSEMHDL hSem);

/*****************************************************************************
 函 数 名  : VOS_ThdSemTimedWait
 功能描述  : 超时等待信号量
 输入参数  : IN VOSSEMHDL hSem         : 信号量句柄
             IN V_UINT32 u32TimeOutMs  : 超时时间(毫秒)
 输出参数  : 无
 返 回 值  : VOS_API V_UINT32 : E_VOS_THD_ERR_NONE：成功
                              : E_VOS_THD_ERR_COND_TIMEOUT: 超时
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月3日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
VOS_API V_UINT32 VOS_ThdSemTimedWait(IN VOSSEMHDL hSem, IN V_UINT32 u32TimeOutMs);



#ifdef __cplusplus
}
#endif

#endif// __OSAL_THREAD__
