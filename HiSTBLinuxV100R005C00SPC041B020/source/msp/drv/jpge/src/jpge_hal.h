/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name          : jpge_hal.h
Version            : Initial Draft
Author             :
Created            : 2016/03/03
Description        :
Function List      :


History            :
Date                  Author                Modification
2016/03/03            y00181162              Created file
******************************************************************************/
#ifndef __JPGE_HAL_H__
#define __JPGE_HAL_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include "drv_jpge_ext.h"
#include "jpge_osal.h"

/*****************************************************************************/


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/
#define JPGE_MAX_HDR_LEN         698

#define JPGE_MAX_INSTANCE_NUM    8


#define CHECK_JPGE_HANDLE_SUPPORT(EncHandle) do{\
if ((EncHandle >= JPGE_MAX_INSTANCE_NUM) || (EncHandle < 0)){\
      return HI_FAILURE;}\
}while(0)

/*************************** Structure Definition ****************************/
typedef struct
{
    HI_U32  PacketLen;        // united 64, include itself
    HI_U32  InvldByte;        // InvalidByteNum

    HI_U8   Reserved0;
    HI_U8   Reserved1;
    HI_U8   Reserved2;
    HI_U8   bLastSlice;

    HI_U32  ChnID;
    HI_U32  Reserved[12];

} Jpge_NaluHdr_S;

typedef struct
{
    HI_U32  SrcYAddr;
    HI_U32  SrcCAddr;
    HI_U32  SrcVAddr;
    HI_U32  SrcYStride;
    HI_U32  SrcCStride;
    HI_U32  StrmBufAddr;
    HI_U32  StrmBufRpAddr;
    HI_U32  StrmBufWpAddr;
    HI_U32  StrmBufSize;
    HI_U32  Vir2BusOffset; /* offset = vir - bus */
    HI_U32  PicWidth;
    HI_U32  PicHeight;
    HI_U32  YuvSampleType;
    HI_U32  YuvStoreType;
    HI_U32  RotationAngle;
    HI_U32  PackageSel;
    HI_U16  SlcSplitEn;
    HI_U16  SplitSize;     /* MCU @ JPGE */
    HI_U8   pJfifHdr[JPGE_MAX_HDR_LEN];
    HI_U8   pJpgeYqt[64];
    HI_U8   pJpgeCqt[64];
    HI_U8   EndOfPic;
    HI_U8   BufFull;
    HI_U32    *pRegBase;
    JPGE_SEM_S IsrMutex;
    HI_U32     PTS0;
    HI_BOOL    bInUse;
} Jpge_EncPara_S;

typedef struct tagJpgeEncInstance_S
{
    Jpge_EncPara_S EncPara;
    HI_BOOL bEncInstanceNull;
}JPGE_ENC_INSTANCE_S;

typedef struct
{
    HI_S32       CurrHandle;  /* used in ISR */
    HI_U32      *pRegBase;
    JPGE_SEM_S   ChnMutex;    /* for channel control */
    JPGE_LOCK_S  ChnLock;     /* lock ChnCtx[MAX_CHN] */

} Jpge_IpCtx_S;




/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/

/** to jpeg encode for ioctl and venc **/
HI_S32  JPEG_ENC_Open           (HI_VOID);
HI_S32  JPEG_ENC_Close          (HI_VOID);
HI_S32  JPEG_ENC_CreateHandle   (HI_S32 *pEncHandle);

/** to jpeg encode for ioctl **/
HI_S32  JPEG_ENC_Create         (Jpge_EncCfg_S *pEncCfg);
HI_S32  JPEG_ENC_Start          (HI_S32  EncHandle,  Jpge_EncIn_S *pEncIn, Jpge_EncOut_S *pEncOut);
HI_S32  JPEG_ENC_Destory        (HI_S32  EncHandle);

/** to jpeg encode for venc **/
HI_VOID JPEG_ENC_OpenDevForVenc (HI_VOID);
HI_VOID JPEG_ENC_CloseForVenc   (HI_VOID);
HI_S32  JPEG_ENC_CtreateForVenc (HI_S32 *pEncHandle, Jpge_EncCfg_S *pEncCfg);
HI_S32  JPEG_ENC_StartForVenc   (HI_S32  EncHandle,  Venc2Jpge_EncIn_S *pEncIn,HI_BOOL *pBufferFull);
HI_S32  JPEG_ENC_DestoryForVenc (HI_S32  EncHandle);


HI_VOID JPEG_ENC_OpenClock  (HI_VOID);
HI_VOID JPEG_ENC_CloseClock (HI_VOID);

/**
 ** ctrl + c will call this function in release function
 **/
HI_VOID JPEG_ENC_Reset      (HI_VOID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__JPGE_HAL_H__ */
