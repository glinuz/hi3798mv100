/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hi3138_info.c
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

HI_S32 hi3138_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(penTunerStatus);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_get_status (u32TunerPort, penTunerStatus);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_status failed.\n");
            return s32Ret;
        }
        
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_get_status (u32TunerPort, penTunerStatus);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_status failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {     
        s32Ret = hiDVBC_get_status (u32TunerPort, penTunerStatus);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_status failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_status is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    HI_S32 s32Ret = HI_FAILURE;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu32Freq);

    HI_TUNER_CHECKPOINTER(pu32Symb);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    { 
        s32Ret = hi3136_get_freq_symb_offset(u32TunerPort, pu32Freq, pu32Symb);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_freq_symb_offset failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
                ||(g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_get_freq_symb_offset(u32TunerPort, pu32Freq, pu32Symb);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_freq_symb_offset failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {     
        s32Ret = hiDVBC_get_freq_symb_offset(u32TunerPort, pu32Freq, pu32Symb);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_freq_symb_offset failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_freq_symb_offset is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_ber(HI_U32 u32TunerPort, HI_U32 * pu32BERa)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);
 
    HI_TUNER_CHECKPOINTER(pu32BERa);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_get_ber(u32TunerPort, pu32BERa);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_ber failed.\n");
            return s32Ret;
        }
        
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                 (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_get_ber(u32TunerPort, pu32BERa);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_ber failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
        || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {     
        s32Ret = hiDVBC_get_ber(u32TunerPort, pu32BERa);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_ber failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_ber is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 hi3138_get_snr(HI_U32 u32TunerPort, HI_U32 * pu32SNR)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    HI_TUNER_CHECKPOINTER(pu32SNR);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_get_snr(u32TunerPort, pu32SNR);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_snr failed.\n");
            return s32Ret;
        }
        
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_get_snr(u32TunerPort, pu32SNR);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_snr failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {     
        s32Ret = hiDVBC_get_snr(u32TunerPort, pu32SNR);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_snr failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_snr is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_signal_strength(HI_U32 u32TunerPort, HI_U32 * pu32strength)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    HI_TUNER_CHECKPOINTER(pu32strength);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_get_signal_strength(u32TunerPort, pu32strength);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_signal_strength failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        s32Ret = hi3137_get_signal_strength(u32TunerPort, pu32strength);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_signal_strength failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {     
        s32Ret = hiDVBC_get_signal_strength(u32TunerPort, pu32strength);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_signal_strength failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_signal_strength is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_U8 u8Temp = 0;
    
    HI_TUNER_CHECKPORT(u32TunerPort);
    
    HI_TUNER_CHECKPOINTER(pstInfo);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_SAT)
    {        
        s32Ret = hi3136_get_signal_info(u32TunerPort, pstInfo);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3136_get_signal_info failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2))
    {
        s32Ret = hi3137_get_signal_info(u32TunerPort, pstInfo);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3137_get_signal_info failed.\n");
            return s32Ret;
        }
    }
    else if (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T)
    {
        pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_ISDB_T;

        /*获取层是否存在的信息*/
        qam_read_byte(u32TunerPort, HI3138_T_PLP_CTRL, &u8Temp);
        u8Temp &= (7 << 5);
        u8Temp = u8Temp >> 5;
        pstInfo->unSignalInfo.stIsdbT.unISDBTLayers.ExistanceFlag = u8Temp;
        HI_INFO_TUNER("-->IN--hi3137_get_signal_info,ExistanceFlag:%d,u8LayerAExist:%d,u8LayerBExist:%d,u8LayerCExist:%d.\n",
            pstInfo->unSignalInfo.stIsdbT.unISDBTLayers.ExistanceFlag,
            pstInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerAExist,
            pstInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerBExist,
            pstInfo->unSignalInfo.stIsdbT.unISDBTLayers.bits.u8LayerCExist);

        /*获取层参数信息*/
        s32Ret = hi3138_get_TMCC_info(u32TunerPort, &(pstInfo->unSignalInfo.stIsdbT.stISDBTTMCCInfo));
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hi3138_get_TMCC_info failed.\n");
            return s32Ret;
        }
    }
    else if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_CAB)
                || (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_J83B))
    {
        s32Ret = hiDVBC_get_signal_info(u32TunerPort, pstInfo);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_TUNER("-->IN--hiDVBC_get_signal_info failed.\n");
            return s32Ret;
        }
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_get_signal_info is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_data_sample(HI_U32 u32TunerPort, TUNER_DATA_SRC_E enDataSrc, HI_U32 u32DataLen, HI_UNF_TUNER_SAMPLE_DATA_S *pstData)
{
    return hi3136_get_data_sample(u32TunerPort, enDataSrc, u32DataLen, pstData);
}


HI_VOID hi3138_read_tuner_registers(HI_U32 u32TunerPort, void *p)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 i;
    HI_U8 u8Data = 0;

    /*打印当前信号标准的寄存器*/
    PROC_PRINT(p,"------------------Tuner Port:%d Reg Info---------------\n", u32TunerPort);
    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return;
    }
    
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

    /*打印IPIO寄存器*/
    PROC_PRINT(p,"-------------------IPIO Reg Info----------------------\n");
    HI3138_IPIO_CINFIG(u32TunerPort);
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

    /*此处的代码为了方便调试,使用cat /proc/msp/tuner_reg命令显示*/
    for (i = 0; i < (sizeof(stHi3138Allocation)/sizeof(stHi3138Allocation[0])); i++)
    {
        PROC_PRINT(p,"------------------------------------------------------\n");
        PROC_PRINT(p,"stHi3138Allocation[%d].u32TunerPort:%d\n",i,stHi3138Allocation[i].u32TunerPort);
        PROC_PRINT(p,"stHi3138Allocation[%d].enHi3138SigType:%d\n",i,stHi3138Allocation[i].enHi3138SigType);
        PROC_PRINT(p,"stHi3138Allocation[%d].enADCId:%d\n",i,stHi3138Allocation[i].enADCId);
        PROC_PRINT(p,"stHi3138Allocation[%d].enInnerDemodId:%d\n",i,stHi3138Allocation[i].enInnerDemodId);
        PROC_PRINT(p,"stHi3138Allocation[%d].enTSOutputId:%d\n",i,stHi3138Allocation[i].enTSOutputId);
        PROC_PRINT(p,"stHi3138Allocation[%d].enI2CGateWaySel:%d\n",i,stHi3138Allocation[i].enI2CGateWaySel);
        PROC_PRINT(p,"stHi3138Allocation[%d].enIFOut:%d\n",i,stHi3138Allocation[i].enIFPath);
        PROC_PRINT(p,"stHi3138Allocation[%d].enTsType:%d\n",i,stHi3138Allocation[i].enTsType);
        PROC_PRINT(p,"stHi3138Allocation[%d].enSyncPin:%d\n",i,stHi3138Allocation[i].enSyncPin);
        PROC_PRINT(p,"stHi3138Allocation[%d].enLNBSrc:%d\n",i,stHi3138Allocation[i].enLNBSrc);
    }
    return;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

