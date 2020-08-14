/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hi3138_ts.c
* Description:
*
* History:
* Version   Date             Author                         DefectNum        Description
* main\1    2016-03-16   w313507                      NULL                Create this file.
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
#include "hi3136.h"
#include "hi3137.h"
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

extern Hi3138_SIGNAL_INFO_ALLOCATION stHi3138Allocation[2];
extern HI_U8 gs_u8InnerCnt;

HI_S32 hi3138_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_INFO_TUNER("enTsType:%d\n", enTsType);

    stHi3138Allocation[gs_u8InnerCnt].enTsType = enTsType;

    gs_u8InnerCnt++;
    gs_u8InnerCnt = gs_u8InnerCnt%2;
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
    HI_UNF_TUNER_TSOUT_SET_S stTsOut,*pstTempTsOut;
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U32 i;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    HI_TUNER_CHECKPOINTER(pstTSOut);

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);

    memcpy(&(stHi3138Allocation[i].stTSOut),pstTSOut,sizeof(HI_UNF_TUNER_TSOUT_SET_S));
    
    stTsOut.enTSOutput[0] = HI_UNF_TUNER_OUTPUT_TSDAT0;
    stTsOut.enTSOutput[1] = HI_UNF_TUNER_OUTPUT_TSDAT1;
    stTsOut.enTSOutput[2] = HI_UNF_TUNER_OUTPUT_TSDAT2;
    stTsOut.enTSOutput[3] = HI_UNF_TUNER_OUTPUT_TSDAT3;
    stTsOut.enTSOutput[4] = HI_UNF_TUNER_OUTPUT_TSDAT4;
    stTsOut.enTSOutput[5] = HI_UNF_TUNER_OUTPUT_TSDAT5;
    stTsOut.enTSOutput[6] = HI_UNF_TUNER_OUTPUT_TSDAT6;
    stTsOut.enTSOutput[7] = HI_UNF_TUNER_OUTPUT_TSDAT7;
    stTsOut.enTSOutput[8] = HI_UNF_TUNER_OUTPUT_TSSYNC;
    stTsOut.enTSOutput[9] = HI_UNF_TUNER_OUTPUT_TSVLD;
    stTsOut.enTSOutput[10] = HI_UNF_TUNER_OUTPUT_TSERR;

    if(stHi3138Allocation[i].bTSCombFlag)
    {
        /*TS合并情况下，S/T/C模块内部TS线序需要配置为默认线序*/
        pstTempTsOut = &stTsOut;
        s32Ret = hi3138_set_comb_ts_out(u32TunerPort, pstTSOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_set_comb_ts_out failed.\n");
            return s32Ret;
        }
    }
    else
    {
        pstTempTsOut = pstTSOut;
        s32Ret = hi3138_set_comb_ts_out(u32TunerPort, &stTsOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_set_comb_ts_out failed.\n");
            return s32Ret;
        }
    }

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        s32Ret = hi3136_set_ts_out(u32TunerPort, pstTempTsOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_set_ts_out failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                 (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_set_ts_out(u32TunerPort, pstTempTsOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_set_ts_out failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        s32Ret = hiDVBC_set_ts_out(u32TunerPort, pstTempTsOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_set_ts_out failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_set_ts_out is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;

}

HI_S32 hi3138_ts_output_swap(HI_U32 u32TunerPort,HI_BOOL bSwapFlag)
{
    HI_U8 u8Data = 0;
    
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI3138_IPIO_CINFIG(u32TunerPort);

    qam_read_byte(u32TunerPort, HI3138_IO_CTRL13, &u8Data);
    if(bSwapFlag)
    {
        u8Data |= (0x1<<1); 
    }
    else
    {
        u8Data &= ~(0x1<<1); 
    }
    qam_write_byte(u32TunerPort, HI3138_IO_CTRL13, u8Data);
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_ts_port_hiz(HI_U32 u32TunerPort, HI_UNF_TUNER_TS_SERIAL_PIN_E enSeriPin)
{
    HI_UNF_TUNER_TS_OUTPUT_PORT_E enTSPort;
    HI_U32 i;
    HI_U8 u8Temp;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI3138_SELECT_ALLOCATION_PORT(u32TunerPort, stHi3138Allocation, i);
    enTSPort = stHi3138Allocation[i].enTSOutputId;

    if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL == g_stTunerOps[u32TunerPort].enTsType)
    {
        switch(enSeriPin)
        {
            case HI_UNF_TUNER_TS_SERIAL_PIN_0:
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0xFE;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0xFE;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                break;
            case HI_UNF_TUNER_TS_SERIAL_PIN_7:
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0x7F;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0x7F;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                break;
            default:
                /*在默认情况下，由于不知道哪几位高阻
                因此，只能都配置为非高阻*/
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                HI_WARN_TUNER("-->IN--hi3138_set_ts_port_hiz,TunerPort:%d,enSeriPin = %d,enTSPort = %d\n", u32TunerPort, enSeriPin, enTSPort);
                break;
        }
    }
    else if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT == g_stTunerOps[u32TunerPort].enTsType)
    {
        switch(enSeriPin)
        {
            case HI_UNF_TUNER_TS_SERIAL_PIN_0:
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0xFC;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0xFC;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                break;
            case HI_UNF_TUNER_TS_SERIAL_PIN_7:
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0x3F;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0x3F;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                break;
            default:
                /*在默认情况下，由于不知道哪几位高阻
                因此，只能都配置为非高阻*/
                if(HI_UNF_TUNER_TS0_OUTPUT == enTSPort)
                {
                    u8Temp = 0;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                }
                else
                {
                    u8Temp = 0;
                    qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                }
                HI_WARN_TUNER("-->IN--hi3138_set_ts_port_hiz,TunerPort:%d,enTSPort = %d\n", u32TunerPort, enTSPort);
                break;
        }
    }
    /*并行模式*/
    else
    {
        switch(enTSPort)
        {
            case HI_UNF_TUNER_TS0_OUTPUT:
                u8Temp = 0x0;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                break;
            case HI_UNF_TUNER_TS1_OUTPUT:
                u8Temp = 0x0;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                break;
            default:
                /*在默认情况下，由于不知道哪个TS高阻
                因此，只能都配置为非高阻*/
                u8Temp = 0x0;
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL10, u8Temp);
                qam_write_byte(u32TunerPort, HI3138_IO_CTRL11, u8Temp);
                HI_WARN_TUNER("-->IN--hi3138_set_ts_port_hiz,TunerPort:%d,enTSPort = %d\n", u32TunerPort, enTSPort);
                break;
        }
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_set_comb_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstTSOut);

    HI3138_IPIO_CINFIG(u32TunerPort);

    qam_write_byte(u32TunerPort, HI3138_MTS_OUT_SEL98, ((pstTSOut->enTSOutput[9]& 0xf)<<4) | pstTSOut->enTSOutput[8]);/*config ts_out_9&8bit*/
    qam_write_byte(u32TunerPort, HI3138_MTS_OUT_SEL76, ((pstTSOut->enTSOutput[7]& 0xf)<<4) | pstTSOut->enTSOutput[6]);/*config ts_out_7&6bit*/
    qam_write_byte(u32TunerPort, HI3138_MTS_OUT_SEL54, ((pstTSOut->enTSOutput[5]& 0xf)<<4) | pstTSOut->enTSOutput[4]);/*config ts_out_5&4bit*/
    qam_write_byte(u32TunerPort, HI3138_MTS_OUT_SEL32, ((pstTSOut->enTSOutput[3]& 0xf)<<4) | pstTSOut->enTSOutput[2]);/*config ts_out_3&2bit*/
    qam_write_byte(u32TunerPort, HI3138_MTS_OUT_SEL10, ((pstTSOut->enTSOutput[1]& 0xf)<<4) | pstTSOut->enTSOutput[0]);/*config ts_out_1&0bit*/

    return HI_SUCCESS;
}

HI_S32 hi3138_mts_serial_config(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType, HI_UNF_TUNER_SIG_TYPE_E enSigType)
{
    HI_UNF_TUNER_TSOUT_SET_S stTsOut;
    HI_U8 u8Data = 0;
    HI_S32 s32Ret;

    HI_TUNER_CHECKPORT(u32TunerPort);

    if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL == enTsType)
    {
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x1);
        qam_write_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL0, 0x0);
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x2);
        qam_write_byte(u32TunerPort, HI3138_S_MAN_RST_CTRL0, 0x0);
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x8);
        s32Ret = hi3136_set_ts_type(u32TunerPort, HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("hi3136_set_ts_type failed.\n");
            return HI_FAILURE;
        }
        
        stTsOut.enTSOutput[0] = HI_UNF_TUNER_OUTPUT_TSDAT0;
        stTsOut.enTSOutput[1] = HI_UNF_TUNER_OUTPUT_TSDAT1;
        stTsOut.enTSOutput[2] = HI_UNF_TUNER_OUTPUT_TSDAT2;
        stTsOut.enTSOutput[3] = HI_UNF_TUNER_OUTPUT_TSDAT3;
        stTsOut.enTSOutput[4] = HI_UNF_TUNER_OUTPUT_TSDAT4;
        stTsOut.enTSOutput[5] = HI_UNF_TUNER_OUTPUT_TSDAT5;
        stTsOut.enTSOutput[6] = HI_UNF_TUNER_OUTPUT_TSDAT6;
        stTsOut.enTSOutput[7] = HI_UNF_TUNER_OUTPUT_TSDAT7;
        stTsOut.enTSOutput[8] = HI_UNF_TUNER_OUTPUT_TSSYNC;
        stTsOut.enTSOutput[9] = HI_UNF_TUNER_OUTPUT_TSVLD;
        stTsOut.enTSOutput[10] = HI_UNF_TUNER_OUTPUT_TSERR;
        s32Ret = hi3136_set_ts_out(u32TunerPort, &stTsOut);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("hi3136_set_ts_out failed.\n");
            return HI_FAILURE;
        }

        // no clk when no vld & 1bit serial
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x3);
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL0, &u8Data);
        u8Data = (u8Data & 0xcc) | 0x30;
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL0, u8Data);

        //set tag_num to 0 & use system clk
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL12, &u8Data);
        u8Data = u8Data & 0xe8;
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL12, u8Data);

        //use system clk
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Data);
        u8Data |= (1 << 5);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Data);

        //enable ts combine
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Data);
        u8Data &=  ~(1 << 6);
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Data);
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Data);
        u8Data |= (1 << 6);
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Data);
    }
    else if(HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT == enTsType)
    {
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x1);
        qam_write_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL0, 0x0);
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x2);
        qam_write_byte(u32TunerPort, HI3138_S_MAN_RST_CTRL0, 0x0);

        // no clk when no vld & 1bit serial
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x3);
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL0, &u8Data);
        u8Data = (u8Data & 0xcc) | 0x32;
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL0, u8Data);

        //set tag_num to 0 & use logic clk
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL12, &u8Data);
        u8Data = (u8Data & 0xd8) | 0x20;
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL12, u8Data);

        //use logic clk
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Data);
        u8Data = (u8Data & 0xdf);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Data);

        //enable ts combine
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Data);
        u8Data &=  ~(1 << 6);
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Data);
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Data);
        u8Data |=  (1 << 6);
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Data);

    }

    return HI_SUCCESS;
}

