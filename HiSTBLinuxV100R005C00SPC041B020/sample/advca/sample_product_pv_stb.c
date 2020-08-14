/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_type.h"
#include "hi_common.h"
#include "hi_unf_advca.h"
#include "hi_unf_ecs.h"
#include "sample_ca_common.h"

/***************************** Macro Definition ******************************/
#define MAX_OTP_SIZE    0x800

typedef enum hiADVCA_PV_TYPE_E
{
    MSID = 0, 
    Version_ID,
    ITCSA3_IMLB,    //only for irdeto
    STB_SN_0,

    boot_mode_sel,
    bootsel_ctrl,

    link_prt_disable,
    ts_csa2_hardonly_en,
    ts_sp_hardonly_en,
    ts_csa3_hardonly_en,
    tskl_csa2_disable,
    tskl_csa3_disable,
    tskl_sp_disable,
    tskl_sm4_disable,
    tskl_tdes_disable,
    dcas_kl_disable,
    misc_kl_disable,
    gg_kl_disable,
    vmx_bl_fuse,    //only for verimatrix advanced
    itcsa3_enable,  //only for irdeto

    runtime_check_en,
    ddr_wakeup_disable,
    wakeup_ddr_check_en,
    version_id_check_en,
    bl_msid_check_en,

    jtag_prt_mode,
    jtag_r_disable,
    boot_info_disable,

    /*Followed items should be set carefully and in the final*/
    ethernet_disable,
    uart_disable,
    usb_disable,
    bload_dec_en,
    bload_enc_disable,
    self_boot_disable,
    soc_tz_enable,
    otp_tz_area_enable,

    SCS_activation,

    //otp_globle_lock_en,    /*Should be set at last, otherwise the OTP can not be writed, the Verimatrix Chipset should not set this bit */

    HI_UNF_ADVCA_PV_TYPE_BUTT
}HI_ADVCA_PV_TYPE_E;

/*************************** Structure Definition ****************************/

typedef struct{
    HI_CHAR name[32];
    HI_ADVCA_PV_TYPE_E  type;
    HI_U32  datavalue;
}HI_OTP_PV_STRUCT_S;

/********************** Global Variable declaration **************************/


HI_OTP_PV_STRUCT_S stDefaultPV[HI_UNF_ADVCA_PV_TYPE_BUTT]={

    /*DATA Configuration*/
    {"MSID",                MSID,       0x0},
    {"Version_ID",          Version_ID, 0x0},
    {"ITCSA3_IMLB",         ITCSA3_IMLB, 0x0},
    {"STB_SN_0",            STB_SN_0,     0x0},

    /* Lockable fuses Configuration*/
    {"boot_mode_sel",       boot_mode_sel,  0x0},
    {"bootsel_ctrl",        bootsel_ctrl,   0x0},   /*setted with boot_mode_sel together */

    {"link_prt_disable",    link_prt_disable,   0x0},
    {"ts_csa2_hardonly_en", ts_csa2_hardonly_en,    0x0},
    {"ts_sp_hardonly_en",   ts_sp_hardonly_en,  0x0},
    {"ts_csa3_hardonly_en", ts_csa3_hardonly_en,    0x0},
    {"tskl_csa2_disable",   tskl_csa2_disable,    0x0},
    {"tskl_csa3_disable",   tskl_csa3_disable,    0x0},
    {"tskl_sp_disable",     tskl_sp_disable,    0x0},
    {"tskl_sm4_disable",    tskl_sm4_disable,   0x0},
    {"tskl_tdes_disable",   tskl_tdes_disable,  0x0},
    {"dcas_kl_disable",     dcas_kl_disable,    0x0},
    {"misc_kl_disable",     misc_kl_disable,    0x0},
    {"gg_kl_disable",       gg_kl_disable,      0x0},
    {"vmx_bl_fuse",         vmx_bl_fuse,        0x0},
    {"itcsa3_enable",       itcsa3_enable,      0x0},
        
    {"runtime_check_en",    runtime_check_en,   0x0},
    {"ddr_wakeup_disable",  ddr_wakeup_disable, 0x0},
    {"wakeup_ddr_check_en", wakeup_ddr_check_en,    0x0},
    {"version_id_check_en", version_id_check_en,    0x0},
    {"bl_msid_check_en",    bl_msid_check_en,   0x0},

    /* OneWay Configuration*/
    {"jtag_prt_mode",    jtag_prt_mode,     0x0},
    {"jtag_r_disable",   jtag_r_disable,    0x0},
    {"boot_info_disable",   boot_info_disable,    0x0},

    /*Followed items should be set carefully and in the final*/

    /* Lockable fuses Configuration*/
    {"ethernet_disable",    ethernet_disable,   0x0},
    {"uart_disable",        uart_disable,   0x0},
    {"usb_disable",        usb_disable,   0x0},
    {"bload_dec_en",        bload_dec_en,   0x0},
    {"bload_enc_disable",   bload_enc_disable, 0x0},
    {"self_boot_disable",   self_boot_disable,  0x0},
    {"soc_tz_enable",       soc_tz_enable,      0x0},
    {"otp_tz_area_enable",  otp_tz_area_enable, 0x0},

    /* OneWay Configuration*/
    {"SCS_activation",      SCS_activation,     0x0},  

    /* Lockable fuses Configuration*/
    //{"otp_globle_lock_en",  otp_globle_lock_en,  0x0}        /*Should be set at last, otherwise the OTP can not be writed*/
};

