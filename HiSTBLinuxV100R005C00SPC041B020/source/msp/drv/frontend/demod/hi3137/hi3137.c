/***********************************************************************************
*              Copyright 2012 - 2013, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi3137.c
* Description:
*
* History:
* Version   Date             Author     DefectNum        Description
* main\1    2013-09-22   w00203631    NULL                Create this file.
***********************************************************************************/

#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>

#include "drv_demod.h"
#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_drv_gpioi2c.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
//#include " hi_unf_ecs_type.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "hi_type.h"
#include "hi_debug.h"
#include "mxl603.h"
#include "mxl608.h"
#include "silabs.h"
#include "R836.h"
#include "tda182i5a.h"
#include "si2144.h"
#include "hi3137.h"
#include "hi3138.h"

#define     hi3137_abs(value)       ((value >=0)?(value):(-value))

extern TUNER_ATTR s_stTunerResumeInfo[];

static HI_UNF_TUNER_DEV_TYPE_E g_stTunerType;
static HI_U32                  g_stXtalClk = 24000;
static HI_U8 g_stI2cChannel;
HI_U32 g_u32TerrestrialIFKhz;  /*中心频率kHz*/
static HI_U8 g_u8MCUSoftWare[] = {0x02,0x05,0x17,0xE4,0xF5,0x17,0x90,0xFE,0x2F,0x74,0x6A,0xF0,0x90,0xFE,0xE0,0x74,
                       0x98,0xF0,0xA3,0xE4,0xF0,0xA3,0x04,0xF0,0xA3,0xE4,0xF0,0xA3,0xF0,0xA3,0x74,0x57,
                       0xF0,0xA3,0xE4,0xF0,0xA3,0x74,0x11,0xF0,0x90,0xFE,0xEB,0xE4,0xF0,0xA3,0xF0,0x90,
                       0xFE,0xE8,0xF0,0xA3,0x74,0x10,0xF0,0xA3,0xE4,0xF0,0xF5,0x08,0xF5,0x09,0xE5,0x09,
                       0x24,0xF0,0xFD,0xE5,0x08,0x34,0xFE,0x8D,0x82,0xF5,0x83,0xE5,0x09,0xF0,0x05,0x09,
                       0xE5,0x09,0x70,0x02,0x05,0x08,0x64,0x10,0x45,0x08,0x70,0xE2,0x90,0xFE,0x89,0xE0,
                       0xF5,0x0A,0x53,0x0A,0xDF,0xE5,0x0A,0xF0,0x90,0xFE,0xED,0xE4,0xF0,0xA3,0x74,0x04,
                       0xF0,0x90,0xFE,0x2C,0xE0,0xF5,0x0A,0xC3,0x13,0x54,0x01,0xF5,0x0C,0xE5,0x0A,0x13,
                       0x13,0x13,0x54,0x01,0xF5,0x0D,0xE5,0x0A,0xC4,0x54,0x01,0xF5,0x0E,0xE5,0x0A,0xC4,
                       0x13,0x54,0x01,0xF5,0x10,0xE5,0x17,0x14,0x70,0x02,0x21,0x1D,0x14,0x70,0x02,0x21,
                       0xC4,0x14,0x70,0x02,0x81,0x75,0x14,0x70,0x02,0x81,0xB4,0x24,0x04,0x60,0x02,0x81,
                       0xE7,0xE5,0x0C,0x64,0x01,0x60,0x02,0x81,0xE7,0x75,0x17,0x01,0x90,0xFE,0x60,0x74,
                       0x23,0xF0,0xA3,0x74,0x8B,0xF0,0x90,0xFE,0x66,0x74,0xD9,0xF0,0xA3,0x74,0x10,0xF0,
                       0xA3,0x74,0x53,0xF0,0xA3,0x74,0x40,0xF0,0x90,0xFE,0x74,0x74,0x20,0xF0,0x90,0xFE,
                       0x62,0x74,0xC3,0xF0,0xE4,0xF5,0x19,0x90,0xFE,0x9F,0xE0,0xF5,0x0A,0x44,0x18,0xF0,
                       0x90,0xFE,0x55,0xE0,0xF5,0x0A,0x13,0x13,0x13,0x54,0x07,0xF5,0x12,0xE5,0x0A,0xC4,
                       0x13,0x13,0x13,0x54,0x01,0xF5,0x1A,0xE5,0x12,0x64,0x05,0x60,0x02,0x81,0xE7,0x90,
                       0xFE,0xEA,0xF0,0x7F,0x01,0xFE,0x91,0xEF,0x90,0xFE,0xEA,0x81,0xE4,0x90,0xFE,0xB3,
                       0xE0,0xC4,0x13,0x13,0x54,0x01,0xFF,0xE5,0x0C,0x70,0x04,0xF5,0x17,0x81,0xE7,0xE5,
                       0x1A,0x60,0x18,0x75,0x17,0x03,0x90,0xFE,0x61,0x74,0x81,0xF0,0xE5,0x12,0x90,0xFE,
                       0x67,0xB4,0x03,0x02,0x81,0xE4,0x74,0x10,0xF0,0x81,0xE7,0xEF,0x64,0x01,0x60,0x02,
                       0x81,0xE7,0x75,0x17,0x02,0x90,0xFE,0xB5,0xE0,0x54,0x03,0xF5,0x15,0x90,0xFE,0xB8,
                       0xE0,0xC4,0x54,0x0F,0xF5,0x11,0xE5,0x12,0x64,0x05,0x70,0x3A,0x90,0xFE,0x61,0x74,
                       0x8B,0xF0,0x90,0xFE,0x66,0x74,0xC9,0xF0,0xE5,0x15,0xB4,0x03,0x04,0x7F,0x96,0x80,
                       0x02,0x7F,0x76,0x90,0xFE,0x67,0xEF,0xF0,0x90,0xFE,0x69,0x74,0x40,0xF0,0x90,0xFE,
                       0x62,0x74,0xD3,0xF0,0xE5,0x11,0x90,0xFE,0x60,0xB4,0x05,0x05,0x74,0x23,0xF0,0x81,
                       0xE7,0x74,0xA3,0xF0,0x81,0xE7,0x90,0xFE,0x66,0x74,0xD9,0xF0,0xA3,0x74,0x10,0xF0,
                       0x90,0xFE,0x61,0x74,0x81,0xF0,0x90,0xFE,0x60,0x74,0xA3,0xF0,0x90,0xFE,0x69,0x74,
                       0x40,0xF0,0x81,0xE7,0xE5,0x0C,0x70,0x04,0xF5,0x17,0x81,0xE7,0xE5,0x0D,0x64,0x01,
                       0x60,0x02,0x81,0xE7,0x75,0x17,0x03,0x90,0xFE,0xC9,0xE0,0xF5,0x0A,0x13,0x13,0x54,
                       0x07,0xF5,0x13,0xE5,0x0A,0xC4,0x13,0x54,0x07,0xF5,0x14,0xE5,0x11,0xB4,0x06,0x09,
                       0xE5,0x14,0xB4,0x04,0x04,0x7F,0x01,0x80,0x02,0x7F,0x00,0x8F,0x18,0xE5,0x12,0x64,
                       0x05,0x60,0x02,0x61,0xBF,0xE5,0x11,0xB4,0x06,0x08,0x90,0xFE,0x61,0x74,0xCB,0xF0,
                       0x80,0x16,0xE5,0x11,0x90,0xFE,0x61,0xB4,0x03,0x05,0x74,0x9B,0xF0,0x80,0x09,0x74,
                       0xFB,0xF0,0x90,0xFE,0x69,0x74,0x10,0xF0,0xE5,0x13,0x70,0x0C,0x90,0xFE,0x67,0x74,
                       0x55,0xF0,0xA3,0x74,0x57,0xF0,0x80,0x4A,0xE5,0x13,0xB4,0x01,0x0C,0x90,0xFE,0x67,
                       0x74,0x76,0xF0,0xA3,0x74,0x59,0xF0,0x80,0x39,0xE5,0x13,0xB4,0x02,0x18,0xE5,0x14,
                       0xC3,0x94,0x02,0x50,0x04,0x7F,0x86,0x80,0x02,0x7F,0x96,0x90,0xFE,0x67,0xEF,0xF0,
                       0xA3,0x74,0x5A,0xF0,0x80,0x1C,0xE5,0x14,0xC3,0x94,0x02,0x50,0x04,0x7F,0x96,0x80,
                       0x02,0x7F,0xB6,0x90,0xFE,0x67,0xEF,0xF0,0xA3,0x74,0x5B,0xF0,0x90,0xFE,0x74,0x74,
                       0x20,0xF0,0xE5,0x11,0x64,0x05,0x60,0x02,0x61,0x92,0xE5,0x15,0x64,0x03,0x60,0x02,
                       0x61,0x92,0x90,0xFE,0xEA,0xE0,0xF5,0x0A,0xE4,0xF5,0x16,0xE5,0x0A,0xC3,0x94,0x40,
                       0x50,0x17,0xE5,0x16,0xC3,0x94,0x08,0x50,0x10,0x7F,0x04,0x7E,0x00,0x91,0xEF,0x05,
                       0x16,0x90,0xFE,0xEA,0xE0,0xF5,0x0A,0x80,0xE2,0xE5,0x0A,0xC3,0x94,0x40,0x50,0x02,
                       0x61,0x8D,0xE4,0xFF,0xFE,0xE4,0xF5,0x08,0xF5,0x09,0x90,0xFE,0xEF,0xE0,0x90,0x10,
                       0x00,0xF0,0x90,0xFE,0xEF,0xE0,0xF5,0x0B,0x90,0x10,0x01,0xF0,0x90,0xFE,0xEF,0xE0,
                       0xFD,0x90,0x10,0x02,0xF0,0x90,0xFE,0xEF,0xE0,0x90,0x10,0x03,0xF0,0xED,0x54,0x1F,
                       0x33,0x33,0x33,0x54,0xF8,0xFC,0xE5,0x0B,0x54,0xE0,0xC4,0x13,0x54,0x07,0x2C,0xFC,
                       0xD3,0x94,0x02,0x40,0x04,0xAB,0x04,0x80,0x02,0x7B,0x00,0xAC,0x03,0xE5,0x09,0x24,
                       0x08,0xFB,0xE5,0x08,0x34,0x10,0x8B,0x82,0xF5,0x83,0xE0,0xFD,0xEF,0x4E,0x70,0x04,
                       0xAD,0x04,0x80,0x03,0xED,0x2C,0xFD,0x8D,0x0F,0xE5,0x09,0x24,0x08,0xFD,0xE5,0x08,
                       0x34,0x10,0x8D,0x82,0xF5,0x83,0xE5,0x0F,0xF0,0xEF,0x64,0x0A,0x4E,0x70,0x06,0xE5,
                       0x09,0x45,0x08,0x60,0x07,0xE5,0x0F,0xD3,0x95,0x1B,0x40,0x03,0x85,0x0F,0x1B,0x05,
                       0x09,0xE5,0x09,0x70,0x02,0x05,0x08,0x70,0x04,0xE5,0x08,0x64,0x01,0x60,0x02,0x41,
                       0xCA,0x0F,0xBF,0x00,0x01,0x0E,0xEF,0x64,0x0B,0x4E,0x60,0x02,0x41,0xC5,0x90,0xFE,
                       0x42,0xE0,0xF5,0x0A,0xB4,0x5B,0x05,0x75,0x19,0x02,0x80,0x16,0xE5,0x1B,0xC3,0x94,
                       0x08,0x40,0x05,0x75,0x19,0x01,0x80,0x0A,0xE4,0xF5,0x19,0x80,0x05,0xE4,0xF5,0x17,
                       0xF5,0x19,0x90,0xFE,0xEA,0xE4,0xF0,0xE5,0x19,0xB4,0x02,0x07,0x90,0xFE,0x67,0x74,
                       0x44,0x80,0x0D,0xE5,0x19,0x64,0x01,0x60,0x02,0x81,0xE7,0x90,0xFE,0x67,0x74,0x55,
                       0xF0,0x90,0xFE,0x74,0x74,0x28,0xF0,0x90,0xFE,0x61,0x74,0xF1,0xF0,0x81,0xE7,0xE5,
                       0x11,0x64,0x01,0x60,0x0B,0xE5,0x11,0x64,0x02,0x60,0x05,0xE5,0x11,0xB4,0x07,0x08,
                       0x90,0xFE,0x61,0x74,0x91,0xF0,0x80,0x19,0xE5,0x11,0x64,0x03,0x60,0x05,0xE5,0x11,
                       0xB4,0x04,0x08,0x90,0xFE,0x61,0x74,0xC1,0xF0,0x80,0x06,0x90,0xFE,0x61,0x74,0xE1,
                       0xF0,0xE5,0x12,0x64,0x03,0x70,0x2E,0x90,0xFE,0x66,0x74,0xC9,0xF0,0xE5,0x13,0x70,
                       0x02,0x80,0x32,0xE5,0x13,0xB4,0x01,0x02,0x80,0x3A,0xE5,0x13,0xB4,0x02,0x11,0xE5,
                       0x18,0xB4,0x01,0x04,0x7F,0x54,0x80,0x02,0x7F,0x86,0x90,0xFE,0x67,0xEF,0x80,0x35,
                       0x90,0xFE,0x67,0x80,0x37,0xE5,0x12,0x64,0x04,0x70,0x3A,0x90,0xFE,0x66,0x74,0xC9,
                       0xF0,0xE5,0x13,0x70,0x0A,0xA3,0x74,0x55,0xF0,0xA3,0x74,0x57,0xF0,0x81,0xE7,0xE5,
                       0x13,0xB4,0x01,0x07,0x90,0xFE,0x67,0x74,0x76,0x80,0x0A,0xE5,0x13,0x90,0xFE,0x67,
                       0xB4,0x02,0x09,0x74,0x86,0xF0,0xA3,0x74,0x59,0xF0,0x81,0xE7,0x74,0x96,0xF0,0xA3,
                       0x74,0x5A,0xF0,0x81,0xE7,0x90,0xFE,0x66,0x74,0xD9,0xF0,0xA3,0x74,0x10,0xF0,0xA3,
                       0x74,0x53,0xF0,0x80,0x72,0xE5,0x0C,0x70,0x04,0xF5,0x17,0x80,0x20,0xE5,0x10,0xB4,
                       0x01,0x1B,0x90,0xFE,0x61,0xE0,0xF5,0x0A,0x54,0x0F,0xF5,0x0B,0xE5,0x0A,0xC4,0x54,
                       0x0F,0xFF,0xBF,0x0F,0x05,0xE5,0x0B,0x24,0xE0,0xF0,0x75,0x17,0x04,0xE5,0x13,0x64,
                       0x03,0x70,0x44,0xE5,0x14,0x64,0x01,0x70,0x3E,0xE5,0x11,0x64,0x03,0x70,0x38,0x90,
                       0xFE,0x74,0x80,0x2B,0xE5,0x0C,0x70,0x04,0xF5,0x17,0x80,0x2B,0xE5,0x13,0x64,0x03,
                       0x70,0x25,0xE5,0x14,0x64,0x01,0x70,0x1F,0xE5,0x11,0xB4,0x03,0x1A,0x90,0xFE,0x7A,
                       0xE0,0xF5,0x0A,0xA3,0xE0,0xF5,0x0A,0xC3,0x94,0x10,0x90,0xFE,0x74,0x50,0x05,0x74,
                       0xA0,0xF0,0x80,0x03,0x74,0x20,0xF0,0x7F,0x08,0x7E,0x00,0x91,0xEF,0x01,0x71,0xE4,
                       0xFD,0xFC,0xC3,0xED,0x9F,0xEC,0x9E,0x50,0x1D,0xE4,0xFB,0xFA,0x05,0x1D,0xE5,0x1D,
                       0x70,0x02,0x05,0x1C,0x0B,0xBB,0x00,0x01,0x0A,0xBA,0x03,0xF0,0xBB,0xE8,0xED,0x0D,
                       0xBD,0x00,0x01,0x0C,0x80,0xDC,0x22,0x78,0x7F,0xE4,0xF6,0xD8,0xFD,0x75,0x81,0x1D,
                       0x02,0x00,0x03};

