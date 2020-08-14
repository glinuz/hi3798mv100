/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hiDVB_S_S2.c
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
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

extern Hi3138_SIGNAL_INFO_ALLOCATION stHi3138Allocation[2];

HI_S32	hi3138_set_isi_id(HI_U32 u32TunerPort, HI_U8 u8IsiId)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3136_set_isi_id(u32TunerPort, u8IsiId);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3136_set_isi_id failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32	hi3138_get_total_isi_number(HI_U32 u32TunerPort,HI_U32 *u32TotalStream)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(u32TotalStream);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3136_get_total_isi_number(u32TunerPort, u32TotalStream);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3136_get_total_isi_number failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32	hi3138_get_isi_id(HI_U32 u32TunerPort,HI_U8 u8Stream, HI_U8 *u8IsiId)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(u8IsiId);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3136_get_isi_id(u32TunerPort, u8Stream, u8IsiId);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3136_get_isi_id failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}


HI_S32 hi3138_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara)
{
    HI_S32 s32Ret;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstPara);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    
    s32Ret = hi3136_blindscan_init(u32TunerPort, pstPara);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("call hi3138_blindscan_init fail.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara)
{
    HI_S32 s32Ret;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstPara);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }
    
    s32Ret = hi3136_blindscan_action(u32TunerPort, pstPara);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("call hi3138_blindscan_init fail.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

/* 0 Power off, 1 power on */
HI_S32 hi3138_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power)
{
    HI_U8 u8Temp = 0;
    HI_S32 s32Ret;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    qam_read_byte(u32TunerPort, 0xb7, &u8Temp);
    if (u8Power)
    {
        u8Temp |= (0x1<<7);
    }
    else
    {
        u8Temp &= ~(0x1<<7);
    }
    qam_write_byte(u32TunerPort, 0xb7, u8Temp);

    return HI_SUCCESS;
}


HI_S32 hi3138_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut)
{
    Hi3138_LNB_POWER_SOURCE enPowerSrc = HI3138_LNB_POWER_BY_ADDR_BUTT;
    HI_U32 i;
    HI_U8 u8Temp = 0,u8Buf = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    qam_read_byte(u32TunerPort, HI3138_I2C_CHN_SEL, &u8Temp);
    u8Temp &= ~(0x0F << 0);
    u8Temp |= 3;
    qam_write_byte(u32TunerPort, HI3138_I2C_CHN_SEL, u8Temp);

    for(i = 0;i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0]));i++)
    {
    	if((u32TunerPort == stHi3138Allocation[i].u32TunerPort) && 
            (HI_UNF_TUNER_SIG_TYPE_SAT == stHi3138Allocation[i].enHi3138SigType))
    	{
    		enPowerSrc = stHi3138Allocation[i].enLNBSrc;
    		break;
    	}
    }

    if(HI3138_LNB_POWER_BY_ADDR_BUTT == enPowerSrc)
    {
        HI_ERR_TUNER("not sat signal.\n");
        return HI_FAILURE;
    }

    qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);
    //   读低3bit，不需要移位
    u8Buf = (u8Temp & 0xF);
    switch (enOut)
    {
        case TUNER_LNB_OUT_0V:
        case TUNER_LNB_OUT_BUTT:
        default:
            return HI_FAILURE;

        case TUNER_LNB_OUT_13V:
        case TUNER_LNB_OUT_14V:
            if(HI3138_LNB_POWER_BY_ADDR0 == enPowerSrc)
            {
                u8Buf &= ~(0x1<<2);
            }
            else
            {
                u8Buf &= ~(0x1<<3);
            }
            break;

        case TUNER_LNB_OUT_18V:
        case TUNER_LNB_OUT_19V:
            if(HI3138_LNB_POWER_BY_ADDR0 == enPowerSrc)
            {
                u8Buf |= (0x1<<2);
            }
            else
            {
                u8Buf |= (0x1<<3);
            }
            break;
    }
    u8Temp = (u8Temp << 1);
    u8Temp = (u8Temp & 0xF0) | u8Buf;
    qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);

    return HI_SUCCESS;
}

HI_S32 hi3138_send_continuous_22K(HI_U32 u32TunerPort,HI_BOOL b22k_on) // 设置22k,
{
    HI_S32 s32Ret;
    
    HI_TUNER_CHECKPORT(u32TunerPort);
    
    if(g_stTunerOps[u32TunerPort].enSigType != HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        HI_ERR_TUNER("not sat signal.\n");
        return HI_FAILURE;
    }
    
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    s32Ret |= hi3136_send_continuous_22K(u32TunerPort,b22k_on);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 hi3138_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg)
{
    HI_S32 s32Ret;
    
    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstSendMsg);

    if(g_stTunerOps[u32TunerPort].enSigType != HI_UNF_TUNER_SIG_TYPE_SAT)
    {
        HI_ERR_TUNER("not sat signal.\n");
        return HI_FAILURE;
    }
    
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    s32Ret |= hi3136_DiSEqC_send_msg(u32TunerPort, pstSendMsg);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 hi3138_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone)
{
    return HI_SUCCESS;
}

HI_S32 hi3138_tp_verify(HI_U32 u32TunerPort, TUNER_TP_VERIFY_PARAMS_S * pstChannel)
{
    HI_S32 s32Ret;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstChannel);

    s32Ret = hi3136_tp_verify(u32TunerPort, pstChannel);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("call hi3138_blindscan_init fail.\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_lnb_link(HI_U32 u32TunerPort)
{
    HI_TUNER_CHECKPORT(u32TunerPort);
    
    if ((stHi3138Allocation[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&     /*仅打开demod0,仅支持S+T*/
        (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        if (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)
        {
            if(HI_UNF_TUNER_ADC0 == stHi3138Allocation[0].enADCId)
            {
                stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
            }
            else
            {
                stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
            }
        }
    }
    else if ((stHi3138Allocation[0].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT) &&
        (stHi3138Allocation[1].enHi3138SigType != HI_UNF_TUNER_SIG_TYPE_BUTT))
    {
        if (((stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) &&    /*T+S1*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            if(stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[0].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
            else if(stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[1].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
        }
        else if (((stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT))
            || ((stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T) &&    /*ISDBT+S1*/
            (stHi3138Allocation[0].enHi3138SigType== HI_UNF_TUNER_SIG_TYPE_SAT)))
        {
            if(stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[0].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
            else if(stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[1].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
        }
        else if (((stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_CAB))
            || ((stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+C*/
            (stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_CAB)))
        {
            if(stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[0].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
            else if(stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[1].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            } 
        }
        else if (((stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_J83B))
            || ((stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+J83B*/
            (stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_J83B)))
        {
            if(stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[0].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
            else if(stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT)
            {
                switch(stHi3138Allocation[1].enADCId)
                {
                    case HI_UNF_TUNER_ADC0:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                        break;
                    case HI_UNF_TUNER_ADC1:
                        stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                        break;
                    default:
                        break;
                }
            }
        }
        else if ((stHi3138Allocation[0].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT) &&    /*S0+S1*/
            (stHi3138Allocation[1].enHi3138SigType == HI_UNF_TUNER_SIG_TYPE_SAT))
        {
            switch(stHi3138Allocation[0].enADCId)
            {
                case HI_UNF_TUNER_ADC0:
                default:
                    stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                    stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                    break;
                case HI_UNF_TUNER_ADC1:
                    stHi3138Allocation[0].enLNBSrc = HI3138_LNB_POWER_BY_ADDR1;
                    stHi3138Allocation[1].enLNBSrc = HI3138_LNB_POWER_BY_ADDR0;
                    break;
            }
        }
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

