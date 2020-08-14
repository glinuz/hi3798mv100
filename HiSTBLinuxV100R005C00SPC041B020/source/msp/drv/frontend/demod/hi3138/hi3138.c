/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hi3138.c
* Description:
*
* History:
* Version   Date             Author                         DefectNum        Description
* main\1    2015-06-26   w203631\w313507         NULL                Create this file.
***********************************************************************************/
#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>
#include <asm/signal.h>

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "drv_demod.h"

#include "av2011.h"
#include "av2018.h"
#include "sharp_qm1d1c004x.h"
#include "M88TS2022.h"
#include "RDA5815.h"
#include "mxl603.h"
#include "mxl608.h"
#include "hi3136.h"
#include "hi3137.h"
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

#define HI3138_ADC_READY_MAX_TIME_MS    5

//extern HI_U32 g_u32TerrestrialIFKhz;  /*中心频率kHz*/
extern HI3136_CFG_ITEM_S hi3136_cfg[TUNER_NUM];

/*HI3138demod支持的信号类型匹配表*/
const HI_U32 u32DemodSignalTable[7][2] = 
{
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_SAT},
    //{HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_DVB_T},    
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_DVB_T2},   
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_ISDB_T},   
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_SAT},      
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_CAB},    
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_J83B},   
    {HI_UNF_TUNER_ADC_BUTT, HI_UNF_TUNER_SIG_TYPE_BUTT},
};

const HI_U32 u32DemodSwapSignalTable[7][2] = 
{
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_SAT},
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_CAB},
    {HI_UNF_TUNER_ADC0, HI_UNF_TUNER_SIG_TYPE_J83B},
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_SAT},
    //{HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_DVB_T},
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_DVB_T2},
    {HI_UNF_TUNER_ADC1, HI_UNF_TUNER_SIG_TYPE_ISDB_T},
    {HI_UNF_TUNER_ADC_BUTT,HI_UNF_TUNER_SIG_TYPE_BUTT},
};

Hi3138_SIGNAL_INFO_ALLOCATION stHi3138Allocation[2] = 
{
    {0,
    HI3138_INNER_DEMOD0,
    HI3138_LNB_POWER_BY_ADDR0,
    HI_UNF_TUNER_SIG_TYPE_BUTT,
    HI_UNF_TUNER_ADC0,
    HI_UNF_TUNER_TS0_OUTPUT,
    HI_UNF_TUNER_IF_OUTPUT_SEL_BOTH_IQ,
    HI_UNF_TUNER_I2C_TRANSMISSION_SEL_SCLT_SDAT,
    HI_UNF_TUNER_SYNC_PIN_MUX_SYNC,
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,
    HI_FALSE},
    
    {0,
    HI3138_INNER_DEMOD1,
    HI3138_LNB_POWER_BY_ADDR1,
    HI_UNF_TUNER_SIG_TYPE_BUTT,
    HI_UNF_TUNER_ADC1,
    HI_UNF_TUNER_TS1_OUTPUT,
    HI_UNF_TUNER_IF_OUTPUT_SEL_BOTH_IQ,
    HI_UNF_TUNER_I2C_TRANSMISSION_SEL_SCLT_SDAT,
    HI_UNF_TUNER_SYNC_PIN_MUX_SYNC,
    HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A,
    HI_FALSE},
};

HI_U8 gs_u8InnerCnt=0;

HI_S32 hi3138_i2c_chn_sel(HI_U32 u32TunerPort)
{
    HI_U8 u8Temp = 0,u8Buf = 0;
    Hi3138_INNER_DEMOD_NUMBER enInnerDemodId;
    HI_U32 i;
    
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI3138_IPIO_CINFIG(u32TunerPort);

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);
    enInnerDemodId = stHi3138Allocation[i].enInnerDemodId;

    /*TSx引脚复用为I2C功能*/
    if( HI_UNF_TUNER_I2C_TRANSMISSION_SEL_TS1_DATAx == stHi3138Allocation[i].enI2CGateWaySel)
    {
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
        u8Buf = (u8Temp & 0xF);
        u8Temp = (u8Temp << 1);
        u8Temp &= ~(0x3 << 6);
        u8Temp |= (2 << 6);
        u8Temp = (u8Temp & 0xF0) | u8Buf;
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
    }
    else
    {
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
        u8Buf = (u8Temp & 0xF);
        u8Temp = (u8Temp << 1);
        u8Temp &= ~(0x03 << 6);
        u8Temp = (u8Temp & 0xF0) | u8Buf;
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        if (HI3138_INNER_DEMOD0 == enInnerDemodId)
        {
            qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);
            u8Temp &= ~(0x0F << 0);
            u8Temp |= (1 << 1);
            qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);
        }
        else if (HI3138_INNER_DEMOD1 == enInnerDemodId)
        {
            qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);
            u8Temp &= ~(0x0F << 0);
            u8Temp |= (1 << 3);
            qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel u32TunerPort[%d] is not support.\n",u32TunerPort);
            return HI_FAILURE;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                 (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                  (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);
        u8Temp &= ~(0x0F << 0);
        u8Temp |= 1;
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);

    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB) ||
                 (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);
        u8Temp &= ~(0x0F << 0);
        u8Temp |= (1 << 2);
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel enSigType[%d] is not support.\n",g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
   
    return HI_SUCCESS;
}

static HI_S32 hi3138_hot_reset(HI_U32 u32TunerPort)
{
    HI_U8 u8Temp = 0;
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }
    
    qam_read_byte(u32TunerPort, HI3138_RSTN_CTRL, &u8Temp);
    u8Temp &= ~(1 << 1);
    qam_write_byte(u32TunerPort, HI3138_RSTN_CTRL, u8Temp);
    msleep(2);
    u8Temp |= 0x2;
    qam_write_byte(u32TunerPort, HI3138_RSTN_CTRL, u8Temp);

    return HI_SUCCESS;
}

