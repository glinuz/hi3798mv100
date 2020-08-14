/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   keyled.h
* Description:
*
* History:
* Version   Date                Author          DefectNum     Description
***********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#ifndef __KEYLED_H__
#define __KEYLED_H__
#include "hi_type.h"
#define KEYLED_PHY_ADDR             (0xF8003000)
#define GPIO5_PHY_ADDR 				(0xF8004000)
#define GPIO5_6 					(0x1 << 6)

#define KEYBUF_LEN            (8)
#define KEY_NUM               (8)

#define DOT_FLASH_TIME  (570)
#define DEFAULT_BLOCKTIME   60000   /*1 minute*/

/* key status */
#define KEY_PRESS             (0x0)           /* press */
#define KEY_HOLD              (0x01)          /* hold */
#define KEY_RELEASE           (0x02)          /* release */

/* LEDC_CONTROL */
#define KEYPAD_ENABLE        (1 << 3)
#define KEYPAD_PRESS_INTR    (1 << 1)
#define KEYPAD_RELEASE_INTR  (1 << 0)

/* LEDC_KEYDATA */
#define KEY_7_PRESS          (1 << 7)
#define KEY_6_PRESS          (1 << 6)
#define KEY_5_PRESS          (1 << 5)
#define KEY_4_PRESS          (1 << 4)
#define KEY_3_PRESS          (1 << 3)
#define KEY_2_PRESS          (1 << 2)
#define KEY_1_PRESS          (1 << 1)
#define KEY_0_PRESS          (1 << 0)

#define KEY_65comb_PRESS (KEY_6_PRESS|KEY_5_PRESS )   /* used for upgrade*/
#define KEYPAD_ERROR         (1000)           /*error key*/

/*clock value*/
#define SYSTEM_CLK                       (24)               /* unit:MHz 108M*/
#define CLKTIM_DEFAULT                   (250)           /* unit:KHz 500*/
#define FRETIM_DEFAULT                   (450)           /* unit:Hz 200H*/
#define FLASHTIM_DEFAULT                 (60)           /* unit:Hz 10Hz*/
#define KEY_DEFAULT                      (60)             /* unit:Hz 4Hz */

/*frequency level*/
#define FREQ_LEVEL1                      (108)           /* 108Hz */
#define FREQ_LEVEL2                      (108*2)         /* 108*2Hz */
#define FREQ_LEVEL3                      (108*3)         /* 108*3Hz */
#define FREQ_LEVEL4                      (108*4)         /* 108*4Hz */
#define FREQ_LEVEL5                      (108*5)         /* 108*5Hz */

/*flash level*/
#define FLASH_LEVEL1                     (1)
#define FLASH_LEVEL2                     (2)
#define FLASH_LEVEL3                     (3)
#define FLASH_LEVEL4                     (4)
#define FLASH_LEVEL5                     (5)

/*registers offset*/
#define LEDC_CONTROL                     (0x000)        
#define LEDC_CONFIG                      (0x004)        
#define LEDC_KEYINT                      (0x008)        
#define LEDC_KEYDATA                     (0x00C)        
#define LEDC_CLKTIM                      (0x010)        
#define LEDC_FRETIM                      (0x014)        
#define LEDC_FLASHTIM                    (0x018)        
#define LEDC_KEYTIM                      (0x01C)        
#define LEDC_DATA1                       (0x020)        
#define LEDC_DATA2                       (0x024)        
#define LEDC_DATA3                       (0x028)        
#define LEDC_DATA4                       (0x02C)        
#define LEDC_DATA5 						 (0x030)        /* */
#define LEDC_SYSTIM 					 (0x34)        /* config system frequeny divide 0~15 to compatible 1*8 keyscan rate*/

/* Ledc control */
#define LEDC_ENABLE                      (1 << 8)       
#define LEDC_FLASH_4                     (1 << 7)       
#define LEDC_FLASH_3                     (1 << 6)       
#define LEDC_FLASH_2                     (1 << 5)       
#define LEDC_FLASH_1                     (1 << 4)       
#define LEDC_FLASH_ALL                   (LEDC_FLASH_1 | LEDC_FLASH_2 | LEDC_FLASH_3 | LEDC_FLASH_4)

/* ledc config */
#define KEY_SCAN_LEVEL      (1 << 4)        /*set 1*8 key output and sampling LEVEL 1: high level 0: low level*/
#define LED_NUM             (1 << 3)        /*select LED number 1: 5LED 0: 4LED*/
#define KEY_SCAN_MODE       (1 << 2)        /*KEY scan mode 1: 8*1  0: 4*2 */
#define LEDC_ANODE          (1 << 1)   //(0 << 1)
#define LEDC_HIGHT_LEVEL    (1 << 0)

#define LED_NEGATIVE

#define HI_ERR_KEYLED_NOT_INIT                      (HI_S32)(0x804B0001)
#define HI_ERR_KEYLED_INVALID_PARA                  (HI_S32)(0x804B0002)
#define HI_ERR_KEYLED_NULL_PTR                      (HI_S32)(0x804B0003)
#define HI_ERR_KEYLED_NO_NEW_KEY                    (HI_S32)(0x804B0004)
#define HI_ERR_KEYLED_TIMEOUT                       (HI_S32)(0x804B0005)
#define HI_ERR_KEYLED_FAILED_SETMODE                (HI_S32)(0x804B0006)
#define HI_ERR_KEYLED_FAILED_DISPLAY                (HI_S32)(0x804B0007)

typedef enum hiHI_KEYLED_LEDNUM_E
{
    HI_KEYLED_4LED = 0x0,
    HI_KEYLED_5LED,
    HI_KEYLED_LEDNUM_MAX
} HI_KEYLED_LEDNUM_E;

typedef enum hiHI_KEYLED_KEYSCAN_E
{
    HI_KEYLED_2MUL4 = 0x0,
    HI_KEYLED_1MUL8,
    HI_KEYLED_KEYSCAN_MAX
} HI_KEYLED_KEYSCAN_E;

typedef enum hiHI_KEYLED_KEYLEVEL_E
{
    HI_KEYLED_1MUL8_LOW = 0x0,
    HI_KEYLED_1MUL8_HIGH,
    HI_KEYLED_KEYLEVEL_MAX
} HI_KEYLED_KEYLEVEL_E;

typedef struct hiUNF_KEYLED_Mode_S
{
	HI_KEYLED_LEDNUM_E LedNum;
	HI_KEYLED_KEYSCAN_E KeyScanMode;
    HI_KEYLED_KEYLEVEL_E KeyLevel;
}HI_KEYLED_MODE_S;

static HI_S32  g_s32KEYLEDFd = 0;

#define CheckKeyledDeviceFd()\
do\
{\
    if(0 >= g_s32KEYLEDFd)\
    {\
        /*printf("keyled not init\n");*/\
        return HI_ERR_KEYLED_NOT_INIT;\
    }\
}while(0)

#define CheckKeyledDeviceRepeatFd()\
do\
{\
    if( 0 < g_s32KEYLEDFd)\
    {\
        /*printf("keyled repeat init\n");*/\
        return HI_SUCCESS;\
    }\
}while(0)

#define CheckKeyledNULLPointer(p)\
do\
{\
    if(HI_NULL == (p))\
    {\
        /*printf("pointer is null\n");*/\
        return HI_ERR_KEYLED_NULL_PTR;\
    }\
}while(0)

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
