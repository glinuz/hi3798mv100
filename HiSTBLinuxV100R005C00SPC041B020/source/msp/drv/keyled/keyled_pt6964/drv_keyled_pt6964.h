/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keypad.h
 * Description:
 *
 * History:
 * Version   Date              Author    DefectNum       Description
 *******************************************************************************/
#ifndef __KEYLED_PT6964_H__
#define __KEYLED_PT6964_H__

#ifdef HI_KEYLED_PT6964_CLOCK_GPIO
#define CLK_PT6964  HI_KEYLED_PT6964_CLOCK_GPIO
#define STB_PT6964  HI_KEYLED_PT6964_STB_GPIO
#define DIN_PT6964  HI_KEYLED_PT6964_DINOUT_GPIO
#define DOUT_PT6964 HI_KEYLED_PT6964_DINOUT_GPIO
#else
#define CLK_PT6964  0x2a
#define STB_PT6964  0x2b
#define DIN_PT6964  0x2c
#define DOUT_PT6964 0x2c
#endif

//#define AHB_CLK (54)  //Mhz
#define SCAN_INTV_PT6964   (30)    //ms
//#define TIMERX_LOAD_VALUE   ((AHB_CLK) * (SCAN_INTV_PT6964 * 1000))

#define KEY_MACRO_NO (0xff)
#define DISPLAY_REG_NUM     (14)
#define DISPLAY_SEG_MAX     (7)
#define DISPLAY_SEG_CURR     (4)
#define KEY_COL_NUM     10
#define KEY_REAL_NUM     30
#define DISPLAY_MODE_NUM 4
#define DISPLAY_REG_START_ADDR  (0x00)

#define SEC_TIMING  (500)   /*500 ms*/

HI_S32 KEYLED_KEY_Open_PT6964(HI_VOID);
HI_S32 KEYLED_KEY_Close_PT6964(HI_VOID);
HI_S32 KEYLED_KEY_Reset_PT6964(HI_VOID);
HI_S32 KEYLED_KEY_GetValue_PT6964(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId);
HI_S32 KEYLED_KEY_SetBlockTime_PT6964(HI_U32 u32BlockTimeMs);
HI_S32 KEYLED_KEY_SetRepTime_PT6964(HI_U32 u32RepTimeMs);
HI_S32 KEYLED_KEY_IsRepKey_PT6964(HI_U32 u32IsRepKey);
HI_S32 KEYLED_KEY_IsKeyUp_PT6964(HI_U32 u32IsKeyUp);

HI_S32 KEYLED_LED_Open_PT6964(HI_VOID);
HI_S32 KEYLED_LED_Close_PT6964(HI_VOID);
HI_S32 KEYLED_LED_Display_PT6964(HI_U32 u32CodeValue);
HI_S32 KEYLED_LED_DisplayTime_PT6964(HI_UNF_KEYLED_TIME_S stKeyLedTime);
HI_S32 KEYLED_LED_SetFlashPin_PT6964(HI_UNF_KEYLED_LIGHT_E enPin);
HI_S32 KEYLED_LED_SetFlashFreq_PT6964(HI_UNF_KEYLED_LEVEL_E enLevel);

HI_S32 KEYLED_GetProcInfo_PT6964(KEYLED_PROC_INFO_S *stInfo);

HI_S32 KEYLED_Suspend_PT6964(HI_VOID);
HI_S32 KEYLED_Resume_PT6964(HI_VOID);



#endif
