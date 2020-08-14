/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : stat.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/12/16
  Last Modified :
  Description   : interrupt and thread timing measurement tool
  Function List :
  History       :
  1.Date        : 2008/12/16
    Author      : z42136
    Modification: Created file

******************************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <sys/time.h>
#include "hi_common.h"
#include "hi_drv_struct.h"
#include "hi_mpi_stat.h"
#include "drv_stat_ioctl.h"
#include "hi_module.h"
#include "hi_osal.h"
#include "hi_mpi_mem.h"
//#define __STAT_USE_HW_TIMER__

#define CALLING_USED
HI_S32 s_s32StatFd = -1;
#ifdef __STAT_USE_HW_TIMER__
volatile HI_VOID * g_Timer7_addr = NULL;
#endif

static pthread_mutex_t   s_StatMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_STAT_LOCK()     (void)pthread_mutex_lock(&s_StatMutex);
#define HI_STAT_UNLOCK()   (void)pthread_mutex_unlock(&s_StatMutex);


HI_U32 STAT_GetTimer0Addr(HI_VOID)
{
    HI_SYS_VERSION_S stSysChipInfo;
    HI_U32 u32RegisterAddr = 0;

    memset(&stSysChipInfo, 0, sizeof(stSysChipInfo));
    
    if (HI_SYS_GetVersion(&stSysChipInfo))
    {
        return 0;
    }

    if ( (stSysChipInfo.enChipTypeHardWare != HI_CHIP_TYPE_BUTT) && (stSysChipInfo.enChipVersion == HI_CHIP_VERSION_V300) )
    {
        u32RegisterAddr = 0x10206020;
    }
    else
    {
        u32RegisterAddr = 0x101e5020;
    }

    return u32RegisterAddr;
}

HI_S32 HI_MPI_STAT_Init(HI_VOID)
{
#ifdef __STAT_USE_HW_TIMER__
    HI_U32 RegAddr;
    HI_U32 RegValue;
#endif

    HI_STAT_LOCK();
    
    if(s_s32StatFd == -1)
    {
        s_s32StatFd = open("/dev/"UMAP_DEVNAME_STAT, O_RDWR);
        if(-1 == s_s32StatFd)
        {
            HI_STAT_UNLOCK();
            HI_ERR_STAT("ERROR: can not open stat device.\n");
            return HI_FAILURE ;
        }

#ifdef __STAT_USE_HW_TIMER__
        if(g_Timer7_addr == NULL)
        {
            RegAddr = STAT_GetTimer0Addr();
            if (0 == RegAddr)
            {
                close(s_s32StatFd);
                s_s32StatFd = -1;
                HI_STAT_UNLOCK();
                return HI_FAILURE ;
            }
            
            g_Timer7_addr = (HI_VOID *)HI_MMAP(RegAddr, 4000);
            if (NULL == g_Timer7_addr)
            {
                close(s_s32StatFd);
                s_s32StatFd = -1;
                
                HI_STAT_UNLOCK();
                
                HI_ERR_STAT("ERROR: map timer reg address error.\n");
                return HI_FAILURE ;
            }

            RegValue = 0xffffffff;
            HI_REG_WRITE32((HI_U32 *)g_Timer7_addr, RegValue);

            /* config timer control, offset 0x28 */
            HI_REG_READ32((HI_U32 *)((HI_U32)g_Timer7_addr+8), RegValue);

            /* reserved high 24-bit and 4th-bit */
            RegValue &= 0xffffff10;

            /* config timer control register. ref TIMERx_CONTROL */
            RegValue |= 0x000000e2;
            HI_REG_WRITE32((HI_U32 *)((HI_U32)g_Timer7_addr+8), RegValue);

            usleep(1*1000);
        }
#endif
    }

    HI_STAT_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_STAT_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_STAT_LOCK();

    if(s_s32StatFd != -1)
    {
        s32Ret = close(s_s32StatFd);

        s_s32StatFd = -1;

#ifdef __STAT_USE_HW_TIMER__
        if (g_Timer7_addr)
        {
            HI_MUNMAP((void *)g_Timer7_addr);
            g_Timer7_addr = NULL;
        }
#endif 
    }

    HI_STAT_UNLOCK();

    return s32Ret;
}

