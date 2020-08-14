/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keypad.h
 * Description:
 *
 * History:
 * Version   Date              Author    DefectNum       Description
 *******************************************************************************/
#ifndef __KEYLED_H__
#define __KEYLED_H__
#include "hi_type.h"

#define LED_BIT_COUNT         4  

#define KEY_PRESS             (0x0)           
#define KEY_HOLD              (0x01)          
#define KEY_RELEASE           (0x02)  

#ifdef HI_KEYLED_CT1642_CLOCK_GPIO
#define CLK_CT1642 HI_KEYLED_CT1642_CLOCK_GPIO
#define DAT_CT1642 HI_KEYLED_CT1642_DAT_GPIO
#define KEY_CT1642 HI_KEYLED_CT1642_KEY_GPIO
#else
#define CLK_CT1642 0x2a
#define DAT_CT1642 0x2c
#define KEY_CT1642 0x2f
#endif

#define KEY_MACRO_NO (0xff)

#define DISPLAY_ON   1
#define DISPLAY_OFF  0
#define KEY_MACRO_NO (0xff)
#define DISPLAY_REG_NUM     (14)
#define DISPLAY_SEG_CURR     (4)
#define KEY_COL_NUM     10
//#define KEY_REAL_NUM     30
#define DISPLAY_MODE_NUM 4
#define DISPLAY_REG_START_ADDR  (0x00)

#define	STB_KEY_MENU	       (11)
#define	STB_KEY_OK 	           (12)
#define	STB_KEY_UP	           (13)
#define	STB_KEY_DOWN	       (14)
#define	STB_KEY_LEFT	       (15)
#define	STB_KEY_RIGHT	       (16)
#define	STB_KEY_EXIT	       (17)
#define	STB_KEY_BUTT           (18)
#define	STB_KEY_NULL	       (0 )
#define STB_KEY_UPGRADE        (STB_KEY_MENU+STB_KEY_OK) 
#define STB_KEY_HOLD           (0x5a)

#define HI_ERR_KEYLED_NOT_INIT                      (HI_S32)(0x804B0001)
#define HI_ERR_KEYLED_INVALID_PARA                  (HI_S32)(0x804B0002)
#define HI_ERR_KEYLED_NULL_PTR                      (HI_S32)(0x804B0003)
#define HI_ERR_KEYLED_NO_NEW_KEY                    (HI_S32)(0x804B0004)
#define HI_ERR_KEYLED_TIMEOUT                       (HI_S32)(0x804B0005)
#define HI_ERR_KEYLED_FAILED_SETMODE                (HI_S32)(0x804B0006)
#define HI_ERR_KEYLED_FAILED_DISPLAY                (HI_S32)(0x804B0007)

static unsigned int g_s32KEYLEDFd = 0;

#define KEYLED_TRACE_LEVEL 9

#ifndef HI_ADVCA_FUNCTION_RELEASE 
#define keyled_trace(level, msg ...) do \
    { \
        if ((level) >= KEYLED_TRACE_LEVEL) \
        {  \
            printf("keyled_trace:%s:%d: ", __FILE__, __LINE__); \
            printf(msg); \
            printf("\n"); \
        } \
    } while (0)
#else
#define keyled_trace(level, msg ...) do \
    { \
    } while (0)
#endif

#define CheckKeyledDeviceFd() \
    do \
    { \
        if (0 >= g_s32KEYLEDFd) \
        { \
            keyled_trace(8, "keyled not init\n"); \
            return HI_ERR_KEYLED_NOT_INIT; \
        } \
    } while (0)

#define CheckKeyledDeviceRepeatFd() \
    do \
    { \
        if (0 < g_s32KEYLEDFd) \
        { \
            keyled_trace(8, "keyled repeat init\n"); \
            return HI_SUCCESS; \
        } \
    } while (0)

#define CheckKeyledNULLPointer(p) \
    do \
    { \
        if (HI_NULL == (p)) \
        { \
            keyled_trace(8, "pointer is null\n"); \
            return HI_ERR_KEYLED_NULL_PTR; \
        } \
    } while (0)


#endif
