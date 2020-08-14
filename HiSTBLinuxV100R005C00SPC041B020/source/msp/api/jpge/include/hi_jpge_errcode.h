/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name          : hi_jpge_errcode.h
Version            : Initial Draft
Author             :
Created            : 2016/03/03
Description        :
Function List      :


History            :
Date                  Author                Modification
2016/03/03            y00181162              Created file
******************************************************************************/
#ifndef __HI_JPGE_ERRCODE_H__
#define __HI_JPGE_ERRCODE_H__


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
#define HI_ID_JPGE 200

/* jpge start err no. */
#define HI_ERR_JPGE_BASE  ((HI_S32)( ((0x80UL + 0x20UL)<<24) | (HI_ID_JPGE << 16 ) | (4 << 13) | 1 ))


/*************************** Structure Definition ****************************/

/***************************  The enum of Jpeg image format  ******************/

enum
{
    HI_ERR_JPGE_DEV_NOT_OPEN = HI_ERR_JPGE_BASE, /**<  jpge device not open yet */
    HI_ERR_JPGE_DEV_OPEN_FAILED,                 /**<  open jpge device failed */
    HI_ERR_JPGE_NULL_PTR,                        /**<  input parameters contain null ptr */
    HI_ERR_JPGE_INVALID_HANDLE,                  /**<  invalid job handle */
    HI_ERR_JPGE_INVALID_PARA,                    /**<  invalid parameter */
    HI_ERR_JPGE_UNSUPPORT
};

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_JPGE_ERRCODE_H__ */
