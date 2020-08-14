/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name    : png_define.h
Version        : Initial Draft
Author        : z00141204
Created        : 2010/10/11
Description    : public macro definition
Function List     :

History           :
Date                Author                Modification
2010/10/11        z00141204        Created file
******************************************************************************/

#ifndef __PNG_DEFINE_H__
#define __PNG_DEFINE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif    /* __cplusplus */
#endif    /* __cplusplus */

#include <linux/semaphore.h>
#include <linux/sched.h>
#include <linux/hardirq.h>

#include "hi_kernel_adapt.h"

#include "hi_png_config.h"
#include "hi_gfx_comm_k.h"

#ifndef CONFIG_PNG_DEBUG_DISABLE
#define PNG_FATAL(fmt...)        HI_GFX_COMM_LOG_FATAL(HIGFX_PNG_ID, fmt)
#define PNG_ERROR(fmt...)        HI_GFX_COMM_LOG_ERROR(HIGFX_PNG_ID,fmt)
#define PNG_WARNING(fmt...)         HI_GFX_COMM_LOG_WARNING(HIGFX_PNG_ID,fmt)
#define PNG_INFO(fmt...)          HI_GFX_COMM_LOG_INFO(HIGFX_PNG_ID,fmt)
#define HIPNG_TRACE(fmt, args... )   HI_GFX_COMM_LOG_INFO(HIGFX_PNG_ID, fmt)
#else
#define PNG_FATAL(fmt...)
#define PNG_ERROR(fmt...)
#define PNG_WARNING(fmt...)
#define PNG_INFO(fmt...)
#define HIPNG_TRACE(fmt, args... )
#endif

#ifndef CONFIG_PNG_DEBUG_DISABLE
#define PNG_ASSERT(EXP)  do {\
    if(!(EXP))\
    {\
        PNG_ERROR( "Assertion [%s] failed! %s:%s(line=%d)\n",#EXP,__FILE__,__FUNCTION__,__LINE__);\
        panic("Assertion panic\n");\
    }\
} while(0)
#else
#define PNG_ASSERT(EXP)
#endif



#define PNG_DOWN_INTERRUPTIBLE(pmutex, ret)    do\
{\
    ret = 0;\
    if(!in_interrupt())\
    {\
        ret = down_interruptible(pmutex);\
        if (ret < 0)\
        {\
            PNG_ERROR("[%s:%d]down_interruptiblie failed!\n", __FUNCTION__, __LINE__);\
        }\
    }\
}while(0)

#define PNG_UP(pmutex) up(pmutex)

#define PNG_UP_INT(pmutex) do\
{\
    if(!in_interrupt())\
    {\
        up(pmutex);\
    }\
}while(0)

#define PNG_LOCK(lock,lockflags) spin_lock_irqsave(lock, lockflags)
#define PNG_UNLOCK(lock,lockflags) spin_unlock_irqrestore(lock, lockflags)

#define PNG_DECLARE_WAITQUEUE(queue_head) DECLARE_WAIT_QUEUE_HEAD(queue_head)
#define PNG_WAIT_EVENT_INTERRUPTIBLE(queue_head, condition) wait_event_interruptible(queue_head, (condition))
#define PNG_WAKE_UP_INTERRUPTIBLE(queue_head_p) wake_up_interruptible(queue_head_p)
#define PNG_WAIT_EVENT_INTERRUPTIBLE_TIMEOUT(queue_head, condition, timeout) wait_event_interruptible_timeout(queue_head, (condition), timeout)

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cplusplus */
#endif    /* __cplusplus */

#endif
