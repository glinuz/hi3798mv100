/***********************************************************************************
*             Copyright 2006 - 2006, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_adpt_thread.h
* Description:the interface of thread function adpter for all module 
*
* History:
* Version   Date         Author     DefectNum    Description
* 1.1       2006-04-10   q60002125  NULL         Create this file.
***********************************************************************************/




#ifndef __HI_ADPT_THREAD_H__
#define __HI_ADPT_THREAD_H__

#include "hi_type.h"
#include <stdarg.h>
//comment by x57522
/*
#if VTOP_OS_TYPE == VTOP_OS_LINUX
#include <errno.h>
#elif VTOP_OS_TYPE == VTOP_OS_WIN32
#else
#error YOU MUST DEFINE VTOP OS TYPE VTOP_OS_TYPE == VTOP_OS_WIN32 OR VTOP_OS_LINUX !  
#endif
*/
//add by x7522
#if HI_OS_TYPE == HI_OS_LINUX
#include <errno.h>
#elif HI_OS_TYPE == HI_OS_WIN32
#else
#error YOU MUST DEFINE HI OS TYPE HI_OS_TYPE == HI_OS_WIN32 OR HI_OS_LINUX !  
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */



typedef HI_U32 HI_Pthread_T;	
	
typedef HI_S32 HI_PID_T;

#define HI_ERR_OSCALL_ERROR  (-1)
#define HI_INFINITE            0xFFFFFFFF 
typedef HI_VOID * (*HI_ThreadFun)(HI_VOID *); 
/* Data structure to describe a process' schedulability.  */
typedef struct HI_sched_param
{
    HI_S32 sched_priority;
}HI_S_SchedParam;

/* Attributes for threads.  */
typedef struct HIpthread_attr_s
{
    HI_S32 detachstate;
    HI_S32 schedpolicy;
    HI_S_SchedParam schedparam;
    HI_S32 inheritsched;
    HI_S32 scope;
    HI_SIZE_T guardsize;
    HI_S32 stackaddr_set;
    HI_VOID *stackaddr;
    HI_SIZE_T stacksize;
} HI_S_ThreadAttr;

/* the thread priority */
typedef enum hiVpriority_which
{
    HI_PRIO_PROCESS = 0,             /* WHO is a process ID.  */
    HI_PRIO_PGRP = 1,                /* WHO is a process group ID.  */
    HI_PRIO_USER = 2                 /* WHO is a user ID.  */
}HI_E_Which;

typedef enum hiVrusage_who
{
    /* The calling process.  */
    HI_RUSAGE_SELF = 0,

    /* All of its terminated child processes.  */
    HI_RUSAGE_CHILDREN = -1,

    /* Both.  */
    HI_RUSAGE_BOTH = -2
}HI_E_Who;

/* get errno */
HI_S32 HI_GetLastErr(HI_VOID);
/* set errno */
HI_VOID HI_SetLastErr(HI_S32 newErrNo);

/************************************************************************
|                                                                                                                             |
|                             process operation                                                                      |
|                                                                                                                             |
************************************************************************/

HI_S32 HI_Execv(const HI_CHAR *path, HI_CHAR *const argv[]);
HI_S32 HI_Execvp(const HI_CHAR *file, HI_CHAR *const argv[]);
HI_S32 HI_Waitpid(HI_PID_T pid,  HI_S32 *status, HI_S32 options);


#ifdef HI_OS_SUPPORT_UCLINUX   
#define HI_Fork vfork
#else
HI_PID_T HI_Fork(HI_VOID);
#endif
HI_PID_T HI_Wait(const HI_S32 *status);


#define HI_REBOOT_CMD_RESTART        0x01234567
#define HI_REBOOT_CMD_HALT           0xCDEF0123
#define HI_REBOOT_CMD_CAD_ON         0x89ABCDEF
#define HI_REBOOT_CMD_CAD_OFF        0x00000000
#define HI_REBOOT_CMD_POWER_OFF      0x4321FEDC
#define HI_REBOOT_CMD_RESTART2       0xA1B2C3D4

HI_S32 HI_Reboot(HI_S32 flag);


/************************************************************************
|                                                                                                                             |
|                            thread operation                                                                         |
|                                                                                                                             |
************************************************************************/

/* Function for handling threads.  */

HI_S32 HI_PthreadAttrInit(HI_S_ThreadAttr *attr);

HI_S32 HI_PthreadAttrDestroy(HI_S_ThreadAttr *attr);

HI_S32 HI_PthreadAttrSetdetachstate(HI_S_ThreadAttr *attr, HI_S32 detachstate);

HI_S32 HI_PthreadSetCancelState(HI_S32 state, HI_S32 *oldstate);

HI_S32 HI_PthreadSetCancelType(HI_S32 type, HI_S32 *oldtype);

/* Indicate that the thread TH is never to be joined with PTHREAD_JOIN.
   The resources of TH will therefore be freed immediately when it
   terminates, instead of waiting for another thread to perform PTHREAD_JOIN
   on it.  */
HI_S32 HI_PthreadDetach (HI_Pthread_T th) ;

HI_S32 HI_GetPriority(HI_S32 which, HI_S32 who);

HI_S32 HI_SetPriority(HI_S32 which, HI_S32 who, HI_S32 prio);



/* Create a thread with given attributes ATTR (or default attributes
   if ATTR is HI_NULL), and call function START_ROUTINE with given
   arguments ARG.  */
HI_S32  HI_PthreadCreate (HI_Pthread_T *threadp,
                                                              const HI_S_ThreadAttr *attr,
                                                              HI_ThreadFun start_routine,
                                                              HI_VOID *arg) ;

/* Obtain the identifier of the current thread.  */
HI_Pthread_T  HI_PthreadSelf (HI_VOID);

/* Terminate calling thread.  */
HI_VOID HI_PthreadExit ( HI_VOID* retVal);
/*pthread cancel*/
 HI_S32 HI_PthreadCancel(HI_Pthread_T thread);
/* Make calling thread wait for termination of the thread TH.  The
   exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
   is not HI_NULL.  */
HI_S32 HI_PthreadJoin (HI_Pthread_T th, HI_VOID **thread_return);


/* sleep in second */
HI_U32 HI_Sleep(HI_U32 seconds);
/* sleep in millisecond */
HI_U32 HI_SleepMs(HI_U32 ms);

/* get current thread pid*/
HI_U32 HI_GetPID(HI_VOID);

/**/
HI_VOID HI_Exit(HI_S32 status);

HI_S32 HI_Kill(HI_PID_T pid, HI_S32 sig);

HI_S32 HI_System(HI_CHAR *cmd);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* __HI_ADPT_THREAD_H__ */