static HI_S32 tuner_init(HI_U32 u32TunerPort)
{
    HI_S32 s32Ret = HI_SUCCESS;

    switch(g_stTunerOps[u32TunerPort].enTunerDevType)
    {
    #ifdef TUNER_DEV_TYPE_MXL603
        case HI_UNF_TUNER_DEV_TYPE_MXL603:
			g_u32TerrestrialIFKhz = 5380;
            //s32Ret = mxl603_init(u32TunerPort);
			break;
	#endif
    
	#ifdef TUNER_DEV_TYPE_MXL608
        case HI_UNF_TUNER_DEV_TYPE_MXL608:
            s32Ret = mxl608_init_tuner(u32TunerPort);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("mxl608 init tuner error.\n");
                return s32Ret;
            }
            break;
	#endif		
    #ifdef TUNER_DEV_TYPE_SI2147
        case HI_UNF_TUNER_DEV_TYPE_SI2147:
            //s32Ret = Si2147_Tune_DTV_Initial(u32TunerPort);
            break;
    #endif
    #ifdef TUNER_DEV_TYPE_RAFAEL836
        case HI_UNF_TUNER_DEV_TYPE_RAFAEL836:
            s32Ret = R840_Init(u32TunerPort);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("R840 init tuner error.\n");
                return s32Ret;
            }
            break;
    #endif
	#ifdef TUNER_DEV_TYPE_TDA182I5A
		case HI_UNF_TUNER_DEV_TYPE_TDA182I5A:
			s32Ret = tda182i5a_init_tuner(u32TunerPort);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("tda182i5a init tuner error.\n");
                return s32Ret;
            }
			break;
	#endif
	#ifdef TUNER_DEV_TYPE_SI2144
		case HI_UNF_TUNER_DEV_TYPE_SI2144:
			s32Ret = si2144_init_tuner(u32TunerPort,g_stTunerOps[u32TunerPort].enI2cChannel);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("si2144 init tuner error.\n");
                return s32Ret;
            }
			break;
	#endif
        default:
            HI_ERR_TUNER("tuner init error.\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 tuner_set_freq(HI_U32 u32TunerPort,HI_U32 u32Frequency,HI_U32 u32BandWidth,HI_U32 u32DVBMode)
{
    HI_S32 s32Ret = HI_SUCCESS;

    switch(g_stTunerOps[u32TunerPort].enTunerDevType)
    {
    
	#ifdef TUNER_DEV_TYPE_MXL603
        case HI_UNF_TUNER_DEV_TYPE_MXL603:
			g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth = u32BandWidth;
            s32Ret=mxl603_set_tuner(u32TunerPort,g_stTunerOps[u32TunerPort].enI2cChannel,u32Frequency);
            if(HI_FAILURE == s32Ret)
            {
                HI_ERR_TUNER("mxl603 set tuner error.\n");
                return s32Ret;
            }
            break;
	#endif	
	#ifdef TUNER_DEV_TYPE_MXL608
			case HI_UNF_TUNER_DEV_TYPE_MXL608:
			g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth = u32BandWidth;
            s32Ret=mxl608_set_tuner(u32TunerPort,g_stTunerOps[u32TunerPort].enI2cChannel,u32Frequency);
            if(HI_FAILURE == s32Ret)
            {
                HI_ERR_TUNER("mxl608 set tuner error.\n");
                return s32Ret;
            }
            break;
	#endif		
    #ifdef TUNER_DEV_TYPE_SI2147
        case HI_UNF_TUNER_DEV_TYPE_SI2147:
            //s32Ret=Si2147_Tune_DTV_Demo(u32TunerPort,u32Frequency*1000,u32BandWidth);
            break;
    #endif
    #ifdef TUNER_DEV_TYPE_RAFAEL836
        case HI_UNF_TUNER_DEV_TYPE_RAFAEL836:
            R840_set_tuner(u32TunerPort,u32Frequency,u32BandWidth,u32DVBMode);
            break;
    #endif
	#ifdef TUNER_DEV_TYPE_TDA182I5A
		case HI_UNF_TUNER_DEV_TYPE_TDA182I5A:
			tda182i5a_set_tuner(u32TunerPort,u32Frequency*1000, u32BandWidth);
			break;
	#endif
	#ifdef TUNER_DEV_TYPE_SI2144
		case HI_UNF_TUNER_DEV_TYPE_SI2144:
			si2144_set_tuner(u32TunerPort,u32Frequency*1000, u32BandWidth);
			break;
	#endif
        default:
            HI_ERR_TUNER("tuner set freq error.\n");
            return HI_FAILURE;
    }

    return s32Ret;
}

static HI_S32 demod_clock_out_reset(HI_U32 u32TunerPort)
{
    HI_U8 u8RegData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, Hi3137_IO_CTRL4, &u8RegData);
    u8RegData = u8RegData & 0xFB;
    u8RegData = (u8RegData & 0xE7) | (3<<3);
    qam_write_byte(u32TunerPort, Hi3137_IO_CTRL4, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_SDR_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xFE;
    qam_write_byte(u32TunerPort, Hi3137_SDR_CTRL, u8RegData);
    tuner_mdelay(100);
    u8RegData = u8RegData | 0x1;
    qam_write_byte(u32TunerPort, Hi3137_SDR_CTRL, u8RegData);
    return HI_SUCCESS;
}

static HI_S32 demod_change_demod_clk(HI_U32 u32TunerPort, HI_U32 u32DemodClkKHz, HI_U32 u32AdcClkKHz)
{
    HI_U32 u32DemoDiv = 0;
    HI_U8  u8DemoDivInt = 0,u8DemoDivFh = 0,u8DemoDivFl = 0;
    HI_U8  u8ClkAdcL = 0,u8ClkAdcM = 0,u8ClkDemL = 0,u8ClkDemM = 0,u8ClkDemH = 0;
    HI_U8  u8InitData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(g_stXtalClk == 0)
    {
        HI_ERR_TUNER("g_stXtalClk is 0.\n");
        return HI_FAILURE;
    }

    //dem clk freq 芯片缺省值是64MHz
    u32DemoDiv = u32DemodClkKHz * 12 * 4096 / g_stXtalClk;
    u8DemoDivInt = u32DemoDiv / 4096;
    u8DemoDivFh = (u32DemoDiv / 256) % 16;
    u8DemoDivFl = u32DemoDiv % 256;

    //switch i2c to the xo clk
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData & 0xFE;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);
    tuner_mdelay(5);
    //power down PLL0
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = u8InitData | 0x10;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);

    //set pll0
    qam_write_byte(u32TunerPort,Hi3137_PLL0_FBDIV,u8DemoDivInt);
    qam_write_byte(u32TunerPort,Hi3137_PLL0_FRAC_L,u8DemoDivFl);
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = (u8InitData & 0xF0) | u8DemoDivFh;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);

    //power up the pll
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = u8InitData & 0xEF;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);

    //delay 5ms,waiting PLL lock
    tuner_mdelay(5);

    //switch i2c to the clk demo
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData | 0x01;//u8InitData & 0xEF;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);

    //adc clk freq 芯片缺省值是32MHz
    switch(u32DemodClkKHz / u32AdcClkKHz)
    {
        case 1:
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL0, &u8InitData);
            u8InitData = (u8InitData & 0xC0) | 0x03;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL0, u8InitData);
            break;
        case 2:
        default:
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL0, &u8InitData);
            u8InitData = (u8InitData & 0xC0) | 0x01;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL0, u8InitData);
            break;
    }

    u8ClkAdcL = u32AdcClkKHz % 256;
    u8ClkAdcM = u32AdcClkKHz / 256;
    qam_write_byte(u32TunerPort,Hi3137_CLK_ADC_L,u8ClkAdcL);
    qam_write_byte(u32TunerPort,Hi3137_CLK_ADC_M,u8ClkAdcM);

    //reset adc ip
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData & 0xBF;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);
    u8InitData = u8InitData | 0x40;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);

    //power up the adc,set into the work mode
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL3,&u8InitData);
    u8InitData = u8InitData | 0x30;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL3,u8InitData);
    tuner_mdelay(5);

    //demo clk
    u8ClkDemL = u32DemodClkKHz % 256;
    u8ClkDemM = (u32DemodClkKHz / 256) % 256;
    u8ClkDemH = (u32DemodClkKHz / 65536) & 0x03;

    qam_write_byte(u32TunerPort,Hi3137_CLK_DEMO_L,u8ClkDemL);
    qam_write_byte(u32TunerPort,Hi3137_CLK_DEMO_M,u8ClkDemM);
    qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0xCF) | (u8ClkDemH << 4);
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);

    return HI_SUCCESS;
}

static HI_S32 demod_download_firmware(HI_U32 u32TunerPort)
{
	struct timeval stStartTime,stEndTime;
	HI_U32 i,u32Total,u32Row,u32Left;
	HI_S32 s32Ret;
	HI_U8 u8DataBuf[16]={0};

	do_gettimeofday(&stStartTime);
	qam_write_byte(u32TunerPort,0x8c,0);
	qam_write_byte(u32TunerPort,0x8b,0);
	u32Total = sizeof(g_u8MCUSoftWare)/sizeof(HI_U8);
	u32Left = u32Total%16;
	u32Row = u32Total/16;
	HI_INFO_TUNER("u32Total:%d,u32Row:%d,u32Left:%d\n",u32Total,u32Row,u32Left);
	for(i=0;i<u32Row;i++)
	{
		if (HI_STD_I2C_NUM > g_stTunerOps[u32TunerPort].enI2cChannel)
	    {
	        s32Ret = HI_DRV_I2C_Write(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, &(g_u8MCUSoftWare[i*16]), 16);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }
	    else
	    {
	        s32Ret = HI_DRV_GPIOI2C_WriteExt(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, &(g_u8MCUSoftWare[i*16]), 16);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }
	}
	
	if(u32Left)
	{
		if (HI_STD_I2C_NUM > g_stTunerOps[u32TunerPort].enI2cChannel)
	    {
	        s32Ret = HI_DRV_I2C_Write(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, &(g_u8MCUSoftWare[u32Row*16]), u32Left);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }
	    else
	    {
	        s32Ret = HI_DRV_GPIOI2C_WriteExt(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, &(g_u8MCUSoftWare[u32Row*16]), u32Left);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }
	}

	//verify firmware download success or fail
	qam_write_byte(u32TunerPort,0x8c,0);
	qam_write_byte(u32TunerPort,0x8b,0);
	for(i=0;i<u32Row;i++)
	{
		if (HI_STD_I2C_NUM > g_stTunerOps[u32TunerPort].enI2cChannel)
		{
			s32Ret = HI_DRV_I2C_Read(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, u8DataBuf, 16);
			if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
			
		}
		else
	    {
	        s32Ret = HI_DRV_GPIOI2C_ReadExt(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, u8DataBuf, 16);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }

		if(0 != memcmp(u8DataBuf,&(g_u8MCUSoftWare[i*16]),16))
		{
			return HI_FAILURE;
		}
	}

	if(u32Left)
	{
		if (HI_STD_I2C_NUM > g_stTunerOps[u32TunerPort].enI2cChannel)
		{
			s32Ret = HI_DRV_I2C_Read(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, u8DataBuf, u32Left);
			if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
			
		}
		else
	    {
	        s32Ret = HI_DRV_GPIOI2C_ReadExt(g_stTunerOps[u32TunerPort].enI2cChannel, g_stTunerOps[u32TunerPort].u32DemodAddress, 0x8a, 1, u8DataBuf, u32Left);
	        if(HI_SUCCESS != s32Ret)
	        {
	            return s32Ret;
	        }
	    }

		if(0 != memcmp(u8DataBuf,&(g_u8MCUSoftWare[u32Row*16]),u32Left))
		{
			return HI_FAILURE;
		}
	}
	
	do_gettimeofday(&stEndTime);
	u32Total = (stEndTime.tv_sec-stStartTime.tv_sec)*1000+(stEndTime.tv_usec-stStartTime.tv_usec)/1000;
	HI_INFO_TUNER("<<<=================download bin use %d ms.\n",u32Total);
	return HI_SUCCESS;
}

