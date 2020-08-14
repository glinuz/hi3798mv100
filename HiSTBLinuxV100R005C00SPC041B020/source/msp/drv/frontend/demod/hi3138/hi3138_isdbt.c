/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hiISDBT.c
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
#include "hi3137.h"
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */



static HI_S32 hiTer_get_layer_info(HI_U16 u16LayerReg, HI_UNF_TUNER_ISDBT_LAYERS_INFO_S *pstIsdbtLayersInfoBits)
{
    HI_TUNER_CHECKPOINTER(pstIsdbtLayersInfoBits);

    pstIsdbtLayersInfoBits->u8LayerSegNum = u16LayerReg & 0xf;
    if (0xf == pstIsdbtLayersInfoBits->u8LayerSegNum)
    {
        pstIsdbtLayersInfoBits->u8LayerSegNum = 0;
    }

    HI_INFO_TUNER("-->IN--hiTer_get_layer_info,u16LayerReg:0x%x\n",u16LayerReg);

    HI_INFO_TUNER("-->IN--hiTer_get_layer_info,u16LayerReg_enLayerModType:0x%x\n",u16LayerReg & (0x7 << 10)>> 10);

    switch ((u16LayerReg & (0x7 << 10)) >> 10)
    {
    	case 0:
    		pstIsdbtLayersInfoBits->enLayerModType = HI_UNF_MOD_TYPE_DQPSK;
    		break;
    	case 1:
    		pstIsdbtLayersInfoBits->enLayerModType = HI_UNF_MOD_TYPE_QPSK;
    		break;
    	case 2:
    		pstIsdbtLayersInfoBits->enLayerModType = HI_UNF_MOD_TYPE_QAM_16;
    		break;
    	case 3:
    		pstIsdbtLayersInfoBits->enLayerModType = HI_UNF_MOD_TYPE_QAM_64;
    		break;
    	default:
    		pstIsdbtLayersInfoBits->enLayerModType = HI_UNF_MOD_TYPE_DEFAULT;
    		break;
    }
    HI_INFO_TUNER("-->IN--hiTer_get_layer_info,u16LayerReg_enLayerFECRate:0x%x\n",u16LayerReg & (0x7 << 7)>> 7);

    switch((u16LayerReg & (0x7 << 7)) >> 7)
    {
    	case 0:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_1_2;
    		break;
    	case 1:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_2_3;
    		break;
    	case 2:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_3_4;
    		break;
    	case 3:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_5_6;
    		break;
    	case 4:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_7_8;
    		break;
    	default:
    		pstIsdbtLayersInfoBits->enLayerFECRate = HI_UNF_TUNER_FE_FEC_AUTO;
    		break;
    }
    HI_INFO_TUNER("-->IN--hiTer_get_layer_info,u16LayerReg_enLayerHierMod:0x%x\n",u16LayerReg & (0x7 << 4)>> 4);

    switch((u16LayerReg & (0x7 << 4)) >> 4)
    {
    	case 0:
    		pstIsdbtLayersInfoBits->enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_NO;
    		break;
    	case 1:
    		pstIsdbtLayersInfoBits->enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA1;
    		break;
    	case 2:
    		pstIsdbtLayersInfoBits->enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA2;
    		break;
    	case 3:
    		pstIsdbtLayersInfoBits->enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_ALHPA4;
    		break;
      default:
    		pstIsdbtLayersInfoBits->enLayerHierMod = HI_UNF_TUNER_FE_HIERARCHY_NO;
    		break;
    }

    HI_INFO_TUNER("-->IN--hiTer_get_layer_info,u8LayerSegNum:%d,enLayerModType:0x%x,enLayerFECRate:%d,enLayerHierMod:%d\n",
                            pstIsdbtLayersInfoBits->u8LayerSegNum,
                            pstIsdbtLayersInfoBits->enLayerModType,
                            pstIsdbtLayersInfoBits->enLayerFECRate,
                            pstIsdbtLayersInfoBits->enLayerHierMod);

    return HI_SUCCESS;
}

