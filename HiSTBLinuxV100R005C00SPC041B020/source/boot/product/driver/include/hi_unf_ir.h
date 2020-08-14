
#ifndef __IR_API_H__
#define __IR_API_H__

#include "hi_type.h"

typedef enum KEY_STATUS
{
	KEY_STAT_DOWN,
	KEY_STAT_HOLD,
	KEY_STAT_UP,
	KEY_STAT_BUTT
}HI_UNF_KEY_STATUS_E;

/*与MSP下UNF定义兼容*/
#define HI_UNF_KEY_STATUS_DOWN KEY_STAT_DOWN
#define HI_UNF_KEY_STATUS_HOLD KEY_STAT_HOLD
#define HI_UNF_KEY_STATUS_UP KEY_STAT_UP
#define HI_UNF_KEY_STATUS_BUTT KEY_STAT_BUTT


HI_S32 HI_UNF_IR_Init(HI_VOID);

HI_S32 HI_UNF_IR_DeInit(HI_VOID);

HI_S32 HI_UNF_IR_GetValueWithProtocol(enum KEY_STATUS *penPressStatus, HI_U64 *pu64KeyId, 
		HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs);

#define HI_UNF_IR_GetValue(penPressStatus, pu64KeyId, u32TimeoutMs) HI_UNF_IR_GetValueWithProtocol(penPressStatus, pu64KeyId, NULL, 0, u32TimeoutMs)

HI_S32 HI_UNF_IR_Enable (HI_BOOL bEnable);

HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable);

HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable);

HI_S32 HI_UNF_IR_SetRepKeyTimeoutAttr(HI_U32 u32TimeoutMs);

HI_S32 HI_UNF_IR_Reset(HI_VOID);


HI_S32 HI_UNF_IR_EnableProtocol(HI_CHAR* pszProtocolName);

HI_S32 HI_UNF_IR_DisableProtocol(HI_CHAR* pszProtocolName);

HI_S32 HI_UNF_IR_GetProtocolEnabled(HI_CHAR* pszProtocolName, HI_BOOL *pbEnabled);

#ifdef HIIR_DEBUG
HI_VOID HI_UNF_IR_SetDbgOn(HI_S32 on);
HI_VOID HI_UNF_IR_SetInfoOn(HI_S32 on);
#endif
#endif