/*1.增加IIC 透传的函数  2.IQ  翻转函数 3.TS  disable   enable函数    4.增加ANT_POWER供电控制函数5.增加时钟缓通输出控制函数  6.*/
//对bit1先写0再写1，完成热复位
static HI_S32 hi3137_hot_reset(HI_U32 u32TunerPort)
{
    HI_U32  u32Dly = 8;
    HI_U8   u8InitData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x01;
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);
    tuner_mdelay(u32Dly);
    u8InitData = (u8InitData & 0xFC) | 0x03;
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    return HI_SUCCESS;
}

HI_VOID hi3137_sdram_check(HI_U32 u32TunerPort)
{
    HI_U8 u8InitData = 0, u8BistRdy, u8BistOk = 0;
    HI_U8 i;

    qam_read_byte(u32TunerPort, Hi3137_SDR_CTRL, &u8InitData);
    u8InitData = u8InitData & 0xFD;   //winbond sdram
    qam_write_byte(u32TunerPort, Hi3137_SDR_CTRL, u8InitData);

    //set bist mode
    qam_read_byte(u32TunerPort, Hi3137_BIST_MODE_STATUS, &u8InitData);
    u8InitData = (u8InitData & 0xF0) | 0x05;
    qam_write_byte(u32TunerPort, Hi3137_BIST_MODE_STATUS, u8InitData);
    u8InitData = 0x1;
    qam_write_byte(u32TunerPort, Hi3137_SDRAM_BIST, u8InitData);
    //reset the bist module
    qam_read_byte(u32TunerPort, Hi3137_RSTN_CTRL, &u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x1;
    qam_write_byte(u32TunerPort, Hi3137_RSTN_CTRL, u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x3;
    qam_write_byte(u32TunerPort, Hi3137_RSTN_CTRL, u8InitData);

    for(i=0;i<20;i++)
    {
        qam_read_byte(u32TunerPort, Hi3137_BIST_MODE_STATUS, &u8InitData);
        u8BistRdy = (u8InitData >> 5) & 0x01;
        if(u8BistRdy == 1)
        {
            u8BistOk = (u8InitData >> 4) & 0x01;
            break;
        }
        //delay 5ms
        tuner_mdelay(5);
    }

    if( u8BistOk != 1)
    {
        //bist fail,change sdram
        qam_read_byte(u32TunerPort, Hi3137_SDR_CTRL, &u8InitData);
        u8InitData = u8InitData | 0x02;  //etron sdram
        qam_write_byte(u32TunerPort, Hi3137_SDR_CTRL, u8InitData);
    }
    //disable the bist
    qam_read_byte(u32TunerPort, Hi3137_BIST_MODE_STATUS, &u8InitData);
    u8InitData = (u8InitData & 0xF0);
    qam_write_byte(u32TunerPort, Hi3137_BIST_MODE_STATUS, u8InitData);
}
#if 0
static HI_S32 hi3137_get_chip_id(HI_U32 u32TunerPort,HI_U16 u16ChipID)
{
    HI_U8 u8ChipID[2] = {0};
    HI_U32 u32Return = HI_SUCCESS;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, Hi3137_CHIP_ID_H, &u8ChipID[0]);
    qam_read_byte(u32TunerPort, Hi3137_CHIP_ID_L, &u8ChipID[1]);
    u16ChipID = 0;

    /*** if get chip id success,chip id = 0x3137 ***/
    u16ChipID = (u8ChipID[0]<<8) + u8ChipID[1];
    if(u16ChipID != 0x3137)
    {
        return HI_FAILURE;
    }

    return u32Return;
}
#endif
HI_S32 hi3137_get_lock_flag(HI_U32 u32TunerPort, HI3137_LOCK_FLAG_E enLockFlag)
{
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8LockFlag = 0,u8Temp;
    HI_U8 u8Mask;
    HI_U8 u8FftSize = 0;
    HI_U8 u8PlpMode = 0;
    static HI_U8 u8T2SigOk = 0;
    
    HI_U8 u8temp = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    
    qam_read_byte(u32TunerPort, Hi3137_LOCK_FLAG, &u8LockFlag);
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    
    if(HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        if((u8LockFlag & 0x08) &&  (u8T2SigOk == 0)) //t2
        {
                //read 0x55, check the fft size
                qam_read_byte(u32TunerPort, HI3138_T_CHN_FFT_GI, &u8FftSize);
                
                if(((u8FftSize>>3)&0x7) == 3) // fft 8K
                {
                    u8temp = 0x54;
                    qam_write_byte(u32TunerPort, HI3138_T_MIN_THRES, u8temp);
                    u8temp = 0x10;
                    qam_write_byte(u32TunerPort, HI3138_T_FD_GRP, u8temp);
                }
                else
                {
                    u8temp = 0x20;
                    qam_write_byte(u32TunerPort, HI3138_T_MIN_THRES, u8temp);
                    u8temp = 0x1b;
                    qam_write_byte(u32TunerPort, HI3138_T_FD_GRP, u8temp);
                }
                //read 0xC9, check the plp mode
                qam_read_byte(u32TunerPort, HI3138_T_PLP_PARAM, &u8PlpMode);
                
                //if qpsk
                if(((u8PlpMode >> 2)&0x3) == 0)
                {
                    u8temp = 0x10;
                    qam_write_byte(u32TunerPort, HI3138_T_ECHO_THRES, u8temp);
                }
                //16qam,64qam,256qam
                else
                {
                    u8temp = 0x20;
                    qam_write_byte(u32TunerPort, HI3138_T_ECHO_THRES, u8temp);
                }
        }
        u8T2SigOk = u8LockFlag & 0x08;
    }
    
    switch(enLockFlag)
    {
        //case AGC_LOCK_FLAG:             u8Mask = 0x01;      break;
        case SYNC_LOCK_FLAG:            u8Mask = 0x02;      break;
        //case SYNC_PRE_LOCK_FLAG:        u8Mask = 0x04;      break;
        case SIG_T2_LOCK_FLAG:          u8Mask = 0x08;      break;
        case TPS_T_LOCK_FLAG:           u8Mask = 0x10;      break;
        case FEC_LOCK_FLAG:             u8Mask = 0x20;      break;
        default:                        u8Mask = 0x0;       break;
    }

    /*在信号模式为1+12（HierA+HierB）的分层模式，信号质量
    能够保证HierA的正常接收（一般为QPSK），HierB此时误码很
    高。由于HierB的数据流远大于HierA，导致TS输出中大部分包
    是错包。Hi3138 ISDBT模式中FEC_OK是和TS_err直接挂钩。因此此
    时FEC_OK多数时刻为0，SDK检测锁定失败概率较高。将FEC_Ok
    的检测方法由读取0x2c改成读取0x2d的[4:1],若返回值为7，则
    认定FEC_OK，返回锁定值。*/
    if ((HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
        &&(HI_UNF_TUNER_SIG_TYPE_ISDB_T == g_stTunerOps[u32TunerPort].enSigType)
        &&(FEC_LOCK_FLAG == enLockFlag))
    {
        qam_read_byte(u32TunerPort, Hi3137_TUNER_SEL, &u8temp);
        HI_INFO_TUNER("use TUNER_SEL(0x%x) instead of FEC.\n",u8temp);
        u8temp &= (0xf<<1);
        if ( (0x7<<1) == u8temp)
        {
            return HI_SUCCESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    
    if(u8LockFlag & u8Mask)
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;
    }
	
	qam_read_byte(u32TunerPort, Hi3137_TUNER_SEL, &u8Temp);
	u8Temp = (u8Temp >> 1);
	if((u8Temp & 0xf) == 0x7)
	{
		s32Ret = HI_SUCCESS;
	}
	
	return s32Ret;
}

HI_S32 hi3137_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType )
{
    HI_U8 u8InitData = 0;

    g_stI2cChannel = enI2cChannel;
    g_stTunerType = enTunerDevType;

    //cold reset
    qam_read_byte(u32TunerPort, Hi3137_RSTN_CTRL, &u8InitData);
    u8InitData = (u8InitData & 0xFC);
    qam_write_byte(u32TunerPort, Hi3137_RSTN_CTRL, u8InitData);
    tuner_mdelay(10);
    u8InitData = (u8InitData & 0xFC) | 0x03;
    qam_write_byte(u32TunerPort, Hi3137_RSTN_CTRL, u8InitData);

    return HI_SUCCESS;
}

HI_S32 hi3137_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
    HI_U32 u32ClkDemoKhz,u32ClkFecKhz,u32ClkIfKhz,u32ClkAdcKhz; /* 单位KHz */
    HI_U32 u32DemoDiv,u32FecDiv,i;
	HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8DemoDivInt,u8DemoDivFh,u8DemoDivFl,u8FecDivInt,u8FecDivFh,u8FecDivFl;
    HI_U8 u8ClkDemH,u8ClkDemM,u8ClkDemL,u8ClkFecH,u8ClkFecM,u8ClkFecL,u8ClkAdcM,u8ClkAdcL,u8ClkIfH,u8ClkIfL;
    HI_U8 u8InitData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pstTerTunerAttr)
    {
        return HI_FAILURE;
    }

    g_stXtalClk = pstTerTunerAttr->u32DemodClk;
    u32ClkDemoKhz = 60000;
    u32ClkFecKhz = 150000;
    u32ClkAdcKhz = 30000;

	switch(g_stTunerOps[u32TunerPort].enTunerDevType)
	{
		case HI_UNF_TUNER_DEV_TYPE_TDA182I5A:
		case HI_UNF_TUNER_DEV_TYPE_SI2144:
			u32ClkIfKhz = 5000;
			break;
		default:
			u32ClkIfKhz = 5380;
			break;
	}

    /***
    **** symtem clock freq config ******
                                    ***/
    //dem clk freq 芯片缺省值是64MHz
    u32DemoDiv = u32ClkDemoKhz * 12 * 4096 / g_stXtalClk;
    u8DemoDivInt = u32DemoDiv / 4096;
    u8DemoDivFh = (u32DemoDiv / 256) % 16;
    u8DemoDivFl = u32DemoDiv % 256;
    u32FecDiv = u32ClkFecKhz * 4 *4096 / g_stXtalClk;
    u8FecDivInt = u32FecDiv / 4096;
    u8FecDivFh = (u32FecDiv / 256) % 16;
    u8FecDivFl = u32FecDiv % 256;

    //switch i2c to the xo clk
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData & 0xFE;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);
    tuner_mdelay(5);
    //power down PLL0
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = u8InitData | 0x10;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);

    //power down PLL1
    qam_read_byte(u32TunerPort,Hi3137_PLL1_PD,&u8InitData);
    u8InitData = u8InitData | 0x10;
    qam_write_byte(u32TunerPort,Hi3137_PLL1_PD,u8InitData);

    //set pll0
    qam_write_byte(u32TunerPort,Hi3137_PLL0_FBDIV,u8DemoDivInt);
    qam_write_byte(u32TunerPort,Hi3137_PLL0_FRAC_L,u8DemoDivFl);
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = (u8InitData & 0xF0) | u8DemoDivFh;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);

    //set pll1
    qam_write_byte(u32TunerPort,Hi3137_PLL1_FBDIV,u8FecDivInt);
    qam_write_byte(u32TunerPort,Hi3137_PLL1_FRAC_L,u8FecDivFl);
    qam_read_byte(u32TunerPort,Hi3137_PLL1_PD,&u8InitData);
    u8InitData = (u8InitData & 0xF0) | u8FecDivFh;
    qam_write_byte(u32TunerPort,Hi3137_PLL1_PD,u8InitData);

    //power up the pll
    qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8InitData);
    u8InitData = u8InitData & 0xEF;
    qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8InitData);
    qam_read_byte(u32TunerPort,Hi3137_PLL1_PD,&u8InitData);
    u8InitData = u8InitData & 0xEF;
    qam_write_byte(u32TunerPort,Hi3137_PLL1_PD,u8InitData);

    //delay 5ms,waiting PLL lock
    tuner_mdelay(5);

    //switch i2c to the clk demo
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData | 0x01;//u8InitData & 0xEF;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);

    //adc clk freq 芯片缺省值是32MHz
    switch(u32ClkDemoKhz / u32ClkAdcKhz)
    {
      /*  case 1:   //codecc, it is a fixed value: 60000/30000. can not reach. 
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL0, &u8InitData);
            u8InitData = (u8InitData & 0xC0) | 0x03;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL0, u8InitData);
            break;*/
        case 2:
        default:
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL0, &u8InitData);
            u8InitData = (u8InitData & 0xC0) | 0x01;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL0, u8InitData);
            break;
    }

    u8ClkAdcL = u32ClkAdcKhz % 256;
    u8ClkAdcM = u32ClkAdcKhz / 256;
    qam_write_byte(u32TunerPort,Hi3137_CLK_ADC_L,u8ClkAdcL);
    qam_write_byte(u32TunerPort,Hi3137_CLK_ADC_M,u8ClkAdcM);

    //reset adc ip
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8InitData);
    u8InitData = u8InitData & 0xBF;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);
    u8InitData = u8InitData | 0x40;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8InitData);

    //power up the adc,set into the work mode
    qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL3,&u8InitData);
    u8InitData = u8InitData | 0x30;
    qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL3,u8InitData);
    tuner_mdelay(5);

    //demo clk
    u8ClkDemL = u32ClkDemoKhz % 256;
    u8ClkDemM = (u32ClkDemoKhz / 256) % 256;
    u8ClkDemH = (u32ClkDemoKhz / 65536) & 0x03;

    qam_write_byte(u32TunerPort,Hi3137_CLK_DEMO_L,u8ClkDemL);
    qam_write_byte(u32TunerPort,Hi3137_CLK_DEMO_M,u8ClkDemM);
    qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0xCF) | (u8ClkDemH << 4);
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);

    //fec clk
    u8ClkFecL = u32ClkFecKhz % 256;
    u8ClkFecM = (u32ClkFecKhz / 256) % 256;
    u8ClkFecH = (u32ClkFecKhz / 65536) & 0x03;

    qam_write_byte(u32TunerPort,Hi3137_CLK_FEC_L,u8ClkFecL);
    qam_write_byte(u32TunerPort,Hi3137_CLK_FEC_M,u8ClkFecM);
    qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0x3F) | (u8ClkFecH << 6);
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);

    //IF freq
    u8ClkIfH = u32ClkIfKhz / 256;
    u8ClkIfL = u32ClkIfKhz % 256;
    qam_write_byte(u32TunerPort,Hi3137_IF_FREQ_L,u8ClkIfL);
    qam_write_byte(u32TunerPort,Hi3137_IF_FREQ_H,u8ClkIfH);

    //-------------demo初始化配置----------------
	 //initial
	//u8InitData = 0x8b;
	//qam_write_byte(u32TunerPort,0x61,u8InitData); 

	//cir threshold
	u8InitData = 0x00;
	qam_write_byte(u32TunerPort,Hi3137_ECHO_THRES,u8InitData); 

    //agc goal
    u8InitData = 0x1C;
    qam_write_byte(u32TunerPort,Hi3137_AGC_GOAL,u8InitData);
    //p1_th
    qam_read_byte(u32TunerPort,Hi3137_P1_THRES,&u8InitData);
    u8InitData = (u8InitData & 0xC0) | 0x1D;
    qam_write_byte(u32TunerPort,Hi3137_P1_THRES,u8InitData);
    //fef_th
    u8InitData = 0x02;
    qam_write_byte(u32TunerPort,Hi3137_FEF_TH,u8InitData);
    //cp_add_ena - disable cp add 
    qam_read_byte(u32TunerPort,Hi3137_TS_0_SEL,&u8InitData);
    u8InitData = u8InitData  & 0x1F;
    qam_write_byte(u32TunerPort,Hi3137_TS_0_SEL,u8InitData);
    //tto
    qam_read_byte(u32TunerPort,Hi3137_OUTP_ISSY,&u8InitData);
    u8InitData = (u8InitData  &  0xC0) | 0x14;
    qam_write_byte(u32TunerPort,Hi3137_OUTP_ISSY,u8InitData);
    //dagc ref
    u8InitData = 0x5a;
    qam_write_byte(u32TunerPort,Hi3137_DAGC_REF,u8InitData);
	//high error reset
	u8InitData = 0xBF;
	qam_write_byte(u32TunerPort,Hi3137_BER_CTRL,u8InitData);

    //L1 post iter num
    u8InitData = 0x20;
    qam_write_byte(u32TunerPort,Hi3137_ITER_NUM_POST,u8InitData);
    //L1 post cfg
    u8InitData = 0x40;
    qam_write_byte(u32TunerPort,Hi3137_SWITCH_DLY,u8InitData);
    u8InitData = 0x08;
    qam_write_byte(u32TunerPort,Hi3137_AUTO_DLY,u8InitData);
    //gate off
    u8InitData = 0x98;
    qam_write_byte(u32TunerPort,Hi3137_ITER_NUM,u8InitData);
    //c_thr_t2
    qam_read_byte(u32TunerPort,Hi3137_T2_CHK_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x03;
    qam_write_byte(u32TunerPort,Hi3137_T2_CHK_CTRL,u8InitData);
    //c_pos_mod
    u8InitData = 0x78;
    qam_write_byte(u32TunerPort,Hi3137_P2_POS_MOD,u8InitData); 
	//cir threshold
	u8InitData = 0x10;
	qam_write_byte(u32TunerPort,Hi3137_ECHO_THRES,u8InitData); 

    //iscr limit
    qam_read_byte(u32TunerPort,Hi3137_OUTP_LIMIT_EN,&u8InitData);
    u8InitData = u8InitData & 0xDF;
    qam_write_byte(u32TunerPort,Hi3137_OUTP_LIMIT_EN,u8InitData);
    //ts out rand
    qam_read_byte(u32TunerPort,Hi3137_OUTP_RAND,&u8InitData);
    u8InitData = u8InitData | 0x1;
    qam_write_byte(u32TunerPort,Hi3137_OUTP_RAND,u8InitData);

    qam_read_byte(u32TunerPort,0x66,&u8InitData);
    u8InitData = (u8InitData & 0xF3) | (0x1<<2);
    qam_write_byte(u32TunerPort,0x66,u8InitData);

	u8InitData = 0;
	qam_write_byte(u32TunerPort,Hi3137_IO_CTRL2,u8InitData);
	
	/*在Multi PLP模式下，数据突发比较严重，
	输出模块上电后的默认配置存在数据溢出的风险*/
	/*在Multi PLP模式下，数据突发比较严重，
	输出模块上电后的默认配置存在数据溢出的风险*/
	qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL0, 0x5a);
	qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL1, 0x71);
	qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL2, 0x20);
	
	qam_read_byte(u32TunerPort, Hi3137_OUTP_PLL4, &u8InitData);
	u8InitData &=0x7;
	qam_write_byte(u32TunerPort, Hi3137_OUTP_PLL4, u8InitData);
	
	qam_read_byte(u32TunerPort, Hi3137_OUTP_MODE_SET, &u8InitData);
	u8InitData &=0xC1;
	u8InitData |= 0xb<<1;
	qam_write_byte(u32TunerPort, Hi3137_OUTP_MODE_SET, u8InitData);

    qam_read_byte(u32TunerPort, Hi3137_NP_IIR_SFT, &u8InitData);
    u8InitData &= 0xE3;
    u8InitData |= 0x4<<2;
    qam_write_byte(u32TunerPort, Hi3137_NP_IIR_SFT, u8InitData);

    hi3137_sdram_check(u32TunerPort);
	for(i = 0;i < 3;i++)
	{
		s32Ret = demod_download_firmware(u32TunerPort);
		if(HI_SUCCESS == s32Ret)
		{
			break;
		}
		else
		{
			HI_ERR_TUNER("demod_download_firmware fail.\n");
		}
	}

