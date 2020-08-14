/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name       : drv_hifb_err.h
Version         : Initial Draft
Author          :
Created         : 2016/02/02
Description     :
Function List   :


History         :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef __DRV_HIFB_ERR_H__
#define __DRV_HIFB_ERR_H__


/*********************************add include here******************************/
#include "hi_type.h"


/*****************************************************************************/


/*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define DRV_HIFB_ERR_BASE   (0x80000000L + 0x10000000L)

/*************************** Structure Definition ****************************/

typedef enum
{
    DRV_HIFB_ERR_HWCREFRESH = DRV_HIFB_ERR_BASE,
    DRV_HIFB_ERR_OTHERREFRESH,
    DRV_HIFB_ERR_SETLAYERINFO,
    DRV_HIFB_ERR_SETSCREENSIZE,
    DRV_HIFB_ERR_WAITEREFRESHDONE,
    DRV_HIFB_ERR_SCROLLTEXT,
    DRV_HIFB_ERR_SETLAYERPRIORITY,
    DRV_HIFB_ERR_COMPRESS,
    DRV_HIFB_ERR_DECOMPRESS,
    DRV_HIFB_ERR_BUTT
}DRV_HIFB_ERR_E;

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
