/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_error.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : Describes the error codes of the Jpeg
                  CNcomment: Jpeg错误码文件 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_HDEC_ERROR_H__
#define __JPEG_HDEC_ERROR_H__


/*********************************add include here******************************/

#include "hi_type.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */




     /*************************** Enum Definition ****************************/

    /** \addtogroup      JPEG ERROR ENUM */
    /** @{ */  /** <!-- 【JPEG ERROR ENUM】 */


    /** the error level */
    /** CNcomment:错误吗级别 */
    typedef enum tagJPEG_LOG_ERRLEVEL_E
    {
        JPEG_LOG_LEVEL_DEBUG    = 0,     /**< debug level                      *//**<CNcomment:调试级别       */
        JPEG_LOG_LEVEL_INFO     = 1,       /**< informational                    *//**<CNcomment:信息           */
        JPEG_LOG_LEVEL_NOTICE   = 2,     /**< normal but significant condition *//**<CNcomment:正常当重要条件 */
        JPEG_LOG_LEVEL_WARNING  = 3,     /**< warning conditions               *//**<CNcomment:告警条件       */
        JPEG_LOG_LEVEL_ERROR    = 4,     /**< error conditions                 *//**<CNcomment:错误条件       */
        JPEG_LOG_LEVEL_CRIT     = 5,     /**< critical conditions              *//**<CNcomment:重要条件       */
        JPEG_LOG_LEVEL_ALERT    = 6,     /**< action must be taken immediately *//**<CNcomment:条件判断       */
        JPEG_LOG_LEVEL_FATAL    = 7,     /**< just for compatibility with previous version *//**<CNcomment:与之前版本兼容性 */
        JPEG_LOG_LEVEL_BUTT
    } JPEG_LOG_ERRLEVEL_E;


    /**Jpeg Module encoding*/
    /** CNcomment:jpeg 模块编码 */
    typedef enum
    {
        JPEG_MOD_COMM      = 0,       /**< comm moduld  *//**<CNcomment:公共模块      */
        JPEG_MOD_MEM,                 /**< comm moduld  *//**<CNcomment:内存管理模块  */
        JPEG_MOD_BUTT
    } HIGO_MOD_E;


    /** Common error codes of the Jpeg */
    /** CNcomment:Jpeg 公共错误码 */
    typedef enum tagJPEG_COMM_ERR_E
    {
        ERR_COMM_UNSUPPORT_SCALE   =  0,  /**< the scale can not support *//**<CNcomment:缩放比例不支持 */
        ERR_COMM_NOMEM,
        ERR_COMM_BUTT
    } JPEG_COMM_ERR_E;

    /** @} */  /*! <!-- enum Definition end */


    /***************************** Macro Definition ******************************/
    /** \addtogroup      JPEG ERROR MACRO */
    /** @{ */  /** <!-- 【JPEG ERROR MACRO】 */

    /** if some where failure,return */
    /** CNcomment:判断是否退出 */
    #define JPEG_ASSERT(cond,retvalue) \
    do  \
    {   \
        if(cond)            \
        {  \
           return retvalue;  \
        }  \
    }   \
    while(0)


    /** Error IDs of JPEG projects */
    /** CNcomment:JPEG 项目错误ID = 0xB0000000 */
    #define JPEG_ERR_APPID           (0x80000000L + 0x30000000L)

    /** Macros for defining the error codes of the Jpeg */
    /** CNcomment:Jpeg 错误码定义宏，高16位表示JPEG中某个某块ID， 13-16位表示错误等级，最后低13位表示错误码*/
    #define JPEG_DEF_ERR( module, errid) \
        ((HI_S32)((JPEG_ERR_APPID) | (((HI_U32)module) << 16) | (((HI_U32)JPEG_LOG_LEVEL_ERROR) << 13) | ((HI_U32)errid)))



    /** the scale can not support (0xB0008000) */
    /** CNcomment:缩放比例不支持 0xB0008000 */
    #define JPEG_ERR_UNSUPPORT_SCALE      JPEG_DEF_ERR(JPEG_MOD_COMM, ERR_COMM_UNSUPPORT_SCALE)


    #define JPEG_ERR_NOMEM      JPEG_DEF_ERR(JPEG_MOD_COMM, ERR_COMM_NOMEM)



    /** @} */    /*! <!-- Macro Definition end */

    /*************************** Structure Definition ****************************/

    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_ERROR_H__*/