#if 0
    //ts init
    qam_read_byte(u32TunerPort,Hi3137_OUTP_TS_MODE,&u8InitData);
    if(pstTerAttr->enTSSerialPIN)
    {
        u8InitData = u8InitData | 0x04;
    }
    else
    {
        u8InitData = u8InitData & 0xFB;
    }
    //时钟边沿选择
    if(pstTerAttr->enTSClkPolar)
    {
        u8InitData = u8InitData & 0xF7;
    }
    else
    {
        u8InitData = u8InitData | 0x08;
    }
    qam_write_byte(u32TunerPort,Hi3137_OUTP_TS_MODE,u8InitData);
    //串行输出模式下同步字宽度选择
    qam_read_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,&u8InitData);
    if(pstTerAttr->enTSSyncHead)
    {
        u8InitData = u8InitData | 0x01;
    }
    else
    {
        u8InitData = u8InitData & 0xFE;
    }
    qam_write_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,u8InitData);
#endif

    demod_clock_out_reset(u32TunerPort);
    tuner_mdelay(10);
    tuner_init(u32TunerPort);

    //------------热复位，hot_rstn-------------------
    qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x01;
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);
    u8InitData = (u8InitData & 0xFC) | 0x03;
    qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8InitData);

    return HI_SUCCESS;
}

static HI_VOID hi3137_connect_3137config(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel, 
                        HI_U32 *pu32FreqOffsetKhz, HI_S32 *ps32Fs, HI_U32 *pu32ChkTim)
{
    static HI_U32 s_u32DemodClk = 60000;
    HI_U32 u32DemodClk = 0;
    HI_U8 u8BW = 4;
    HI_U8 u8Thr = 0;
    HI_U8 u8PosMod = 0;
    HI_U8 u8InitData = 0;
    HI_U8 u8RegData = 0;
    HI_U8 u8RegData2 = 0;


    switch(pstChannel->u32Frequency)
    {
        case 842000:
        case 722000:
        case 778000:
        case 810000:
        case 658000:
            u32DemodClk = 61000;
            *pu32FreqOffsetKhz = 0;
			u8InitData = 0x5A;
            break;
        case 514000:
            u32DemodClk = 60000;
            *pu32FreqOffsetKhz = 350; /*151103*/
			u8InitData = 0x5A;
            break;
		case 546000:
			u32DemodClk = 60000;
            *pu32FreqOffsetKhz = 0;
			u8InitData = 0x5B;
			break;
        default:
            u32DemodClk = 60000;
            *pu32FreqOffsetKhz = 0;
			u8InitData = 0x5A;
            break;
    }
	qam_write_byte(u32TunerPort, Hi3137_DAGC_REF, u8InitData);

    //add by wangjq 20150413,reset mcu
    qam_write_byte(u32TunerPort, 0x8e, 0x44);
    
    if(s_u32DemodClk != u32DemodClk)
    {
        s_u32DemodClk = u32DemodClk;
        demod_change_demod_clk(u32TunerPort,s_u32DemodClk, s_u32DemodClk/2);
    }

    //receive bandwidth config
    switch(pstChannel->unSRBW.u32BandWidth)
    {
        case 8000:      u8BW = 0x4;     u8Thr = 3;      u8PosMod = 0x78;    *ps32Fs = 9143;   break;
        case 7000:      u8BW = 0x3;     u8Thr = 2;      u8PosMod = 0x78;    *ps32Fs = 8000;   break;
        case 6000:      u8BW = 0x2;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 6857;   break;
        case 5000:      u8BW = 0x1;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 5714;   break;
        case 1700:      u8BW = 0x0;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 1845;   break;
        default:        u8BW = 0x4;     u8Thr = 2;      u8PosMod = 0x78;    *ps32Fs = 9143;   break;
    }
    qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, (u8BW<<4));

    //c_thr_t2
    qam_read_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, &u8RegData);
    u8RegData = (u8RegData & 0xFC) | u8Thr;      
    qam_write_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, u8RegData);

    //c_pos_mod
    qam_write_byte(u32TunerPort, Hi3137_P2_POS_MOD, u8PosMod);

    qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8RegData);
    u8RegData = u8RegData & 0x9F;
    if(pstChannel->u8DVBTMode)
    {
        u8RegData = u8RegData | (1 << 5);
    }
    qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xCF;
    qam_read_byte(u32TunerPort, Hi3137_OUTP_ISSY, &u8RegData2);
    if(pstChannel->unTer.enDVBT2.enChannelAttr == HI_UNF_TUNER_TER_MODE_LITE)
    {
        u8RegData = u8RegData | (1 << 4);
        //u8InitData = 0x0;
        //*pu32ChkTim = 80;
        u8RegData2 = 0x20;
    }
    else
    {
        //u8InitData = 0x10;
    }
    qam_write_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, u8RegData);
    //qam_write_byte(u32TunerPort, Hi3137_ECHO_THRES, u8InitData);
    qam_write_byte(u32TunerPort,Hi3137_OUTP_ISSY,u8RegData2);

    if(!(pstChannel->u8DVBTMode))   //锁频时，初始化plp至初始状态，plp id切换放到setplpid函数中操作
    {
        qam_read_byte(u32TunerPort,Hi3137_LOOP_BW,&u8InitData);
        u8InitData = u8InitData | 0x80;
        qam_write_byte(u32TunerPort,Hi3137_LOOP_BW,u8InitData);
        qam_read_byte(u32TunerPort,0x62,&u8InitData);
        u8InitData = u8InitData & 0xe7;
        qam_write_byte(u32TunerPort,0x62,u8InitData);

        qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
        u8RegData = u8RegData & 0xFD;
        qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);
        u8RegData = 0;
        qam_write_byte(u32TunerPort, Hi3137_PLP_ID0, u8RegData);
        u8RegData = 0x20;
        qam_write_byte(u32TunerPort, Hi3137_STATE_WAITS, u8RegData);
    }
    else
    {
        qam_read_byte(u32TunerPort,Hi3137_LOOP_BW,&u8InitData);
        u8InitData = u8InitData & 0x7f;
        qam_write_byte(u32TunerPort,Hi3137_LOOP_BW,u8InitData);
        qam_read_byte(u32TunerPort,0x62,&u8InitData);
        u8InitData = (u8InitData & 0xe7) | (0x2 << 3);
        qam_write_byte(u32TunerPort,0x62,u8InitData);

        if(pstChannel->unTer.enDVBT == HI_UNF_TUNER_TS_PRIORITY_LP)
        {
            qam_read_byte(u32TunerPort, Hi3137_AUTO_DLY, &u8RegData);
            u8RegData = u8RegData | 0x02;
            qam_write_byte(u32TunerPort, Hi3137_AUTO_DLY, u8RegData);
        }
        else
        {
            qam_read_byte(u32TunerPort, Hi3137_AUTO_DLY, &u8RegData);
            u8RegData = u8RegData & 0xFD;
            qam_write_byte(u32TunerPort, Hi3137_AUTO_DLY, u8RegData);
        }

        u8RegData = 0x20;
        qam_write_byte(u32TunerPort, Hi3137_STATE_WAITS, u8RegData);
        *pu32ChkTim = 25;
    }

    return;
}

