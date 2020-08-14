/******************************************************************************

  Copyright (C), 2012-2050, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hiDVBC.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/09/12
  Description   : 
  History       :
  1.Date        : 2014/09/12
    Author      : w00203631
    Modification: Created file
******************************************************************************/
#ifndef __HI_DTV100_DVBC_H__
#define __HI_DTV100_DVBC_H__

#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "drv_demod.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

#define HiFone_J83B_CHECKPOINTER(ptr)                                   \
    do                                                      \
    {                                                       \
        if (!(ptr))                                         \
        {                                                   \
            HI_INFO_TUNER("pointer is null\n");             \
            return HI_ERR_TUNER_INVALID_POINT;                     \
        }                                                   \
    } while (0)	


#define			HIFONE_J83B_CHN_SEL						0x0
#define			HIFONE_J83B_MAN_RST_CTRL0				0x20
#define			HIFONE_J83B_MAN_RST_CTRL1				0x21
#define			HIFONE_J83B_DEMO_FREQ_L					0x23
#define			HIFONE_J83B_DEMO_FREQ_M					0x24
#define			HIFONE_J83B_FEC_FREQ_L					0x26
#define			HIFONE_J83B_FEC_FREQ_M					0x27
#define			HIFONE_J83B_CATCH_CTRL					0x2A
#define			HIFONE_J83B_TOUT_CTRL					0x2B
#define			HIFONE_J83B_LOCK_FLAG					0x2C
#define			HIFONE_J83B_MAN_STATE					0x2D
#define			HIFONE_J83B_DEMO_FEC_FREQ_H			0x2E
#define			HIFONE_J83B_ILA_SEL						0x2F
#define			HIFONE_J83B_AGC_SPEED_BOUND			0x30
#define			HIFONE_J83B_AGC_GOAL					0x31
#define			HIFONE_J83B_AGCOK_WAIT					0x32
#define			HIFONE_J83B_AGC_CTRL					0x33
#define			HIFONE_J83B_AGC_DC_I					0x34
#define			HIFONE_J83B_AGC_DC_Q					0x35
#define			HIFONE_J83B_DAGC_CTRL					0x36
#define			HIFONE_J83B_AGC_CTRL_H					0x37
#define			HIFONE_J83B_AGC_CTRL_L					0x38
#define			HIFONE_J83B_AMP_ERR_IIR					0x39
#define			HIFONE_J83B_PDM_CTRL_H					0x3A
#define			HIFONE_J83B_PDM_CTRL_L					0x3B
#define			HIFONE_J83B_USE_PWM						0x3C
#define			HIFONE_J83B_SF_RMV_CTRL					0x41
#define			HIFONE_J83B_DAGC_REF					0x42
#define			HIFONE_J83B_SCAN_WAIT					0x43
#define			HIFONE_J83B_FREQ_DEV_L					0x44
#define			HIFONE_J83B_FREQ_DEV_H					0x45
#define			HIFONE_J83B_FS_L							0x46
#define			HIFONE_J83B_FS_H							0x47
#define			HIFONE_J83B_TR_CR_CTRL					0x48
#define			HIFONE_J83B_TR_SCAN_CTRL				0x4A
#define			HIFONE_J83B_FREQ_ADC_L					0x4B
#define			HIFONE_J83B_FREQ_ADC_M					0x4C
#define			HIFONE_J83B_TR_LOOP_CTRL				0x4D
#define			HIFONE_J83B_EQU_CTRL					0x63
#define			HIFONE_J83B_EQU_STEP_WAIT				0x64
#define			HIFONE_J83B_EQU_STATUS					0x66
#define			HIFONE_J83B_NOISE_POW_L					0x6C
#define			HIFONE_J83B_NOISE_POW_H					0x6D
#define			HIFONE_J83B_PT_CTRL						0x6E
#define			HIFONE_J83B_PT2_CR_CTRL					0x70
#define			HIFONE_J83B_QLLR_CTRL					0x90
#define			HIFONE_J83B_DEMAP_LOW_CTRL				0x91
#define			HIFONE_J83B_SYNC_CTRL					0x92
#define			HIFONE_J83B_ITLV_CW						0x94
#define			HIFONE_J83B_ITLV_AUTO					0x9C
#define			HIFONE_J83B_J83B_RS2_BER				0x9F
#define			HIFONE_J83B_OUTP_PLL_INI				0xB0
#define			HIFONE_J83B_OUTP_CAP_SET				0xB1
#define			HIFONE_J83B_OUTP_PLLUP_FRE				0xB2
#define			HIFONE_J83B_OUTP_PLL0					0xB3
#define			HIFONE_J83B_OUTP_PLL1					0xB4
#define			HIFONE_J83B_OUTP_PLL2					0xB5
#define			HIFONE_J83B_OUTP_PLL3					0xB6
#define			HIFONE_J83B_OUTP_PLL4					0xB7
#define			HIFONE_J83B_OUTP_I2C_CLK0				0xB8
#define			HIFONE_J83B_OUTP_I2C_CLK1				0xB9
#define			HIFONE_J83B_OUTP_I2C_CLK2				0xBA
#define			HIFONE_J83B_OUTP_RAM_THRE				0xBB
#define			HIFONE_J83B_OUTP_TS_MODE				0xBC
#define			HIFONE_J83B_OUTP_SYNC_BYTE				0xBD
#define			HIFONE_J83B_OUTP_PKT_SET				0xBE
#define			HIFONE_J83B_OUTP_RAND					0xBF
#define			HIFONE_J83B_OUTP_TS_10_SEL				0xC0
#define			HIFONE_J83B_OUTP_TS_32_SEL				0xC1
#define			HIFONE_J83B_OUTP_TS_54_SEL				0xC2
#define			HIFONE_J83B_OUTP_TS_76_SEL				0xC3
#define			HIFONE_J83B_OUTP_TS_98_SEL				0xC4
#define			HIFONE_J83B_OUTP_BER					0xC5
#define			HIFONE_J83B_OUTP_PKT_MODE				0xC6
#define			HIFONE_J83B_OUTP_NEAR_FUL				0xC7
#define			HIFONE_J83B_OUTP_SYS_CLK0				0xC8
#define			HIFONE_J83B_OUTP_SYS_CLK1				0xC9
#define			HIFONE_J83B_OUTP_PLL_CTRL				0xCA
#define			HIFONE_J83B_OUTP_BER_RES0				0xCB
#define			HIFONE_J83B_OUTP_BER_RES1				0xCC
#define			HIFONE_J83B_ERR_LMT_SET					0xCD
#define			HIFONE_J83B_ERR_PKT_L					0xCE
#define			HIFONE_J83B_ERR_PKT_H					0xCF


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif

