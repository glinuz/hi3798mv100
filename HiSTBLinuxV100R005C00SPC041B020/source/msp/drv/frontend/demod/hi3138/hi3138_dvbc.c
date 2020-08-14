/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hiDVBC.c
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

#include "mxl603.h"
#include "mxl608.h"
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

static HI_S32 hiDVBC_get_lock_flag(HI_U32 u32TunerPort,HI3138_C_LOCK_FLAG_E enLockFlag)
{
    HI_U8 u8LockFlag = 0;
    HI_U8 u8Mask;

    HI_TUNER_CHECKPORT(u32TunerPort);

    qam_read_byte(u32TunerPort, HI3138_C_LOCK_FLAG, &u8LockFlag);

    u8Mask = 1 << enLockFlag;
    if(u8LockFlag & u8Mask)	 
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_FAILURE;
    }
}

static HI_S32 hiDVBC_hot_reset(HI_U32 u32TunerPort)
{
    HI_U8 u8Temp = 0;
    
    HI_TUNER_CHECKPORT(u32TunerPort);
    
    qam_read_byte(u32TunerPort, HI3138_RSTN_CTRL, &u8Temp);
    u8Temp &= ~(1 << 1);
    qam_write_byte(u32TunerPort, HI3138_RSTN_CTRL, u8Temp);
    msleep(2);      /*datasheet上要求1ms以上*/
    u8Temp |= 0x2;
    qam_write_byte(u32TunerPort, HI3138_RSTN_CTRL, u8Temp);

    return HI_SUCCESS;
}

HI_S32 hiDVBC_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U32 u32SymbolRate = 0,u32Chk = 300,i;
    HI_U8  u8FsLow,u8FsHigh;
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    HI_TUNER_CHECKPOINTER(pstChannel);

    HI_INFO_TUNER("pstChannel->u32Frequency:%d kHz, pstChannel->unSRBW.u32SymbolRate:%d\n", 
                            pstChannel->u32Frequency, 
                            pstChannel->unSRBW.u32SymbolRate);
    
    /*符号率*/
    u32SymbolRate = pstChannel->unSRBW.u32SymbolRate / 1000; //kHz
    u8FsLow = (u32SymbolRate & 0xFF);
    u8FsHigh = (u32SymbolRate>>8) & 0xFF;
    qam_write_byte(u32TunerPort, HI3138_C_FS_L, u8FsLow);
    qam_write_byte(u32TunerPort, HI3138_C_FS_H, u8FsHigh);
    
    /*调制模式*/
    qam_read_byte(u32TunerPort,HI3138_C_EQU_CTRL, &u8Temp);
    u8Temp &= ~(0xf << 2);
    switch(pstChannel->enQamType)
    {
        case QAM_TYPE_16:
        {
            u8Temp |= 4 << 2;
            break;
        }
        case QAM_TYPE_32:
        {
            u8Temp |= 5 << 2;
            break;
        }
        case QAM_TYPE_64:
        {
            u8Temp |= 6 << 2;
            break;
        }
        case QAM_TYPE_128:
        {
            u8Temp |= 7 << 2;
            break;
        }
        case QAM_TYPE_256:
        /*now jump to default*/
        default:
        {
            u8Temp |= 8 << 2;
            break;
        }
    }
    qam_write_byte(u32TunerPort,HI3138_C_EQU_CTRL, u8Temp);

    qam_write_byte(u32TunerPort,HI3138_C_PT_CTRL1, 0x53);
    qam_write_byte(u32TunerPort,HI3138_C_PLS_CTRL1, 0xff);
    qam_write_byte(u32TunerPort,HI3138_C_PLS_CTRL2, 0xff);
    qam_write_byte(u32TunerPort,HI3138_C_PLS_CTRL3, 0x82);
    qam_write_bit(u32TunerPort,HI3138_C_TR_CR_CTRL, 3,0);
    
    /*对所选解调模块做一次热复位*/
    s32Ret = hiDVBC_hot_reset(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hiDVBC_hot_reset failed.\n");
        return s32Ret;
    }
    
    /*判断是否已确实锁频*/
    for(i = 0;i < u32Chk;i+=2)
    {
        qam_read_byte(u32TunerPort,HI3138_C_LOCK_FLAG, &u8Temp);
        if((u8Temp & 0x9f) == 0x9f)
        {
            break;
        }
        tuner_mdelay(2);
    }

    if(i>=u32Chk)
    {
        HI_WARN_TUNER("Demod lock registor:%#x\n",u8Temp);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hiDVBC_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(penTunerStatus);

    s32Ret = hiDVBC_get_lock_flag(u32TunerPort, HI3138_C_FEC_LOCK_FLAG);
    if (s32Ret == HI_SUCCESS)
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    else
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    }

    HI_INFO_TUNER("-->IN--hiDVBC_get_status,*penTunerStatus:%d\n",*penTunerStatus);

    return HI_SUCCESS;
}