extern HI_S32 HI_UNF_ADVCA_DisableLinkProtection(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_LockSPHardCW(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_LockCSA3HardCW(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_DisableDCasKl(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_EnableRuntimeCheck(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_DisableDDRWakeup(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_EnableDDRWakeupCheck(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_DisableJtagRead(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_LockGlobalOTP(HI_VOID);
extern HI_S32 HI_UNF_ADVCA_GetCSA3HardCWStat(HI_BOOL *pbLock);
extern HI_S32 HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_TYPE_E enType, HI_U8 *pu8ParamIn, HI_U32 u32ParamLen);
extern HI_S32 HI_UNF_ADVCA_OtpRead(HI_U32 Addr, HI_U32 *pValue);
extern HI_S32 HI_MPI_OTP_TEST(HI_U8 u8TestValue[16]);
extern HI_S32 HI_UNF_ADVCA_SetTZEnable(HI_BOOL bValue);
extern HI_S32 HI_UNF_ADVCA_GetTZEnStatus(HI_BOOL *pbEn);

/******************************* API declaration *****************************/
static HI_S32 SetPVItemValue(HI_OTP_PV_STRUCT_S *pstPVcfg)
{
    HI_U32 num = sizeof(stDefaultPV)/sizeof(HI_OTP_PV_STRUCT_S);
    HI_U32 i;
    
    HI_DEBUG_ADVCA("%-32s  0x%x\n", pstPVcfg->name, pstPVcfg->datavalue);
    for(i=0; i<num; i++)
    {
        if(0==strncmp(pstPVcfg->name, stDefaultPV[i].name, strlen(stDefaultPV[i].name)))
        {
            stDefaultPV[i].datavalue = pstPVcfg->datavalue;
            return HI_SUCCESS;
        }
    }
    return HI_FAILURE;
}

static HI_S32 GetOTPItemValue(FILE * fp_pv_out)
{
    HI_U32 num = sizeof(stDefaultPV)/sizeof(HI_OTP_PV_STRUCT_S);
    HI_OTP_PV_STRUCT_S   *pstPVItem;
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    
    for(i=0; i<num; i++)
    {
        pstPVItem = &stDefaultPV[i];
        pstPVItem->datavalue = 0;
        if(pstPVItem->type == MSID)
        {
            HI_U8 u8MarketId[4];
            ret = HI_UNF_ADVCA_GetMarketId(u8MarketId);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = (u8MarketId[0]<<24)|(u8MarketId[1]<<16)|(u8MarketId[2]<<8)|u8MarketId[3];
            }
        }
        else if(pstPVItem->type == Version_ID)
        {
            HI_U8 u8VersionId[4];
            ret = HI_UNF_ADVCA_GetVersionId(u8VersionId);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = (u8VersionId[0]<<24)|(u8VersionId[1]<<16)|(u8VersionId[2]<<8)|u8VersionId[3];
            }
        }
        else if(pstPVItem->type == ITCSA3_IMLB)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_ITCSA3_IMLB, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = (stOtpFuseAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[0]<<8)
                                        |stOtpFuseAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[1];
            }
        }
        else if(pstPVItem->type == boot_mode_sel)
        {
            //HI_BOOL bSecEnable;
            //ret = HI_UNF_ADVCA_GetSecBootStat(&bSecEnable, (HI_UNF_ADVCA_FLASH_TYPE_E *) &pstPVItem->datavalue);
            
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType;
            }
        }
        else if(pstPVItem->type == bootsel_ctrl)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, &stOtpFuseAttr);
            if((HI_SUCCESS == ret)&&(stOtpFuseAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType < HI_UNF_ADVCA_FLASH_TYPE_BUTT))
            {
                pstPVItem->datavalue = 1;
            }
        }
        else if(pstPVItem->type == link_prt_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_LP_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == ts_csa2_hardonly_en)
        {
            //ret = HI_UNF_ADVCA_GetHardCwSelStat((HI_BOOL *) &pstPVItem->datavalue);
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == ts_sp_hardonly_en)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == ts_csa3_hardonly_en)
        {
            //ret = HI_UNF_ADVCA_GetCSA3HardCWStat((HI_BOOL *) &pstPVItem->datavalue);
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == tskl_csa2_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == tskl_csa3_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == tskl_sp_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == tskl_sm4_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == tskl_tdes_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == dcas_kl_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == misc_kl_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == gg_kl_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == vmx_bl_fuse)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_VMX_BL_FUSE, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == itcsa3_enable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == runtime_check_en)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == ddr_wakeup_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == wakeup_ddr_check_en)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == version_id_check_en)
        {
            ret = HI_UNF_ADVCA_GetVersionCheckStat(&pstPVItem->datavalue);
        }
        else if(pstPVItem->type == bl_msid_check_en)
        {
            ret = HI_UNF_ADVCA_GetBootMSIDCheckStat(&pstPVItem->datavalue);
        }
        else if(pstPVItem->type == jtag_prt_mode)
        {
            ret = HI_UNF_ADVCA_GetJtagMode((HI_UNF_ADVCA_JTAG_MODE_E *) &pstPVItem->datavalue);
        }
        else if(pstPVItem->type == jtag_r_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == boot_info_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == bload_dec_en)
        {
            ret = HI_UNF_ADVCA_GetBootDecEnStat(&pstPVItem->datavalue);
        }
        else if(pstPVItem->type == self_boot_disable)
        {
            ret = HI_UNF_ADVCA_GetSelfBootStat((HI_BOOL *) &pstPVItem->datavalue);
        }
        else if(pstPVItem->type == ethernet_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == uart_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == usb_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_USB_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if(pstPVItem->type == SCS_activation)
        {
//            HI_UNF_ADVCA_FLASH_TYPE_E enFlashType;
//            ret = HI_UNF_ADVCA_GetSecBootStat((HI_BOOL *) &pstPVItem->datavalue, &enFlashType);
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stEnableSecureBoot.bEnable;
            }
        }