static HI_VOID hi3137_connect_3138config(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel, HI_S32 *ps32Fs, HI_U32 *pu32ChkTim)
{
    HI_U8 u8BW = 4;
    HI_U8 u8Thr = 0;
    HI_U8 u8PosMod = 0;
    HI_U8 u8InitData = 0;
    HI_U8 u8RegData = 0;
    HI_U8 u8RegData2 = 0;


    //receive bandwidth config
    switch(pstChannel->unSRBW.u32BandWidth)
    {
        case 8000:      u8BW = 0x4;     u8Thr = 2;      u8PosMod = 0x78;    *ps32Fs = 9143;   break;
        case 7000:      u8BW = 0x3;     u8Thr = 2;      u8PosMod = 0x78;    *ps32Fs = 8000;   break;
        case 6000:      u8BW = 0x2;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 6857;   break;
        case 5000:      u8BW = 0x1;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 5714;   break;
        case 1700:      u8BW = 0x0;     u8Thr = 3;      u8PosMod = 0x90;    *ps32Fs = 1845;   break;
        default:         u8BW = 0x4;     u8Thr = 3;      u8PosMod = 0x78;   *ps32Fs = 9143;   break;
    }
    qam_write_byte(u32TunerPort, HI3138_T_BAND_WIDTH, (u8BW<<4));

    //c_thr_t2
    qam_read_byte(u32TunerPort, HI3138_T_T2_CHK_CTRL, &u8RegData);
    u8RegData = (u8RegData & 0xFC) | u8Thr;       
    qam_write_byte(u32TunerPort, HI3138_T_T2_CHK_CTRL, u8RegData);
    //c_pos_mod
    qam_write_byte(u32TunerPort, HI3138_T_P2_POS_MOD, u8PosMod);
        
    if(!(pstChannel->u8DVBTMode))   //dvbt2
    {
        //only search t2
        qam_read_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL1, &u8RegData);
        u8RegData = u8RegData & 0x9F;
        qam_write_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL1, u8RegData);

        //t2_base or lite
        qam_read_byte(u32TunerPort, HI3138_T_T2_CHK_CTRL, &u8RegData);
       
        //issy tto 
        qam_read_byte(u32TunerPort, HI3138_T_OUTP_ISSY, &u8RegData2);
        
        if(pstChannel->unTer.enDVBT2.enChannelAttr == HI_UNF_TUNER_TER_MODE_LITE)
        { //lite
            u8RegData2 = 0x20;
            u8RegData = (u8RegData & 0xCF) | (1<<4);
            u8InitData = 0x10;                      
        }
        else
        { //base
            u8RegData2 = 0x10;
            u8RegData = u8RegData & 0xCF;
            u8InitData = 0x20;
        } 
        qam_write_byte(u32TunerPort,HI3138_T_OUTP_ISSY,u8RegData2);
        qam_write_byte(u32TunerPort,HI3138_T_T2_CHK_CTRL,u8RegData);
        qam_write_byte(u32TunerPort,HI3138_T_ECHO_THRES, u8InitData);
        
        //t2 special
        qam_read_byte(u32TunerPort, HI3138_T_TS_0_SEL, &u8InitData);
        u8InitData = ((u8InitData & 0x0F) | 0x10);
        qam_write_byte(u32TunerPort,HI3138_T_TS_0_SEL, u8InitData);
        
        //PLP 
        qam_read_byte(u32TunerPort, HI3138_T_PLP_CTRL, &u8RegData);
        u8RegData = u8RegData & 0xFD;
        qam_write_byte(u32TunerPort, HI3138_T_PLP_CTRL, u8RegData);
        u8RegData = 0;
        qam_write_byte(u32TunerPort, HI3138_T_PLP_ID0, u8RegData);
        u8RegData = 0x14;
        qam_write_byte(u32TunerPort, HI3138_T_STATE_WAITS, u8RegData);
        HI_INFO_TUNER("<<<---DVB-T2.\n");
    }
    else        //dvbt or isdbt
    {        
        //only search t
        qam_read_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL1, &u8RegData);
        u8RegData = u8RegData & 0x9F;
        u8RegData = u8RegData | (1 << 5);
        qam_write_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL1, u8RegData);

        
        u8RegData = 0x20;
        qam_write_byte(u32TunerPort,HI3138_T_ECHO_THRES, u8RegData);
        u8RegData = 0x20;
        qam_write_byte(u32TunerPort,HI3138_T_MIN_THRES , u8RegData);
        u8RegData = 0x1A;
        qam_write_byte(u32TunerPort,HI3138_T_FD_GRP , u8RegData); //24Hz
        
        
        qam_read_byte(u32TunerPort, HI3138_T_TS_0_SEL, &u8InitData);
        u8InitData = u8InitData & 0x0F;
        qam_write_byte(u32TunerPort,HI3138_T_TS_0_SEL, u8InitData);
        // low or high priority stream
        if(pstChannel->unTer.enDVBT == HI_UNF_TUNER_TS_PRIORITY_LP)
        {
            qam_read_byte(u32TunerPort, HI3138_T_AUTO_DLY, &u8RegData);
            u8RegData = u8RegData | 0x02;
            qam_write_byte(u32TunerPort, HI3138_T_AUTO_DLY, u8RegData);
        }
        else
        {
            qam_read_byte(u32TunerPort, HI3138_T_AUTO_DLY, &u8RegData);
            u8RegData = u8RegData & 0xFD;
            qam_write_byte(u32TunerPort, HI3138_T_AUTO_DLY, u8RegData);
        }
        u8RegData = 0x16;
        qam_write_byte(u32TunerPort, HI3138_T_STATE_WAITS, u8RegData);

        u8RegData = 0x10;
        qam_write_byte(u32TunerPort, HI3138_T_FD_GRP, u8RegData);
        
        *pu32ChkTim = 25;
        HI_INFO_TUNER("<<<---DVB-T.\n");
    }

    return;
}


HI_S32 hi3137_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_U32 i = 0;
    HI_U32 u32ChkTim = 100;
    HI_U32 u32SyncChk = 50;
    HI_U32 u32Return = HI_SUCCESS;
    HI_U32 u32FreqOffsetKhz = 0;
    HI_S32 s32CarrOffset = 0;
    HI_S32 s32Fs = 0;
    HI_U8 u8CarrVal[2] = {0};
    HI_U8 u8RegData   = 0;
    HI_U8 u8RegData2  = 0;
	HI_U8 u8pp = 0;
    HI_U8 u8plp_mod = 0;
    HI_U8 u8plp_cod = 0;
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif


    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == pstChannel)
    {
        return HI_FAILURE;
    }
    
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        hi3137_connect_3137config(u32TunerPort, pstChannel, &u32FreqOffsetKhz, &s32Fs, &u32ChkTim);
    }
    else if(HI_UNF_DEMOD_DEV_TYPE_3138 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        hi3137_connect_3138config(u32TunerPort, pstChannel, &s32Fs, &u32ChkTim);
    }

    HI_INFO_TUNER("--1--TUNER Freq %d KHz\n",pstChannel->u32Frequency + u32FreqOffsetKhz);
    tuner_set_freq(u32TunerPort, (pstChannel->u32Frequency + u32FreqOffsetKhz), pstChannel->unSRBW.u32BandWidth, pstChannel->u8DVBTMode);
    tuner_mdelay(20);
    hi3137_hot_reset(u32TunerPort);
    
#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
    	//start mcu
    	qam_write_byte(u32TunerPort, 0x8e, 0x33);
    	tuner_mdelay(20);
    }

    //查询sync锁定
	for(i = 0; i < u32SyncChk; i++)
    {
        u32Return = hi3137_get_lock_flag(u32TunerPort, SYNC_LOCK_FLAG);

        if(u32Return == HI_SUCCESS)
        {
            break;
        }
        else if(13 == i) 
        {
            if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
            {
        	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
            }
            
        	qam_read_byte(u32TunerPort, HI3138_T_T2_CHK_CTRL, &u8RegData);
        	u8RegData = (u8RegData & 0xFC) | 0x01;	
        	qam_write_byte(u32TunerPort, HI3138_T_T2_CHK_CTRL, u8RegData);
            
            if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
            {
        	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
            }
        }
        tuner_mdelay(20);   //??
    }

    if(i >= u32SyncChk)
    {
        HI_ERR_TUNER("hi3137 sync fail.\n");
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    
    //补偿tuner载波偏差
    qam_read_byte(u32TunerPort, Hi3137_CAR_OFFSET_L, &u8CarrVal[0]);
    qam_read_byte(u32TunerPort, Hi3137_CAR_OFFSET_H, &u8CarrVal[1]);
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    s32CarrOffset = (u8CarrVal[1] << 8) + u8CarrVal[0];
    if(u8CarrVal[1] & 0x80)
    {
        s32CarrOffset = s32CarrOffset -65536;
    }
    s32CarrOffset = s32CarrOffset * s32Fs /(1<<18) ;   //unit KHz
	HI_INFO_TUNER("s32CarrOffset:%d\n",s32CarrOffset);
	
    //补偿tuner载波偏差
    for(i = 0;i < u32ChkTim; i++)
    {
        u32Return = hi3137_get_lock_flag(u32TunerPort, SIG_T2_LOCK_FLAG); //t2
        u32Return &=  hi3137_get_lock_flag(u32TunerPort, TPS_T_LOCK_FLAG); //t

        if(u32Return == HI_SUCCESS)
        {
            if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
            {
        	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
            }
            qam_read_byte(u32TunerPort,Hi3137_PLP_PARAM,&u8RegData);
            qam_read_byte(u32TunerPort,Hi3137_PP_VERSION,&u8RegData2);

            u8pp = (u8RegData2>>4);
            u8plp_cod = (u8RegData >>5);
            u8plp_mod = (u8RegData >>2) & 0x07;

            if(!((u8pp==3) && (u8plp_cod == 1) && (u8plp_mod == 3))) 
            {
                if(hi3137_abs(s32CarrOffset) > 100)
                {
    	            HI_INFO_TUNER("--2--TUNER Freq %d KHz\n",pstChannel->u32Frequency+s32CarrOffset + u32FreqOffsetKhz);
        			tuner_set_freq(u32TunerPort ,(pstChannel->u32Frequency+s32CarrOffset + u32FreqOffsetKhz),pstChannel->unSRBW.u32BandWidth,pstChannel->u8DVBTMode);
                    hi3137_hot_reset(u32TunerPort);
                }
            }
            
            if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
            {
        	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
            }
            break;
        }

        tuner_mdelay(20);
    }    
	
    //查询fec锁定
    for(i = 0;i < u32ChkTim;i++)
    {
        u32Return = hi3137_get_lock_flag(u32TunerPort, FEC_LOCK_FLAG);

        if(u32Return == HI_SUCCESS)
        {
            break;
        }
        tuner_mdelay(20);
    }

    #ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3137_connect time used(signal:%d):[%d(mSec)]\n", g_stTunerOps[u32TunerPort].enSigType,  (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
    #endif

    if (i >= u32ChkTim)
    {
        HI_ERR_TUNER("hi3137 fec fail.u32ChkTim:%d,i:%d\n",u32ChkTim,i);
        return HI_FAILURE;
    }
    
    
    /**config T/T2 auto searching, in case signal type changed from T to T2,or signal type changede from T2 to T**/
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8RegData);
    u8RegData = (u8RegData & 0x9F) | (2 << 5);
    qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8RegData);
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3137_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U32 u32Return;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == penTunerStatus)
    {
        return HI_FAILURE;
    }

    u32Return = hi3137_get_lock_flag(u32TunerPort,FEC_LOCK_FLAG);
    if(u32Return == HI_SUCCESS)
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    else
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    }

    return HI_SUCCESS;
}

HI_VOID hi3137_read_tuner_registers(HI_U32 u32TunerPort, void * p)
{
    HI_U8 i;
    HI_U8 u8Data = 0;

    PROC_PRINT(p,"  addr ");
    for(i = 0;i <= 0x0F;i++)
    {
        PROC_PRINT(p,"%2x ",i);
    }
    
    for(i = 0;i < 0xFF;i++)
    {
    	qam_read_byte(u32TunerPort, i, &u8Data);
    	if(i % 16 == 0)
    	{
    		PROC_PRINT(p,"\n %04x0:",i/16);
    	}
    	PROC_PRINT(p,"%2x ",u8Data);
    }
    PROC_PRINT(p,"\n");
}

