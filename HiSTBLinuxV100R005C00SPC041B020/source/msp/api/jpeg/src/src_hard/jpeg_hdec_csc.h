/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_csc.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : the mem mangage
                  CNcomment: 颜色空间转换 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/
#ifndef __JPEG_HDEC_CSC_H__
#define __JPEG_HDEC_CSC_H__

#include  "hi_jpeg_config.h"


#ifndef CONFIG_JPEG_CSC_DISABLE

/*********************************add include here******************************/

#include  "jpeg_hdec_api.h"


#if   defined(CHIP_TYPE_hi3712)      \
   || defined(CHIP_TYPE_hi3716m)     \
   || defined(CHIP_TYPE_hi3716mv310) \
   || defined(CHIP_TYPE_hi3110ev500) \
   || defined(CHIP_TYPE_hi3716mv320) \
   || defined(CHIP_TYPE_hi3716mv330)
#include  "hi_tde_type.h"
#else
#include  "hi_tde_api.h"
#endif

/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/
    /** \addtogroup      JPEG MEM MACRO */
    /** @{ */  /** <!-- 【JPEG MEM MACRO】 */

     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/

    /** \addtogroup      JPEG MEM ENUM */
    /** @{ */  /** <!-- 【JPEG MEM ENUM】 */



    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/

    /** \addtogroup      JPEG MEM STRUCTURE */
    /** @{ */  /** <!-- 【JPEG MEM STRUCTURE】 */

    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /** \addtogroup      JPEG MEM API */
    /** @{ */  /** <!-- 【JPEG MEM API】 */


     /*****************************************************************************
     * func          : JPEG_HDEC_CSC_Open
     * description     : Open the csc device
                       CNcomment: CSC转换设备打开 CNend\n
     * retval         : HI_SUCCESS     CNcomment: 成功       CNend\n
     * retval         : HI_FAILURE     CNcomment: 失败       CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_S32 JPEG_HDEC_CSC_Open();

     /*****************************************************************************
     * func          : JPEG_HDEC_CSC_Close
     * description     : close the csc device
                       CNcomment: CSC转换设备关闭 CNend\n
     * param[in]     : s32CscDev     CNcomment: CSC 设备   CNend\n
     * retval         : HI_SUCCESS     CNcomment: 成功       CNend\n
     * retval         : HI_FAILURE     CNcomment: 失败       CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_VOID JPEG_HDEC_CSC_Close(HI_S32 s32CscDev);

     /*****************************************************************************
     * func          : JPEG_HDEC_CSC_BeginJob
     * description     : create csc task
                       CNcomment: 创建CSC任务 CNend\n
     * param[in]     : s32CscDev     CNcomment: CSC 设备   CNend\n
     * retval         : HI_SUCCESS     CNcomment: 成功       CNend\n
     * retval         : HI_FAILURE     CNcomment: 失败       CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_S32 JPEG_HDEC_CSC_BeginJob(HI_S32 s32CscDev);

     /*****************************************************************************
     * func          : JPEG_HDEC_CSC_MbBlit
     * description     : run csc task
                       CNcomment: 执行任务 CNend\n
     * param[in]     : s32CscDev     CNcomment: CSC 设备   CNend\n
     * retval         : HI_SUCCESS     CNcomment: 成功       CNend\n
     * retval         : HI_FAILURE     CNcomment: 失败       CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_S32 JPEG_HDEC_CSC_MbBlit(TDE_HANDLE s32Handle, TDE2_MB_S* pstMB, TDE2_RECT_S  *pstMbRect, TDE2_SURFACE_S* pstDst,TDE2_RECT_S  *pstDstRect,TDE2_MBOPT_S* pstMbOpt,HI_S32 s32CscDev);


     /*****************************************************************************
     * func          : JPEG_HDEC_CSC_EndJob
     * description     : submit csc task
                       CNcomment: 提交任务 CNend\n
     * param[in]     : s32CscDev     CNcomment: CSC 设备   CNend\n
     * retval         : HI_SUCCESS     CNcomment: 成功       CNend\n
     * retval         : HI_FAILURE     CNcomment: 失败       CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_S32 JPEG_HDEC_CSC_EndJob(TDE_HANDLE s32Handle, HI_BOOL bSync, HI_BOOL bBlock, HI_U32 u32TimeOut,HI_S32 s32CscDev);


    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */


#endif


#endif /* __JPEG_HDEC_CSC_H__*/
