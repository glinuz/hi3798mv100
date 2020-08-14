/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_lnbctrl.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/25
  Description   :
  History       :
  1.Date        : 2012/02/25
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For LNB control device, e.g. MPS8125 ISL9492
 */

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include <linux/delay.h>
#include <linux/vmalloc.h>

#include "drv_lnbctrl.h"
#include "hi_drv_mem.h"
#include "hi_kernel_adapt.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

#define CHECK_INIT \
    if (!s_bInited) \
    { \
        HI_INIT_MUTEX(&(s_stSemaphore)); \
        s_bInited = HI_TRUE; \
    }

#define LNBCTRL_COMMON_LOCK do \
    { \
        HI_S32 lockRet; \
        if (1) \
        { \
            lockRet = down_interruptible(&(s_stSemaphore)); \
        } \
    } while (0)
    
#define LNBCTRL_COMMON_UNLOCK up(&(s_stSemaphore))

/*-------------------- STATIC STRUCTURE DECLARATIONS -----------------------------------*/

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

/*------------------------- STATIC DECLARATIONS ----------------------------------------*/

static struct semaphore s_stSemaphore;
static HI_BOOL s_bInited = HI_FALSE;
static LNBCTRL_DEV_PARAM_S* s_pstLNBCtrlDevHead = HI_NULL;

/*------------------------------------ CODE --------------------------------------------*/

/* Malloc and insert a node */
LNBCTRL_DEV_PARAM_S* lnbctrl_queue_insert(HI_U32 u32TunerPort, HI_U32 u32I2CNum,
                                          HI_U8 u8DevAddr, HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType)
{
    LNBCTRL_DEV_PARAM_S* pstNew;
    LNBCTRL_DEV_PARAM_S* pstTail = s_pstLNBCtrlDevHead;

    CHECK_INIT;

    pstNew = HI_VMALLOC(HI_ID_TUNER, sizeof(LNBCTRL_DEV_PARAM_S));
    if (HI_NULL == pstNew)
    {
        HI_ERR_TUNER("LNB ctrl dev vmalloc fail.\n");
        return HI_NULL;
    }

    pstNew->u32TunerPort = u32TunerPort;
    pstNew->u32I2CNum = u32I2CNum;
    pstNew->u8DevAddr = u8DevAddr;
    pstNew->enDemodDevType = enDemodDevType;
    pstNew->bInited  = HI_FALSE;
    pstNew->enLNBOut = TUNER_LNB_OUT_0V;
    pstNew->pstNext = HI_NULL;

    LNBCTRL_COMMON_LOCK;
    if (HI_NULL == pstTail)
    {
        s_pstLNBCtrlDevHead = pstNew;
    }
    else
    {
        while (HI_NULL != pstTail->pstNext)
        {
            pstTail = pstTail->pstNext;
        }

        pstTail->pstNext = pstNew;
    }
    LNBCTRL_COMMON_UNLOCK;

    return pstNew;
}

/* Find and return a node */
LNBCTRL_DEV_PARAM_S* lnbctrl_queue_get(HI_U32 u32TunerPort)
{
    LNBCTRL_DEV_PARAM_S* pstTemp = s_pstLNBCtrlDevHead;

    while (HI_NULL != pstTemp)
    {
        if (u32TunerPort == pstTemp->u32TunerPort)
        {
            break;
        }

        pstTemp = pstTemp->pstNext;
    }

    return pstTemp;
}

/* Remove a node */
HI_VOID lnbctrl_queue_remove(HI_U32 u32TunerPort)
{
    LNBCTRL_DEV_PARAM_S* pstTemp = s_pstLNBCtrlDevHead;
    LNBCTRL_DEV_PARAM_S* pstLast = s_pstLNBCtrlDevHead;

    CHECK_INIT;

    LNBCTRL_COMMON_LOCK;
    while (HI_NULL != pstTemp)
    {
        if (u32TunerPort == pstTemp->u32TunerPort)
        {
            if (pstTemp == s_pstLNBCtrlDevHead)
            {
                s_pstLNBCtrlDevHead = pstTemp->pstNext;
            }

            pstLast->pstNext = pstTemp->pstNext;
            HI_VFREE(HI_ID_TUNER, pstTemp);
            break;
        }

        pstLast = pstTemp;
        pstTemp = pstTemp->pstNext;
    }
    LNBCTRL_COMMON_UNLOCK;
}
