/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpeg_hdec_adp.h
Version            : Initial Draft
Author            :
Created            : 2014/06/20
Description        : the adp realize in this file
                  CNcomment: 适配的实现都在这个文件里 CNend\n
Function List     :


History           :
Date                Author                Modification
2014/06/20            y00181162              Created file
******************************************************************************/

#ifndef __JPEG_HDEC_ADP_H__
#define __JPEG_HDEC_ADP_H__


/*********************************add include here******************************/

#include  "jpeglib.h"
#include  "hi_type.h"


/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



    /***************************** Macro Definition ******************************/
    /** \addtogroup      JPEG ADP MACRO */
    /** @{ */  /** <!-- 【JPEG ADP MACRO】 */


     /** @} */    /*! <!-- Macro Definition end */


     /*************************** Enum Definition ****************************/

    /** \addtogroup      JPEG ADP ENUM */
    /** @{ */  /** <!-- 【JPEG ADP ENUM】 */



    /** @} */  /*! <!-- enum Definition end */

    /*************************** Structure Definition ****************************/

    /** \addtogroup      JPEG ADP STRUCTURE */
    /** @{ */  /** <!-- 【JPEG ADP STRUCTURE】 */

    /** @} */  /*! <!-- Structure Definition end */


    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/

    /** \addtogroup      JPEG ADP API */
    /** @{ */  /** <!-- 【JPEG ADP API】 */

    HI_VOID JPEG_HDEC_CalcOutputComponents(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_GetImagInfo
    * description    : get jpeg picture information
                      CNcomment:  获取图片信息 CNend\n
    * param[in]     : cinfo       CNcomment: 解码对象    CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_GetImagInfo(j_decompress_ptr cinfo);


     /*****************************************************************************
     * func          : JPEG_HDEC_SetPara
     * description     : set the parameter that hard decode need
                       CNcomment: 配置硬件解码需要的参数信息        CNend\n
     * param[in]     : cinfo         CNcomment: 解码对象    CNend\n
     * retval         : HI_SUCCESS    CNcomment: 成功        CNend\n
     * retval         : HI_FAILURE    CNcomment: 失败        CNend\n
     * others:         : NA
     *****************************************************************************/
     HI_S32 JPEG_HDEC_SetPara(const struct jpeg_decompress_struct *cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SetStreamBuf
    * description    : set stream buffer message
                      CNcomment: 设置码流buffer寄存器        CNend\n
    * param[in]     : cinfo         CNcomment: 解码对象    CNend\n
    * retval        : NA
    * others:        : NA
    *****************************************************************************/
    HI_VOID JPEG_HDEC_SetStreamBuf(const struct jpeg_decompress_struct *cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromPhyMem
    * description    : get the stream from physics memory
                      CNcomment:  码流来源连续的物理内存的处理方式   CNend\n
    * param[in]     : cinfo       CNcomment:  解码对象     CNend\n
    * retval        : HI_SUCCESS  CNcomment:  成功         CNend\n
    * retval        : HI_FAILURE  CNcomment:  失败         CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromPhyMem(j_decompress_ptr cinfo);


    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromReturnPhyMem
    * description    : get the stream from physics memory
                      CNcomment:  码流来源连续的物理内存的处理方式，具有码流回绕的处理   CNend\n
    * param[in]     : cinfo       CNcomment:  解码对象     CNend\n
    * retval        : HI_SUCCESS  CNcomment:  成功         CNend\n
    * retval        : HI_FAILURE  CNcomment:  失败         CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromReturnPhyMem(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromVirMem
    * description    : get the stream from virtual memory
                      CNcomment:  码流来源虚拟内存的处理方式   CNend\n
    * param[in]     : cinfo       CNcomment:  解码对象     CNend\n
    * retval        : HI_SUCCESS  CNcomment:  成功         CNend\n
    * retval        : HI_FAILURE  CNcomment:  失败         CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromVirMem(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromFile
    * description    : get the stream from file
                      CNcomment:  码流来源文件的处理方式
    * param[in]     : cinfo       CNcomment:  解码对象
    * param[in]     : NA
    * retval        : HI_SUCCESS  CNcomment:  成功
    * retval        : HI_FAILURE  CNcomment:  失败
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromFile(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromFileWithConDDRChange
    * description    : get the stream from file
                        CNcomment:  码流来源文件的处理方式,这里是测试
                                      续码流地址发生变化的情况
    * param[in]     : cinfo       CNcomment:  解码对象
    * param[in]     : NA
    * retval        : HI_SUCCESS  CNcomment:  成功
    * retval        : HI_FAILURE  CNcomment:  失败
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromFileWithConDDRChange(j_decompress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HDEC_SendStreamFromCallBack
    * description    : CNcomment:  码流来源外部处理
    * param[in]     : cinfo       CNcomment:  解码对象
    * param[in]     : NA
    * retval        : HI_SUCCESS  CNcomment:  成功
    * retval        : HI_FAILURE  CNcomment:  失败
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HDEC_SendStreamFromCallBack(j_decompress_ptr cinfo);


    /** @} */  /*! <!-- API declaration end */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __JPEG_HDEC_ADP_H__*/