HI_S32 hiISDBT_monitor_layer(HI_U32 u32TunerPort, HI_UNF_TUNER_ISDBT_MONITOR_LAYER_E enMonitorLayer)
{
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);
    
    /*œ»«Â¡„*/
    qam_read_byte(u32TunerPort, HI3138_T_PLP_CTRL, &u8Temp);
    u8Temp &= ~(7 << 5);
    qam_write_byte(u32TunerPort, HI3138_T_PLP_CTRL, u8Temp);

    switch (enMonitorLayer)
    {
        case HI_UNF_TUNER_ISDBT_MONITOR_LAYER_A:
                u8Temp |= 1 << 5;
                break;
        case HI_UNF_TUNER_ISDBT_MONITOR_LAYER_B:
                u8Temp |= 1 << 6;
                break;
        case HI_UNF_TUNER_ISDBT_MONITOR_LAYER_C:
                u8Temp |= 1 << 7;
                break;
        case HI_UNF_TUNER_ISDBT_MONITOR_LAYER_ALL:
        /*now jump to default*/
        default:
                u8Temp |= 7 << 5;
                break;
    }

    HI_INFO_TUNER("-->IN--hi3138_monitor_ISDBT_layer,HI3138_T_PLP_CTRL:0x%x\n",u8Temp);
    qam_write_byte(u32TunerPort, HI3138_T_PLP_CTRL, u8Temp);
    
    return HI_SUCCESS;

}

HI_S32 hiISDBT_get_TMCC_info(HI_U32 u32TunerPort, HI_UNF_TUNER_TMCC_INFO_S *pstTMCCInfo)
{
    HI_U8 hier_a_l = 0,hier_a_h = 0,hier_b_l = 0,hier_b_h = 0,hier_c_l = 0,hier_c_h = 0;
    HI_U16 hier_a = 0,hier_b = 0,hier_c = 0;
    HI_UNF_TUNER_TMCC_INFO_S stTMCCInfo;
    HI_U8 u8Temp = 0;
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstTMCCInfo);

    memset(&stTMCCInfo,0,sizeof(HI_UNF_TUNER_TMCC_INFO_S));
    
    qam_read_byte(u32TunerPort, HI3138_T_TMCC1, &u8Temp);
    stTMCCInfo.u8EmergencyFlag = (u8Temp & 0x80) >> 7;
    stTMCCInfo.u8PhaseShiftCorr= u8Temp & 0x07;
    
    qam_read_byte(u32TunerPort, HI3138_T_TMCC2, &hier_a_l);
    qam_read_byte(u32TunerPort, HI3138_T_TMCC3, &hier_a_h);
    stTMCCInfo.u8PartialFlag = (hier_a_h & 0x80) >> 7;
    hier_a_h &= (0x1f);
    hier_a = (hier_a_h << 8) + hier_a_l;
    
    qam_read_byte(u32TunerPort, HI3138_T_TMCC4, &hier_b_l);
    qam_read_byte(u32TunerPort, HI3138_T_TMCC5, &hier_b_h);
    hier_b_h &= (0x1f);
    hier_b = (hier_b_h << 8) + hier_b_l;
    
    qam_read_byte(u32TunerPort, HI3138_T_TMCC6, &hier_c_l);
    qam_read_byte(u32TunerPort, HI3138_T_TMCC7, &hier_c_h);
    hier_c_h &= (0x1f);
    hier_c = (hier_c_h << 8) + hier_c_l;

    s32Ret = hiTer_get_layer_info(hier_a, (HI_UNF_TUNER_ISDBT_LAYERS_INFO_S *)&(stTMCCInfo.stIsdbtLayersAInfoBits));
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hiTer_get_layer_info A failed.\n");
        return s32Ret;
    }

    s32Ret = hiTer_get_layer_info(hier_b, (HI_UNF_TUNER_ISDBT_LAYERS_INFO_S *)&(stTMCCInfo.stIsdbtLayersBInfoBits));
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hiTer_get_layer_info B failed.\n");
        return s32Ret;
    }

    s32Ret = hiTer_get_layer_info(hier_c, (HI_UNF_TUNER_ISDBT_LAYERS_INFO_S *)&(stTMCCInfo.stIsdbtLayersCInfoBits));
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hiTer_get_layer_info C failed.\n");
        return s32Ret;
    }

    HI_INFO_TUNER("-->IN--hiISDBT_get_TMCC_info,u8EmergencyFlag:%d,u8PhaseShiftCorr:%d,u8PartialFlag:%d\n",
                        stTMCCInfo.u8EmergencyFlag,
                        stTMCCInfo.u8PhaseShiftCorr,
                        stTMCCInfo.u8PartialFlag);
        
    memcpy(pstTMCCInfo,&stTMCCInfo,sizeof(HI_UNF_TUNER_TMCC_INFO_S));

    HI_INFO_TUNER("enLayerModType:%d,enLayerFECRate:%d,enLayerHierMod:%d\n",
        pstTMCCInfo->stIsdbtLayersAInfoBits.enLayerModType,
        pstTMCCInfo->stIsdbtLayersAInfoBits.enLayerFECRate,
        pstTMCCInfo->stIsdbtLayersAInfoBits.enLayerHierMod);
    
    return HI_SUCCESS;

}