HI_S32 hiDVBC_get_ber(HI_U32 u32TunerPort, HI_U32 * pu32BERa)
{
    HI_U8 u8BerLow = 0, u8BerHigh = 0;
    HI_U32  u32Ber = 0;
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8LockFlag = 0;
    
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32BERa);

    s32Ret = hiDVBC_get_lock_flag(u32TunerPort, HI3138_C_FEC_LOCK_FLAG);
    if(HI_SUCCESS != s32Ret)
    {
        qam_read_byte(u32TunerPort, HI3138_C_LOCK_FLAG, &u8LockFlag);
        HI_WARN_TUNER("Demod lock registor:%#x\n",u8LockFlag);
        return s32Ret;
    }
    
    /*err_pkt_ratio=(ERR_PKT_H[4:0]*256+ERR_PKT_L[7:0])/16384*100%;*/
    qam_read_byte(u32TunerPort,HI3138_C_ERR_PKT_L, &u8BerLow);
    qam_read_byte(u32TunerPort,HI3138_C_ERR_PKT_H, &u8BerHigh);
    u8BerHigh &= ~(7 << 5);
    u32Ber = ((u8BerHigh << 8) | u8BerLow) / 16384;

    pu32BERa[0] = u32Ber;
    pu32BERa[1] = 0;
    pu32BERa[2] = 0;
    HI_INFO_TUNER("-->IN--hiDVBC_get_ber,*pu32BERa:%d\n",*pu32BERa);
    
    return HI_SUCCESS;

}

HI_S32 hiDVBC_get_snr(HI_U32 u32TunerPort, HI_U32 * pu32SignalStrength)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8SnrLow = 0, u8SnrHigh = 0;
    HI_U32 u32Snr = 0;
    HI_U8 u8LockFlag = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32SignalStrength);

    s32Ret = hiDVBC_get_lock_flag(u32TunerPort, HI3138_C_EQU_LOCK_FLAG);
    if(HI_SUCCESS != s32Ret)
    {
        qam_read_byte(u32TunerPort, HI3138_C_LOCK_FLAG, &u8LockFlag);
        HI_WARN_TUNER("Demod lock registor:%#x\n",u8LockFlag);
        return s32Ret;
    }

    /*SNR = 10.0*log10(sig_pow*2048/(noise_pow_h [7:0]*256+noise_pow_l))*/
    qam_read_byte(u32TunerPort,HI3138_C_NOISE_POW_L, &u8SnrLow);
    qam_read_byte(u32TunerPort,HI3138_C_NOISE_POW_H, &u8SnrHigh);
    u32Snr =  u8SnrHigh*256 + u8SnrLow;

    *pu32SignalStrength = u32Snr;

    HI_INFO_TUNER("-->IN--hiDVBC_get_snr,*pu32SignalStrength:%d\n",*pu32SignalStrength);
    
    return HI_SUCCESS;

}

HI_S32 hiDVBC_get_signal_strength(HI_U32 u32TunerPort, HI_U32 * pu32strength)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8AgcLow = 0, u8AgcHigh = 0;
    HI_U32 u32Agc = 0;
    HI_U8 u8LockFlag = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32strength);

    s32Ret = hiDVBC_get_lock_flag(u32TunerPort, HI3138_C_AGC_LOCK_FLAG);
    if(HI_SUCCESS != s32Ret)
    {
        qam_read_byte(u32TunerPort, HI3138_C_LOCK_FLAG, &u8LockFlag);
        HI_WARN_TUNER("Demod lock registor:%#x\n",u8LockFlag);
        return s32Ret;
    }
    
    /*sig_strength=AGC_CTRL_H[3:0]*256+AGC_CTRL_L[7:0]*/
    qam_read_byte(u32TunerPort,HI3138_C_AGC_CTRL_L, &u8AgcLow);
    qam_read_byte(u32TunerPort,HI3138_C_AGC_CTRL_H, &u8AgcHigh);
    u32Agc =  ((u8AgcHigh & 0x0f) << 8 ) + u8AgcLow;

    *pu32strength = u32Agc;

    HI_INFO_TUNER("-->IN--hiDVBC_get_signal_strength,*pu32strength:%d\n",*pu32strength);
    
    return HI_SUCCESS;

}