static HI_S32 hi3138_tuner_init(HI_U32 u32TunerPort, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{    
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif

    HI_TUNER_CHECKPORT(u32TunerPort);

#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif

    HI_INFO_TUNER("enTunerDevType:%d\n", enTunerDevType);

    /* Init tuner fuctions */
    switch (enTunerDevType)
    {
/*以下为DVB-S中初始化的tuner*/
#if defined(TUNER_DEV_TYPE_AV2011) /*||defined(OPENTV5_SIGNAL_DVB_S)*/
        case HI_UNF_TUNER_DEV_TYPE_AV2011:
            av2011_init_tuner(u32TunerPort);
            break;
#endif

#if defined(TUNER_DEV_TYPE_AV2018) /*||defined(OPENTV5_SIGNAL_DVB_S)*/
        case HI_UNF_TUNER_DEV_TYPE_AV2018:
            av2018_init_tuner(u32TunerPort);
            break;
#endif

#if defined(TUNER_DEV_TYPE_SHARP7903)
        case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
            sharp7903_init_tuner(u32TunerPort);
            break;
#endif

#if defined(TUNER_DEV_TYPE_M88TS2022)
        case HI_UNF_TUNER_DEV_TYPE_M88TS2022:
            M88TS2022_init_tuner(u32TunerPort);
            break;
#endif

#if defined(TUNER_DEV_TYPE_RDA5815)
        case HI_UNF_TUNER_DEV_TYPE_RDA5815:
            RDA5815_init_tuner(u32TunerPort);
            break;
#endif

/*以下为DVB-T中初始化的tuner*/
#if defined (TUNER_DEV_TYPE_MXL603)
        case HI_UNF_TUNER_DEV_TYPE_MXL603:
            //mxl603_init(u32TunerPort);
            break;
#endif

#if defined (TUNER_DEV_TYPE_MXL608)
        case HI_UNF_TUNER_DEV_TYPE_MXL608:
            mxl608_init_tuner(u32TunerPort);
            break;
#endif

        default:
            HI_ERR_TUNER("Tuner type not supported!g_stTunerOps[%d].enTunerDevType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enTunerDevType);
            return HI_FAILURE;
    }

#ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3138_tuner_init time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
#endif

    return HI_SUCCESS;
}

static HI_S32 hi3138_tuner_set(HI_U32 u32TunerPort,HI_U32 u32Frequency,HI_U32 u32BandWidth)
{
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_INFO_TUNER("u32Frequency:%d, u32BandWidth:%d\n", u32Frequency, u32BandWidth);
    
    switch(g_stTunerOps[u32TunerPort].enTunerDevType)
    {
#if defined (TUNER_DEV_TYPE_MXL601)
        case HI_UNF_TUNER_DEV_TYPE_MXL601:
            //mxl601_set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel,u32Frequency,u32BandWidth);
            break;
#endif

#if defined (TUNER_DEV_TYPE_MXL603)
        case HI_UNF_TUNER_DEV_TYPE_MXL603:
            mxl603_set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, u32Frequency);
            break;
#endif

        default:
            HI_ERR_TUNER("Tuner type not supported!g_stTunerOps[%d].enTunerDevType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enTunerDevType);
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_if_freq_set(HI_U32 u32TunerPort)
{
    HI_U32 u32IfFreqKhz;
    HI_U8 u8Data;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    switch(g_stTunerOps[u32TunerPort].enSigType)
    {
        case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
        /*now jump to HI_UNF_TUNER_SIG_TYPE_DVB_T*/
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
        /*now jump to HI_UNF_TUNER_SIG_TYPE_DVB_T2*/
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            u32IfFreqKhz = 5380;
            //g_u32TerrestrialIFKhz = 5380;
            u8Data = u32IfFreqKhz & 0xff;
            qam_write_byte(u32TunerPort, HI3138_T_IF_FREQ_L, u8Data);
            u8Data = (u32IfFreqKhz>>8) & 0xff;
            qam_write_byte(u32TunerPort, HI3138_T_IF_FREQ_H, u8Data);
            break;
        case HI_UNF_TUNER_SIG_TYPE_CAB:
        /*now jump to HI_UNF_TUNER_SIG_TYPE_J83B*/
        case HI_UNF_TUNER_SIG_TYPE_J83B:
            u32IfFreqKhz = 5380;
            //g_u32TerrestrialIFKhz = 5380;
            u8Data = u32IfFreqKhz & 0xff;
            qam_write_byte(u32TunerPort, HI3138_C_FREQ_DEV_L, u8Data);
            u8Data = (u32IfFreqKhz>>8) & 0xff;
            qam_write_byte(u32TunerPort, HI3138_C_FREQ_DEV_H, u8Data);
            break;
        default:
            HI_ERR_TUNER("Signal type not supported!g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            break;
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_pll_config(HI_U32 u32TunerPort,HI_U32 u32DemodClk)
{
    HI_U32 clk_xo_khz = 0,clk_pll0_khz = 0,clk_pll1_khz= 0;
    HI_U32 pll0_div = 0,pll0_div_int = 0,pll0_div_fh = 0,pll0_div_fl = 0;
    HI_U32 pll1_div = 0,pll1_div_int = 0,pll1_div_fh = 0,pll1_div_fl = 0;
    HI_U32 u32DemoClkKhz, u32FecClkKhz;
    HI_U8  u8Data = 0, i = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    if(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        u32DemoClkKhz = 124000;
        u32FecClkKhz = 186000;

        clk_pll0_khz = u32DemoClkKhz * 6;
        clk_pll1_khz = u32FecClkKhz;
    }
    else if((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
        ||(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        u32DemoClkKhz = 62000;
        u32FecClkKhz = 186000;

        clk_pll0_khz = u32DemoClkKhz * 12;
        clk_pll1_khz = u32FecClkKhz;
    }
    else if((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        ||(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        u32DemoClkKhz = 93000;
        u32FecClkKhz = 186000;

        clk_pll0_khz = u32DemoClkKhz * 8;
        clk_pll1_khz = u32FecClkKhz;
    }
    

    clk_xo_khz = u32DemodClk;
    if(0 == clk_xo_khz)
    {
        HI_ERR_TUNER("clk_xo_khz is zero.\n");
        return HI_FAILURE;
    }
	pll0_div = clk_pll0_khz * 1 * 4096 / clk_xo_khz;
	pll0_div_int= pll0_div / 4096;
	pll0_div_fh = (pll0_div / 256) % 16;
	pll0_div_fl = pll0_div % 256;

	pll1_div = clk_pll1_khz * 3 * 4096 / clk_xo_khz;
	pll1_div_int= pll1_div / 4096;
	pll1_div_fh = (pll1_div / 256) % 16;
	pll1_div_fl = pll1_div % 256;

	//I2C_CHN_SEL config 3,selected IP_IO work
	qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Data);
	u8Data = (u8Data & 0xF0) + 0x3;
	qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Data);
	
	qam_write_bit(u32TunerPort, HI3138_CRG_CTRL0, 0, 0);	// change to xo clock
	//set PLL0 for demo clock
	qam_write_bit (u32TunerPort, HI3138_PLL0_CTRL1, 4, 1);	// power down
	u8Data = (HI_U8)pll0_div_int;
	qam_write_byte(u32TunerPort, HI3138_PLL0_CTRL3, u8Data);
	qam_read_byte(u32TunerPort, HI3138_PLL0_CTRL1, &u8Data);
	u8Data= (u8Data & 0xF0) + (HI_U8)pll0_div_fh;
	qam_write_byte(u32TunerPort, HI3138_PLL0_CTRL1, u8Data);
	u8Data = (HI_U8)pll0_div_fl;
	qam_write_byte(u32TunerPort, HI3138_PLL0_CTRL0, u8Data);
	//set PLL1 for FEC clock
	qam_write_bit (u32TunerPort, HI3138_PLL1_CTRL1, 4, 1);	// power down
	u8Data = (HI_U8)pll1_div_int;
	qam_write_byte(u32TunerPort, HI3138_PLL1_CTRL3, u8Data);
	qam_read_byte(u32TunerPort, HI3138_PLL1_CTRL1, &u8Data);
	u8Data= (u8Data & 0xF0) + (HI_U8)pll1_div_fh;
	qam_write_byte(u32TunerPort, HI3138_PLL1_CTRL1, u8Data);
	u8Data = (HI_U8)pll1_div_fl;
	qam_write_byte(u32TunerPort, HI3138_PLL1_CTRL0, u8Data);

	tuner_mdelay(5);
	qam_write_bit (u32TunerPort, HI3138_PLL0_CTRL1, 4, 0);	// power up pll0
	qam_write_bit (u32TunerPort, HI3138_PLL1_CTRL1, 4, 0);	// power up pll1
	//check pll0 and pll1 is locked or not
	do
	{
		tuner_mdelay(1);
		qam_read_byte(u32TunerPort, HI3138_PLL_STATUS, &u8Data);
		i++;
	}while((0xC0 != (u8Data & 0xC0))&& i < 10); //max delay is 10ms

	if(i >= 10)
	{
        HI_ERR_TUNER("hi3138 wait pll lock timeout.u32TunerPort:%d,HI3138_PLL_STATUS:0x%x\n", 
                                u32TunerPort,
                                u8Data);
        return HI_FAILURE;
	}

	qam_write_bit (u32TunerPort, HI3138_PLL0_CTRL1, 5, 0);		//bit[5] pll0_foutvcopd clear 0
	qam_write_bit (u32TunerPort, HI3138_PLL1_CTRL1, 5, 0);		//bit[5] pll1_foutvcopd clear 0
	qam_write_bit(u32TunerPort, HI3138_CRG_CTRL0, 0, 1);	// change to Fout0/6

	return HI_SUCCESS;
}

static HI_S32 hi3138_set_adc0_channel(HI_U32 u32TunerPort, HI_UNF_TUNER_SIG_TYPE_E enSigType)
{
    HI_U8 u8Data = 0;
    
    switch(enSigType)
	{
		case HI_UNF_TUNER_SIG_TYPE_SAT:
			qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Data);
			u8Data = (u8Data & 0xE0) + 14;
			qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Data);
			break;
		case HI_UNF_TUNER_SIG_TYPE_DVB_T:
		case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
		case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
		case HI_UNF_TUNER_SIG_TYPE_CAB:
		case HI_UNF_TUNER_SIG_TYPE_J83B:
			qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Data);
			u8Data = (u8Data & 0xFC) + 0x1;
			qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Data);
			break;
		default:
			HI_ERR_TUNER("signal type not support.SigType:%d\n",enSigType);
			break;
	}

    return HI_SUCCESS;
}

static HI_S32 hi3138_set_adc1_channel(HI_U32 u32TunerPort, HI_UNF_TUNER_SIG_TYPE_E enSigType)
{
    HI_U8 u8Data = 0;
    
    switch(enSigType)
	{
		case HI_UNF_TUNER_SIG_TYPE_SAT:
			qam_read_byte(u32TunerPort, HI3138_CRG_CTRL2, &u8Data);
			u8Data = (u8Data & 0xE0) + 14;
			qam_write_byte(u32TunerPort, HI3138_CRG_CTRL2, u8Data);
			break;
		case HI_UNF_TUNER_SIG_TYPE_CAB:
		case HI_UNF_TUNER_SIG_TYPE_J83B:
		case HI_UNF_TUNER_SIG_TYPE_DVB_T:
		case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
		case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
			qam_read_byte(u32TunerPort, HI3138_CRG_CTRL2, &u8Data);
			u8Data = (u8Data & 0xFC) + 0x1;
			qam_write_byte(u32TunerPort, HI3138_CRG_CTRL2, u8Data);
			break;
		default:
			HI_ERR_TUNER("signal type not support.enSigType:%d\n",enSigType);
			break;;
	}

    return HI_SUCCESS;
}

static HI_VOID hi3138_init_adc0(HI_U32 u32TunerPort, HI_UNF_TUNER_SIG_TYPE_E enSigType,
    HI_UNF_TUNER_IF_PATH_SEL_E enIfPath,HI_U8 u8WhichTimes)
{
    HI_U32 i;
    HI_U8 u8Data = 0;
    
    switch(enSigType)
	{
        case HI_UNF_TUNER_SIG_TYPE_SAT:
            qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 7, 1);
            qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL1, 0, 1);
            //opm powerdown
            qam_read_byte(u32TunerPort, HI3138_ADC0_CTRL0, &u8Data);
            u8Data = (u8Data & 0xCF);
            qam_write_byte(u32TunerPort, HI3138_ADC0_CTRL0, u8Data);
            //reset adc0
            qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 1, 0);
            qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 1, 1);
            //opm normal
            qam_read_byte(u32TunerPort, HI3138_ADC0_CTRL0, &u8Data);
            u8Data = (u8Data & 0xCF)+(3<<4);
            qam_write_byte(u32TunerPort, HI3138_ADC0_CTRL0, u8Data);
            for(i = 0;i < HI3138_ADC_READY_MAX_TIME_MS;i++)
            {
				qam_read_bit(u32TunerPort, HI3138_ADC0_STATUS, 6 , &u8Data);
                if(u8Data)
                {               
                    break;
                }
				tuner_mdelay(1);
			}
			
			if(i>=HI3138_ADC_READY_MAX_TIME_MS)
			{
				HI_ERR_TUNER("ADC0 not ready.\n");
			}

			qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 7, 0);
			tuner_mdelay(5);
			qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 7, 1);
			break;
		case HI_UNF_TUNER_SIG_TYPE_CAB:
		case HI_UNF_TUNER_SIG_TYPE_J83B:
            if(!u8WhichTimes)
            {
                if (( HI_UNF_TUNER_SIG_TYPE_SAT== stHi3138Allocation[1].enHi3138SigType)
                    || ( HI_UNF_TUNER_SIG_TYPE_BUTT== stHi3138Allocation[1].enHi3138SigType))
                {
                    HI_INFO_TUNER("operate T on the mode:demod0=S/S2,demod1=C.\n");
                    qam_write_bit(u32TunerPort, HI3138_CHN_ON, 0, 1);   /*enable T*/
                    qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 7, 0);   /*ppsel*/
                    qam_write_bit(u32TunerPort, HI3138_CHN_ON, 0, 0);   /*disenable T*/
                }
                /*now jump to HI_UNF_TUNER_SIG_TYPE_DVB_T*/
            }
		case HI_UNF_TUNER_SIG_TYPE_DVB_T:
		case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
		case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
			qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 7, 0);   /*单通道*/
            if(HI_UNF_TUNER_IF_OUTPUT_SEL_SINGLE_I == enIfPath)
            {
            	//I output
            	qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL1, 0, 0);   /*I路输出*/
            }
            else if(HI_UNF_TUNER_IF_OUTPUT_SEL_SINGLE_Q == enIfPath)
            {
                //Q output
            	qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL1, 0, 1);   /*I/Q两路输出*/
            }
            else
            {
                HI_ERR_TUNER("IF output err.\n");
            }
			//opm powerdown
			qam_read_byte(u32TunerPort, HI3138_ADC0_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF);
			qam_write_byte(u32TunerPort, HI3138_ADC0_CTRL0, u8Data);
			//reset adc0
			qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 1, 0);
			qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 1, 1);
			//opm normal
			qam_read_byte(u32TunerPort, HI3138_ADC0_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF)+(3<<4);
			qam_write_byte(u32TunerPort, HI3138_ADC0_CTRL0, u8Data);
			for(i = 0;i < HI3138_ADC_READY_MAX_TIME_MS;i++)
			{
				qam_read_bit(u32TunerPort, HI3138_ADC0_STATUS, 6 , &u8Data);
				if(u8Data)
                {               
                    break;
                }
				tuner_mdelay(1);
			}
			
			if(i>=HI3138_ADC_READY_MAX_TIME_MS)
			{
				HI_ERR_TUNER("ADC0 not ready.\n");
			}
			break;
		default:
			HI_ERR_TUNER("adc0 not in use.\n");
			break;
	}
}