static HI_U32 hi3137_get_ber_t2_m(HI_U32 u32FrameNum, HI_U8 u8FecType)
{
    HI_U32 u32M;

    
    switch(u32FrameNum)
    {
        case 0:     u32M = (u8FecType == 1)?32 : 128;           break;
        case 1:     u32M = (u8FecType == 1)?64 : 256;           break;
        case 2:     u32M = (u8FecType == 1)?128 : 512;          break;
        case 3:     u32M = (u8FecType == 1)?256 : 1024;         break;
        case 4:     u32M = (u8FecType == 1)?512 : 2048;         break;
        case 5:     u32M = (u8FecType == 1)?1024 : 4096;        break;
        case 6:     u32M = (u8FecType == 1)?2048 : 8192;        break;
        default:    u32M = (u8FecType == 1)?4096 : 16384;       break;
    }

    return u32M;
}
static HI_VOID hi3137_get_ber_mn(HI_U32 u32TunerPort, HI_U32 *pu32M, HI_U32 *pu32N)
{
    HI_U8 u8DVBT = 0;
    HI_U32 u32M;
    HI_U32 u32N;
    HI_U8 u8Reg = 0;
    HI_U32 u32FrameNum;
    HI_U8 u8FecType;
    HI_U8 u8CodeRate;
    
    
    //frame num
    qam_read_byte(u32TunerPort, Hi3137_BER_CTRL, &u8Reg);
    u32FrameNum = (u8Reg & 0x70) >> 4;   //default = 3

    qam_read_byte(u32TunerPort, Hi3137_CHN_FFT_GI, &u8DVBT);
    if(u8DVBT & 0x80)
    {
        /*BER = error_cnt/(8*204*frams)*/
        u32M = 204 * 8;
        switch(u32FrameNum)
        {
            case 0:     u32N = 16;          break;
            case 1:     u32N = 64;          break;
            case 2:     u32N = 256;         break;
            case 3:     u32N = 1024;        break;
            case 4:     u32N = 4096;        break;
            case 5:     u32N = 16384;       break;
            case 6:     u32N = 65536;       break;
            case 7:     u32N = 262144;       break;
            default:    u32N = 1024;        break;
        }
    }
    else
    {
        /*BER = error_cnt/(N*frams)*/
        //FEC type and code rate
        qam_read_byte(u32TunerPort, Hi3137_PLP_PARAM, &u8Reg);
        u8FecType = u8Reg & 0x01; /*0：16K LDPC,1：64KLDPC*/
        u8CodeRate = (u8Reg >> 5) & 0x07;
        switch(u8CodeRate)
        {
            case 1:     u32N = (u8FecType == 1)?38880 : 9720;       break;  /*3/5*/
            case 2:     u32N = (u8FecType == 1)?43200 : 10800;      break;  /*2/3*/
            case 3:     u32N = (u8FecType == 1)?48600 : 11800;      break;  /*3/4*/
            case 4:     u32N = (u8FecType == 1)?51840 : 12600;      break;  /*4/5*/
            case 5:     u32N = (u8FecType == 1)?54000 : 13320;      break;  /*5/6*/
            case 6:     u32N = 5400;                                break;  /*1/3*/
            case 7:     u32N = 6480;                                break;  /*2/5*/
            default:
            /*case 0:*/ u32N = (u8FecType == 1)?32400 : 7200;       break;  /*1/2*/
        }

        u32M = hi3137_get_ber_t2_m(u32FrameNum, u8FecType);
    }

    *pu32M = u32M;
    *pu32N = u32N;

    return;
}

HI_S32 hi3137_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    HI_U32  u32N, u32M, u32ErrSum;
    HI_U8 u8BerL = 0;
    HI_U8 u8BerH = 0;


    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == pu32ber)
    {
        return HI_FAILURE;
    }

    //error sum
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    qam_read_byte(u32TunerPort, Hi3137_FEC_BER_L, &u8BerL);
    qam_read_byte(u32TunerPort, Hi3137_FEC_BER_H, &u8BerH);
    
    u32ErrSum = (u8BerH << 8) | (u8BerL << 0);

    //M & N
    hi3137_get_ber_mn(u32TunerPort, &u32M, &u32N);
    
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    pu32ber[0] = (1000000000 / (u32N * u32M)) * u32ErrSum;
	pu32ber[1] = 0;
    pu32ber[2] = 9;

    HI_INFO_TUNER("pu32ber[0]:%d\n", pu32ber[0]);

    return HI_SUCCESS;
}

HI_S32 hi3137_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    HI_U8 u8Snr[2] = {0};
    HI_U32 u32CN;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == pu32SNR)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    qam_read_byte(u32TunerPort, Hi3137_SNR_L, &u8Snr[0]);
    qam_read_byte(u32TunerPort, Hi3137_SNR_H, &u8Snr[1]);
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    u32CN = u8Snr[1] * 256 + u8Snr[0];
    *pu32SNR = u32CN;

    return HI_SUCCESS;
}

static HI_VOID hi3137_tp_scan_set_plp_type(HI_U32 u32TunerPort, HI_U8 u8NumPlp, HI_UNF_TUNER_TER_TPINFO_S *pstScanResult)
{
    HI_U8 j;
    HI_U8 u8PlpId = 0;
    HI_U8 u8PlpGrpId = 0;
    HI_U8 u8PlpType = 0;


    for(j = 0; j < u8NumPlp; j++)
    {
        qam_write_byte(u32TunerPort, Hi3137_PLP_ID0, j);
        qam_read_byte(u32TunerPort, Hi3137_PLP_ID, &u8PlpId);
        qam_read_byte(u32TunerPort, Hi3137_PLP_TYPE, &u8PlpType);
        qam_read_byte(u32TunerPort, Hi3137_PLP_GRP_ID, &u8PlpGrpId);
        pstScanResult->enPlpAttr[j].u8PlpIndex = j;
        pstScanResult->enPlpAttr[j].u8PlpId = u8PlpId;
        pstScanResult->enPlpAttr[j].u8PlpGrpId = u8PlpGrpId;
        u8PlpType = (u8PlpType & 0xe0) >> 5;
        switch(u8PlpType)
        {
            case 0:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_COM;
                break;
            case 1:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT1;
                break;
            case 2:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT2;
                break;
            default:
                HI_ERR_TUNER("plp type unknow.\n");
                break;
        }
    }

    return;
}

static HI_VOID hi3137_tp_scan_set_plp_type_mixed(HI_U32 u32TunerPort, HI_U8 u8NumPlp, HI_UNF_TUNER_TER_TPINFO_S *pstScanResult)
{
    HI_U8 i, j;
    HI_U8 u8PlpId = 0;
    HI_U8 u8PlpGrpId = 0;
    HI_U8 u8PlpType = 0;


    for(i = 0, j = pstScanResult->u8ProgNum; i < u8NumPlp; i++, j++)
    {
        qam_write_byte(u32TunerPort, Hi3137_PLP_ID0, i);
        qam_read_byte(u32TunerPort, Hi3137_PLP_ID, &u8PlpId);
        qam_read_byte(u32TunerPort, Hi3137_PLP_TYPE, &u8PlpType);
        qam_read_byte(u32TunerPort, Hi3137_PLP_GRP_ID, &u8PlpGrpId);
        pstScanResult->enPlpAttr[j].u8PlpId = u8PlpId;
        pstScanResult->enPlpAttr[j].u8PlpGrpId = u8PlpGrpId;
        u8PlpType = (u8PlpType & 0xe0) >> 5;
        switch(u8PlpType)
        {
            case 0:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_COM;
                break;
            case 1:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT1;
                break;
            case 2:
                pstScanResult->enPlpAttr[j].enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT2;
                break;
            default:
                HI_ERR_TUNER("plp type unknow.\n");
                break;
        }
    }

    return;
}

static HI_S32 hi3137_tp_scan_set_result_mixed(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_TPINFO_S *pstScanResult)
{
    HI_U8 u8InitData = 0;
    HI_U8 u8ChkTim = 30;
    HI_U8 i;
    HI_U8 u8NumPlp = 0;

    
    qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8InitData);
    u8InitData = u8InitData & 0x9F;
    qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8InitData);

    qam_read_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, &u8InitData);
    if(pstScanResult->enChannelAttr == HI_UNF_TUNER_TER_MODE_BASE)
    {
        u8InitData = (u8InitData & 0xCF) | (1 << 4);
        u8ChkTim = 25;
    }
    else if(pstScanResult->enChannelAttr == HI_UNF_TUNER_TER_MODE_LITE)
    {
        u8InitData = u8InitData & 0xCF;
        u8ChkTim = 8;
    }
    else
    {
        HI_ERR_TUNER("channel attr unknown.\n");
    }
    qam_write_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, u8InitData);

    hi3137_hot_reset(u32TunerPort);
    for(i = 0; i < u8ChkTim; i++)
    {
        tuner_mdelay(50);
        qam_read_byte(u32TunerPort, Hi3137_LOCK_FLAG, &u8InitData);
        if(u8InitData & 0x08)
        {
            break;
        }
    }

    if(i >= u8ChkTim)
    {
        HI_ERR_TUNER("dvb-t2 not lock!\n");
        return HI_FAILURE;
    }

    for(i = 0;i < u8ChkTim; i++)
    {
        tuner_mdelay(50);
        qam_read_byte(u32TunerPort, Hi3137_LOCK_FLAG, &u8InitData);
        if(u8InitData & 0x20)
        {
            break;
        }
    }

    if(i >= u8ChkTim)
    {
        HI_ERR_TUNER("dvb-t2 fec not lock!\n");
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, Hi3137_NUM_PLP, &u8NumPlp);
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8InitData);
    u8InitData = u8InitData | 0x1;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8InitData);
    hi3137_tp_scan_set_plp_type_mixed(u32TunerPort, u8NumPlp, pstScanResult);
    pstScanResult->u8ProgNum += u8NumPlp;

    return HI_SUCCESS;
}


static HI_S32 hi3137_tp_scan_set_result(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanAttr, HI_UNF_TUNER_TER_TPINFO_S *pstScanResult)
{
    HI_UNF_TUNER_TER_TPINFO_S stScanResult = {0};
    HI_U8 u8InitData = 0;
    HI_U8 u8ChkTim = 30;
    HI_U8 i;
    HI_U8 u8NumPlp = 0;
    HI_U8 u8RegData = 0;
    HI_S32 s32Ret;


    qam_read_byte(u32TunerPort, Hi3137_CHN_FFT_GI, &u8InitData);
    if(u8InitData & 0x80)  //dvb-t
    {
        u8ChkTim = 6;
        stScanResult.u8DVBTMode = 1;
    }
    else   //dvb-t2
    {
        stScanResult.u8DVBTMode = 0;
        qam_read_byte(u32TunerPort, Hi3137_P1_SIGNAL, &u8InitData);
        u8InitData = (u8InitData & 0x70) >> 4;
        switch(u8InitData)
        {
            case 0:
            case 1:   //base mode
                u8ChkTim = 8;
                stScanResult.enChannelAttr = HI_UNF_TUNER_TER_MODE_BASE;
                break;
            case 3:   //lite mode
            case 4:
                u8ChkTim = 25;
                stScanResult.enChannelAttr = HI_UNF_TUNER_TER_MODE_LITE;
                break;
            default:
                HI_ERR_TUNER("dvb-t2 mode fail.\n");
                return HI_FAILURE;
        }
    }

    for(i = 0; i < u8ChkTim; i++)
    {
        tuner_mdelay(50);
        qam_read_byte(u32TunerPort, Hi3137_LOCK_FLAG, &u8InitData);
        if(u8InitData & 0x20)  //fec lock
        {
            if(stScanResult.u8DVBTMode == 1)   //dvb-t
            {
                qam_read_byte(u32TunerPort, Hi3137_TPS_DVBT, &u8InitData);
                if(u8InitData & 0x30)  //分层模式
                {
                    stScanResult.u8DVBTHier = 1;
                    stScanResult.u8ProgNum += 2;
                }
                else   //非分层模式
                {
                    stScanResult.u8DVBTHier = 0;
                    stScanResult.u8ProgNum += 1;
                }
            }
            else   //dvb-t2
            {
                qam_read_byte(u32TunerPort, Hi3137_NUM_PLP, &u8NumPlp);
                qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8InitData);
                u8InitData = u8InitData | 0x1;
                qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8InitData);
                stScanResult.u8ProgNum += u8NumPlp;
                hi3137_tp_scan_set_plp_type(u32TunerPort, u8NumPlp, &stScanResult);

                if(pstScanAttr->bScanLite)
                {
                    qam_read_byte(u32TunerPort, Hi3137_P1_SIGNAL, &u8InitData);
                    qam_read_byte(u32TunerPort, Hi3137_PP_VERSION, &u8RegData);
                    if(((u8InitData & 0x01) == 1) && ((u8RegData & 0x0F) == 2))   //P1_SIGNAL[0]=1 && PP_VERSION[t2_version]=2
                    {
                        stScanResult.enChannelMode = HI_UNF_TUNER_TER_MIXED_CHANNEL;
                    }
                    else
                    {
                        stScanResult.enChannelMode = HI_UNF_TUNER_TER_PURE_CHANNEL;
                    }
                }
                else
                {
                    stScanResult.enChannelMode = HI_UNF_TUNER_TER_PURE_CHANNEL;
                }

            }

            break;
        }
    }

    if(i >= u8ChkTim)
    {
        HI_ERR_TUNER("fec not lock!\n");
        return HI_FAILURE;
    }

    if((stScanResult.u8DVBTMode == 0) && (stScanResult.enChannelMode == HI_UNF_TUNER_TER_MIXED_CHANNEL))
    {
        s32Ret = hi3137_tp_scan_set_result_mixed(u32TunerPort, &stScanResult);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("hi3137_tp_scan_set_result_mixed failed.\n");
            return HI_FAILURE;
        }
    }

    *pstScanResult = stScanResult;
    
    return HI_SUCCESS;
}

