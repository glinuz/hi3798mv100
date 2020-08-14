#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"

#define LED_BIT_COUNT  4  
#if 0
#define STB_KEY_MENU    0x01 
#define STB_KEY_OK      0x02 
#define STB_KEY_UP      0x04 
#define STB_KEY_DOWN    0x08 
#define STB_KEY_LEFT    0x10 
#define STB_KEY_RIGHT   0x20 
#define STB_KEY_EXIT    0x40 
#define STB_KEY_NULL    0x00
#else
#define STB_KEY_MENU	1         
#define STB_KEY_OK     	2
#define STB_KEY_UP     	3 
#define STB_KEY_DOWN   	4
#define STB_KEY_LEFT	5
#define STB_KEY_RIGHT   6 
#define STB_KEY_EXIT	7
#define STB_KEY_BUTT    8
#define STB_KEY_NULL    0
#define STB_KEY_HOLD    0x5a

#endif



#if 0//!defined(BOARD_TYPE_hi3716mdmo3avera) && !defined(BOARD_TYPE_hi3716mdmo3bvera) 
#define GROUP   (5)
#define DAT     (0)
#define CLK     (1)
#define KEY     (6)
#endif

#ifdef HI_KEYLED_CT1642_CLOCK_GPIO
#define CLK_CT1642 HI_KEYLED_CT1642_CLOCK_GPIO
#define DAT_CT1642 HI_KEYLED_CT1642_DAT_GPIO
#define KEY_CT1642 HI_KEYLED_CT1642_KEY_GPIO
#else
#define CLK_CT1642 0x2a
#define DAT_CT1642 0x2c
#define KEY_CT1642 0x2f
#endif

#define SCAN_INTV_CT1642    (100)
#define KEY_MACRO_NO (0xff)

HI_S32 KEYLED_KEY_Open_CT1642(HI_VOID);
HI_S32 KEYLED_KEY_Close_CT1642(HI_VOID);
HI_S32 KEYLED_KEY_Reset_CT1642(HI_VOID);
HI_S32 KEYLED_KEY_GetValue_CT1642(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId);
HI_S32 KEYLED_KEY_SetBlockTime_CT1642(HI_U32 u32BlockTimeMs);
HI_S32 KEYLED_KEY_SetRepTime_CT1642(HI_U32 u32RepTimeMs);
HI_S32 KEYLED_KEY_IsRepKey_CT1642(HI_U32 u32IsRepKey);
HI_S32 KEYLED_KEY_IsKeyUp_CT1642(HI_U32 u32IsKeyUp);

HI_S32 KEYLED_LED_Open_CT1642(HI_VOID);
HI_S32 KEYLED_LED_Close_CT1642(HI_VOID);
HI_S32 KEYLED_LED_Display_CT1642(HI_U32 u32CodeValue);
HI_S32 KEYLED_LED_DisplayTime_CT1642(HI_UNF_KEYLED_TIME_S stKeyLedTime);
HI_S32 KEYLED_LED_SetFlashPin_CT1642(HI_UNF_KEYLED_LIGHT_E enPin);
HI_S32 KEYLED_LED_SetFlashFreq_CT1642(HI_UNF_KEYLED_LEVEL_E enLevel);

HI_S32 KEYLED_GetProcInfo_CT1642(KEYLED_PROC_INFO_S *stInfo);

HI_S32 KEYLED_Suspend_CT1642(HI_VOID);
HI_S32 KEYLED_Resume_CT1642(HI_VOID);