static HI_VOID hi3138_init_adc1(HI_U32 u32TunerPort, HI_UNF_TUNER_SIG_TYPE_E enSigType,
    HI_UNF_TUNER_IF_PATH_SEL_E enIfPath,HI_U8 u8WhichTimes)
{
    HI_U32 i;
    HI_U8 u8Data = 0;
    
    switch(enSigType)
	{
		case HI_UNF_TUNER_SIG_TYPE_SAT:
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 7, 1);
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL1, 0, 1);
			//opm powerdown
			qam_read_byte(u32TunerPort, HI3138_ADC1_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF);
			qam_write_byte(u32TunerPort, HI3138_ADC1_CTRL0, u8Data);
			//reset adc0
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 1, 0);
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 1, 1);
			//opm normal
			qam_read_byte(u32TunerPort, HI3138_ADC1_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF)+(3<<4);
			qam_write_byte(u32TunerPort, HI3138_ADC1_CTRL0, u8Data);
			for(i = 0;i < HI3138_ADC_READY_MAX_TIME_MS;i++)
			{
				qam_read_bit(u32TunerPort, HI3138_ADC1_STATUS, 6 , &u8Data);
				if(u8Data)
                {               
                    break;
                }
				tuner_mdelay(1);
			}
			
			if(i>=HI3138_ADC_READY_MAX_TIME_MS)
			{
				HI_ERR_TUNER("ADC0 not ready.\n");
			}

			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 7, 0);
			tuner_mdelay(5);
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 7, 1);
			break;
		case HI_UNF_TUNER_SIG_TYPE_DVB_T:
		case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
		case HI_UNF_TUNER_SIG_TYPE_ISDB_T:
            if(u8WhichTimes)
            {
                if((HI_UNF_TUNER_SIG_TYPE_SAT == stHi3138Allocation[0].enHi3138SigType)
                    || ( HI_UNF_TUNER_SIG_TYPE_BUTT== stHi3138Allocation[0].enHi3138SigType))
                {
                    /*在demod交换且demod0为T,demod1为S的情况下*/
                    HI_INFO_TUNER("operate C on the mode(swaped):demod0=T,demod1=S/S2.\n");
                    qam_write_bit(u32TunerPort,HI3138_CHN_ON, 2, 1);		 //enable C
                    qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0,7,0);    //ppsel
                    qam_write_bit(u32TunerPort,HI3138_CHN_ON, 2, 0);		 //disable C
                }
                /*now jump to HI_UNF_TUNER_SIG_TYPE_CAB*/
            }
		case HI_UNF_TUNER_SIG_TYPE_CAB:
		case HI_UNF_TUNER_SIG_TYPE_J83B:
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 7, 0);
            if(HI_UNF_TUNER_IF_OUTPUT_SEL_SINGLE_I == enIfPath)
            {
				//I output
				qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL1, 0, 0);
            }
            else if(HI_UNF_TUNER_IF_OUTPUT_SEL_SINGLE_Q == enIfPath)
            {
                //Q output
				qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL1, 0, 1);
            }
            else
            {
                HI_ERR_TUNER("IF output err.\n");
            }
			//opm powerdown
			qam_read_byte(u32TunerPort, HI3138_ADC1_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF);
			qam_write_byte(u32TunerPort, HI3138_ADC1_CTRL0, u8Data);
			//reset adc0
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 1, 0);
			qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 1, 1);
			//opm normal
			qam_read_byte(u32TunerPort, HI3138_ADC1_CTRL0, &u8Data);
			u8Data = (u8Data & 0xCF)+(3<<4);
			qam_write_byte(u32TunerPort, HI3138_ADC1_CTRL0, u8Data);
			for(i = 0;i < HI3138_ADC_READY_MAX_TIME_MS;i++)
			{
				qam_read_bit(u32TunerPort, HI3138_ADC1_STATUS, 6 , &u8Data);
				if(u8Data)
                            {               
                                break;
                            }
				tuner_mdelay(1);
			}
			
			if(i >= HI3138_ADC_READY_MAX_TIME_MS)
			{
				HI_ERR_TUNER("ADC1 not ready.\n");
			}
			break;
		default:
			HI_ERR_TUNER(" adc1 not in use.\n");
			break;
	}
}

static HI_S32 hi3138_adc_config(HI_U32 u32TunerPort)
{
    HI_U8 u8Data = 0;

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }
    
	//I2C_CHN_SEL config 3,selected IP_IO work
	qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Data);
	u8Data = (u8Data & 0xF0) + 0x3;
	qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Data);

	//打开共模电压
	qam_write_bit(u32TunerPort, HI3138_ADC0_CTRL0, 0, 1);
	qam_write_bit(u32TunerPort, HI3138_ADC1_CTRL0, 0, 1);
	//配置第一路ADC工作时钟
	if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[0].enADCId)
	{
		hi3138_set_adc0_channel(u32TunerPort, stHi3138Allocation[0].enHi3138SigType);
	}
	else
	{
		hi3138_set_adc1_channel(u32TunerPort, stHi3138Allocation[0].enHi3138SigType);
	}

	//配置第二路ADC工作时钟
	if(HI_UNF_TUNER_ADC1 == stHi3138Allocation[1].enADCId)
	{
		hi3138_set_adc1_channel(u32TunerPort, stHi3138Allocation[1].enHi3138SigType);
	}
	else
	{
		hi3138_set_adc0_channel(u32TunerPort, stHi3138Allocation[1].enHi3138SigType);
	}
	//配置第一路ADC工作参数
	if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[0].enADCId)
	{
		hi3138_init_adc0(u32TunerPort, stHi3138Allocation[0].enHi3138SigType,
            stHi3138Allocation[0].enIFPath, 0);
	}
	else
	{
		hi3138_init_adc1(u32TunerPort, stHi3138Allocation[0].enHi3138SigType,
            stHi3138Allocation[0].enIFPath, 0);
	}
	//配置第二路ADC工作参数
	if(HI_UNF_TUNER_ADC1 == stHi3138Allocation[1].enADCId)
	{
		hi3138_init_adc1(u32TunerPort, stHi3138Allocation[1].enHi3138SigType,
            stHi3138Allocation[1].enIFPath, 1);
	}
	else
	{
		hi3138_init_adc0(u32TunerPort, stHi3138Allocation[1].enHi3138SigType,
            stHi3138Allocation[1].enIFPath, 1);
	}

	return HI_SUCCESS;
}


