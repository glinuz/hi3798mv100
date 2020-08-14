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
#include "hi_drv_gpio.h"

#define KEYBUF_LEN            (8)
#define KEY_NUM               (8)

#define DOT_FLASH_TIME  (570)
#define DEFAULT_BLOCKTIME   60000   /*1 minute*/


#define KEY_PRESS             (0x00)           
#define KEY_HOLD              (0x01)          
#define KEY_RELEASE           (0x02)  

#ifdef HI_KEYLED_FD650_CLOCK_GPIO
#define CLK_FD650  HI_KEYLED_FD650_CLOCK_GPIO
#define DINOUT_FD650  HI_KEYLED_FD650_DINOUT_GPIO
#else
#define CLK_FD650  0x2b
#define DINOUT_FD650  0x2d
#endif


//#define GROUP   (5)
//#define STB     (5)   // for FD650 , STB is no use
//#define DIO     (45)   // for CT1668 and PT6964, use one gpio for DI and DO
//#define CLK     (43)

//#define	FD650_GROUP     (GROUP)
#define	FD650_SCL       (CLK_FD650)
#define	FD650_SDA       (DINOUT_FD650)
#define HIGH    (1)
#define LOW     (0)
#define FD50_OUTPUT LOW
#define FD50_INPUT HIGH

#define DELAY	udelay(1)
#define FD650_SCL_SET        HI_DRV_GPIO_WriteBit(FD650_SCL, HIGH)
#define FD650_SCL_CLR        HI_DRV_GPIO_WriteBit(FD650_SCL, LOW)
#define FD650_SCL_D_OUT      HI_DRV_GPIO_SetDirBit(FD650_SCL, FD50_OUTPUT)
#define FD650_SDA_SET        HI_DRV_GPIO_WriteBit(FD650_SDA, HIGH)
#define FD650_SDA_CLR        HI_DRV_GPIO_WriteBit(FD650_SDA, LOW)
#define FD650_SDA_IN         hi_gpio_read_SDA_IN()
#define FD650_SDA_D_OUT      HI_DRV_GPIO_SetDirBit(FD650_SDA, FD50_OUTPUT)
#define FD650_SDA_D_IN       HI_DRV_GPIO_SetDirBit(FD650_SDA, FD50_INPUT)

#define FD650_BIT_ENABLE     0x01 
#define FD650_BIT_SLEEP      0x04
#define FD650_BIT_7SEG       0x08
#define FD650_BIT_INTENS1    0x10
#define FD650_BIT_INTENS2    0x20
#define FD650_BIT_INTENS3    0x30
#define FD650_BIT_INTENS4    0x40
#define FD650_BIT_INTENS5    0x50
#define FD650_BIT_INTENS6    0x60
#define FD650_BIT_INTENS7    0x70
#define FD650_BIT_INTENS8    0x00

#define FD650_SYSOFF     0x0400
#define FD650_SYSON      ( FD650_SYSOFF | FD650_BIT_ENABLE )
#define FD650_SLEEPOFF    FD650_SYSOFF 
#define FD650_SLEEPON    ( FD650_SYSOFF | FD650_BIT_SLEEP )
#define FD650_7SEG_ON    ( FD650_SYSON | FD650_BIT_7SEG )
#define FD650_8SEG_ON    ( FD650_SYSON | 0x00 )
#define FD650_SYSON_1    ( FD650_SYSON | FD650_BIT_INTENS1 )
#define FD650_SYSON_2    ( FD650_SYSON | FD650_BIT_INTENS2 )
#define FD650_SYSON_3    ( FD650_SYSON | FD650_BIT_INTENS3 )
#define FD650_SYSON_4    ( FD650_SYSON | FD650_BIT_INTENS4 )
#define FD650_SYSON_5    ( FD650_SYSON | FD650_BIT_INTENS5 )
#define FD650_SYSON_6    ( FD650_SYSON | FD650_BIT_INTENS6 )
#define FD650_SYSON_7    ( FD650_SYSON | FD650_BIT_INTENS7 )
#define FD650_SYSON_8    ( FD650_SYSON | FD650_BIT_INTENS8 )


#define FD650_DIG0        0x1400            
#define FD650_DIG1        0x1500            
#define FD650_DIG2        0x1600            
#define FD650_DIG3        0x1700            
#define FD650_DOT         0x0080 


#define FD650_GET_KEY     0x0700

#define DISPLAY_ON  1
#define DISPLAY_OFF  0
#define KEY_MACRO_NO (0xff)
#define DISPLAY_REG_NUM     (14)
#define DISPLAY_SEG_CURR     (4)
#define KEY_COL_NUM     10
//#define KEY_REAL_NUM     30
#define DISPLAY_MODE_NUM 4
#define DISPLAY_REG_START_ADDR  (0x00)

#define HI_ERR_KEYLED_NOT_INIT                      (HI_S32)(0x804B0001)
#define HI_ERR_KEYLED_INVALID_PARA                  (HI_S32)(0x804B0002)
#define HI_ERR_KEYLED_NULL_PTR                      (HI_S32)(0x804B0003)
#define HI_ERR_KEYLED_NO_NEW_KEY                    (HI_S32)(0x804B0004)
#define HI_ERR_KEYLED_TIMEOUT                       (HI_S32)(0x804B0005)
#define HI_ERR_KEYLED_FAILED_SETMODE                (HI_S32)(0x804B0006)
#define HI_ERR_KEYLED_FAILED_DISPLAY                (HI_S32)(0x804B0007)

static unsigned int g_s32KEYLEDFd = 0;

#define KEYLED_TRACE_LEVEL 8

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
            printf("keyled not init\n"); \
            return HI_ERR_KEYLED_NOT_INIT; \
        } \
    } while (0)

#define CheckKeyledDeviceRepeatFd() \
    do \
    { \
        if (0 < g_s32KEYLEDFd) \
        { \
            printf("keyled repeat init\n"); \
            return HI_SUCCESS; \
        } \
    } while (0)

#define CheckKeyledNULLPointer(p) \
    do \
    { \
        if (HI_NULL == (p)) \
        { \
            printf("pointer is null\n"); \
            return HI_ERR_KEYLED_NULL_PTR; \
        } \
    } while (0)


#endif