HI_S32 hi3137_tp_scan(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanAttr, HI_UNF_TUNER_TER_TPINFO_S *pstTpInfo)
{
    HI_UNF_TUNER_TER_TPINFO_S stScanResult;
    HI_U8 u8InitData = 0;
    HI_U8 u8ChkTim = 30;
    HI_U8 i;
    HI_S32 s32Ret = HI_FAILURE;


    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL== pstScanAttr)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == pstTpInfo)
    {
        return HI_FAILURE;
    }

    if(pstScanAttr->bScanLite)
    {
        //scan base and lite
        qam_read_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, &u8InitData);
        u8InitData = (u8InitData & 0xCF) | (2 << 4);
        qam_write_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, u8InitData);
    }
    else
    {
        //only scan base,set u8ChkTim 15
        qam_read_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, &u8InitData);
        u8InitData = u8InitData & 0xCF;
        qam_write_byte(u32TunerPort, Hi3137_T2_CHK_CTRL, u8InitData);

        u8ChkTim = 15;
    }

    switch(pstScanAttr->enScanMode)
    {
        case HI_UNF_TUNER_TER_SCAN_DVB_T2:
            qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8InitData);
            u8InitData = u8InitData & 0x9F;
            qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8InitData);
            break;
        case HI_UNF_TUNER_TER_SCAN_DVB_T:
            qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8InitData);
            u8InitData = (u8InitData & 0x9F) | (1 << 5);
            qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8InitData);

            //only scan dvb-t,set u8ChkTim 6
            u8ChkTim = 6;
            break;
        case HI_UNF_TUNER_TER_SCAN_DVB_T_T2_ALL:
            qam_read_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, &u8InitData);
            u8InitData = (u8InitData & 0x9F) | (2 << 5);
            qam_write_byte(u32TunerPort, Hi3137_MAN_RST_CTRL1, u8InitData);
            break;
        default:
            HI_ERR_TUNER("set scan attr error.\n");
            return HI_FAILURE;
    }

    //plp init
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8InitData);
    u8InitData = u8InitData & 0xFD;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8InitData);
    u8InitData = 0;
    qam_write_byte(u32TunerPort, Hi3137_PLP_ID0, u8InitData);

    //cfg tuner freq
    tuner_set_freq(u32TunerPort,pstScanAttr->u32Frequency,pstScanAttr->u32BandWidth,pstScanAttr->enScanMode);

    //cfg bandwidth
    switch(pstScanAttr->u32BandWidth)
    {
        case 1700:
            qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8InitData);
            u8InitData = u8InitData & 0x0F;
            qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, u8InitData);
            break;
        case 5000:
            qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8InitData);
            u8InitData = (u8InitData & 0x0F) | (1 << 4);
            qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, u8InitData);
            break;
        case 6000:
            qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8InitData);
            u8InitData = (u8InitData & 0x0F) | (2 << 4);
            qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, u8InitData);
            break;
        case 7000:
            qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8InitData);
            u8InitData = (u8InitData & 0x0F) | (3 << 4);
            qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, u8InitData);
            break;
        case 8000:
            qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8InitData);
            u8InitData = (u8InitData & 0x0F) | (4 << 4);
            qam_write_byte(u32TunerPort, Hi3137_BAND_WIDTH, u8InitData);
            break;
        default:
            HI_ERR_TUNER("bandwidth set err.\n");
            return HI_FAILURE;
    }

    hi3137_hot_reset(u32TunerPort);

    for(i = 0; i < u8ChkTim; i++)
    {
        tuner_mdelay(50);
        qam_read_byte(u32TunerPort, Hi3137_LOCK_FLAG, &u8InitData);
        if(u8InitData & 0x18)
        {
            break;
        }
    }

    if(i >= u8ChkTim)
    {
        HI_ERR_TUNER("scan fail.\n");
        return HI_FAILURE;
    }

    //query signal info and re-init u8ChkTim
    memset(&stScanResult, 0, sizeof(HI_UNF_TUNER_TER_TPINFO_S));
    s32Ret = hi3137_tp_scan_set_result(u32TunerPort, pstScanAttr, &stScanResult);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_TUNER("hi3137_tp_scan_set_result fail.\n");
        return s32Ret;
    }

    memcpy(pstTpInfo ,&stScanResult, sizeof(HI_UNF_TUNER_TER_TPINFO_S));

    return HI_SUCCESS;
}

HI_S32 hi3137_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    HI_U8 u8InitData = 0,u8Temp = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, Hi3137_OUTP_TS_MODE, &u8InitData);
    switch (enTsType)
    {
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
            u8InitData = u8InitData & 0xFC;

            u8Temp = 0x41;
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_CLK_SET,u8Temp);

            qam_read_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,&u8Temp);
        	u8Temp &=0x3F;
            u8Temp |= (0x1<<6);
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,u8Temp);
            break;
        case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT:
            u8InitData = u8InitData & 0xFE;
            u8InitData = u8InitData | 0x06;

        	u8Temp =0x0;
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_CLK_SET,u8Temp);
            
            qam_read_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,&u8Temp);
        	u8Temp &=0x3F;
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,u8Temp);
            break;
        case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
        default:
            u8InitData = u8InitData | 0x01;

        	u8Temp =0x0;
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_CLK_SET,u8Temp);
            
        	qam_read_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,&u8Temp);
        	u8Temp &=0x3F;
        	qam_write_byte(u32TunerPort,Hi3137_OUTP_MODE_SET,u8Temp);
            break;
    }

    //数据无效时，屏蔽时钟
    u8InitData = u8InitData | 0x10;

    qam_write_byte(u32TunerPort, Hi3137_OUTP_TS_MODE, u8InitData);

    return HI_SUCCESS;
}

HI_S32 hi3137_TS_out_enable(HI_U32 u32TunerPort)
{
    return HI_SUCCESS;
}

HI_S32 hi3137_TS_out_disable(HI_U32 u32TunerPort)
{
    return HI_SUCCESS;
}

HI_S32 hi3137_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
    HI_U8 u8Temp = 0, i;

    if(HI_NULL == pstTSOut)
    {
        return HI_FAILURE;
    }

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    for (i = 0; i < MAX_TS_LINE; i++)
    {
        if (HI_UNF_TUNER_OUTPUT_BUTT <= pstTSOut->enTSOutput[i])
        {
            HI_ERR_TUNER("Error pin define!\n");
            return HI_FAILURE;
        }
    }

    qam_read_byte(u32TunerPort, Hi3137_TS_0_SEL, &u8Temp);
    u8Temp = u8Temp & 0xf0;
    u8Temp = u8Temp | (pstTSOut->enTSOutput[0] & 0xf);
    qam_write_byte(u32TunerPort, Hi3137_TS_0_SEL, u8Temp);

    qam_write_byte(u32TunerPort, Hi3137_TS_A9_SEL, ((pstTSOut->enTSOutput[10]& 0xf)<<4) | pstTSOut->enTSOutput[9]);/*config ts_out_10&9bit*/
    qam_write_byte(u32TunerPort, Hi3137_TS_87_SEL, ((pstTSOut->enTSOutput[8]& 0xf)<<4) | pstTSOut->enTSOutput[7]);/*config ts_out_8&7bit*/
    qam_write_byte(u32TunerPort, Hi3137_TS_65_SEL, ((pstTSOut->enTSOutput[6]& 0xf)<<4) | pstTSOut->enTSOutput[5]);/*config ts_out_6&5bit*/
    qam_write_byte(u32TunerPort, Hi3137_TS_43_SEL, ((pstTSOut->enTSOutput[4]& 0xf)<<4) | pstTSOut->enTSOutput[3]);/*config ts_out_4&3bit*/
    qam_write_byte(u32TunerPort, Hi3137_TS_21_SEL, ((pstTSOut->enTSOutput[2]& 0xf)<<4) | pstTSOut->enTSOutput[1]);/*config ts_out_2&1bit*/

    s_stTunerResumeInfo[u32TunerPort].bSetTsOut = HI_TRUE;
    memcpy(&s_stTunerResumeInfo[u32TunerPort].stTsOut, pstTSOut, sizeof(HI_UNF_TUNER_TSOUT_SET_S));

    return HI_SUCCESS;
}

//agc自控方式时，获取信号强度请调用tuner提供的接口
//agc外控方式时，获取信号强度请调用hi3137提供的接口	
HI_S32 hi3137_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    HI_U32 u32Agc_Ctrl,u32AgcPow;
    HI_U8  u8AgcRd[2] = {0, 0};

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pu32SignalStrength)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    qam_read_byte(u32TunerPort, Hi3137_AGC_CTRL_L,&u8AgcRd[0]);
    qam_read_byte(u32TunerPort, Hi3137_AGC_CTRL_H,&u8AgcRd[1]);
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    u32Agc_Ctrl = (u8AgcRd[0] & 0x0F) + u8AgcRd[1] * 16;

    if(u32Agc_Ctrl >= 3200)
        u32AgcPow = 0;
    else if(u32Agc_Ctrl <= 640)
        u32AgcPow = 99;
    else
        u32AgcPow = 124 - u32Agc_Ctrl*80/2048;

    pu32SignalStrength[1] = u32AgcPow;

    return HI_SUCCESS;
}

HI_S32 hi3137_config_i2c_out(HI_U32 u32TunerPort)
{
    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    (HI_VOID)qam_write_byte(u32TunerPort, 0x2d,1);

    return HI_SUCCESS;
}

HI_S32 hi3137_config_i2c_close(HI_U32 u32TunerPort)
{
	HI_U8 u8temp;
    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    (HI_VOID)qam_read_byte(u32TunerPort, 0x2c,&u8temp);

    return HI_SUCCESS;
}

static HI_UNF_TUNER_FE_FECRATE_E hi3137_get_lowpri_fec_rate(HI_U8 u8CrL)
{
    HI_UNF_TUNER_FE_FECRATE_E enLowPriFECRate;


    switch (u8CrL)
    {
        case 0:
            enLowPriFECRate = HI_UNF_TUNER_FE_FEC_1_2;
            break;
        case 1:
            enLowPriFECRate = HI_UNF_TUNER_FE_FEC_2_3;
            break;
        case 2:
            enLowPriFECRate = HI_UNF_TUNER_FE_FEC_3_4;
            break;
        case 3:
            enLowPriFECRate = HI_UNF_TUNER_FE_FEC_5_6;
            break;
        default:
            enLowPriFECRate = HI_UNF_TUNER_FE_FEC_7_8;
            break;
    }

    return enLowPriFECRate;
}

static HI_VOID hi3137_get_signal_info_t(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    HI_U8   u8Register0 = 0;
    HI_U8   u8Register1 = 0;
    HI_U8   u8Register2 = 0;
    HI_U8   u8Register3 = 0;
    HI_U8   u8Register4 = 0;
    HI_U8   u8Grd,u8FFT,u8Cons,u8CR,u8Hier,u8CrL,u8tsPri;


    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T;

    qam_read_byte(u32TunerPort, Hi3137_TPS_DVBT, &u8Register0);  //{mod,hier,indepth,cod}
    qam_read_byte(u32TunerPort, Hi3137_CHN_FFT_GI, &u8Register1);  //{dvbt,spec,fft_size,gi_mode}
    qam_read_byte(u32TunerPort, Hi3137_CODE_RATE_DVBT, &u8Register2);
    qam_read_byte(u32TunerPort, Hi3137_AUTO_DLY, &u8Register3);
    qam_read_byte(u32TunerPort, Hi3137_CODE_RATE_DVBT, &u8Register4);
    u8Grd = (u8Register1 & 0x3);
    u8Cons = (u8Register0 & 0xc0) >> 6;
    u8FFT = (u8Register1 & 0x38) >> 3;
    u8Hier = (u8Register0 & 0x30) >> 4;
    u8CrL = (u8Register2 & 0x7);
    u8tsPri = (u8Register3 & 0x2) >> 1;
    if(u8Hier == 0)
    {
        u8CR = (u8Register0 &0x7);
    }
    else
    {
        if(u8tsPri == 1)
        {
            u8CR = u8Register4 & 0x7;
        }
        else
        {
            u8CR = u8Register0 & 0x7;
        }
    }
        
    switch (u8Grd)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
            break;
    }

    switch (u8Cons)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
    }

    switch (u8FFT)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_1K;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
            break;
    }

    switch (u8CR)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
            break;
    }

    switch (u8Hier)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_NO;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA1;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA2;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA4;
            break;
    }

    pstInfo->unSignalInfo.stTer.enLowPriFECRate = hi3137_get_lowpri_fec_rate(u8CrL);

    switch (u8tsPri)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_HP;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_LP;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enTsPriority = HI_UNF_TUNER_TS_PRIORITY_NONE;
            break;
    }
    
}

static HI_UNF_TUNER_FE_FECRATE_E hi3137_get_fec_rate(HI_U8 u8CR)
{
    HI_UNF_TUNER_FE_FECRATE_E enFECRate;


    switch (u8CR)
    {
        case 0:
            enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
            break;
        case 1:
            enFECRate = HI_UNF_TUNER_FE_FEC_3_5;
            break;
        case 2:
            enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
            break;
        case 3:
            enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
            break;
        case 4:
            enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
            break;
        case 5:
            enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
            break;
        case 6:
            enFECRate = HI_UNF_TUNER_FE_FEC_1_3;
            break;
        default:
            enFECRate = HI_UNF_TUNER_FE_FEC_2_5;
            break;
    }

    return enFECRate;
}

