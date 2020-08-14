/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpge_henc_err.h
Version            : Initial Draft
Author            :
Created            : 2014/05/14
Description        : The user will use this api to realize some function
Function List     :


History           :
Date                Author                Modification
2014/05/14            y00181162              Created file
******************************************************************************/

#ifndef __JPGE_HENC_ERR_H__
#define __JPGE_HENC_ERR_H__


#ifdef CONFIG_GFX_JPGE_ENC_ENABLE

/*********************************add include here******************************/

#include  "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



     /***************************** Macro Definition ******************************/

     #define HI_ID_JPGE 200

     #define ERR_JPGE_BASE    \
              ((HI_S32)( ((0x80UL + 0x20UL)<<24) | (HI_ID_JPGE << 16 ) | (4 << 13) | 1 ))

     /*************************** Enum Definition ****************************/

    typedef enum tagJPEG_HENC_ERR_E
    {
        JPGE_DEV_NOT_OPEN      = ERR_JPGE_BASE    , /**<  jpge device not open yet          */
        JPGE_DEV_OPEN_FAILED,                      /**<  open jpge device failed           */
        JPGE_NULL_PTR,                              /**<  input parameters contain null ptr */
        JPGE_INVALID_HANDLE,                       /**<  invalid job handle                */
        JPGE_INVALID_PARA,                         /**<  invalid parameter                 */
        JPGE_UNSUPPORT
    }JPEG_HENC_ERR_E;

    /*************************** Structure Definition ****************************/

    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/


    /** \addtogroup      JPGE */
    /** @{ */    /** <!--[JPGE]*/



    /** @} */  /** <!-- ==== API Declaration End ==== */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

    }
#endif
#endif /* __cplusplus */

#endif/** CONFIG_GFX_JPGE_ENC_ENABLE **/

#endif /* __JPGE_HENC_ERR_H__*/
