/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : jpge_osal.c
Version             : Initial Draft
Author              :
Created             : 2016/03/03
Description         :
                      CNcomment:  CNend\n
Function List       :


History             :
Date                        Author                  Modification
2016/03/03                  y00181162               Created file
******************************************************************************/


/*********************************add include here******************************/
#include "jpge_osal.h"
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include "hi_gfx_comm_k.h"



/***************************** Macro Definition ******************************/

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/

/********************** API forward declarations     **************************/

static HI_VOID (*ptrJpgeCallBack)(HI_VOID);



/********************** API forward release          **************************/

static irqreturn_t JpgeOsal_JencISR(HI_S32 Irq, HI_VOID* DevID)
{
    (*ptrJpgeCallBack)();
    return IRQ_HANDLED;
}


/***************************************************************************
* func          : JpgeOsal_IrqInit
* description   :
                  CNcomment: 中断初始化 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_IrqInit( HI_U32 Irq, HI_VOID (*ptrCallBack)(HI_VOID) )
{

    HI_S32 s32Ret = HI_SUCCESS;
    ptrJpgeCallBack = ptrCallBack;

    if (HI_SUCCESS != request_irq(Irq, JpgeOsal_JencISR, IRQF_DISABLED, "hi_jpge_irq", NULL) )
    {
        return HI_FAILURE;
    }

    s32Ret = HI_GFX_SetIrq(HIGFX_JPGENC_ID,Irq);
    if (HI_SUCCESS != s32Ret)
    {
        free_irq(Irq, NULL);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/***************************************************************************
* func          : JpgeOsal_IrqFree
* description   :
                  CNcomment: 释放中断响应 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_IrqFree( HI_U32 Irq )
{
    free_irq(Irq, NULL);

    return HI_SUCCESS;
}


/***************************************************************************
* func          : JpgeOsal_LockInit
* description   : init lock
                  CNcomment: 锁初始化 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_LockInit( JPGE_LOCK_S *pLock )
{
    spin_lock_init( pLock );

    return HI_SUCCESS;
}

/***************************************************************************
* func          : JpgeOsal_Lock
* description   : lock
                  CNcomment: 加锁 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_VOID JpgeOsal_Lock( JPGE_LOCK_S *pLock, JPGE_LOCK_FLAG *pFlag )
{
    spin_lock_irqsave((spinlock_t *)pLock, *pFlag);
}

/***************************************************************************
* func          : JpgeOsal_Unlock
* description   : unlock
                  CNcomment: 解锁 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_VOID JpgeOsal_Unlock( JPGE_LOCK_S *pLock, JPGE_LOCK_FLAG *pFlag )
{
    spin_unlock_irqrestore((spinlock_t *)pLock, *pFlag);
}



/***************************************************************************
* func          : JpgeOsal_MutexInit
* description   : init mutex
                  CNcomment: 信号量初始化 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_MutexInit( JPGE_SEM_S *pMutex )
{
    sema_init(pMutex,1);
    return HI_SUCCESS;
}

/***************************************************************************
* func          : JpgeOsal_MutexLock
* description   : lock with mutex
                  CNcomment: 使用信号量锁 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_MutexLock( JPGE_SEM_S *pMutex )
{
    if (down_interruptible(pMutex))
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/***************************************************************************
* func          : JpgeOsal_MutexUnlock
* description   : unlock with mutex
                  CNcomment: 使用信号量解锁 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_S32 JpgeOsal_MutexUnlock(JPGE_SEM_S *pMutex)
{
    up(pMutex);
    return HI_SUCCESS;
}



/***************************************************************************
* func          : JpgeOsal_MapRegisterAddr
* description   : map register ddr
                  CNcomment: 映射寄存器地址 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_VOID* JpgeOsal_MapRegisterAddr( HI_U32 phyAddr, HI_U32 len )
{
    return HI_GFX_REG_MAP( phyAddr, len );
}


/***************************************************************************
* func          : JpgeOsal_UnmapRegisterAddr
* description   : unmap register ddr
                  CNcomment: 逆映射寄存器地址 CNend\n
* retval        : NA
* others:       : NA
****************************************************************************/
HI_VOID JpgeOsal_UnmapRegisterAddr( HI_VOID* pVir )
{
    HI_GFX_REG_UNMAP( pVir);
    return;
}