static HI_UNF_TUNER_TER_PILOT_PATTERN_E hi3137_get_pilot_pattern(HI_U8 u8PP)
{
    HI_UNF_TUNER_TER_PILOT_PATTERN_E enPilotPattern;

    
    switch (u8PP)
    {
        case 0:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP1;
            break;
        case 1:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP2;
            break;
        case 2:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP3;
            break;
        case 3:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP4;
            break;
        case 4:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP5;
            break;
        case 5:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP6;
            break;
        case 6:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP7;
            break;
        default:
            enPilotPattern = HI_UNF_TUNER_T2_PILOT_PATTERN_PP8;
            break;
    }

    return enPilotPattern;
}
static HI_VOID hi3137_get_signal_info_t2(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)    
{
    HI_U8   u8Register0 = 0;
    HI_U8   u8Register1 = 0;
    HI_U8   u8Grd,u8FFT,u8Cons,u8CR,u8BW,u8FEC,u8Rotation;
    HI_U8   u8PLP_Type,u8PP;


    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DVB_T2;

    qam_read_byte(u32TunerPort, Hi3137_TPS, &u8Register0);  //{mod,hier,indepth,cod}
    u8Grd = (u8Register0 & 0x1c) >> 2;
    u8BW = (u8Register0 & 0x20) >>5;
    switch (u8Grd)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_32;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_16;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_8;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_4;
            break;
        case 4:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_1_128;
            break;
        case 5:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_128;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enGuardIntv = HI_UNF_TUNER_FE_GUARD_INTV_19_256;
            break;
    }

    if (u8BW)
    {
         pstInfo->unSignalInfo.stTer.enCarrierMode = HI_UNF_TUNER_TER_EXTEND_CARRIER;
    }
    else
    {
        pstInfo->unSignalInfo.stTer.enCarrierMode = HI_UNF_TUNER_TER_NORMAL_CARRIER;
    }

    qam_read_byte(u32TunerPort, Hi3137_CHN_FFT_GI, &u8Register1);  //{dvbt,spec,fft_size,gi_mode}
    u8FFT = (u8Register1 & 0x38) >> 3;
    switch (u8FFT)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_1K;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_2K;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_4K;
            break;
        case 3:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_8K;
            break;
        case 4:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_16K;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enFFTMode = HI_UNF_TUNER_FE_FFT_32K;
            break;
    }

    qam_read_byte(u32TunerPort, Hi3137_PLP_PARAM, &u8Register0);
    u8CR = (u8Register0 &0xe0) >> 5;
    u8FEC = (u8Register0 & 0x1);
    u8Rotation = (u8Register0 &0x2) >>1;
    u8Cons = (u8Register0 & 0x1c) >> 2;
    if (u8Rotation)
    {
        pstInfo->unSignalInfo.stTer.enConstellationMode = HI_UNF_TUNER_CONSTELLATION_ROTATION;
    }
    else
    {
        pstInfo->unSignalInfo.stTer.enConstellationMode = HI_UNF_TUNER_CONSTELLATION_STANDARD;
    }

    if (u8FEC)
    {
        pstInfo->unSignalInfo.stTer.enFECFrameMode = HI_UNF_TUNER_TER_FEC_FRAME_NORMAL;
    }
    else
    {
        pstInfo->unSignalInfo.stTer.enFECFrameMode = HI_UNF_TUNER_TER_FEC_FRAME_SHORT;
    }

    switch (u8Cons)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QPSK;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 2:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
    }

    pstInfo->unSignalInfo.stTer.enFECRate = hi3137_get_fec_rate(u8CR);

    qam_read_byte(u32TunerPort, Hi3137_PLP_TYPE, &u8Register0);
    u8PLP_Type = (u8Register0 & 0xe0) >> 5;
    switch (u8PLP_Type)
    {
        case 0:
            pstInfo->unSignalInfo.stTer.enPLPType = HI_UNF_TUNER_T2_PLP_TYPE_COM;
            break;
        case 1:
            pstInfo->unSignalInfo.stTer.enPLPType = HI_UNF_TUNER_T2_PLP_TYPE_DAT1;
            break;
        default:
            pstInfo->unSignalInfo.stTer.enPLPType = HI_UNF_TUNER_T2_PLP_TYPE_DAT2;
            break;
    }

    qam_read_byte(u32TunerPort, Hi3137_PP_VERSION, &u8Register0);
    u8PP = (u8Register0 & 0xf0) >> 4;
    pstInfo->unSignalInfo.stTer.enPilotPattern = hi3137_get_pilot_pattern(u8PP);
}

HI_S32 hi3137_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    HI_S32  S32Ret;
    HI_U8   u8Status    = 0;
    HI_U8   u8DVBT;


    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_NULL == pstInfo)
    {
        return HI_FAILURE;
    }

    S32Ret = hi3137_get_lock_flag(u32TunerPort, FEC_LOCK_FLAG);
    if (HI_SUCCESS != S32Ret)
    {
        HI_ERR_TUNER("hi3137 not locked\n");
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }
    qam_read_byte(u32TunerPort, Hi3137_CHN_FFT_GI, &u8Status);
    u8DVBT = u8Status & 0x80;
    if (u8DVBT)
    {
        hi3137_get_signal_info_t(u32TunerPort, pstInfo);
    }
    else
    {
        hi3137_get_signal_info_t2(u32TunerPort, pstInfo);
    }
    
    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }
    return HI_SUCCESS;
}

HI_S32 hi3137_setplpid(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode)
{
    //HI_U8   u8PlpNum    = 0;
    //HI_U8   u8DataPlpId = 0;
    HI_U8   u8RegData   = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (u32Mode > 1)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_write_byte(u32TunerPort, Hi3137_PLP_ID0, u8PLpId);

    if (!u32Mode)   //若是切换plp模式时，还需要hot reset
    {
        qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
        u8RegData = u8RegData & 0xFE;
        qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);
        hi3137_hot_reset(u32TunerPort);
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    return HI_SUCCESS;
}

HI_S32 hi3137_set_common_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId)
{
    //HI_U8   u8PlpNum    = 0;
    //HI_U8   u8ComPlpId  = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_write_byte(u32TunerPort, Hi3137_PLP_ID1, u8PLpId);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    return HI_SUCCESS;
}

HI_S32 hi3137_set_common_plp_combination(HI_U32 u32TunerPort, HI_U8 u8ComPlpEna)
{
    HI_U8 u8InitData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(u8ComPlpEna > 1)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8InitData);
    u8InitData = (u8InitData & 0xFD) | (u8ComPlpEna << 1);
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8InitData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    return HI_SUCCESS;
}

HI_S32 hi3137_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    HI_S16  s16Err  = 0;
    HI_S16  s16Fs   = 0;
    HI_S8   s8Status[2] = {0, 0};
    HI_U8   u8BW    = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pu32Freq)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pu32Symb)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort, Hi3137_BAND_WIDTH, &u8BW);
    u8BW = (u8BW >> 4);

    /*freq_offset=(CAR_OFFSET_H*256+CAR_OFFSET_L)/2^8*fs*/
    qam_read_byte(u32TunerPort, Hi3137_CAR_OFFSET_L, &s8Status[0]);
    qam_read_byte(u32TunerPort, Hi3137_CAR_OFFSET_H, &s8Status[1]);
    s16Err = (HI_S16)((HI_U16)(s8Status[1] * 256) + (HI_U8)s8Status[0]);

    switch(u8BW)
    {
        case 0:     s16Fs = 1845;       break;
        case 1:     s16Fs = 5714;       break;
        case 2:     s16Fs = 6857;       break;
        case 3:     s16Fs = 8000;       break;
        case 4:     s16Fs = 9143;       break;
        default:    s16Fs = 9143;       break;
    }


    *pu32Freq = ((s16Err * s16Fs) >> 8)/1000;   /*转换为kHz*/


    //读取带宽误差，并存储
    /*fs_offset =(tim_offset*4 - (tim_loop_h*256+tim_loop_l)/16)/2^10*fs,单位为Hz*/
    qam_read_byte(u32TunerPort, Hi3137_TIM_LOOP_L, &s8Status[0]);
    qam_read_byte(u32TunerPort, Hi3137_TIM_LOOP_H, &s8Status[1]);   /*该寄存器为6位*/

    if (s8Status[1] & 0x20)     //TIM_LOOP_H最高位为符号位，为1表示负数
    {
        s16Err = (HI_S16)((HI_U16)((s8Status[1]&0x3f) * 256) + (HI_U8)s8Status[0] - (HI_U16)(1 << (6+8)));
    }
    else    //最高符号位为0，表示正数
    {
        s16Err = (HI_S16)((s8Status[1]&0x3f) * 256 + s8Status[0]);
    }
    qam_read_byte(u32TunerPort, Hi3137_TIM_OFFSET, &s8Status[0]);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    *pu32Symb = (((HI_S16)(s8Status[0] * 4) -(s16Err>>4))*s16Fs)>> 10;

    HI_INFO_TUNER("-->IN--%s,*pu32Freq:%d kHz,*pu32Symb:%d Symbol\n",__FUNCTION__,*pu32Freq,*pu32Symb);

    return HI_SUCCESS;
}

HI_VOID hi3137_connect_timeout(HI_U32 u32ConnectTimeout)
{
    return;
}

HI_S32 hi3137_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum)
{
    HI_U8   u8MumPlp    = 0;
    HI_U8   u8RegData   = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pu8PLPNum)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData | 0x01;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_NUM_PLP, &u8MumPlp);

    //close the plp read mode
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xFE;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    *pu8PLPNum = u8MumPlp;
    HI_INFO_TUNER("-->IN--hi3137_get_plp_num:%d\n",u8MumPlp);

    return HI_SUCCESS;
}

HI_S32 hi3137_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *pstPLPType)
{
    HI_UNF_TUNER_T2_PLP_TYPE_E enPlpType;
    HI_U8   u8PlpType   = 0;
    HI_U8   u8RegData   = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == pstPLPType)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    //qam_read_byte(u32TunerPort, Hi3137_NUM_PLP, &u8NumPlp);
    //open the plp read mode,only read the plp parameter,do not change the plp being processed by the demod
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData | 0x01;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_PLP_TYPE, &u8RegData);
    u8PlpType = (u8RegData & 0xe0) >> 5;
    if(u8PlpType == 0)
    {
        enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_COM;
    }
    else if(u8PlpType == 1)
    {
        enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT1;
    }
    else
    {
        enPlpType = HI_UNF_TUNER_T2_PLP_TYPE_DAT2;
    }

    //close the plp read mode
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xFE;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    *pstPLPType = enPlpType;

    return HI_SUCCESS;
}

HI_S32 hi3137_get_plp_id(HI_U32 u32TunerPort, HI_U8 *u8PlpId)
{
    HI_U8   u8DataPlpId = 0;
    HI_U8   u8RegData   = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData | 0x01;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_PLP_ID, &u8DataPlpId);

    //close the plp read mode
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xFE;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    *u8PlpId = u8DataPlpId;

    return HI_SUCCESS;
}

HI_S32 hi3137_get_plp_group_id(HI_U32 u32TunerPort, HI_U8 *u8GrpPlpId)
{
    HI_U8   u8PlpGrp    = 0;
    HI_U8   u8RegData   = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 0);
    }

    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData | 0x01;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    qam_read_byte(u32TunerPort, Hi3137_PLP_GRP_ID, &u8PlpGrp);

    //close the plp read mode
    qam_read_byte(u32TunerPort, Hi3137_PLP_CTRL, &u8RegData);
    u8RegData = u8RegData & 0xFE;
    qam_write_byte(u32TunerPort, Hi3137_PLP_CTRL, u8RegData);

    if(HI_UNF_DEMOD_DEV_TYPE_3137 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
	    qam_write_byte(u32TunerPort, Hi3137_MCU_HOLD, 1);
    }

    *u8GrpPlpId = u8PlpGrp;

    return HI_SUCCESS;
}

HI_S32 hi3137_set_antena_power(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ANTENNA_POWER_E enPower)
{
    HI_U8 u8RegData = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, Hi3137_IO_CTRL4, &u8RegData);
    u8RegData = u8RegData & 0xFD;
    u8RegData = (u8RegData & 0x3F) | (3<<6);

    switch(enPower)
    {
        case HI_UNF_TUNER_TER_ANTENNA_POWER_OFF:
            u8RegData = u8RegData & 0xDF;
            break;
        case HI_UNF_TUNER_TER_ANTENNA_POWER_ON:
            u8RegData = u8RegData | 0x20;
            break;
        default:
            HI_ERR_TUNER("antena power value err.\n");
            return HI_FAILURE;
    }
    qam_write_byte(u32TunerPort, Hi3137_IO_CTRL4, u8RegData);

    return HI_SUCCESS;
}

HI_S32 hi3137_standby(HI_U32 u32TunerPort, HI_U32 u32Status)
{
    HI_U8 u8RegData = 0;
    HI_UNF_TUNER_DEMOD_STATUS_E enStatus;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    enStatus = (HI_UNF_TUNER_DEMOD_STATUS_E)u32Status;

    switch(enStatus)
    {
        case HI_UNF_TUNER_DEMODE_WAKE_UP:
            //power up PLL0
            qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8RegData);
            u8RegData = u8RegData & 0xEF;
            qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8RegData);

            //power up PLL1
            qam_read_byte(u32TunerPort,Hi3137_PLL1_PD,&u8RegData);
            u8RegData = u8RegData & 0xEF;
            qam_write_byte(u32TunerPort,Hi3137_PLL1_PD,u8RegData);

            //switch i2c from xo to pll
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8RegData);
            u8RegData = u8RegData | 0x1;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8RegData);

            //realse hi3137 from hot reset
            qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8RegData);
            u8RegData = (u8RegData & 0xFC) | 0x03;
            qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8RegData);
            break;
        case HI_UNF_TUNER_DEMODE_STANDBY:
            //enable hi3137 into hot reset
            qam_read_byte(u32TunerPort,Hi3137_RSTN_CTRL,&u8RegData);
            u8RegData = (u8RegData & 0xFC) | 0x01;
            qam_write_byte(u32TunerPort,Hi3137_RSTN_CTRL,u8RegData);

            //switch i2c from pll to xo
            qam_read_byte(u32TunerPort,Hi3137_ADC_CTRL1,&u8RegData);
            u8RegData = u8RegData & 0xFE;
            qam_write_byte(u32TunerPort,Hi3137_ADC_CTRL1,u8RegData);

            //power down PLL0
            qam_read_byte(u32TunerPort,Hi3137_PLL0_PD,&u8RegData);
            u8RegData = u8RegData | 0x10;
            qam_write_byte(u32TunerPort,Hi3137_PLL0_PD,u8RegData);

            //power down PLL1
            qam_read_byte(u32TunerPort,Hi3137_PLL1_PD,&u8RegData);
            u8RegData = u8RegData | 0x10;
            qam_write_byte(u32TunerPort,Hi3137_PLL1_PD,u8RegData);
            break;
        default:
            HI_ERR_TUNER("demode status err.\n");
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}


