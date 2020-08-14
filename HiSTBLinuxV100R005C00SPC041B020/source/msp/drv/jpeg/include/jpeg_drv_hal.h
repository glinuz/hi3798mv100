/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_drv_hal.h
Version            : Initial Draft
Author            :
Created            : 2016/02/02
Description        :
Function List     :


History           :
Date                Author                Modification
2016/02/02            y00181162              Created file
******************************************************************************/
#ifndef  _JPEG_DRV_HAL_H_
#define  _JPEG_DRV_HAL_H_


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


    /*************************** Structure Definition ****************************/

    /***************************  The enum of Jpeg image format  ******************/

    /********************** Global Variable declaration **************************/


    /******************************* API declaration *****************************/



    /*****************************************************************************
    * func              : JPEG_HAL_Init
    * description      : initial the jpeg device
    * param[in]       : none
    * retval          : none
    * output          : none
    * others:          : nothing
    *****************************************************************************/
    HI_VOID JPEG_HAL_Init(volatile HI_U32 *pu32JpegRegBase);


     /*****************************************************************************
    * func              : JPEG_HAL_DInit
    * description      : exit initial the jpeg device
    * param[in]       : none
    * retval          : none
    * output          : none
    * others:          : nothing
    *****************************************************************************/

    HI_VOID JPEG_HAL_DInit(HI_VOID);


    HI_VOID DRV_JPEG_GetIntStatus(HI_U32 *pIntStatus);

    HI_VOID DRV_JPEG_ClearIntStatus(HI_U32 IntStatus);

    HI_VOID DRV_JPEG_SetIntMask(HI_U32 IntMask);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