#if 0         
        else if(pstPVItem->type == otp_globle_lock_en)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
#endif        
        else if (pstPVItem->type == bload_enc_disable)
        {
            HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
            memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
            ret = HI_UNF_ADVCA_GetOtpFuse(HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION, &stOtpFuseAttr);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            }
        }
        else if (pstPVItem->type == STB_SN_0)
        {
            HI_U8 u8STBSN[4];
            ret = HI_UNF_ADVCA_GetStbSn(u8STBSN);
            if(HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = (u8STBSN[0]<<24)|(u8STBSN[1]<<16)|(u8STBSN[2]<<8)|u8STBSN[3];
            }
        }
        else if (pstPVItem->type == soc_tz_enable)
        {
            HI_BOOL bEnable;
            ret = HI_UNF_ADVCA_GetTZEnStatus(&bEnable);
            if (HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = bEnable;
            }
        }
        else if (pstPVItem->type == otp_tz_area_enable)
        {
            HI_BOOL bEnable;
            ret = HI_UNF_ADVCA_GetTZEnStatus(&bEnable);
            if (HI_SUCCESS == ret)
            {
                pstPVItem->datavalue = bEnable;
            }
        }
        else
        {
            HI_DEBUG_ADVCA("Unknown PV Item ---->  %s\n", pstPVItem->name);
        }
        
        if(HI_SUCCESS != ret) 
        {
            HI_DEBUG_ADVCA("Get %s  value failed\n", pstPVItem->name);
        }
        else if(fp_pv_out)
        {
            fprintf(fp_pv_out, "%-32s  0x%x\n", pstPVItem->name, pstPVItem->datavalue);
        }
        else
        {
            HI_DEBUG_ADVCA("%-32s  0x%x\n", pstPVItem->name, pstPVItem->datavalue);
        }
    }

    if(fp_pv_out)
    {
        fprintf(fp_pv_out, "\n***************************************\n");
    }
    else
    {
        HI_DEBUG_ADVCA("\n***************************************\n");
    }

    return ret;
}


static HI_S32 GetOTPValue(FILE * fp_pv_out)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 *buf = HI_NULL;
    HI_U32 i;
	     
    buf = (HI_U32*)malloc(MAX_OTP_SIZE);
    if(HI_NULL == buf)
    {
        HI_DEBUG_ADVCA("malloc buffer error!\n");
        ret = HI_FAILURE;
        goto EXIT_CLOSE;
    }
    memset(buf, 0, MAX_OTP_SIZE / 4);
    
    for(i = 0; i < MAX_OTP_SIZE / 4; i++)
    {
        ret = HI_UNF_ADVCA_OtpRead(i*4, buf + i);
		if(ret != HI_SUCCESS)
		{
			HI_DEBUG_ADVCA("Read OTP error!\n");
			goto EXIT_CLOSE;
		}
    }
    
    if(HI_NULL != fp_pv_out)
    {
//        fwrite(buf,1,MAX_OTP_SIZE,fp_pv_out);
        for(i = 0; i < MAX_OTP_SIZE / 4; i++)
        {
            if(0 == (i % 4))
            {
                fprintf(fp_pv_out, "\n%03x:  ", i*4);
            }
            if(0 == (i%4))
            {
                fprintf(fp_pv_out, " ");
            }
            fprintf(fp_pv_out, "%08x ", buf[i]);
        }
    }
    else
    {
        for(i = 0; i < MAX_OTP_SIZE / 4; i++)
        {
            if(0 == (i % 4))
            {
                printf("\n%03x:  ", i*4);
            }
            if(0 == (i % 4))
            {
                printf(" ");
            }
            printf("%08x ", buf[i]);
        }
    }

EXIT_CLOSE:    

    if(HI_NULL != buf)
    {
        free(buf);
    }
    return ret;
}


static HI_S32 SetOTPValue(HI_VOID)
{
    HI_U32 num = sizeof(stDefaultPV)/sizeof(HI_OTP_PV_STRUCT_S);
    HI_OTP_PV_STRUCT_S   *pstPVItem;
    HI_U32 i;
    HI_S32 ret = HI_SUCCESS;
    
    for(i=0; i<num; i++)
    {
        pstPVItem = &stDefaultPV[i];
        switch(pstPVItem->type)
        {
            case MSID:
                if(pstPVItem->datavalue != 0)
                {
                    HI_U8 u8MarketId[4];
                    u8MarketId[0] = (pstPVItem->datavalue >> 24) & 0xFF;
                    u8MarketId[1] = (pstPVItem->datavalue >> 16) & 0xFF;
                    u8MarketId[2] = (pstPVItem->datavalue >> 8) & 0xFF;
                    u8MarketId[3] = pstPVItem->datavalue & 0xFF;
                    ret = HI_UNF_ADVCA_SetMarketId(u8MarketId);
                }
            break;

            case Version_ID:
                if(pstPVItem->datavalue != 0)
                {
                    HI_U8 u8VersionId[4];
                    u8VersionId[0] = (pstPVItem->datavalue >> 24) & 0xFF;
                    u8VersionId[1] = (pstPVItem->datavalue >> 16) & 0xFF;
                    u8VersionId[2] = (pstPVItem->datavalue >> 8) & 0xFF;
                    u8VersionId[3] = pstPVItem->datavalue & 0xFF;
                    ret = HI_UNF_ADVCA_SetVersionId(u8VersionId);
                }
            break;
            
            case ITCSA3_IMLB:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_VENDORID_E enCaVendorID = 0;
                    ret = HI_UNF_ADVCA_GetVendorID((HI_U32 *)&enCaVendorID);
                    if((HI_SUCCESS == ret)&&(HI_UNF_ADVCA_IRDETO == enCaVendorID))
                    {
                        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                        memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                        stOtpFuseAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[0] = (pstPVItem->datavalue >> 8) & 0xFF;
                        stOtpFuseAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB[1] = pstPVItem->datavalue & 0xFF;
                        ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_ITCSA3_IMLB,&stOtpFuseAttr);
                    }
                }
            break;

            case boot_mode_sel:
            case bootsel_ctrl:
                if((stDefaultPV[boot_mode_sel].datavalue != 0)||(stDefaultPV[bootsel_ctrl].datavalue != 0))
                {
                    ret = HI_UNF_ADVCA_SetFlashTypeEx((HI_UNF_ADVCA_FLASH_TYPE_E)stDefaultPV[boot_mode_sel].datavalue);
                }
            break;

            case link_prt_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_LP_DEACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_DisableLinkProtection();
                }
            break;
            
            case ts_csa2_hardonly_en:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_LockHardCwSel();
                }
            break;

            case ts_sp_hardonly_en:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_LockSPHardCW();
                }
            break;

            case ts_csa3_hardonly_en:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_LockCSA3HardCW();
                }
            break;

            case tskl_csa2_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case tskl_csa3_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_TSKL_CSA3_DISABLE, NULL, 0);
                }
            break;

            case tskl_sp_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case tskl_sm4_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case tskl_tdes_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION,&stOtpFuseAttr);
                }
                break;
                
            case dcas_kl_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_DisableDCasKl();
                }
            break;

            case misc_kl_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_MISC_KL_DISABLE, NULL, 0);
                }
            break;

            case gg_kl_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION,&stOtpFuseAttr);
                    //ret = HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_GG_KL_DISABLE, NULL, 0);
                }
            break;

            case vmx_bl_fuse:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_VENDORID_E enCaVendorID = 0;
                    ret = HI_UNF_ADVCA_GetVendorID((HI_U32 *)&enCaVendorID);
                    if((HI_SUCCESS == ret)&&(HI_UNF_ADVCA_VERIMATRIX == enCaVendorID))
                    {
                        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                        memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                        stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                        ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_VMX_BL_FUSE,&stOtpFuseAttr);
                    }
                }
                break;

            case itcsa3_enable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_VENDORID_E enCaVendorID = 0;
                    ret = HI_UNF_ADVCA_GetVendorID((HI_U32 *)&enCaVendorID);
                    if((HI_SUCCESS == ret)&&(HI_UNF_ADVCA_IRDETO == enCaVendorID))
                    {
                        HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                        memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                        stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                        ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION,&stOtpFuseAttr);
                    }
                }
                break;

            case runtime_check_en:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_EnableRuntimeCheck();
                }
            break;

            case ddr_wakeup_disable:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_DisableDDRWakeup();
                }
            break;

            case wakeup_ddr_check_en:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_EnableDDRWakeupCheck();
                }
            break;

            case version_id_check_en:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_EnableVersionCheck();
                }
            break;

            case bl_msid_check_en:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_EnableBootMSIDCheck();
                }
            break;

            case jtag_prt_mode:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_SetJtagMode((HI_UNF_ADVCA_JTAG_MODE_E)pstPVItem->datavalue);
                }
            break;

            case jtag_r_disable:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_DisableJtagRead();
                }
            break;

            case boot_info_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case bload_dec_en:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_LockBootDecEn();
                }
            break;

            case self_boot_disable:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_DisableSelfBoot();
                }
            break;
            
            case ethernet_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case uart_disable:
                if(pstPVItem->datavalue != 0)
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = HI_TRUE;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION,&stOtpFuseAttr);
                }
                break;
                
            case usb_disable:
                if((pstPVItem->datavalue == 0 ) || (pstPVItem->datavalue == 1))
                {
                    HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                    memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                    stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = pstPVItem->datavalue;
                    ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_USB_DEACTIVATION,&stOtpFuseAttr);
                }
                break;

            case SCS_activation:
                if(pstPVItem->datavalue != 0)
                {
                    ret = HI_UNF_ADVCA_EnableSecBootEx();
                }
                break;
            case bload_enc_disable:
            {
                HI_UNF_ADVCA_OTP_ATTR_S stOtpFuseAttr;
                memset(&stOtpFuseAttr, 0, sizeof(HI_UNF_ADVCA_OTP_ATTR_S));
                stOtpFuseAttr.unOtpFuseAttr.stDefaultAttr.bEnable = pstPVItem->datavalue;
                ret = HI_UNF_ADVCA_SetOtpFuse(HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION,&stOtpFuseAttr);
            }

            break;
            case otp_tz_area_enable:
            {
                ret = HI_UNF_ADVCA_SetTZEnable(pstPVItem->datavalue);
            }
            break;
            case soc_tz_enable:
            {
                ret = HI_UNF_ADVCA_SetTZEnable(pstPVItem->datavalue);    
            }
            break;
            case STB_SN_0:
                if(pstPVItem->datavalue != 0)
                {
                    HI_U8 u8STBSN[4];
                    u8STBSN[0] = (pstPVItem->datavalue >> 24) & 0xFF;
                    u8STBSN[1] = (pstPVItem->datavalue >> 16) & 0xFF;
                    u8STBSN[2] = (pstPVItem->datavalue >> 8) & 0xFF;
                    u8STBSN[3] = pstPVItem->datavalue & 0xFF;
                    ret = HI_UNF_ADVCA_SetStbSn(u8STBSN);
                }
                break;
            default:
                HI_DEBUG_ADVCA("Unknown PV Item ---->  %s\n", pstPVItem->name);
            break;
        }
        
        if(HI_SUCCESS != ret) 
        {
            HI_DEBUG_ADVCA("Set %s  to 0x%x failed\n", pstPVItem->name, pstPVItem->datavalue);
        }
    }

    return ret;
}

