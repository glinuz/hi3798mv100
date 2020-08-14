/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#ifndef __KEYLED_H__
#define __KEYLED_H__
#include "hi_type.h"
 

#define KEY_PRESS             (0x0)           
#define KEY_HOLD              (0x01)          
#define KEY_RELEASE           (0x02)  

#define KEY_PWR   HI_KEYLED_GPIOKEY_POWER_GPIO  
#define KEY_OK    HI_KEYLED_GPIOKEY_OK_GPIO
#define KEY_MENU  HI_KEYLED_GPIOKEY_MENU_GPIO
#define KEY_UP    HI_KEYLED_GPIOKEY_UP_GPIO 
#define KEY_DOWN  HI_KEYLED_GPIOKEY_DOWN_GPIO 
#define KEY_LEFT  HI_KEYLED_GPIOKEY_LEFT_GPIO
#define KEY_RIGHT HI_KEYLED_GPIOKEY_RIGHT_GPIO 

#define KEY_MACRO_NO (0xff)

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
        { \
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