#ifdef __STAT_USE_HW_TIMER__
// 270MHz / 256
//#define TIMER_PRE 1054687
#define TIMER_PRE 27000000
#endif

#ifdef CALLING_USED
HI_S32 stat_fast_get_time(struct timeval * tv)
{
#ifdef __STAT_USE_HW_TIMER__
    /* use TIMER0: 270MHz, 256 de-frequency*/
    HI_U32 time_now;
    long long time_diff;

    time_now = *((HI_U32 *)(g_Timer7_addr+4));
    time_diff = 0xffffffff - time_now;
//    HI_ERR_STAT("time_now = %x, time_diff = %d\n", time_now, time_diff);
    tv->tv_sec = (time_diff / TIMER_PRE);
    tv->tv_usec = (time_diff % TIMER_PRE) * 1000000 / TIMER_PRE;
#else
    /*use gettimeofday first, may be change to read hardware timer to get time subsequently */
    (HI_VOID)gettimeofday(tv, NULL);
#endif

    return HI_SUCCESS;
}


HI_S32 HI_MPI_STAT_ThreadReset(HI_STAT_HANDLE handle)
{
    handle->stat_thread_uvirtaddr->avg_time = 0;
    handle->stat_thread_uvirtaddr->min_time = 0;
    handle->stat_thread_uvirtaddr->max_time = 0;

    handle->counter = 0;
    handle->time_total = 0;

    memset(&handle->tv_last, 0, sizeof(struct timeval));

    return HI_FAILURE;
}