static HI_S32 hi3138_io_config(HI_U32 u32TunerPort)
{
    HI_U8 u8Temp = 0,u8Buf;
    HI_U8 i = 0;
    
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI3138_IPIO_CINFIG(u32TunerPort);

    if ((stHi3138Allocation[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&     /*仅打开demod0,仅支持S+T*/
        (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        if ((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T)
            || (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2))
        {
            /*DISEQC0_OUT-->FEF_REG*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= 1;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T)
        {
            /*DISEQC0_OUT-->earthquake inspect*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= 1;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);
            
            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[0].enADCId)
            {
                /*DISEQC1_IN-->LNB_EN*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
                u8Temp &= ~(3 << 6);
                u8Temp |= (1 << 6);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);
                
                /*ADDR0-->HV_SEL*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
                u8Temp &= ~(1 << 6);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
            }
            else
            {
                /*DISEQC0_IN-->LNB_EN*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
                u8Temp &= ~(3 << 4);
                u8Temp |= (1 << 4);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);
                
                /*ADDR1-->HV_SEL*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
                u8Temp &= ~(1 << 7);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
            }

            /*以下情况需要DISEQC0_IN由DISEQC1_IN替换，同时DISEQC0_OUT由DISEQC1_OUT替换*/
            if(((HI3138_INNER_DEMOD1 == stHi3138Allocation[0].enInnerDemodId) 
                && (HI_UNF_TUNER_ADC0 == stHi3138Allocation[0].enADCId))
                ||((HI3138_INNER_DEMOD0 == stHi3138Allocation[0].enInnerDemodId) 
                && (HI_UNF_TUNER_ADC1 == stHi3138Allocation[0].enADCId)))
            {
                /*demod0的DISEQC0_IN由DISEQC1_IN替换*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
                u8Temp &= ~(1 << 2);
                u8Temp |= 1 << 2;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

                /*demod0的DISEQC0_OUT由DISEQC1_OUT替换*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
                u8Temp &= ~(1 << 3);
                u8Temp |= 1 << 3;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            }

        }
        else if (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)
        {
            ;
        }
        else if (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)
        {
            ;
        }
        else
        {
            HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",stHi3138Allocation[0].enHi3138SigType,stHi3138Allocation[1].enHi3138SigType);
            return HI_FAILURE;
        }
    }
    else if ((stHi3138Allocation[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&
        (stHi3138Allocation[1].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&         /*T+C*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&         /*T+C*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            /*DISEQC0_OUT-->FEF_REG*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= 1;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+J83B*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+J83B*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            /*DISEQC0_OUT-->FEF_REG*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= 1;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+C*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+C*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+J83B*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+J83B*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            /*DISEQC0_IN-->LNB_EN*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 4);
            u8Temp |= (1 << 4);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*DISEQC0_OUT-->FEF_REG*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= 1;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);

            /*ADDR1-->HV_SEL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 7);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp); 
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            /*DISEQC0_IN-->LNB_EN*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 4);
            u8Temp |= (1 << 4);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR0-->ANT_CTRL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);

            /*ADDR1-->HV_SEL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 7);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            /*DISEQC0_IN-->LNB_EN*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 4);
            u8Temp |= (1 << 4);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR1-->HV_SEL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 7);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);

            /*demod0的DISEQC0_IN由DISEQC1_IN替换*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 2);
            u8Temp |= 1 << 2;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

            /*demod0的DISEQC0_OUT由DISEQC1_OUT替换*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 3);
            u8Temp |= 1 << 3;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            /*DISEQC0_IN-->LNB_EN*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL16, &u8Temp);
            u8Temp &= ~(3 << 4);
            u8Temp |= (1 << 4);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL16, u8Temp);

            /*ADDR1-->HV_SEL*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(1 << 7);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);

            /*demod0的DISEQC0_IN由DISEQC1_IN替换*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 2);
            u8Temp |= 1 << 2;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

            /*demod0的DISEQC0_OUT由DISEQC1_OUT替换*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 3);
            u8Temp |= 1 << 3;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            
        }
        else if ((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+S1*/
            (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
        {
            if(HI_UNF_TUNER_SYNC_PIN_MUX_SYNC == stHi3138Allocation[0].enSyncPin)
            {
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL14, &u8Temp);
                u8Temp &=  ~(3 << 2);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL14, u8Temp);
            }
            else if(HI_UNF_TUNER_SYNC_PIN_MUX_LNB_EN == stHi3138Allocation[0].enSyncPin)
            {
                /*TS0_SYNC-->LNB_EN0*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
                u8Temp &= ~(1 << 2);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
                
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL14, &u8Temp);
                u8Temp &=  ~(3 << 2);
                u8Temp |= (2 << 2);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL14, u8Temp);
            }

            if(HI_UNF_TUNER_SYNC_PIN_MUX_SYNC == stHi3138Allocation[1].enSyncPin)
            {
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL15, &u8Temp);
                u8Temp &= ~(3 << 2);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL15, u8Temp);
            }
            else if(HI_UNF_TUNER_SYNC_PIN_MUX_LNB_EN == stHi3138Allocation[1].enSyncPin)
            {
                /*TS1_SYNC-->LNB_EN1*/
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
                u8Temp &= ~(1 << 5);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
                
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL15, &u8Temp);
                u8Temp &= ~(3 << 2);
                u8Temp |= (2 << 2);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL15, u8Temp);
            }
#if 0
//            /*TS1_DATA1-->SDA_T0,TS1_DATA0-->SCL_T0*/
//            qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
//            u8Temp &= ~(1 << 5);
//            u8Temp |= (1 << 5);
//            qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
//            
//            qam_read_byte(u32TunerPort, HI3138_ADI2C_SEL, &u8Temp);
//            u8Temp &= ~(3 << 6);
//            u8Temp |= (2 << 6);
//            qam_write_byte(u32TunerPort, HI3138_ADI2C_SEL, u8Temp);
//
//            qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
//            u8Temp &= ~(3 << 6);
//            u8Temp |= (2 << 6);
//            qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
#endif
            /*ADDR1、0-->HV_SEL1、HV_SEL0*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL12, &u8Temp);
            u8Temp &= ~(3 << 6);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL12, u8Temp);
        }
        else
        {
            HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",stHi3138Allocation[0].enHi3138SigType,stHi3138Allocation[1].enHi3138SigType);
            return HI_FAILURE;
        }
    }

    /*如果使能，说明该tuner需要开启I2C转发，故需配置IO复用*/
    for(i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    {
        if ((stHi3138Allocation[i].u32TunerPort == u32TunerPort)
            && ( HI_UNF_TUNER_I2C_TRANSMISSION_SEL_TS1_DATAx == stHi3138Allocation[i].enI2CGateWaySel))
        {
            HI_INFO_TUNER("Demod TSx use to I2C Repeat Enable.\n");
            /*TS1_DATA1-->SDA_T0,TS1_DATA0-->SCL_T0*/
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
            u8Buf = u8Temp & 0xF;
            u8Temp = (u8Temp << 1);
            u8Temp &= ~(1 << 5);
            u8Temp |= (1 << 5);
            u8Temp = (u8Temp & 0xF0) | u8Buf;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
            
            qam_read_byte(u32TunerPort, HI3138_ADI2C_SEL, &u8Temp);
            u8Temp &= ~(3 << 6);
            u8Temp |= (2 << 6);
            qam_write_byte(u32TunerPort, HI3138_ADI2C_SEL, u8Temp);

            qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
            u8Buf = u8Temp & 0xF;
            u8Temp = (u8Temp << 1);
            u8Temp &= ~(3 << 6);
            u8Temp |= (2 << 6);
            u8Temp = (u8Temp & 0xF0) | u8Buf;
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_demo_fec_clk_config(HI_U32 u32TunerPort)
{
    HI_U32 u32DemoClkKhz,u32FecClkKhz,u32SdcClkKhz,u32AdcClkKhz;
    HI_U8 u8Data = 0,u8Clk_l,u8Clk_m,u8Clk_h;

    HI_TUNER_CHECKPORT(u32TunerPort);

    if(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        u32DemoClkKhz = 124000;
        u32FecClkKhz = 186000;

        /*关闭盲扫*/
        u8Data = 0;
        qam_write_byte(u32TunerPort, HI3138_S_CBS_CTRL_RDADDR, u8Data);

        u8Clk_l = u32DemoClkKhz & 0xff;
        u8Clk_m = (u32DemoClkKhz >> 8) & 0xff;
        u8Clk_h = (u32DemoClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_S_CLK_DEMO_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_S_CLK_DEMO_M, u8Clk_m);
        qam_write_byte(u32TunerPort, HI3138_S_CLK_DEMO_H, u8Clk_h);

        u8Clk_l = u32FecClkKhz & 0xff;
        u8Clk_m = (u32FecClkKhz >> 8) & 0xff;
        u8Clk_h = (u32FecClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_S_CLK_FEC_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_S_CLK_FEC_M, u8Clk_m);
        qam_write_byte(u32TunerPort, HI3138_S_CLK_FEC_H, u8Clk_h);
        hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = u32DemoClkKhz;
    }
    else if((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
        ||(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        u32DemoClkKhz = 62000;
        u32FecClkKhz = 186000;
        u32SdcClkKhz = 93000;

        u8Clk_l = u32DemoClkKhz & 0xff;
        u8Clk_m = (u32DemoClkKhz >> 8) & 0xff;
        u8Clk_h = (u32DemoClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_T_CLK_DEMO_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_T_CLK_DEMO_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_T_RSTN_CTRL, &u8Data);
        u8Data &= ~(0x3 << 4);
        u8Data |= (u8Clk_h << 4);
        qam_write_byte(u32TunerPort, HI3138_T_RSTN_CTRL, u8Data);

        u8Clk_l = u32FecClkKhz & 0xff;
        u8Clk_m = (u32FecClkKhz >> 8) & 0xff;
        u8Clk_h = (u32FecClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_T_CLK_FEC_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_T_CLK_FEC_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_T_RSTN_CTRL, &u8Data);
        u8Data &= ~(0x3 << 6);
        u8Data |= (u8Clk_h << 6);
        qam_write_byte(u32TunerPort, 0x2e, u8Data);

        u8Clk_l = u32SdcClkKhz & 0xff;
        u8Clk_m = (u32SdcClkKhz >> 8) & 0xff;
        u8Clk_h = (u32SdcClkKhz >> 16) & 0x1;
        qam_write_byte(u32TunerPort, HI3138_T_CLK_SDC_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_T_CLK_SDC_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_T_SDC_CTRL, &u8Data);
        u8Data &= ~(0x1 << 0);
        u8Data |= (u8Clk_h << 0);
        qam_write_byte(u32TunerPort, HI3138_T_SDC_CTRL, u8Data);
    }
    else if((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        ||(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        u32DemoClkKhz = 93000;
        u32FecClkKhz = 93000;
        
        /*ADC时钟配置为31MHz*/
        u32AdcClkKhz = 31000;
        u8Clk_l = u32AdcClkKhz & 0xff;
        u8Clk_m = (u32AdcClkKhz >> 8) & 0xff;
        u8Clk_h = (u32AdcClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_C_FREQ_ADC_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_C_FREQ_ADC_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_C_TR_LOOP_CTRL, &u8Data);
        u8Data &= ~(0x3 << 0);
        u8Data |= (u8Clk_h << 0);
        qam_write_byte(u32TunerPort, HI3138_C_TR_LOOP_CTRL, u8Data);
        
        u8Clk_l = u32DemoClkKhz & 0xff;
        u8Clk_m = (u32DemoClkKhz >> 8) & 0xff;
        u8Clk_h = (u32DemoClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_C_DEMO_FREQ_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_C_DEMO_FREQ_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_C_RSTN_CTRL, &u8Data);
        u8Data &= ~(0x3 << 4);
        u8Data |= (u8Clk_h << 4);
        qam_write_byte(u32TunerPort, HI3138_C_RSTN_CTRL, u8Data);

        u8Clk_l = u32FecClkKhz & 0xff;
        u8Clk_m = (u32FecClkKhz >> 8) & 0xff;
        u8Clk_h = (u32FecClkKhz >> 16) & 0x3;
        qam_write_byte(u32TunerPort, HI3138_C_FEC_FREQ_L, u8Clk_l);
        qam_write_byte(u32TunerPort, HI3138_C_FEC_FREQ_M, u8Clk_m);
        qam_read_byte(u32TunerPort, HI3138_C_RSTN_CTRL, &u8Data);
        u8Data &= ~(0x3 << 6);
        u8Data |= (u8Clk_h << 6);
        qam_write_byte(u32TunerPort, HI3138_C_RSTN_CTRL, u8Data);
    }

    return HI_SUCCESS;
}

static HI_VOID hi3138_set_demodid(Hi3138_INNER_DEMOD_NUMBER enInnerDemodId, HI_U8 u8ADCSwapFlag)
{
    Hi3138_INNER_DEMOD_NUMBER stInnerCnt = HI3138_INNER_DEMOD0;


    if(u8ADCSwapFlag)
    {
        for(stInnerCnt = HI3138_INNER_DEMOD0; stInnerCnt <= enInnerDemodId; stInnerCnt++)
        {
            if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[stInnerCnt].enADCId)
            {
                stHi3138Allocation[stInnerCnt].enInnerDemodId = HI3138_INNER_DEMOD1;
            }
            else
            {
                stHi3138Allocation[stInnerCnt].enInnerDemodId = HI3138_INNER_DEMOD0;
            }
        }
    }
    else
    {
        for(stInnerCnt = HI3138_INNER_DEMOD0;stInnerCnt <= enInnerDemodId; stInnerCnt++)
        {
            if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[stInnerCnt].enADCId)
            {
                stHi3138Allocation[stInnerCnt].enInnerDemodId = HI3138_INNER_DEMOD0;
            }
            else
            {
                stHi3138Allocation[stInnerCnt].enInnerDemodId = HI3138_INNER_DEMOD1;
            }
        }
    }

    return;
}

static HI_S32 hi3138_check_demod_signal_type(HI_U32 u32TunerPort, Hi3138_INNER_DEMOD_NUMBER enInnerDemodId)
{
    Hi3138_INNER_DEMOD_NUMBER stInnerCnt = HI3138_INNER_DEMOD0;
    HI_U8 u8ADCSwapFlag = 0,u8TSSwapFlag = 0;
    HI_U8 i = 0;
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    if(enInnerDemodId >= HI3138_INNER_DEMOD_BUTT)
    {
        HI_ERR_TUNER("max demod number is 2,please check.enInnerDemodId:%d\n",enInnerDemodId);
        return HI_FAILURE;
    }

    HI3138_IPIO_CINFIG(u32TunerPort);

    for(stInnerCnt = HI3138_INNER_DEMOD0; stInnerCnt <= enInnerDemodId; stInnerCnt++)
    {
        for (i=0; i<(sizeof(u32DemodSignalTable)/sizeof(u32DemodSignalTable[0])); i++)
        {
            if((u32DemodSignalTable [i][0] == stHi3138Allocation[stInnerCnt].enADCId)
    			&&( u32DemodSignalTable [i][1] == stHi3138Allocation[stInnerCnt].enHi3138SigType))
            {
                u8ADCSwapFlag += 0;   /*此时说明已匹配上枚举到的正确的模式*/
                HI_INFO_TUNER("match to u32DemodSignalTable[%d]\n", i);
            }
            else if((u32DemodSwapSignalTable [i][0] == stHi3138Allocation[stInnerCnt].enADCId)
    			&&( u32DemodSwapSignalTable [i][1] == stHi3138Allocation[stInnerCnt].enHi3138SigType))
            {
                u8ADCSwapFlag += 1;   /*此时说明已匹配上枚举到的正确的模式*/
                HI_INFO_TUNER("match to u32DemodSwapSignalTable[%d]\n", i);
            }
        }
    }

    hi3138_set_demodid(enInnerDemodId, u8ADCSwapFlag);

    /*防止绑定在一个demod上*/
    if(HI3138_INNER_DEMOD1 == enInnerDemodId)
    {
        if((HI3138_INNER_DEMOD0 == stHi3138Allocation[HI3138_INNER_DEMOD1].enInnerDemodId)
            && (HI3138_INNER_DEMOD0 == stHi3138Allocation[HI3138_INNER_DEMOD0].enInnerDemodId))
        {
            stHi3138Allocation[HI3138_INNER_DEMOD0].enInnerDemodId = HI3138_INNER_DEMOD1;
        }
        else if((HI3138_INNER_DEMOD1 == stHi3138Allocation[HI3138_INNER_DEMOD1].enInnerDemodId)
            && (HI3138_INNER_DEMOD1 == stHi3138Allocation[HI3138_INNER_DEMOD0].enInnerDemodId))
        {
            stHi3138Allocation[HI3138_INNER_DEMOD1].enInnerDemodId = HI3138_INNER_DEMOD0;
        }
    }

    if(((HI_UNF_TUNER_ADC0 == stHi3138Allocation[enInnerDemodId].enADCId) 
        && (HI3138_INNER_DEMOD1 == stHi3138Allocation[enInnerDemodId].enInnerDemodId))
        || ((HI_UNF_TUNER_ADC1 == stHi3138Allocation[enInnerDemodId].enADCId) 
        && (HI3138_INNER_DEMOD0 == stHi3138Allocation[enInnerDemodId].enInnerDemodId)))
    {
        u8ADCSwapFlag = 1;
    }

    if(u8ADCSwapFlag)
    {
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 0);
        u8Temp |= 1;
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

        /*demod0的DISEQC0_IN由DISEQC1_IN替换*/
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 2);
        u8Temp |= (1 << 2);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

        /*demod0的DISEQC0_OUT由DISEQC1_OUT替换*/
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 3);
        u8Temp |= (1 << 3);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
    }
    else
    {
        /*ADC输入复原，不交换*/
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 0);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

        /*DISEQC0_IN输入复原，不交换*/
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 2);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);

        /*DISEQC1_IN输入复原，不交换*/
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 3);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
    }
    //HI_INFO_TUNER("-->IN--hi3138_check_demod_signal_type, HI3138_IO_CTRL13:0x%x\n", u8Temp);

    if(((HI_UNF_TUNER_TS0_OUTPUT == stHi3138Allocation[enInnerDemodId].enTSOutputId) && (HI3138_INNER_DEMOD1 == stHi3138Allocation[enInnerDemodId].enInnerDemodId))
        || ((HI_UNF_TUNER_TS1_OUTPUT == stHi3138Allocation[enInnerDemodId].enTSOutputId) && (HI3138_INNER_DEMOD0 == stHi3138Allocation[enInnerDemodId].enInnerDemodId)))
    {
        u8TSSwapFlag = 1;
    }

    if(HI_TRUE == stHi3138Allocation[enInnerDemodId].bTSCombFlag)
    {
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
        u8Temp &= ~(1 << 1);
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
    }
    else
    {
        if(u8TSSwapFlag)
        {
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 1);
            u8Temp |= (1 << 1);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
        }
        else
        {
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 1);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
        }
    }
    //HI_INFO_TUNER("-->IN--hi3138_check_demod_signal_type, HI3138_IO_CTRL13:0x%x\n", u8Temp);

    return HI_SUCCESS;
}

static HI_S32 hi3138_open_demod(HI_U32 u32TunerPort, Hi3138_SIGNAL_INFO_ALLOCATION *pstDemodAttr)
{
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI3138_IPIO_CINFIG(u32TunerPort);
    
    if ((pstDemodAttr[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&     /*仅打开demod0,仅支持S+T*/
        (pstDemodAttr[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        qam_read_byte(u32TunerPort, HI3138_CHN_ON, &u8Temp);
        u8Temp &= 0xE0;                                                                  /*所有解调模块关闭*/
        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);
        
        if (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2)
        {
            u8Temp |= 0x01;
        }
        else if (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T)
        {
            u8Temp |= 0x01;
        }
        else if (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            if(HI3138_INNER_DEMOD0 == pstDemodAttr[0].enInnerDemodId)
            {
                u8Temp |= 0x02;
            }
            else
            {
                u8Temp |= 0x08;
            }
        }
        else if (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)
        {
            u8Temp |= 0x14;
        }
        else if (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)
        {
            u8Temp |= 0x04;
        }
        else
        {
            HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",pstDemodAttr[0].enHi3138SigType,pstDemodAttr[1].enHi3138SigType);
            return HI_FAILURE;
        }
        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);
    }
    #if 0
//    else if ((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_BUTT) &&     /*仅打开demod1,仅支持S+C*/
//        (pstDemodAttr[1].enHi3138SigType!= HI_UNF_TUNER_SIG_TYPE_BUTT))
//    {
//        qam_read_byte(u32TunerPort, HI3138_CHN_ON, &u8Temp);
//        u8Temp &= 0xE0;                                                                  /*所有解调模块关闭*/
//        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);
//
//        if (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)
//        {
//            u8Temp |= 0x04;
//        }
//        else if (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)
//        {
//            u8Temp |= 0x14;
//        }
//        else if (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)
//        {
//            u8Temp |= 0x08;
//        }
//        else
//        {
//            HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",pstDemodAttr[0].enHi3138SigType,pstDemodAttr[1].enHi3138SigType);
//            return HI_FAILURE;
//        }
//        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);
//    }
    #endif
    else if ((pstDemodAttr[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&
        (pstDemodAttr[1].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        qam_read_byte(u32TunerPort, HI3138_CHN_ON, &u8Temp);
        u8Temp &= 0xE0;                                                                  /*所有解调模块关闭*/
        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);

        if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&         /*T+C*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&         /*T+C*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            u8Temp |= 0x15;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+J83B*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+J83B*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            u8Temp |= 0x05;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+C*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+C*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            u8Temp |= 0x15;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+J83B*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+J83B*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            u8Temp |= 0x05;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            u8Temp |= 0x09;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            u8Temp |= 0x09;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            u8Temp |= 0x16;
        }
        else if (((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            u8Temp |= 0x06;
        }
        else if ((pstDemodAttr[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+S1*/
            (pstDemodAttr[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
        {
            u8Temp |= 0x0A;
        }
        else
        {
            HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",pstDemodAttr[0].enHi3138SigType,pstDemodAttr[1].enHi3138SigType);
            return HI_FAILURE;
        }
        qam_write_byte(u32TunerPort, HI3138_CHN_ON, u8Temp);
    }
    else
    {
        HI_ERR_TUNER("config demod failed,the current pu8DemodSignal[%d,%d]\n",pstDemodAttr[0].enHi3138SigType,pstDemodAttr[1].enHi3138SigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_INFO_TUNER("u32TunerPort:%d, enI2cChannel:%d, enTunerDevType:%d\n",u32TunerPort, enI2cChannel, enTunerDevType);
    
    HI3138_IPIO_CINFIG(u32TunerPort);

    u8Temp = 0xDD;
    qam_write_byte(u32TunerPort, HI3138_IO_CTRL2, u8Temp);
    u8Temp = 0xDD;
    qam_write_byte(u32TunerPort, HI3138_IO_CTRL6, u8Temp);
    
    return HI_SUCCESS;
}

static HI_S32 hi3138_config_ext_attr(HI_U32 u32TunerPort)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8InitData = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_INFO_TUNER("u32TunerPort:%d\n",u32TunerPort);
    
    /*初始化对应的模块驱动前，选择I2C子通道号*/
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }
    
    /*根据信号类型，初始化对应的驱动模块*/
    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {

    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        //agc goal
        u8InitData = 0x1C;
        qam_write_byte(u32TunerPort,HI3138_T_AGC_GOAL,u8InitData);
        
        //dagc goal
        u8InitData = 0x54;
        qam_write_byte(u32TunerPort,HI3138_T_DAGC_REF,u8InitData);
        
        //disable cp_add_ena
        qam_read_byte(u32TunerPort, HI3138_T_TS_0_SEL, &u8InitData);
        u8InitData = u8InitData & 0x0F;
        qam_write_byte(u32TunerPort,HI3138_T_TS_0_SEL,u8InitData);
        
        //c_max_thmid (39db)
        qam_read_byte(u32TunerPort, HI3138_T_MAX_TH, &u8InitData);
        u8InitData = (u8InitData & 0xF9) | 0x02;
        qam_write_byte(u32TunerPort,HI3138_T_MAX_TH,u8InitData);

        //cir threshold (thres high /medium)
        u8InitData = 0x20;						
        qam_write_byte(u32TunerPort,HI3138_T_ECHO_THRES, u8InitData);
        
        //thres low
        qam_read_byte(u32TunerPort, HI3138_T_ECHO_THL, &u8InitData);
        u8InitData = (u8InitData & 0xF0) | 0x04;
        qam_write_byte(u32TunerPort,HI3138_T_ECHO_THL,u8InitData);
        
         //c_th_min
        u8InitData = 0x20;
        qam_write_byte(u32TunerPort,HI3138_T_MIN_THRES, u8InitData);
        
        //soac check threshold
        qam_read_byte(u32TunerPort,HI3138_T_SOAC_TH, &u8InitData);
        u8InitData = ((u8InitData & 0xF0) | 0x06);
        qam_write_byte(u32TunerPort,HI3138_T_SOAC_TH, u8InitData);

        //c_p2_pos_mod (for t2 synchronization)
        u8InitData = 0x90;
        qam_write_byte(u32TunerPort,HI3138_T_P2_POS_MOD, u8InitData);
        
        //c_cpe_ena  (for cpe of dvbt2)
        qam_read_byte(u32TunerPort,HI3138_T_CPE, &u8InitData);
        u8InitData = (u8InitData & 0xE7) | (0x08);
        qam_write_byte(u32TunerPort,HI3138_T_CPE, u8InitData);

        //init dop_th (for dynamic echo check)
        qam_read_byte(u32TunerPort, HI3138_T_FD_GRP, &u8InitData);
        u8InitData = (u8InitData & 0xF0) | (0x1A); 
        qam_write_byte(u32TunerPort,HI3138_T_FD_GRP, u8InitData);

        //c_echo_th (for isi check)
        qam_read_byte(u32TunerPort,HI3138_T_ECHO_ISI, &u8InitData);
        u8InitData = (u8InitData & 0xF0) | 0x08;
        qam_write_byte(u32TunerPort,HI3138_T_ECHO_ISI, u8InitData);

        //L1 post cfg 
        u8InitData = 0x40;
        qam_write_byte(u32TunerPort,HI3138_T_SWITCH_DLY, u8InitData);
        u8InitData = 0x08;
        qam_write_byte(u32TunerPort,HI3138_T_AUTO_DLY, u8InitData);

        //high err rst
        qam_read_byte(u32TunerPort,HI3138_T_BER_CTRL, &u8InitData);
        u8InitData = (u8InitData & 0xF0) | 0x0F;
        qam_write_byte(u32TunerPort,HI3138_T_BER_CTRL, u8InitData);

        //iter number
        qam_read_byte(u32TunerPort,HI3138_T_ITER_CTRL, &u8InitData);
        u8InitData = (u8InitData & 0xC0) | 0x2C; //44
        qam_write_byte(u32TunerPort,HI3138_T_ITER_CTRL, u8InitData);

        qam_read_byte(u32TunerPort,HI3138_T_ITER_NUM, &u8InitData);
        u8InitData = (u8InitData & 0xC0) | 0x18; //24
        qam_write_byte(u32TunerPort,HI3138_T_ITER_NUM, u8InitData);

        qam_read_byte(u32TunerPort,HI3138_T_ITER_NUM_POST, &u8InitData);
        u8InitData = (u8InitData & 0xC0) | 0x20; //32
        qam_write_byte(u32TunerPort,HI3138_T_ITER_NUM_POST, u8InitData);

        /*在Multi PLP模式下，数据突发比较严重，
        输出模块上电后的默认配置存在数据溢出的风险*/
        qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL0, 0x5a);
        qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL1, 0x71);
        qam_write_byte(u32TunerPort,Hi3137_OUTP_PLL2, 0x20);

        qam_read_byte(u32TunerPort, Hi3137_OUTP_PLL4, &u8InitData);
        u8InitData &=0x7;
        qam_write_byte(u32TunerPort, Hi3137_OUTP_PLL4, u8InitData);

        qam_read_byte(u32TunerPort, Hi3137_OUTP_MODE_SET, &u8InitData);
        u8InitData &=0xc1;
        u8InitData |= 0xb<<1;
        qam_write_byte(u32TunerPort, Hi3137_OUTP_MODE_SET, u8InitData);

    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        /*CAB参数配置*/
        qam_read_byte(u32TunerPort, HI3138_C_TR_CR_CTRL, &u8InitData);
        u8InitData &= ~(1<<2);
        qam_write_byte(u32TunerPort, HI3138_C_TR_CR_CTRL, u8InitData);
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_set_demod_attr is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;

}
    
HI_S32 hi3138_set_multi_mode_chip_attr(HI_U32 u32TunerPort,HI_UNF_TUNER_MULTI_MODE_CHIP_ATTR_S *enPortAttr)
{
    HI_S32 s32Ret;
    HI_U32 i,j;
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(enPortAttr);

    /*保存配置的信号类型*/
    for(i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    {
    	if(HI_UNF_TUNER_SIG_TYPE_BUTT == stHi3138Allocation[i].enHi3138SigType)
    	{
            stHi3138Allocation[i].u32TunerPort= u32TunerPort;
            stHi3138Allocation[i].enHi3138SigType= g_stTunerOps[u32TunerPort].enSigType;
            stHi3138Allocation[i].enADCId = enPortAttr->enADCPort;
            stHi3138Allocation[i].enTSOutputId= enPortAttr->enTSOutputPort;
            stHi3138Allocation[i].enIFPath = enPortAttr->enIFPath;
            stHi3138Allocation[i].enI2CGateWaySel = enPortAttr->enI2CGateWaySel;
            stHi3138Allocation[i].enSyncPin = enPortAttr->enSyncPin;
            for(j = 0;j < MAX_TS_LINE;j++)
            {
                stHi3138Allocation[i].stTSOut.enTSOutput[j] = j;
            }
            #if 0
    		switch(stHi3138Allocation[i].enADCId)
    		{
    			case HI_UNF_TUNER_ADC0:
    				stHi3138Allocation[i].enInnerDemodId = HI3138_INNER_DEMOD0;
    				break;
    			case HI_UNF_TUNER_ADC1:
    				stHi3138Allocation[i].enInnerDemodId = HI3138_INNER_DEMOD1;
    				break;
    			default:
    				break;
    		}
            #endif
    		break;
    	}
    }

    /*判断是否配置下来的信号类型符合要求*/  
    s32Ret = hi3138_check_demod_signal_type(u32TunerPort, i);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_checkout_signal_type failed.\n");
        return s32Ret;
    }

#if 0
    //for(i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    //{
    //    HI_INFO_TUNER("stHi3138Allocation[%d].enInnerDemodId:%d,"
    //                            "stHi3138Allocation[%d].enTSOutputId:%d,"
    //                            "stHi3138Allocation[%d].enTsType:%d\n",
    //                            i,stHi3138Allocation[i].enInnerDemodId,
    //                            i,stHi3138Allocation[i].enTSOutputId,
    //                            i,stHi3138Allocation[i].enTsType);
    //}
#endif

    s32Ret = hi3138_single_stream_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("hi3138_single_stream_config err.\n");
        return s32Ret;
    }
    
    /*配置TS流的输出是否需要交换*/
    for (i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    {
        if ((stHi3138Allocation[i].u32TunerPort == u32TunerPort) && (HI_FALSE == stHi3138Allocation[i].bTSCombFlag))
        {
            /*以下两种情况需要TS交换*/
            if((HI3138_INNER_DEMOD0 == stHi3138Allocation[i].enInnerDemodId) 
                && (HI_UNF_TUNER_TS1_OUTPUT == stHi3138Allocation[i].enTSOutputId))
            {
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
                u8Temp &= ~(1 << 1);
                u8Temp |= 1 << 1;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            }
            else if((HI3138_INNER_DEMOD1 == stHi3138Allocation[i].enInnerDemodId) 
                && (HI_UNF_TUNER_TS0_OUTPUT == stHi3138Allocation[i].enTSOutputId))
            {
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
                u8Temp &= ~(1 << 1);
                u8Temp |= 1 << 1;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            }
            else
            {
                qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
                u8Temp &= ~(1 << 1);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
            }
        }
        else
        {
            qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Temp);
            u8Temp &= ~(1 << 1);
            qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Temp);
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
    Hi3138_INNER_DEMOD_NUMBER stInnerDemod = HI3138_INNER_DEMOD0;
    HI_S32 s32Ret;
    HI_U32 u32DemodClkKhz = 0,i;
    HI_U8 u8Temp = 0;
    
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstSatTunerAttr);

#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        hi3136_cfg[u32TunerPort].u16TunerLPF_100Khz = pstSatTunerAttr->u16TunerMaxLPF * 10;
        hi3136_cfg[u32TunerPort].enIQSpectrum = pstSatTunerAttr->enIQSpectrum; 
    }

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);

    stHi3138Allocation[i].enHi3138SigType = g_stTunerOps[u32TunerPort].enSigType;
    /*check whether the other demod is working*/
    if(HI_UNF_TUNER_SIG_TYPE_BUTT == stHi3138Allocation[(i+1)%2].enHi3138SigType)
    {
        stInnerDemod = HI3138_INNER_DEMOD0;
    }
    else
    {
        stInnerDemod = HI3138_INNER_DEMOD1;
    } 
    s32Ret = hi3138_check_demod_signal_type(u32TunerPort, stInnerDemod);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_checkout_signal_type failed.\n");
        return s32Ret;
    }

    /*打开需要解调的模块*/
    s32Ret = hi3138_open_demod(u32TunerPort, stHi3138Allocation);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_open_demod failed.\n");
        return s32Ret;
    }

    /*配置时钟模块*/
    u32DemodClkKhz = pstSatTunerAttr->u32DemodClk;
    s32Ret = hi3138_pll_config(u32TunerPort, u32DemodClkKhz);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_pll_config failed.\n");
        return s32Ret;
    }

    /*配置ADC模块*/
    s32Ret = hi3138_adc_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_adc_config failed.\n");
        return s32Ret;
    }
    
    /*配置IO复用关系*/
    s32Ret = hi3138_io_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_io_configuration failed.\n");
        return s32Ret;
    }

    /*配置LNB电路受ADDR0还是ADDR1控制*/
    s32Ret = hi3138_lnb_link(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_lnb_link failed.\n");
        return s32Ret;
    }

    s32Ret = hi3138_set_ts_port_hiz(u32TunerPort, pstSatTunerAttr->enTSSerialPIN);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_set_ts_port_hiz failed.\n");
        return s32Ret;
    }

    /*HI3138_CRG_CTRL1[6,5] 1:串行1 bit且为C/S模式，0:其它*/
    if (g_stTunerOps[u32TunerPort].enTsType == HI_UNF_TUNER_OUTPUT_MODE_SERIAL)
    {
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Temp);
        switch(stHi3138Allocation[i].enInnerDemodId)
        {
            case HI3138_INNER_DEMOD0:
                u8Temp |= (1<<5);
                break;
            case HI3138_INNER_DEMOD1:
                u8Temp |= (1<<6);
                break;
            default:
                break;
        }
        HI_INFO_TUNER("-->IN--hi3138_set_sat_attr,stHi3138Allocation[%d].enInnerDemodId:%d,HI3138_CRG_CTRL1:0x%x\n",
                                i,stHi3138Allocation[i].enInnerDemodId,u8Temp);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Temp);
    }
    else
    {
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Temp);
        switch(stHi3138Allocation[i].enInnerDemodId)
        {
            case HI3138_INNER_DEMOD0:
                u8Temp &= ~(1<<5);
                break;
            case HI3138_INNER_DEMOD1:
                u8Temp &= ~(1<<6);
                break;
            default:
                break;
        }
        #if 0
        HI_INFO_TUNER("-->IN--hi3138_set_sat_attr,stHi3138Allocation[%d].enInnerDemodId:%d,HI3138_CRG_CTRL1:0x%x\n",
                                i,stHi3138Allocation[i].enInnerDemodId,u8Temp);
        #endif
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Temp);
    }
    
    /*初始化对应的模块驱动前，选择I2C子通道号*/
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3138_demo_fec_clk_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_demo_fec_clk_config failed.\n");
        return s32Ret;
    }
    
    //修改agc_inverse
    switch (pstSatTunerAttr->enRFAGC)
    {
        case HI_UNF_TUNER_RFAGC_INVERT:
        default:
            qam_write_bit(u32TunerPort, AGC_CTRL, 1, 1);
            break;
        case HI_UNF_TUNER_RFAGC_NORMAL:
            qam_write_bit(u32TunerPort, AGC_CTRL, 1, 0);
            break;
    }

    /*DVBS/S2时，0xf5[5]写0，使能vld信号输出*/
    if (g_stTunerOps[u32TunerPort].enTsType == HI_UNF_TUNER_OUTPUT_MODE_SERIAL)
    {
        qam_write_bit(u32TunerPort, 0xf5, 5, 0);

        qam_write_bit(u32TunerPort, HI3138_S_TS_CTRL0, 3, 1);
    }
    
    /*根据信号类型，初始化对应的tuner*/
    s32Ret = hi3138_tuner_init(u32TunerPort, g_stTunerOps[u32TunerPort].enTunerDevType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_tuner_init failed.\n");
        return s32Ret;
    }

    /*对所选解调模块做一次热复位*/
    s32Ret = hi3138_hot_reset(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_hot_reset failed.\n");
        return s32Ret;
    }

    if(HI_FALSE == stHi3138Allocation[i].bTSCombFlag)
    {
        /*设置TS的输出类型*/
        /*TS合并时，内部TS输出类型必须配置为并行*/
        s32Ret = hi3136_set_ts_type(u32TunerPort, stHi3138Allocation[i].enTsType);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_set_ts_type failed.\n");
            return s32Ret;
        }

        s32Ret = hi3136_set_ts_out(u32TunerPort , &(stHi3138Allocation[i].stTSOut));
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_set_ts_out failed.\n");
            return s32Ret;
        }
    }

#ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3138_set_sat_attr time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
#endif

    return HI_SUCCESS;
}

HI_S32 hi3138_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
    Hi3138_INNER_DEMOD_NUMBER stInnerDemod = HI3138_INNER_DEMOD0;
    HI_S32 s32Ret;
    HI_U32 u32DemodClkKhz = 0,i;
    HI_U8 u8Temp = 0;
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstTerTunerAttr);

#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);

    stHi3138Allocation[i].enHi3138SigType = g_stTunerOps[u32TunerPort].enSigType;
    /*check whether the other demod is working*/
    if(HI_UNF_TUNER_SIG_TYPE_BUTT == stHi3138Allocation[(i+1)%2].enHi3138SigType)
    {
        stInnerDemod = HI3138_INNER_DEMOD0;
    }
    else
    {
        stInnerDemod = HI3138_INNER_DEMOD1;
    } 
    s32Ret = hi3138_check_demod_signal_type(u32TunerPort, stInnerDemod);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_checkout_signal_type failed.\n");
        return s32Ret;
    }

    /*打开需要解调的模块*/
    s32Ret = hi3138_open_demod(u32TunerPort, stHi3138Allocation);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_open_demod failed.\n");
        return s32Ret;
    }

    /*配置时钟模块*/
    u32DemodClkKhz = pstTerTunerAttr->u32DemodClk;
    s32Ret = hi3138_pll_config(u32TunerPort, u32DemodClkKhz);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_pll_config failed.\n");
        return s32Ret;
    }

    /*配置ADC模块*/
    s32Ret = hi3138_adc_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_adc_config failed.\n");
        return s32Ret;
    }
    
    /*配置IO复用关系*/
    s32Ret = hi3138_io_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_io_configuration failed.\n");
        return s32Ret;
    }
    
    /*配置LNB电路受ADDR0还是ADDR1控制*/
    s32Ret = hi3138_lnb_link(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_lnb_link failed.\n");
        return s32Ret;
    }

    /*将TS流不用的数据管脚配置成高阻*/
    s32Ret = hi3138_set_ts_port_hiz(u32TunerPort, pstTerTunerAttr->enTSSerialPIN);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_set_ts_port_hiz failed.\n");
        return s32Ret;
    }
    
    /*初始化对应的模块驱动前，选择I2C子通道号*/
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    /*配置中频*/
    s32Ret = hi3138_if_freq_set(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_if_freq_set failed.\n");
        return s32Ret;
    }

    s32Ret = hi3138_demo_fec_clk_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_demo_fec_clk_config failed.\n");
        return s32Ret;
    }

    /*HI3138在DVB-T/T2/ISDB-T模式下，做额外的初始化配置*/
    s32Ret = hi3138_config_ext_attr(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_config_ext_attr failed.\n");
        return s32Ret;
    }
    
    /*根据信号类型，初始化对应的tuner*/
    s32Ret = hi3138_tuner_init(u32TunerPort, g_stTunerOps[u32TunerPort].enTunerDevType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_tuner_init failed.\n");
        return s32Ret;
    }

    /*对所选解调模块做一次热复位*/
    s32Ret = hi3138_hot_reset(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_hot_reset failed.\n");
        return s32Ret;
    }

    /*设置TS的输出类型*/
     HI_INFO_TUNER("stHi3138Allocation[%d].enTsType:%d\n",i,stHi3138Allocation[i].enTsType);
    s32Ret = hi3137_set_ts_type(u32TunerPort, stHi3138Allocation[i].enTsType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_set_ts_type failed.\n");
        return s32Ret;
    }

    /*在串行1bit和2bit的时候，T/T2/ISDB-T模式下0xAC寄存器msb_1st默认配置为高位优先。
    因此，默认是从TS的DATA6和DATA7出来。
    但是，T/T2/ISDB-T模式下0x7d寄存器默认值为0xDB，此时TS_DATA[7,2]是输出的，TS_DATA[1,0]
    是高阻的；假若0x7d = 0x9B，此时TS_DATA[5,0]是输出的，TS_DATA[7,6]是高阻的。*/
    if  ((HI_UNF_TUNER_OUTPUT_MODE_SERIAL == stHi3138Allocation[i].enTsType)
        || (HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT == stHi3138Allocation[i].enTsType))
    {
        if (HI_UNF_TUNER_TS_SERIAL_PIN_0 == pstTerTunerAttr->enTSSerialPIN)
        {
            qam_read_byte(u32TunerPort, HI3138_T_CEQ_ADDR_1D, &u8Temp);
            u8Temp &= ~(1 << 6);
            qam_write_byte(u32TunerPort, HI3138_T_CEQ_ADDR_1D, u8Temp);  /*打开TS_DATA0-TS_DATA5*/
        }
        /*HI_UNF_TUNER_TS_SERIAL_PIN_7*/
        else if (HI_UNF_TUNER_TS_SERIAL_PIN_7 == pstTerTunerAttr->enTSSerialPIN)
        {
            qam_read_byte(u32TunerPort, HI3138_T_CEQ_ADDR_1D, &u8Temp);
            u8Temp |= (1 << 6);
            qam_write_byte(u32TunerPort, HI3138_T_CEQ_ADDR_1D, u8Temp);  /*默认打开TS_DATA2-TS_DATA7*/
        }
        else
        {
            HI_WARN_TUNER("hi3138_set_ter_attr,pstTerTunerAttr->enTSSerialPIN:%d is error.\n", pstTerTunerAttr->enTSSerialPIN);
        }
         HI_INFO_TUNER("hi3138_set_ter_attr,pstTerTunerAttr->enTSSerialPIN:%d,HI3138_T_CEQ_ADDR_1D=0x%x\n", 
                                    pstTerTunerAttr->enTSSerialPIN, u8Temp);
    }

    s32Ret = hi3137_set_ts_out(u32TunerPort , &(stHi3138Allocation[i].stTSOut));
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_set_ts_type failed.\n");
        return s32Ret;
    }

#ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3138_set_ter_attr time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
#endif

    return HI_SUCCESS;
}

HI_S32 hi3138_set_cab_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_CAB_ATTR_S *pstCabTunerAttr)
{
    Hi3138_INNER_DEMOD_NUMBER stInnerDemod = HI3138_INNER_DEMOD0;
    HI_S32 s32Ret;
    HI_U32 u32DemodClkKhz = 0,i = 0;
    HI_U8 u8Temp = 0;
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstCabTunerAttr);

#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);
    
    stHi3138Allocation[i].enHi3138SigType = g_stTunerOps[u32TunerPort].enSigType;
    /*check whether the other demod is working*/
    if(HI_UNF_TUNER_SIG_TYPE_BUTT == stHi3138Allocation[(i+1)%2].enHi3138SigType)
    {
        stInnerDemod = HI3138_INNER_DEMOD0;
    }
    else
    {
        stInnerDemod = HI3138_INNER_DEMOD1;
    } 
    s32Ret = hi3138_check_demod_signal_type(u32TunerPort, stInnerDemod);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_checkout_signal_type failed.\n");
        return s32Ret;
    }

    /*打开需要解调的模块*/
    s32Ret = hi3138_open_demod(u32TunerPort, stHi3138Allocation);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_open_demod failed.\n");
        return s32Ret;
    }

    /*配置时钟模块*/
    u32DemodClkKhz = pstCabTunerAttr->u32DemodClk;
    s32Ret = hi3138_pll_config(u32TunerPort, u32DemodClkKhz);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_pll_config failed.\n");
        return s32Ret;
    }

    /*配置ADC模块*/
    s32Ret = hi3138_adc_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_adc_config failed.\n");
        return s32Ret;
    }
    
    /*配置IO复用关系*/
    s32Ret = hi3138_io_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_io_configuration failed.\n");
        return s32Ret;
    }
    
    /*配置LNB电路受ADDR0还是ADDR1控制*/
    s32Ret = hi3138_lnb_link(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_lnb_link failed.\n");
        return s32Ret;
    }

    /*将TS流不用的数据管脚配置成高阻*/
    s32Ret = hi3138_set_ts_port_hiz(u32TunerPort, pstCabTunerAttr->enTSSerialPIN);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_set_ts_port_hiz failed.\n");
        return s32Ret;
    }

    /*HI3138_CRG_CTRL1[6,5] 1:串行1 bit且为C/S模式，0:其它*/
    if (g_stTunerOps[u32TunerPort].enTsType == HI_UNF_TUNER_OUTPUT_MODE_SERIAL)
    {
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Temp);
        switch(stHi3138Allocation[i].enInnerDemodId)
        {
            case HI3138_INNER_DEMOD0:
                u8Temp |= (1<<5);
                break;
            case HI3138_INNER_DEMOD1:
                u8Temp |= (1<<6);
                break;
            default:
                break;
        }
        //HI_INFO_TUNER("-->IN--hi3138_set_cab_attr,HI3138_CRG_CTRL1:%d\n",HI3138_CRG_CTRL1);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Temp);
    }
    else
    {
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Temp);
        switch(stHi3138Allocation[i].enInnerDemodId)
        {
            case HI3138_INNER_DEMOD0:
                u8Temp &= ~(1<<5);
                break;
            case HI3138_INNER_DEMOD1:
                u8Temp &= ~(1<<6);
                break;
            default:
                break;
        }
        HI_INFO_TUNER("-->IN--hi3138_set_sat_attr,stHi3138Allocation[%d].enInnerDemodId:%d,HI3138_CRG_CTRL1:0x%x\n",
                                i,stHi3138Allocation[i].enInnerDemodId,u8Temp);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Temp);
    }
    
    /*初始化对应的模块驱动前，选择I2C子通道号*/
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    /*配置中频*/
    s32Ret = hi3138_if_freq_set(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_if_freq_set failed.\n");
        return s32Ret;
    }

    s32Ret = hi3138_demo_fec_clk_config(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_demo_fec_clk_config failed.\n");
        return s32Ret;
    }

    /*HI3138在DVB-C/J83B模式下，做额外的初始化配置*/
    s32Ret = hi3138_config_ext_attr(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_config_ext_attr failed.\n");
        return s32Ret;
    }
    
    /*根据信号类型，初始化对应的tuner*/
    s32Ret = hi3138_tuner_init(u32TunerPort, g_stTunerOps[u32TunerPort].enTunerDevType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_tuner_init failed.\n");
        return s32Ret;
    }

    /*对所选解调模块做一次热复位*/
    s32Ret = hi3138_hot_reset(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_hot_reset failed.\n");
        return s32Ret;
    }

    /*设置TS的输出类型*/
    if(HI_FALSE == stHi3138Allocation[i].bTSCombFlag)
    {
        /*TS合并时，内部TS输出类型必须配置为并行*/
        qam_read_byte(u32TunerPort, HI3138_C_OUTP_TS_MODE, &u8Temp);
        //HI_INFO_TUNER("stHi3138Allocation[%d].enTsType:%d\n", i, stHi3138Allocation[i].enTsType);
        switch (stHi3138Allocation[i].enTsType)
        {
            case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
                u8Temp = u8Temp & 0xFC;
                break;
            case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT:
                u8Temp = u8Temp & 0xFE;
                u8Temp = u8Temp | 0x06;
                break;
            case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
            default:
                u8Temp = u8Temp | 0x01;
                break;
        }

        //数据无效时，屏蔽时钟
        u8Temp = u8Temp | 0x10;

        qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_MODE, u8Temp);

        s32Ret = hiDVBC_set_ts_out(u32TunerPort , &(stHi3138Allocation[i].stTSOut));
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_set_ts_out failed.\n");
            return s32Ret;
        }
    }

#ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3138_set_cab_attr time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
#endif

    return HI_SUCCESS;
}

HI_S32 hi3138_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_S32  s32Ret = HI_SUCCESS;
#ifdef TIME_CONSUMING
    struct timeval stStartTime,stEndTime;
#endif

    HI_TUNER_CHECKPORT(u32TunerPort);
 
    HI_TUNER_CHECKPOINTER(pstChannel);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }
        
#ifdef TIME_CONSUMING
    do_gettimeofday(&stStartTime);
#endif
    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_connect(u32TunerPort, pstChannel);
        if(HI_SUCCESS != s32Ret)
        {
            #ifdef TIME_CONSUMING
            do_gettimeofday(&stEndTime);
            HI_INFO_TUNER("hi3138_connect time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
            #endif
            HI_ERR_TUNER("-->IN--hi3136_connect failed.\n");
            return s32Ret;
        }
        
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2))
    {
        s32Ret = hi3137_connect(u32TunerPort, pstChannel);
        if(HI_SUCCESS != s32Ret)
        {
            #ifdef TIME_CONSUMING
            do_gettimeofday(&stEndTime);
            HI_INFO_TUNER("hi3138_connect time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
            #endif
            HI_ERR_TUNER("-->IN--hi3137_connect failed.\n");
            return s32Ret;
        }
    }
    else if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T)
    {
        s32Ret = hi3138_tuner_set(u32TunerPort,pstChannel->u32Frequency,pstChannel->unSRBW.u32BandWidth);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_tuner_set failed.\n");
            return s32Ret;
        }

        tuner_mdelay(20);
        
        s32Ret = hiISDBT_connect(u32TunerPort, pstChannel);
        if(HI_SUCCESS != s32Ret)
        {
            #ifdef TIME_CONSUMING
            do_gettimeofday(&stEndTime);
            HI_INFO_TUNER("hi3138_connect time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
            #endif
            HI_ERR_TUNER("-->IN--hi3137_connect failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    { 
        s32Ret = hi3138_tuner_set(u32TunerPort,pstChannel->u32Frequency,pstChannel->unSRBW.u32BandWidth);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_tuner_set failed.\n");
            return s32Ret;
        }

        tuner_mdelay(20);
        s32Ret = hiDVBC_connect(u32TunerPort, pstChannel);
        if(HI_SUCCESS != s32Ret)
        {
            #ifdef TIME_CONSUMING
            do_gettimeofday(&stEndTime);
            HI_INFO_TUNER("hi3138_connect time used:[%d(mSec)]\n", (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
            #endif
            HI_ERR_TUNER("-->IN--hiDVBC_connect failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_connect is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }

#ifdef TIME_CONSUMING
    do_gettimeofday(&stEndTime);
    HI_INFO_TUNER("hi3138_connect time used(signal:%d):[%d(mSec)]\n", g_stTunerOps[u32TunerPort].enSigType,  (HI_U32)(stEndTime.tv_sec*1000 + stEndTime.tv_usec/1000) - (HI_U32)(stStartTime.tv_sec*1000 + stStartTime.tv_usec/1000));
#endif

    return HI_SUCCESS;
}

HI_VOID hi3138_connect_timeout(HI_U32 u32ConnectTimeout)
{
    return;
}

HI_S32 hi3138_deinit(HI_VOID)
{
    HI_U8 i = 0,j = 0;

    HI_INFO_TUNER("-->IN--hi3138_deinit sizeof(stHi3138Allocation):%d\n",sizeof(stHi3138Allocation));
    memset(&stHi3138Allocation,0,sizeof(stHi3138Allocation));
    gs_u8InnerCnt = 0;

    for(i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    {
        stHi3138Allocation[i].enInnerDemodId= i;
        stHi3138Allocation[i].enHi3138SigType = HI_UNF_TUNER_SIG_TYPE_BUTT;
        stHi3138Allocation[i].enADCId = i;
        stHi3138Allocation[i].enTSOutputId= i;
        stHi3138Allocation[i].bTSCombFlag = HI_FALSE;
        for(j = 0;j < MAX_TS_LINE;j++)
        {
            stHi3138Allocation[i].stTSOut.enTSOutput[j] = j;
        }
    }

    //hi3138_set_separate_ts_comb();
    
    return HI_SUCCESS;
}

static HI_S32 hi3138_demod0_wakeup(HI_U32 u32TunerPort, HI_U8 u8AdcSwapFlag)
{
    HI_U8 u8Temp = 0;
    HI_S32 s32Ret;


    if (u8AdcSwapFlag)
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {        
             u8Temp = 0xff;
             qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                     (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
        {
             u8Temp = 0xff;
             qam_write_byte(u32TunerPort,HI3138_T_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
    }
    /*ADC没有交换的情况*/
    else
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            u8Temp = 0xff;
            qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
            
            HI3138_IPIO_CINFIG(u32TunerPort);
            qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= (2 << 0);
            qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                     (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
        {
            u8Temp = 0xff;
            qam_write_byte(u32TunerPort,HI3138_T_MAN_RST_CTRL0,u8Temp);
            
            HI3138_IPIO_CINFIG(u32TunerPort);
            qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
            u8Temp &= ~(3 << 0);
            u8Temp |= (1 << 0);
            qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }

        /*重新配置ADC模块*/
        s32Ret = hi3138_adc_config(u32TunerPort);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_adc_config failed.\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_demod0_sleep(HI_U32 u32TunerPort, HI_U8 u8AdcSwapFlag)
{
    HI_U8 u8Temp = 0;


    if (u8AdcSwapFlag)
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {        
             u8Temp = 0;
             qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                     (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
        {
             u8Temp = 0;
             qam_write_byte(u32TunerPort,HI3138_T_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
    }
    /*ADC没有交换的情况*/
    else
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            u8Temp = 0;
            qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                     (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
        {
            u8Temp = 0;
            qam_write_byte(u32TunerPort,HI3138_T_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
        
        HI3138_IPIO_CINFIG(u32TunerPort);
        qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
        u8Temp &= ~(3 << 0);
        qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_demod1_wakeup(HI_U32 u32TunerPort, HI_U8 u8AdcSwapFlag)
{
    HI_S32 s32Ret;
    HI_U8 u8Temp = 0;


    if (u8AdcSwapFlag)
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {        
             u8Temp = 0xff;
             qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB) ||
         (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
        {
             u8Temp = 0xff;
             qam_write_byte(u32TunerPort,HI3138_C_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
    }
    /*ADC没有交换的情况*/
    else
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            u8Temp = 0xff;
            qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
            
            HI3138_IPIO_CINFIG(u32TunerPort);
            qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
            u8Temp &= ~(7 << 2);
            u8Temp |= (2 << 2);
            qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
        }
        else if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB) 
        {
            u8Temp = 0xff;
            qam_write_byte(u32TunerPort,HI3138_C_MAN_RST_CTRL0,u8Temp);
            
            HI3138_IPIO_CINFIG(u32TunerPort);
            qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
            u8Temp &= ~(7 << 2);
            u8Temp |= (5 << 2);
            qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
        }
        else if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B)
        {
            u8Temp = 0xff;
            qam_write_byte(u32TunerPort,HI3138_C_MAN_RST_CTRL0,u8Temp);
            
            HI3138_IPIO_CINFIG(u32TunerPort);
            qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
            u8Temp &= ~(7 << 2);
            u8Temp |= (1 << 2);
            qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }

        /*重新配置ADC模块*/
        s32Ret = hi3138_adc_config(u32TunerPort);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_adc_config failed.\n");
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

static HI_S32 hi3138_demod1_sleep(HI_U32 u32TunerPort, HI_U8 u8AdcSwapFlag)
{
    HI_U8 u8Temp = 0;


    if (u8AdcSwapFlag)
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {        
             u8Temp = 0;
             qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
        {
             u8Temp = 0;
             qam_write_byte(u32TunerPort,HI3138_C_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
    }
    /*ADC没有交换的情况*/
    else
    {
        if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            u8Temp = 0;
            qam_write_byte(u32TunerPort,HI3138_S_MAN_RST_CTRL0,u8Temp);
        }
        else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB) ||
                    (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
        {
            u8Temp = 0;
            qam_write_byte(u32TunerPort,HI3138_C_MAN_RST_CTRL0,u8Temp);
        }
        else
        {
            HI_ERR_TUNER("-->IN--hi3138_set_standby is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
            return HI_FAILURE;
        }
        
        HI3138_IPIO_CINFIG(u32TunerPort);
        qam_read_byte(u32TunerPort,HI3138_CHN_ON,&u8Temp);
        u8Temp &= ~(7 << 2);
        qam_write_byte(u32TunerPort,HI3138_CHN_ON,u8Temp);
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_standby(HI_U32 u32TunerPort, HI_U32 u32Status)
{
    HI_S32 s32Ret;
    HI_U8 i = 0;
    HI_U8 u8AdcSwapFlag = 0;

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);

    /*判断ADC是否有交换*/
    if((HI3138_INNER_DEMOD0 == stHi3138Allocation[HI3138_INNER_DEMOD1].enInnerDemodId)
        || (HI3138_INNER_DEMOD1 == stHi3138Allocation[HI3138_INNER_DEMOD0].enInnerDemodId))
    {
        u8AdcSwapFlag = 1;
    }

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    HI_INFO_TUNER("-->IN--hi3138_set_standby, stHi3138Allocation[%d].enInnerDemodId:%d,u8AdcSwapFlag:%d,enStatus:%s\n",
                            i,
                            stHi3138Allocation[i].enInnerDemodId,
                            u8AdcSwapFlag,
                            ((HI_UNF_TUNER_DEMOD_STATUS_E)u32Status) == 0 ? "WAKE_UP":"STANDBY" );

    if (stHi3138Allocation[i].enInnerDemodId == HI3138_INNER_DEMOD0)
    {
        if (HI_UNF_TUNER_DEMODE_WAKE_UP == (HI_UNF_TUNER_DEMOD_STATUS_E)u32Status)
        {
            s32Ret = hi3138_demod0_wakeup(u32TunerPort, u8AdcSwapFlag);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("hi3138_demod0_wakeup failed.\n");
                return s32Ret;
            }
        }
        /*休眠模式*/
        else
        {
            s32Ret = hi3138_demod0_sleep(u32TunerPort, u8AdcSwapFlag);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("hi3138_demod0_sleep failed.\n");
                return s32Ret;
            }
        }
    }
    else if (stHi3138Allocation[i].enInnerDemodId == HI3138_INNER_DEMOD1)
    {
        if (HI_UNF_TUNER_DEMODE_WAKE_UP == (HI_UNF_TUNER_DEMOD_STATUS_E)u32Status)
        {
            s32Ret = hi3138_demod1_wakeup(u32TunerPort, u8AdcSwapFlag);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("hi3138_demod1_wakeup failed.\n");
                return s32Ret;
            }
        }
        /*休眠模式*/
        else
        {
            s32Ret = hi3138_demod1_sleep(u32TunerPort, u8AdcSwapFlag);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("hi3138_demod1_sleep failed.\n");
                return s32Ret;
            }
        }
    }

    /*上面有IPIO操作，为确保可靠性，重新切回来*/
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode)
{
    HI_TUNER_CHECKPORT(u32TunerPort);
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