HI_S32 hi3138_single_stream_config(HI_U32 u32TunerPort)
{
    HI_S32 s32Ret;
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    if((HI_UNF_TUNER_TS0_OUTPUT == stHi3138Allocation[0].enTSOutputId) 
        && (HI_UNF_TUNER_TS0_OUTPUT == stHi3138Allocation[1].enTSOutputId))
    {
        if (((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) || (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T)) 
            && (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
        {
            stHi3138Allocation[0].bTSCombFlag = HI_TRUE;
            stHi3138Allocation[1].bTSCombFlag = HI_TRUE;
            s32Ret = hi3138_mts_serial_config(u32TunerPort, stHi3138Allocation[0].enTsType, HI_UNF_TUNER_SIG_TYPE_SAT);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("--<---hi3138_mts_serial_config err.\n");
            }
        }
        else if (((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_DVB_T2) || (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_ISDB_T)) 
            && (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT))
        {
            stHi3138Allocation[0].bTSCombFlag = HI_TRUE;
            stHi3138Allocation[1].bTSCombFlag = HI_TRUE;
            s32Ret = hi3138_mts_serial_config(u32TunerPort, stHi3138Allocation[1].enTsType, HI_UNF_TUNER_SIG_TYPE_SAT);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("--<---hi3138_mts_serial_config err.\n");
            }
        }
        else if(((stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB) || (stHi3138Allocation[1].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)) 
            && (stHi3138Allocation[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT))
        {
            stHi3138Allocation[0].bTSCombFlag = HI_TRUE;
            stHi3138Allocation[1].bTSCombFlag = HI_TRUE;
            s32Ret = hi3138_mts_serial_config(u32TunerPort, stHi3138Allocation[1].enTsType, HI_UNF_TUNER_SIG_TYPE_CAB);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("--<---hi3138_mts_serial_config err.\n");
            }
        }
        else if(((stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_CAB) || (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_J83B)) 
            && (stHi3138Allocation[1].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT))
        {
            stHi3138Allocation[0].bTSCombFlag = HI_TRUE;
            stHi3138Allocation[1].bTSCombFlag = HI_TRUE;
            s32Ret = hi3138_mts_serial_config(u32TunerPort, stHi3138Allocation[0].enTsType, HI_UNF_TUNER_SIG_TYPE_CAB);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_TUNER("--<---hi3138_mts_serial_config err.\n");
            }
        }
        else
        {
            qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x3);
            
            qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Temp);
            u8Temp &= ~(0x1 << 6);
            qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Temp);
        }
    }
    else
    {
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x1);
        qam_write_byte(u32TunerPort, HI3138_T_MAN_RST_CTRL0, 0xff);
        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x2);
        qam_write_byte(u32TunerPort, HI3138_S_MAN_RST_CTRL0, 0xff);

        qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, 0x3);
        qam_read_byte(u32TunerPort, HI3138_CRG_CTRL1, &u8Temp);
        u8Temp &= ~(1 << 5);
        qam_write_byte(u32TunerPort, HI3138_CRG_CTRL1, u8Temp);

        //disable ts combine
        qam_read_byte(u32TunerPort, HI3138_MTS_CTRL1, &u8Temp);
        u8Temp &=  ~(1 << 6);
        qam_write_byte(u32TunerPort, HI3138_MTS_CTRL1, u8Temp);
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

