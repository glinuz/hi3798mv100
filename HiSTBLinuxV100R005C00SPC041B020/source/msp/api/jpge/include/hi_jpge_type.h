/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name          : drv_jpge_ioctl.h
Version            : Initial Draft
Author             :
Created            : 2016/03/03
Description        :
Function List      :


History            :
Date                  Author                Modification
2016/03/03            y00181162              Created file
******************************************************************************/
#ifndef __HI_JPGE_TYPE_H__
#define __HI_JPGE_TYPE_H__


/*********************************add include here******************************/
#include "hi_jpge_errcode.h"


/*****************************************************************************/


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
/*JPEG handle define*/
/*CNcomment:JPGE¾ä±ú¶¨Òå*/
typedef HI_S32 JPGE_HANDLE;

#define JPGE_YUV420       0
#define JPGE_YUV422       1
#define JPGE_YUV444       2

#define JPGE_SEMIPLANNAR  0
#define JPGE_PLANNAR      1
#define JPGE_PACKAGE      2

#define JPGE_ROTATION_0   0
#define JPGE_ROTATION_90  1
#define JPGE_ROTATION_270 2
#define JPGE_ROTATION_180 3


/*************************** Structure Definition ****************************/
typedef struct
{
    HI_U32  FrameWidth;    /* 16-4096, 4-byte aligned */
    HI_U32  FrameHeight;   /* 16-4096, 4-byte aligned */

    HI_U32  YuvSampleType; /* 420 422 or 444 */
    HI_U32  YuvStoreType;  /* SemiPlannar, Plannar or Package */
    HI_U32  RotationAngle; /* 0 90 180 or 270 degree */

    HI_U16  SlcSplitEn;    /* Slice split enable */
    HI_U16  SplitSize;     /* MCU Number @ Slice  */

    HI_U32  Qlevel;        /* 1 ~ 99, large value = high image quality = large JFIF size */

} Jpge_EncCfg_S;

typedef struct
{
    HI_U32  BusViY;      /* 16-byte aligned  */
    HI_U32  BusViC;      /* 16-byte aligned  */  /* N/A @ package */
    HI_U32  BusViV;      /* 16-byte aligned  */  /* N/A @ package & semiplannar */
    HI_U32  ViYStride;   /* 16-byte aligned  */
    HI_U32  ViCStride;   /* 16-byte aligned  */  /* N/A @ package */

    HI_U32  BusOutBuf;   /* Stream Buf Bus Addr */
    HI_U8  *pOutBuf;     /* Stream Buf Virtual Addr */
    HI_U32  OutBufSize;  /* in byte */
} Jpge_EncIn_S;

typedef struct
{
    HI_U32  BusViY;      /* 16-byte aligned  */
    HI_U32  BusViC;      /* 16-byte aligned  */  /* N/A @ package */
    HI_U32  BusViV;      /* 16-byte aligned  */  /* N/A @ package & semiplannar */
    HI_U32  ViYStride;   /* 16-byte aligned  */
    HI_U32  ViCStride;   /* 16-byte aligned  */  /* N/A @ package */

    HI_U32  YuvSampleType; /* 420 422 or 444 */
    HI_U32  YuvStoreType;  /* SemiPlannar, Plannar or Package */
    HI_U32  PackageSel;          /* add by l00228308*/
    HI_U32  RotationAngle; /* 0 90 180 or 270 degree */

    HI_U32  StrmBufAddr;
    HI_U32  StrmBufRpAddr;
    HI_U32  StrmBufWpAddr;
    HI_U32  StrmBufSize;
    HI_U32  Vir2BusOffset;

    HI_U32  FrameWidth;    /* 16-4096, 4-byte aligned */
    HI_U32  FrameHeight;   /* 16-4096, 4-byte aligned */

    HI_U32  Qlevel;        /* 1 ~ 99, large value = high image quality = large JFIF size */
    HI_U8   *pJpgeYqt;
    HI_U8   *pJpgeCqt;

    HI_U32   PTS0;
    //HI_U32  BusOutBuf;   /* Stream Buf Bus Addr */
    //HI_U8  *pOutBuf;     /* Stream Buf Virtual Addr */
    //HI_U32  OutBufSize;  /* in byte */
} Venc2Jpge_EncIn_S;


typedef struct
{
    HI_U32  BusStream;   /* JFIF Stream Bus Addr */
    HI_U8  *pStream;     /* JFIF Stream Virtual Addr */
    HI_U32  StrmSize;    /* JFIF Stream Size in byte */
}Jpge_EncOut_S;

typedef struct _Jpge_EncInfo_S
{
    JPGE_HANDLE   EncHandle;
    Jpge_EncIn_S  EncIn;
    Jpge_EncOut_S EncOut;
}Jpge_EncInfo_S;

/** 64bit kernel need **/
typedef struct
{
    HI_U32  BusViY;
    HI_U32  BusViC;
    HI_U32  BusViV;
    HI_U32  ViYStride;
    HI_U32  ViCStride;
    HI_U32  BusOutBuf;
    HI_U32  pOutBuf;   /** pointer 4 byte so here use u32 type **/
    HI_U32  OutBufSize;
} Jpge_Compat_EncIn_S;

typedef struct
{
    HI_U32  BusStream;
    HI_U32  pStream;
    HI_U32  StrmSize;
}Jpge_Compat_EncOut_S;

typedef struct _Jpge_Compat_EncInfo_S
{
    JPGE_HANDLE          EncHandle;
    Jpge_Compat_EncIn_S  EncIn;
    Jpge_Compat_EncOut_S EncOut;
}Jpge_Compat_EncInfo_S;


/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__HI_JPGE_TYPE_H__ */
