/***********************************************************************************
*              Copyright 2015 - 2016, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   :  hi_tee_scrambler.h
* Description:  tee interfaces of demux scrambler.
*
* History:
* Version      Date         Author        DefectNum    Description
* main\1    20160509    NA                 NULL      Create this file.
***********************************************************************************/

#ifndef __TEE_SCRAMBLER_H__
#define __TEE_SCRAMBLER_H__

#include "hi_type.h"
#include "hi_tee_errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      Scrambler */
/** @{ */  /** <!-- [Scrambler] */

/**Type of the scrambler protocol.*/
/**CNcomment:加扰器协议类型*/
typedef enum hiTEE_DMX_SCRAMBLER_TYPE_E
{
    HI_TEE_DMX_SCRAMBLER_TYPE_AES_IPTV  ,      /**<AES IPTV of SPE*/
    HI_TEE_DMX_SCRAMBLER_TYPE_AES_ECB   ,      /**<SPE AES ECB*/
    HI_TEE_DMX_SCRAMBLER_TYPE_AES_CI    ,      /**<SPE AES CIPLUS*/
    HI_TEE_DMX_SCRAMBLER_TYPE_AES_NS    ,      /**<AES NS-Mode*/
    HI_TEE_DMX_SCRAMBLER_TYPE_AES_CBC   ,      /**<AES CBC*/
    HI_TEE_DMX_SCRAMBLER_TYPE_TDES_IPTV,
    HI_TEE_DMX_SCRAMBLER_TYPE_TDES_ECB,
    HI_TEE_DMX_SCRAMBLER_TYPE_TDES_CBC,
    HI_TEE_DMX_SCRAMBLER_TYPE_BUTT
} HI_TEE_DMX_SCRAMBLER_TYPE_E;

typedef enum hiTEE_DMX_SCRAMBLER_KEY_TYPE_E
{
    HI_TEE_DMX_SCRAMBLER_EVEN_KEY,
    HI_TEE_DMX_SCRAMBLER_ODD_KEY,
}HI_TEE_DMX_SCRAMBLER_KEY_TYPE_E;

/**Attribute of the key area.*/
/**CNcomment:密钥区属性*/
typedef struct hiTEE_DMX_SCRAMBLER_ATTR_S
{
    HI_TEE_DMX_SCRAMBLER_TYPE_E enScramblerType;
    HI_TEE_DMX_SCRAMBLER_KEY_TYPE_E enActiveKey;
}HI_TEE_DMX_SCRAMBLER_ATTR_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API Declaration *****************************/
/** \addtogroup      Scrambler */
/** @{ */  /** <!--[Scrambler]*/

HI_S32 HI_TEE_DMX_GetScramblerDefaultAttr(HI_TEE_DMX_SCRAMBLER_ATTR_S *pstScramblerAttr);

HI_S32 HI_TEE_DMX_CreateScrambler(HI_U32 u32DmxId, const HI_TEE_DMX_SCRAMBLER_ATTR_S *pstSramblerAttr, HI_HANDLE *phKey);
HI_S32 HI_TEE_DMX_DestroyScrambler(HI_HANDLE hKey);

HI_S32 HI_TEE_DMX_GetScramblerAttr(HI_HANDLE hKey, HI_TEE_DMX_SCRAMBLER_ATTR_S *pstAttr);
HI_S32 HI_TEE_DMX_SetScramblerAttr(HI_HANDLE hKey, const HI_TEE_DMX_SCRAMBLER_ATTR_S *pstAttr);

HI_S32 HI_TEE_DMX_SetScramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey);
HI_S32 HI_TEE_DMX_SetScramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey);

HI_S32 HI_TEE_DMX_AttachScrambler(HI_HANDLE hKey, HI_HANDLE hChannel);
HI_S32 HI_TEE_DMX_DetachScrambler(HI_HANDLE hKey, HI_HANDLE hChannel);

HI_S32 HI_TEE_DMX_GetScramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* __TEE_SCRAMBLER_H__ */
