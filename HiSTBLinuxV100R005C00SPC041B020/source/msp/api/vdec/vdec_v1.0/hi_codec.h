/******************************************************************************

  Copyright (C), 2012-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_codec.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/07/31
  Description   : Definitions of HI_CODEC.
  History       :
  1.Date        : 2012/07/31
    Author      : l00185424
    Modification: Created file

*******************************************************************************/

#ifndef __HI_CODEC_H__
#define __HI_CODEC_H__

/******************************* Include Files *******************************/

#include "hi_video_codec.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/****************************** Macro Definition *****************************/

#ifndef HI_VDEC_REG_CODEC_SUPPORT
#define HI_VDEC_REG_CODEC_SUPPORT (0)
#endif

#define HI_ERR_CODEC_NOT_INIT           (HI_ERR_VDEC_NOT_INIT)

/* HANDLE: 
 * BITS[31-16]  USED BY PLATFORM
 * BITS[15-8]   USED BY HI_CODEC
 * BITS[7-0]    USED BY EVERY CODEC 
 */
#define HI_CODEC_LIB_HANDLE(hInst)  ((hInst>>8) & 0xFF)
#define HI_CODEC_INST_HANDLE(hInst) (hInst & 0xFF)

#define HI_CODEC_MAX_NUMBER         (16)

/*************************** Structure Definition ****************************/
/** \addtogroup       */
/** @{ */  /** <!--  */


/** @} */  /** <!-- ==== Structure Definition End ==== */

/******************************* API Declaration *****************************/
/** \addtogroup       */
/** @{ */  /** <!--  */

HI_S32 HI_CODEC_Init(HI_VOID);
HI_S32 HI_CODEC_DeInit(HI_VOID);
#if (HI_VDEC_REG_CODEC_SUPPORT == 1)
HI_S32 HI_CODEC_RegisterLib(const HI_CHAR *pszCodecDllName);
HI_S32 HI_CODEC_UnRegisterLib(const HI_CHAR *pszCodecDllName);
HI_S32 HI_CODEC_RegisterFunc(const HI_CODEC_S *pszCodec);
#endif
HI_S32 HI_CODEC_Register(HI_CODEC_S* pstCodec);
HI_S32 HI_CODEC_UnRegister(const HI_CODEC_S* pstCodec);
HI_CODEC_S* HI_CODEC_Create(HI_HANDLE* phInst, const HI_CODEC_OPENPARAM_S * pstParam);
HI_S32 HI_CODEC_Destory(HI_HANDLE hInst);
const HI_CHAR* HI_CODEC_GetName(HI_HANDLE hInst);
HI_BOOL HI_CODEC_SupportDecode(const HI_CODEC_S* pstCodec, HI_CODEC_ID_E enID);
HI_CODEC_VERSION_U HI_CODEC_GetVersion(HI_HANDLE hInst);
HI_BOOL HI_CODEC_NeedFrameBuf(HI_HANDLE hInst);


/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_CODEC_H__ */

