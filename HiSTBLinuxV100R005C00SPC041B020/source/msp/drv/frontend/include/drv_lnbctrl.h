/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_lnbctrl.h
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
 * \brief For LNB control chipsets, e.g. MPS8125, ISL9492.
 */
#ifndef __DRV_LNBCTRL_H__
#define __DRV_LNBCTRL_H__

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include <linux/semaphore.h>
#include "drv_tuner_ioctl.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

/*-------------------- GLOBAL STRUCTURE DECLARATIONS -----------------------------------*/

typedef struct LNBCTRL_DEV_S
{
    HI_U32                  u32TunerPort;
    HI_U32                  u32I2CNum;
    HI_U8                   u8DevAddr;
    HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType;
    HI_BOOL                 bInited;
    TUNER_LNB_OUT_E         enLNBOut;
    struct LNBCTRL_DEV_S*   pstNext;
} LNBCTRL_DEV_PARAM_S;

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

LNBCTRL_DEV_PARAM_S* lnbctrl_queue_insert(HI_U32 u32TunerPort, HI_U32 u32I2CNum,
                                          HI_U8 u8DevAddr, HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType);
LNBCTRL_DEV_PARAM_S* lnbctrl_queue_get(HI_U32 u32TunerPort);
HI_VOID              lnbctrl_queue_remove(HI_U32 u32TunerPort);

#endif /* __LNBCTRL_COMMON_H__ */