HI_S32 hiDVBC_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstInfo);

    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;

    return HI_SUCCESS;
}

HI_S32 hiDVBC_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
    //HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 i = 0;
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstTSOut);

    for (i = 0; i < MAX_TS_LINE; i++)
    {
        if (HI_UNF_TUNER_OUTPUT_BUTT <= pstTSOut->enTSOutput[i])
        {
            HI_ERR_TUNER("Error pin define!\n");
            return HI_FAILURE;
        }
    }

    qam_read_byte(u32TunerPort, HI3138_C_OUTP_BER, &u8Temp); 
    u8Temp &= 0xf;
    u8Temp = ((pstTSOut->enTSOutput[10] & 0xf)<<4) | u8Temp; 
    qam_write_byte(u32TunerPort, HI3138_C_OUTP_BER, u8Temp);/*config ts_out_10bit*/

    qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_98_SEL, ((pstTSOut->enTSOutput[9]& 0xf)<<4) | pstTSOut->enTSOutput[8]);/*config ts_out_9&8bit*/
    qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_76_SEL, ((pstTSOut->enTSOutput[7]& 0xf)<<4) | pstTSOut->enTSOutput[6]);/*config ts_out_7&6bit*/
    qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_54_SEL, ((pstTSOut->enTSOutput[5]& 0xf)<<4) | pstTSOut->enTSOutput[4]);/*config ts_out_5&4bit*/
    qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_32_SEL, ((pstTSOut->enTSOutput[3]& 0xf)<<4) | pstTSOut->enTSOutput[2]);/*config ts_out_3&2bit*/
    qam_write_byte(u32TunerPort, HI3138_C_OUTP_TS_10_SEL, ((pstTSOut->enTSOutput[1]& 0xf)<<4) | pstTSOut->enTSOutput[0]);/*config ts_out_1&0bit*/
    
    return HI_SUCCESS;

}

HI_S32 hiDVBC_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    HI_U32 u32Freq = 0;
    HI_S32 s32FreqOffset = 0;
    HI_U8  au8RegData[2] = { 0 };
    HI_S32 s32RealSymb = 0;
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8LockFlag = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32Freq);

    HI_TUNER_CHECKPOINTER(pu32Symb);
	
    s32Ret = hiDVBC_get_lock_flag(u32TunerPort, HI3138_C_FEC_LOCK_FLAG);
    if(HI_SUCCESS != s32Ret)
    {
        qam_read_byte(u32TunerPort, HI3138_C_LOCK_FLAG, &u8LockFlag);
        HI_WARN_TUNER("Demod lock registor:%#x\n",u8LockFlag);
        return s32Ret;
    }
    
    /* calc freq */
    qam_read_byte(u32TunerPort,HI3138_C_FREQ_DEV_L, &au8RegData[0]);
    qam_read_byte(u32TunerPort,HI3138_C_FREQ_DEV_H, &au8RegData[1]);	//先低后高读寄存器
    u32Freq = au8RegData[0] + au8RegData[1]*256;
    if(u32Freq >= 32768)
    {
        s32FreqOffset = u32Freq - 65536;
    }
    else
    {
        s32FreqOffset = u32Freq;
    }
    
    *pu32Freq = s32FreqOffset;
    HI_INFO_TUNER("-->IN--hi3138_DVBC_get_freq_symb_offset,s32FreqOffset = %d kHz\n",s32FreqOffset);
    
    /* calc symbolrate */
    qam_read_byte(u32TunerPort,HI3138_C_FS_L, &au8RegData[0]);
    qam_read_byte(u32TunerPort,HI3138_C_FS_H, &au8RegData[1]);	
    
    s32RealSymb = au8RegData[0] + au8RegData[1]*256;
    *pu32Symb = s32RealSymb;
    HI_INFO_TUNER("-->IN--hi3138_DVBC_get_freq_symb_offset,s32RealSymb = %d kSymbol/s\n",s32RealSymb);
  
    return HI_SUCCESS;
}
#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