HI_S32 hiISDBT_connect(HI_U32 u32TunerPort,TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_U8 u8Temp = 0,u8BandWidth = 0;
    HI_U32 i,u32Cnt = 75;
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstChannel);

    /*≈‰÷√¥¯øÌ*/
    qam_read_byte(u32TunerPort, HI3138_T_BAND_WIDTH, &u8Temp);
    switch(pstChannel->unSRBW.u32BandWidth)
    {
    	case 6000:
            u8BandWidth = 6;
            break;
    	case 7000:
            u8BandWidth = 7;
            break;
    	default:
            u8BandWidth = 8;
            break;
    }
    u8Temp &= ~(0xf << 4);
    u8Temp |= (u8BandWidth << 4);
    qam_write_byte(u32TunerPort, HI3138_T_BAND_WIDTH, u8Temp);
    
    /*≈‰÷√∆•≈‰¬À≤®∆˜*/
    qam_read_byte(u32TunerPort, HI3138_T_DAGC_INNER_CTRL, &u8Temp);
    u8Temp &= ~(0x0f << 0);
    u8Temp |= 0x07;
    qam_write_byte(u32TunerPort, HI3138_T_DAGC_INNER_CTRL, u8Temp);

    //≤È—ØtpsÀ¯∂®
    for (i = 0;i < u32Cnt;i++)
    {
        s32Ret = hi3137_get_lock_flag(u32TunerPort, TPS_T_LOCK_FLAG);

        if (s32Ret == HI_SUCCESS)
        {
            break;
        }
        tuner_mdelay(20);
    }

    if (i >= u32Cnt)
    {
        HI_ERR_TUNER("-->IN--hi3138_get_lock_flag tps failed.\n");
        return HI_FAILURE;
    }

    //≤È—ØfecÀ¯∂®
    for (i = 0;i < u32Cnt;i++)
    {
        s32Ret = hi3137_get_lock_flag(u32TunerPort, FEC_LOCK_FLAG);

        if (s32Ret == HI_SUCCESS)
        {
            break;
        }
        tuner_mdelay(20);
    }

    if (i >= u32Cnt)
    {
        HI_ERR_TUNER("-->IN--hi3138_get_lock_flag fec failed.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 hi3138_monitor_layer(HI_U32 u32TunerPort, HI_UNF_TUNER_MONITOR_LAYERS_CONFIG_S *pstMonLayersConfig)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstMonLayersConfig);

    HI_INFO_TUNER("-->IN--hi3138_monitor_ISDBT_layer,pstMonLayersConfig->enMonitorLayer:%d\n",
                            pstMonLayersConfig->enMonitorLayer);
    
    s32Ret = hiISDBT_monitor_layer(u32TunerPort, pstMonLayersConfig->enMonitorLayer);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("call hiISDBT_monitor_layer fail.\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;

}

HI_S32 hi3138_get_TMCC_info(HI_U32 u32TunerPort, HI_UNF_TUNER_TMCC_INFO_S *pstTMCCInfo)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstTMCCInfo);

    s32Ret = hiISDBT_get_TMCC_info(u32TunerPort, pstTMCCInfo);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("call hiISDBT_get_TMCC_info fail.\n");
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;

}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

