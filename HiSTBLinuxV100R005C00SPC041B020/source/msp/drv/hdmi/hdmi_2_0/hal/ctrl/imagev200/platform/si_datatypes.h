/******************************************************************************
*
* Copyright 2013, Silicon Image, Inc.  All rights reserved.
* No part of this work may be reproduced, modified, distributed, transmitted,
* transcribed, or translated into any language or computer format, in any form
* or by any means without written permission of
* Silicon Image, Inc., 1140 East Arques Avenue, Sunnyvale, California 94085
*
*****************************************************************************/
/**
* @file si_datatypes.h
*
* @brief Standard datatypes
*
*****************************************************************************/

#ifndef __SI_DATATYPES_H__
#define __SI_DATATYPES_H__   

/***** #include statements ***************************************************/
#include "hi_type.h"
#ifndef HDMI_BUILD_IN_BOOT
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include "hi_debug.h"
#else
#if defined(CONFIG_HDMI_STB_SDK)
#ifndef HI_MINIBOOT_SUPPORT
#include <uboot.h>
extern int vsnrprintf(char *str, size_t size, const char *format, va_list ap);
#else
#include "stdio.h"
#endif
#elif defined(CONFIG_HDMI_BVT_SDK)
#include "ubi_uboot.h"
#include "malloc.h"
#endif
#endif
/***** public macro definitions **********************************************/

#define SII_MEMCPY(pdes, psrc, size)            memcpy(pdes, psrc, size)
#define SII_MEMCMP(pdes, psrc, size)            memcmp(pdes, psrc, size)
#define SII_MEMSET(pdes, value, size)           memset(pdes, value, size)
#define SII_STRCPY(pdes, psrc, len)             strncpy(pdes, psrc, len)
#define SII_STRCMP(pdes, psrc, len)             strncmp(pdes, psrc, len)
#define SII_STRLEN(pstr)                        strlen(pstr)
#define SII_VSPRINTF(dstr, len, fstr, arg)      vsnprintf(dstr, len, fstr, arg)
#define SII_SPRINTF(arg)                        sprintf arg                  // use macro as: SII_SPRINTF((dstr, "%d", i));
#if defined(CONFIG_HDMI_STB_SDK)
#define SII_SNPRINTF(arg)                       snprintf arg   
#elif defined(CONFIG_HDMI_BVT_SDK)
#define SII_SNPRINTF(arg)                       1   
#endif

#ifndef HDMI_BUILD_IN_BOOT
#ifndef HI_ADVCA_FUNCTION_RELEASE
#define SII_PRINTF(fmt, args...)                printk(fmt, ## args)
#else
#define SII_PRINTF(fmt, args...)
#endif
#define SII_VNSPRINTF(dstr, len, fstr, arg)     vsnprintf(dstr, len, fstr, arg)
#else
#if defined(CONFIG_HDMI_STB_SDK)
#define SII_VNSPRINTF(dstr, len, fstr, arg)     vsnrprintf(dstr, len, fstr, arg)
#elif defined(CONFIG_HDMI_BVT_SDK)
#define SII_VNSPRINTF(dstr, len, fstr, arg)     1
#endif
#ifndef HI_ADVCA_FUNCTION_RELEASE
#define SII_PRINTF(fmt, args...)                printf(fmt, ## args)
#else
#define SII_PRINTF(fmt, args...)
#endif

#endif
#define SII_DEF_PI                      3.14159265358979323846f

#ifndef bool_t
//#ifndef HDMI_BUILD_IN_BOOT
#define bool_t                          HI_BOOL
//#else
//typedef unsigned char bool_t;
//#endif
#endif

#ifndef __cplusplus
#define true                            (1)
#define false                           (0)
#endif // __cplusplus

#ifndef TRUE
#define TRUE                            (true)
#endif

#ifndef FALSE
#define FALSE                           (false)
#endif

/* Keil 8051 specific definitions */
#define SII_ROM                          code
#define SII_XDATA                        xdata

#ifndef NULL
#define NULL                             ((void*)0)
#endif

#define SII_BIT0                         0x01
#define SII_BIT1                         0x02
#define SII_BIT2                         0x04
#define SII_BIT3                         0x08
#define SII_BIT4                         0x10
#define SII_BIT5                         0x20
#define SII_BIT6                         0x40
#define SII_BIT7                         0x80
#define SII_BIT8                         0x0100
#define SII_BIT9                         0x0200
#define SII_BIT10                        0x0400
#define SII_BIT11                        0x0800
#define SII_BIT12                        0x1000
#define SII_BIT13                        0x2000
#define SII_BIT14                        0x4000
#define SII_BIT15                        0x8000

#define SET_BITS                         (0xFF)
#define CLEAR_BITS                       (0x00)

/* Bit manipulation macros */
#define SII_SET_BIT(p, bit)              ( *(p) |= (1<<(bit)) )
#define SII_CLR_BIT(p, bit)              ( *(p) &= (~(1<<(bit))) )
#define SII_PUT_BIT(p, bit, b)           ( *(p) = (b) ? (*(p)|(1<<(bit))) : (*(p)&(~(1<<(bit)))) )

/***** public type definitions ***********************************************/

/* C99 defined data types.  */
#if 0
typedef unsigned char      uint8_t;
typedef unsigned int       uint16_t;
typedef unsigned long int  uint32_t;

typedef signed char        int8_t;
typedef signed int         int16_t;
typedef signed long int    int32_t;
#endif
//typedef bool_t bool
#ifndef HDMI_BUILD_IN_BOOT
typedef int32_t            int_t;
typedef uint32_t           uint_t;
#else
#if 0
typedef unsigned char      uint8_t;
typedef unsigned int       uint16_t;
typedef unsigned long int  uint32_t;

typedef signed char        int8_t;
typedef signed int         int16_t;
typedef signed long int    int32_t;
typedef unsigned long int  size_t;
#endif
typedef unsigned long int  uint_t;
#endif
// Bit field types used in structures
typedef unsigned char      bit_fld_t;
typedef unsigned short     bit_fld16_t;

#endif  // __SI_DATATYPES_H__
