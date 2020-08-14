/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hi_unf_keyled.h
* Description:
*
* History:
* Version   Date                Author          DefectNum     Description
* 1.0        2007-10-16       w54542        NULL             Create this file.
***********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#ifndef  __HI_KEYLED_H__
#define  __HI_KEYLED_H__

#include "hi_type.h"

typedef enum  hiUNF_KEYLED_TYPE_E
{
    HI_UNF_KEYLED_TYPE_STD = 0x0, /**<KEYLED type inherent in HiSilicon chips*/   /**<CNcomment:海思芯片自带的KEYLED类型 */
    HI_UNF_KEYLED_TYPE_PT6961,    /**<KEYLED type: PT6961*/                       /**<CNcomment:KEYLED型号：PT6961 */
    HI_UNF_KEYLED_TYPE_CT1642,    /**<KEYLED type: CT1642*/                       /**<CNcomment:KEYLED型号：CT1642 */
    HI_UNF_KEYLED_TYPE_PT6964,	  /**<KEYLED type: PT6964*/ 
    HI_UNF_KEYLED_TYPE_FD650,	  /**<KEYLED type: FD650*/                        /**<CNcomment:KEYLED型号：FD650 */
    HI_UNF_KEYLED_TYPE_GPIOKEY,   /**<KEYLED type :GPIOKEY*/                      /**<CNcomment:GPIO按键面板*/
    HI_UNF_KEYLED_TYPE_BUTT       
}HI_UNF_KEYLED_TYPE_E;

HI_S32 HI_UNF_KEYLED_Init(HI_VOID);
HI_S32 HI_UNF_KEYLED_DeInit(HI_VOID);
HI_S32 HI_UNF_LED_Display(HI_U32 u32CodeValue);
HI_S32 HI_UNF_KEY_Open(HI_VOID);
HI_S32 HI_UNF_KEY_Close(HI_VOID);
HI_S32 HI_UNF_LED_Open(HI_VOID);
HI_S32 HI_UNF_LED_Close(HI_VOID);
HI_S32 HI_UNF_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);
HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUp);
HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKey);
HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32BlockTimeMs);
HI_S32 HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif
