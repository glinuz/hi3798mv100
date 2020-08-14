/***********************************************************************************
*
*  Copyright (C) 2015 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
* FileName: hiDVB_T_T2.c
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
#include "hi3137.h"
#include "hi3138.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */


HI_S32 hi3138_tp_scan(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_SCAN_ATTR_S *pstScanAttr, HI_UNF_TUNER_TER_TPINFO_S *pstTpInfo)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstScanAttr);
    
    HI_TUNER_CHECKPOINTER(pstTpInfo);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_tp_scan(u32TunerPort,pstScanAttr,pstTpInfo);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_tp_scan failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId, HI_U32 u32Mode)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_setplpid(u32TunerPort, u8PLpId, u32Mode);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_setplpid failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_common_plp_id(HI_U32 u32TunerPort, HI_U8 u8PLpId)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_set_common_plp_id(u32TunerPort, u8PLpId);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_set_common_plp_id failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_set_common_plp_combination(HI_U32 u32TunerPort, HI_U8 u8ComPlpEna)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_set_common_plp_combination(u32TunerPort, u8ComPlpEna);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_set_common_plp_combination failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_plp_num(HI_U32 u32TunerPort, HI_U8 *pu8PLPNum)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pu8PLPNum);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_get_plp_num(u32TunerPort, pu8PLPNum);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_get_plp_num failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_current_plp_type(HI_U32 u32TunerPort, HI_UNF_TUNER_T2_PLP_TYPE_E *pstPLPType)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(pstPLPType);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_get_current_plp_type(u32TunerPort, pstPLPType);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_get_current_plp_type failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_plp_id(HI_U32 u32TunerPort, HI_U8 *u8PlpId)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(u8PlpId);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_get_plp_id(u32TunerPort, u8PlpId);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_get_plp_id failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 hi3138_get_plp_group_id(HI_U32 u32TunerPort, HI_U8 *u8GrpPlpId)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_TUNER_CHECKPOINTER(u8GrpPlpId);

    s32Ret = hi3138_i2c_chn_sel(u32TunerPort);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3138_i2c_chn_sel failed.\n");
        return s32Ret;
    }

    s32Ret = hi3137_get_plp_group_id(u32TunerPort, u8GrpPlpId);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER("-->IN--hi3137_get_plp_group_id failed.\n");
        return s32Ret;
    }
    
    return HI_SUCCESS;
}


HI_S32 hi3138_set_antena_power(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ANTENNA_POWER_E enPower)
{
    HI_U8 u8Temp = 0;

    HI_TUNER_CHECKPORT(u32TunerPort);

    HI_INFO_TUNER("u32TunerPort:%d,enPower:%d\n",u32TunerPort,enPower);

    HI3138_IPIO_CINFIG(u32TunerPort);

    if ((g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T) ||
                (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_DVB_T2) ||
                 (g_stTunerOps[u32TunerPort].enSigType == HI_UNF_TUNER_SIG_TYPE_ISDB_T))
    {
        
        qam_read_byte(u32TunerPort, HI3138_IO_CTRL17, &u8Temp);

        switch(enPower)
        {
            case HI_UNF_TUNER_TER_ANTENNA_POWER_OFF:
                u8Temp &= ~(1 << 2);
                break;
            case HI_UNF_TUNER_TER_ANTENNA_POWER_ON:
                u8Temp |= (1 << 2);
                break;
            default:
                HI_ERR_TUNER("antena power value err.\n");
                return HI_FAILURE;
        }
        qam_write_byte(u32TunerPort, HI3138_IO_CTRL17, u8Temp);
    }
    else
    {
        HI_ERR_TUNER("-->IN--hi3138_set_antena_power is not support g_stTunerOps[%d].enSigType:%d\n",u32TunerPort,g_stTunerOps[u32TunerPort].enSigType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

