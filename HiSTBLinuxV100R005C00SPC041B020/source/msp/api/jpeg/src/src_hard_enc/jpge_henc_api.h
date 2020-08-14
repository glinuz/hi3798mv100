/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name        : jpge_henc_api.h
Version            : Initial Draft
Author            :
Created            : 2014/05/14
Description        : The user will use this api to realize some function
Function List     :


History           :
Date                Author                Modification
2014/05/14            y00181162              Created file
******************************************************************************/

#ifndef __JPGE_HENC_API_H__
#define __JPGE_HENC_API_H__


#ifdef CONFIG_GFX_JPGE_ENC_ENABLE

/*********************************add include here******************************/

#include  "jpeglib.h"
#include  "hi_type.h"
#include "hi_jpge_henc_api.h"

/*****************************************************************************/


#ifdef __cplusplus
#if __cplusplus
   extern "C" {
#endif
#endif /* __cplusplus */



     /***************************** Macro Definition ******************************/
     #define JPGE_YUV420          0
     #define JPGE_YUV422          1
     #define JPGE_YUV444          2

     #define JPGE_SEMIPLANNAR     0
     #define JPGE_PLANNAR         1
     #define JPGE_PACKAGE         2

     #define JPGE_ROTATION_0      0
     #define JPGE_ROTATION_90     1
     #define JPGE_ROTATION_270    2
     #define JPGE_ROTATION_180    3

     #define MAX_COMPONENT_NUM     3

     #define JPGE_TAG           "jpge"
     #define JPGE_MODULE        "JPGE_BUF"

     /*************************** Enum Definition ****************************/



    /*************************** Structure Definition ****************************/
    /*
    the attributes of src jpeg data
    */
    typedef struct tagJPGE_ENCCFG_S
    {
        HI_U32  FrameWidth;    /* 16-4096, 4-byte aligned */
        HI_U32  FrameHeight;   /* 16-4096, 4-byte aligned */

        HI_U32  YuvSampleType; /* 420 422 or 444 */
        HI_U32  YuvStoreType;  /* SemiPlannar, Plannar or Package */
        HI_U32  RotationAngle; /* 0 90 180 or 270 degree */

        HI_U16  SlcSplitEn;    /* Slice split enable */
        HI_U16  SplitSize;     /* MCU Number @ Slice  */

        HI_U32  Qlevel;        /* 1 ~ 99, large value = high image quality = large JFIF size */

    } JPGE_ENCCFG_S;


     typedef struct tagJPGE_ENCIN_S
     {
         HI_U32  BusViY;      /* 16-byte aligned  */
         HI_U32  BusViC;      /* 16-byte aligned  */  /* N/A @ package */
         HI_U32  BusViV;      /* 16-byte aligned  */  /* N/A @ package & semiplannar */
         HI_U32  ViYStride;   /* 16-byte aligned  */
         HI_U32  ViCStride;   /* 16-byte aligned  */  /* N/A @ package */

         HI_U32  BusOutBuf;   /* Stream Buf Bus Addr */
         HI_U8    *pOutBuf;       /* Stream Buf Virtual Addr */
         HI_U32  OutBufSize;  /* in byte */
     } JPGE_ENCIN_S;

     typedef struct tagJPGE_ENCOUT_S
     {
         HI_U32  BusStream;   /* JFIF Stream Bus Addr */
         HI_U8    *pStream;      /* JFIF Stream Virtual Addr */
         HI_U32  StrmSize;      /* JFIF Stream Size in byte */
     } JPGE_ENCOUT_S;

     typedef struct tagJPGE_ENCCFGINFO_S
     {
         HI_S32       *pEncHandle;
         JPGE_ENCCFG_S EncCfg;
     }JPGE_ENCCFGINFO_S;


     typedef struct tagJPGE_ENCINFO_S
     {
         HI_S32        EncHandle;
         JPGE_ENCIN_S  EncIn;
         JPGE_ENCOUT_S EncOut;
     }JPGE_ENCINFO_S;

     /** venc need **/
     typedef struct tagJPGE_VENC2JPGE_IN_S
     {
         HI_U32  BusViY;      /* 16-byte aligned  */
         HI_U32  BusViC;      /* 16-byte aligned  */  /* N/A @ package */
         HI_U32  BusViV;      /* 16-byte aligned  */  /* N/A @ package & semiplannar */
         HI_U32  ViYStride;   /* 16-byte aligned  */
         HI_U32  ViCStride;   /* 16-byte aligned  */  /* N/A @ package */

         HI_U32  YuvSampleType; /* 420 422 or 444 */
         HI_U32  YuvStoreType;    /* SemiPlannar, Plannar or Package */
         HI_U32  RotationAngle; /* 0 90 180 or 270 degree */

         HI_U32  StrmBufAddr;
         HI_U32  StrmBufRpAddr;
         HI_U32  StrmBufWpAddr;
         HI_U32  StrmBufSize;
         HI_U32  Vir2BusOffset;

         HI_U32  FrameWidth;    /* 16-4096, 4-byte aligned */
         HI_U32  FrameHeight;    /* 16-4096, 4-byte aligned */

         HI_U32  Qlevel;        /* 1 ~ 99, large value = high image quality = large JFIF size */
         HI_U8     *pJpgeYqt;
         HI_U8     *pJpgeCqt;

         HI_U32   PTS0;

     } JPGE_VENC2JPGE_IN_S;

     /** the private structure of hard encode */
     /** CNcomment:硬件编码增加的私有结构体 */
     typedef struct tagJPGE_HENC_HANDLE_S
     {
            HI_BOOL  bEncSupport;
           HI_BOOL  bHasSetStreamInfo;
           HI_BOOL  bCanUserPhyMem;
           HI_BOOL  bEncMCU;
           HI_S32    s32ClientData;
           HI_S32    s32MMZDev;
           HI_S32   s32Quality;
           HI_S32   s32YRow;
           HI_S32   s32CRow;
           HI_U32   u32FlexionSize;
           HI_VOID  *pMemHandle;
           HI_S32   s32FreeBufSize;
           HI_BOOL  bWriteFile;
           HI_BOOL  bEncodeSuccess;
           JPGE_ENCCFG_S EncCfg;
           JPGE_ENCIN_S  EncIn;
           JPGE_ENCOUT_S EncOut;
           HI_JPEG_STREAM_INFO_S stStreamInfo;
           HI_JPEG_STREAM_INFO_S stMiddleBuf;
     }JPGE_HENC_HANDLE_S,*JPGE_HENC_HANDLE_S_PTR;

    /********************** Global Variable declaration **************************/

    /******************************* API declaration *****************************/


    /** \addtogroup      JPGE */
    /** @{ */    /** <!--[JPGE]*/

    /*****************************************************************************
    * func            : JPGE_HENC_Init
    * description    : init the private structure para
                      CNcomment: 初始化私有结构体变量    CNend\n
    * param[in]     : cinfo       CNcomment: 编码对象    CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功  CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败  CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPGE_HENC_Init(j_common_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HENC_CloseDev
    * description    : closxe some device that encode need
                      CNcomment: 关闭编码打开的相关设备        CNend\n
    * param[in]     : cinfo         CNcomment: 编码对象    CNend\n
    * retval        : HI_SUCCESS    CNcomment: 成功        CNend\n
    * retval        : HI_FAILURE    CNcomment: 失败        CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HENC_CloseDev(const struct jpeg_common_struct *cinfo);

    /*****************************************************************************
    * func            : JPEG_HENC_Abort
    * description    : when want use the encode again,call this
                      CNcomment:  如果想继续使用编码器，调用该接口 CNend\n
    * param[in]     : cinfo       CNcomment: 编码对象               CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功  CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败  CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPEG_HENC_Abort(const struct jpeg_common_struct *cinfo);

    /*****************************************************************************
    * func            : JPGE_HENC_DInit
    * description    : dinit the hard compress
                      CNcomment: 销毁硬件编码器         CNend\n
    * param[in]     : cinfo       CNcomment: 解码对象    CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功       CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败       CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPGE_HENC_DInit(const struct jpeg_common_struct *cinfo);

    /*****************************************************************************
    * func            : JPGE_HENC_IfSupport
    * description    : if the hard encode support
                      CNcomment: 硬件编码是否支持       CNend\n
    * param[in]     : cinfo       CNcomment: 解码对象    CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功       CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败       CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32 JPGE_HENC_IfSupport(j_compress_ptr cinfo);

    /*****************************************************************************
    * func            : JPEG_HENC_Encode
    * description    : jpeg encode
                      CNcomment: 开始编码       CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功  CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败  CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32    JPEG_HENC_Encode(const struct jpeg_compress_struct *cinfo);


    /*****************************************************************************
    * func            : JPEG_HENC_WriteData
    * description    : write data to dest
                      CNcomment: 将编码后的数据写到目标位置  CNend\n
    * retval        : HI_SUCCESS  CNcomment: 成功            CNend\n
    * retval        : HI_FAILURE  CNcomment: 失败            CNend\n
    * others:        : NA
    *****************************************************************************/
    HI_S32    JPEG_HENC_WriteData(j_compress_ptr cinfo);


    /** @} */  /** <!-- ==== API Declaration End ==== */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif/** CONFIG_GFX_JPGE_ENC_ENABLE **/

#endif /* __JPGE_HENC_API_H__*/