HI_S32 HI_MPI_STAT_ThreadResetAll(HI_VOID)
{
    HI_S32 ret;
    HI_BOOL bNeedOpen = HI_FALSE;
	
	HI_STAT_LOCK();

    if(s_s32StatFd == -1)
    {
        bNeedOpen = HI_TRUE;
        HI_MPI_STAT_Init();
    }

    ret = ioctl(s_s32StatFd, UMAPC_CMPI_STAT_RESETALL);
    if(ret != HI_SUCCESS)
    {
        HI_STAT_UNLOCK();
        HI_ERR_STAT("ioctl of UMAPC_CMPI_STAT_RESETALL err = %x\n", ret);
        return HI_FAILURE;
    }

    HI_STAT_UNLOCK();

    if(bNeedOpen == HI_TRUE)
    {
        HI_MPI_STAT_DeInit();
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_STAT_ThreadRegister(char * name, HI_STAT_HANDLE * pHandle)
{
    HI_S32 ret;

    *pHandle = (HI_STAT_HANDLE)HI_MALLOC(HI_ID_MEM, sizeof(HI_STAT_HANDLE_S));
	if (NULL == * pHandle)
	{
		return HI_FAILURE;
	}

    HI_STAT_LOCK();
    ret = ioctl(s_s32StatFd, UMAPC_CMPI_STAT_REGISTER, &((*pHandle)->stat_thread_phyaddr));
    if(ret != HI_SUCCESS)
    {
        HI_STAT_UNLOCK();
        
        HI_ERR_STAT("ioctl of UMAPC_CMPI_STAT_REGISTER err = %x\n", ret);

        free(*pHandle);
        *pHandle = NULL;

        return HI_FAILURE;
    }

    HI_STAT_UNLOCK();

    (*pHandle)->stat_thread_uvirtaddr = (STAT_USERSPACE_S *)HI_MMAP((*pHandle)->stat_thread_phyaddr, sizeof(STAT_USERSPACE_S));
    if (NULL == ((*pHandle)->stat_thread_uvirtaddr))
    {
        free(*pHandle);
        *pHandle = NULL;
        return HI_FAILURE;
    }
    HI_OSAL_Strncpy((*pHandle)->stat_thread_uvirtaddr->name, name, sizeof(THREAD_NAME)-1);

    return HI_SUCCESS;
}


HI_S32 HI_MPI_STAT_ThreadUnregister(HI_STAT_HANDLE * pHandle)
{
	if (NULL == (*pHandle)->stat_thread_uvirtaddr)
	{
		return HI_FAILURE;
	}
    memset((*pHandle)->stat_thread_uvirtaddr, 0, sizeof(STAT_USERSPACE_S));
    if ((void *)(*pHandle)->stat_thread_uvirtaddr)
    {
        (HI_VOID)HI_MUNMAP((void *)(*pHandle)->stat_thread_uvirtaddr);
    }
    HI_FREE(HI_ID_MEM, *pHandle);
    (*pHandle) = NULL;

    return HI_SUCCESS;
}

#define STAT_TIME_COST(tv_s,tv_e)    \
    ((tv_e.tv_sec-tv_s.tv_sec)*1000000 + (tv_e.tv_usec - tv_s.tv_usec))

HI_S32 HI_MPI_STAT_ThreadProbe(HI_STAT_HANDLE handle)
{
    struct timeval tv_tmp;
    HI_U32 time_cost;

    stat_fast_get_time(&tv_tmp);

    if((handle->tv_last.tv_sec == 0) && (handle->tv_last.tv_usec == 0))
    {
        /*first probe*/
        handle->tv_last.tv_sec = tv_tmp.tv_sec;
        handle->tv_last.tv_usec = tv_tmp.tv_usec;

        return HI_SUCCESS;
    }

    handle->counter++;
    time_cost = (HI_U32)STAT_TIME_COST(handle->tv_last, tv_tmp);

    handle->tv_last.tv_sec = tv_tmp.tv_sec;
    handle->tv_last.tv_usec = tv_tmp.tv_usec;

    if(time_cost > handle->stat_thread_uvirtaddr->max_time)
    {
        handle->stat_thread_uvirtaddr->max_time = time_cost;
    }

    if((handle->stat_thread_uvirtaddr->min_time == 0) ||
        (time_cost < handle->stat_thread_uvirtaddr->min_time))
    {
        handle->stat_thread_uvirtaddr->min_time = time_cost;
    }

    handle->time_total += (HI_U64)time_cost;
    handle->stat_thread_uvirtaddr->avg_time = (HI_U32)((handle->time_total)/((HI_U64)(handle->counter)));

    return HI_FAILURE;
}
#endif

HI_S32 HI_MPI_STAT_Event(STAT_EVENT_E enEvent, HI_U32 Value)
{
    STAT_EVENT_S   StatEvent;
    HI_S32         Ret;

    StatEvent.enEvent = enEvent;
    StatEvent.Value = Value;

    HI_STAT_LOCK();

    Ret = ioctl(s_s32StatFd, UMAPC_CMPI_STAT_EVENT, &StatEvent);
    if(Ret != HI_SUCCESS)
    {
        HI_STAT_UNLOCK();
        
        HI_ERR_STAT("ioctl of UMAPC_CMPI_STAT_EVENT err = %x\n", Ret);
        return HI_FAILURE;
    }

    HI_STAT_UNLOCK();

    return HI_SUCCESS;
}

#ifdef CALLING_USED
HI_U32 HI_MPI_STAT_GetTick(HI_VOID)
{
    HI_S32 Ret;
    HI_U32 Tick;

    HI_STAT_LOCK();
    
    Ret = ioctl(s_s32StatFd, UMAPC_CMPI_STAT_GETTICK, &Tick);
    if(Ret != HI_SUCCESS)
    {
        HI_STAT_UNLOCK();
        
        HI_ERR_STAT("ioctl of UMAPC_CMPI_STAT_GETTICK err = %x\n", Ret);
        return 0;
    }

    HI_STAT_UNLOCK();

    return Tick;
}
#endif


HI_S32 HI_MPI_STAT_NotifyLowDelayEvent (HI_LD_Event_S * evt)
{
    HI_S32 Ret;

    if (!evt)
        return HI_FAILURE;

    Ret = ioctl(s_s32StatFd, UMAPC_CMPI_STAT_LD_EVENT, evt);
    if(Ret != HI_SUCCESS)
    {  
        HI_ERR_STAT("ioctl of UMAPC_CMPI_STAT_LD_EVENT err = %x\n", Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}