/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_suspend.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : the suspend dispose
                  CNcomment: 待机处理 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_HDEC_SUSPEND_H__
#define __JPEG_HDEC_SUSPEND_H__


/*********************************add include here******************************/
#include "hi_jpeg_config.h"

#ifdef CONFIG_JPEG_SUSPEND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include  "jpeglib.h"
#include  "hi_type.h"

#include  "jpeg_hdec_api.h"

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/
    /** \addtogroup      JPEG SUSPEND MACRO */
    /** @{ */  /** <!-- 【JPEG SUSPEND MACRO】 */

     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/

    /** \addtogroup      JPEG SUSPEND ENUM */
    /** @{ */  /** <!-- 【JPEG SUSPEND ENUM】 */


    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/
    /** \addtogroup      JPEG SUSPEND STRUCTURE */
    /** @{ */  /** <!-- 【JPEG SUSPEND STRUCTURE】 */


    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /** \addtogroup      JPEG SUSPEND API */
    /** @{ */  /** <!-- 【JPEG SUSPEND API】 */

    /*****************************************************************************
    * func            : JPEG_HDEC_GetSuspendSignal
    * description    : get the suspend signal
                      CNcomment:  获取待机信号                  CNend\n
    * param[in]     :  *pJpegHandle   CNcomment:  解码器句柄  CNend\n
    * param[out]     : bSuspendSignal  CNcomment:  待机信号    CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_GetSuspendSignal(const JPEG_HDEC_HANDLE_S  *pJpegHandle, \
                                                         HI_BOOL *bSuspendSignal);

    /*****************************************************************************
    * func            : JPEG_HDEC_GetResumeSignal
    * description    : get the resume signal
                      CNcomment:  获取待机唤醒信号               CNend\n
    * param[in]     :  *pJpegHandle   CNcomment:   解码器句柄  CNend\n
    * param[out]     : bResumeSignal   CNcomment:   待机唤醒信号 CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_GetResumeSignal(const JPEG_HDEC_HANDLE_S  *pJpegHandle,\
                                                        HI_BOOL *bResumeSignal);

    /*****************************************************************************
    * func            : JPEG_HDEC_Resume
    * description    : suspend resume
                      CNcomment: 待机唤醒 CNend\n
    * param[in]     : cinfo       CNcomment:  解码对象         CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_Resume(const struct jpeg_decompress_struct *cinfo);

    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* END IF -DCONFIG_JPEG_SUSPEND*/


#endif /* __JPEG_HDEC_SUSPEND_H__*/
