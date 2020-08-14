/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keypad.h
 * Description:
 *
 * History:
 * Version   Date              Author    DefectNum       Description
 *******************************************************************************/
#ifndef __KEYLED_PT6961_H__
#define __KEYLED_PT6961_H__

#if 0//!defined(BOARD_TYPE_hi3716mdmo3avera) && !defined(BOARD_TYPE_hi3716mdmo3bvera) 
#define GROUP   (5)
#define STB     (5)
#define DIN     (0)
#define DOUT    (3)
#define CLK     (1)
#endif

#ifdef HI_KEYLED_PT6961_CLOCK_GPIO
#define CLK_PT6961  HI_KEYLED_PT6961_CLOCK_GPIO
#define STB_PT6961  HI_KEYLED_PT6961_STB_GPIO
#define DIN_PT6961  HI_KEYLED_PT6961_DIN_GPIO
#define DOUT_PT6961 HI_KEYLED_PT6961_DOUT_GPIO
#else
#define CLK_PT6961 0x2a
#define STB_PT6961 0x2b
#define DIN_PT6961 0x2c
#define DOUT_PT6961 0x2d
#endif

#define SCAN_INTV   (30)    //ms

#define DISPLAY_ON  1
#define DISPLAY_OFF  0
#define KEY_MACRO_NO (0xff)
#define DISPLAY_REG_NUM     (14)
#define DISPLAY_SEG_CURR     (4)
#define KEY_COL_NUM     10
#define KEY_REAL_NUM     30
#define DISPLAY_MODE_NUM 4
#define DISPLAY_REG_START_ADDR  (0x00)


HI_S32 KEYLED_KEY_Open_PT6961(HI_VOID);
HI_S32 KEYLED_KEY_Close_PT6961(HI_VOID);
HI_S32 KEYLED_KEY_Reset_PT6961(HI_VOID);
HI_S32 KEYLED_KEY_GetValue_PT6961(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId);
HI_S32 KEYLED_KEY_SetBlockTime_PT6961(HI_U32 u32BlockTimeMs);
HI_S32 KEYLED_KEY_SetRepTime_PT6961(HI_U32 u32RepTimeMs);
HI_S32 KEYLED_KEY_IsRepKey_PT6961(HI_U32 u32IsRepKey);
HI_S32 KEYLED_KEY_IsKeyUp_PT6961(HI_U32 u32IsKeyUp);

HI_S32 KEYLED_LED_Open_PT6961(HI_VOID);
HI_S32 KEYLED_LED_Close_PT6961(HI_VOID);
HI_S32 KEYLED_LED_Display_PT6961(HI_U32 u32CodeValue);
HI_S32 KEYLED_LED_DisplayTime_PT6961(HI_UNF_KEYLED_TIME_S stKeyLedTime);
HI_S32 KEYLED_LED_SetFlashPin_PT6961(HI_UNF_KEYLED_LIGHT_E enPin);
HI_S32 KEYLED_LED_SetFlashFreq_PT6961(HI_UNF_KEYLED_LEVEL_E enLevel);

HI_S32 KEYLED_GetProcInfo_PT6961(KEYLED_PROC_INFO_S *stInfo);

HI_S32 KEYLED_Suspend_PT6961(HI_VOID);
HI_S32 KEYLED_Resume_PT6961(HI_VOID);



#endif