static HI_VOID PrintHelpInfo(HI_VOID)
{
    printf("Usage: ./sample_product_pv_stb 0 pv_out_file  ----> get pv value\n");
    printf("Usage: ./sample_product_pv_stb 1 pv_cfg_file [pv_out_file] ---->  set pv value\n");
    return;
}

HI_S32 main(HI_S32 argc,HI_CHAR **argv)
{
    HI_S32 Ret = HI_SUCCESS;

    FILE * fp_pv_in = HI_NULL;
    FILE * fp_pv_out = HI_NULL;
    HI_CHAR *pFileNamePVCfg = HI_NULL;
    HI_CHAR *pFileNamePVOut = HI_NULL;
    HI_OTP_PV_STRUCT_S stPVCustomerCfg;
    HI_U32 operationmode = 0;
    HI_S32 count;

    if(argc < 3)
    {
        PrintHelpInfo();
        return HI_FAILURE;
    }

    operationmode = strtol(argv[1],NULL,0);
    
    if(0 == operationmode)
    {
        pFileNamePVOut  = argv[2];
    }
    else if(argc < 3)
    {
        PrintHelpInfo();
        return HI_FAILURE;
    }
    else
    {
        pFileNamePVCfg  = argv[2];
        if(argc > 3)
        {
            pFileNamePVOut  = argv[3];
        }
    }

    if(HI_NULL != pFileNamePVCfg)
    {
        fp_pv_in = fopen(pFileNamePVCfg,"rb");
        if(!fp_pv_in)
        {
            HI_DEBUG_ADVCA("open file %s error!\n", pFileNamePVCfg);
            Ret = HI_FAILURE;
            goto EXIT_CLOSE_FILE;
        }
    }

    if(HI_NULL != pFileNamePVOut)
    {
        fp_pv_out = fopen(pFileNamePVOut,"w");
        if(!fp_pv_out)
        {
            HI_DEBUG_ADVCA("open file %s error!\n",pFileNamePVOut);
            Ret = HI_FAILURE;
            goto EXIT_CLOSE_FILE;
        }
    }

    if(HI_NULL != fp_pv_in)
    {
        HI_DEBUG_ADVCA("\n*********** Get customer PV config list***********\n");
        while(1)
        {
            count = fscanf(fp_pv_in, "%32s %16x", stPVCustomerCfg.name, &stPVCustomerCfg.datavalue);
            if(count <= 0)
            {
                //HI_DEBUG_ADVCA("Read to end of the file\n");
                break;
            }
            Ret = SetPVItemValue(&stPVCustomerCfg);
        }    
    }


    Ret = HI_UNF_ADVCA_Init();
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("call HI_UNF_ADVCA_Init() failed, ret=0x%x\n", Ret);
    }

    if(0 != operationmode)
    {
        HI_DEBUG_ADVCA("\n********  SET OTP VALUE *********\n");
        Ret = SetOTPValue();
    }

    {
        HI_DEBUG_ADVCA("\n********  GET OTP VALUE *********\n");
        Ret = GetOTPItemValue(fp_pv_out);
        Ret = GetOTPValue(fp_pv_out);
    }

    HI_UNF_ADVCA_DeInit();

EXIT_CLOSE_FILE:
    if(HI_NULL != fp_pv_in)
    {
        fclose(fp_pv_in);
    }
    if(HI_NULL != fp_pv_out)
    {
        fclose(fp_pv_out);
    }

    HI_DEBUG_ADVCA("\r\n************ Set/Get PV List End, Ret=0x%x *************\n\n", Ret);
    
    return Ret;
}


