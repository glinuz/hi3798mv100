#ifndef  __HI_DRV_KEYLED_H__
#define  __HI_DRV_KEYLED_H__

#include "hi_unf_keyled.h"

#include "hi_debug.h"

#define HI_FATAL_KEYLED(fmt...) HI_FATAL_PRINT(HI_ID_KEYLED, fmt)

#define HI_ERR_KEYLED(fmt...)   HI_ERR_PRINT(HI_ID_KEYLED, fmt)

#define HI_WARN_KEYLED(fmt...)  HI_WARN_PRINT(HI_ID_KEYLED, fmt)

#define HI_INFO_KEYLED(fmt...)  HI_INFO_PRINT(HI_ID_KEYLED, fmt)

#define KEYLED_ADDR_SIZE    0X1000

HI_S32 HI_DRV_KEYLED_Init(HI_VOID);
HI_S32 HI_DRV_KEYLED_DeInit(HI_VOID);
HI_S32 HI_DRV_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType);
HI_S32 HI_DRV_KEY_Open(HI_VOID);
HI_S32 HI_DRV_KEY_Close(HI_VOID);
HI_S32 HI_DRV_LED_Open(HI_VOID);
HI_S32 HI_DRV_LED_Close(HI_VOID);
HI_S32 HI_DRV_LED_Display(HI_U32 u32CodeValue);
HI_S32 HI_DRV_LED_DisplayTime(HI_UNF_KEYLED_TIME_S stLedTime);
HI_S32 HI_DRV_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_E enPin);
HI_S32 HI_DRV_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_E enLevel);
HI_S32 HI_DRV_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId);
HI_S32 HI_DRV_KEY_IsKeyUp(HI_U32 u32IsKeyUp);
HI_S32 HI_DRV_KEY_IsRepKey(HI_U32 u32IsRepKey);
HI_S32 HI_DRV_KEY_SetBlockTime(HI_U32 u32BlockTimeMs);
HI_S32 HI_DRV_KEY_RepKeyTimeoutVal(HI_U32 u32RepTimeMs);

#endif  /*  __HI_DRV_KEYLED_H__ */
