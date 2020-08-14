/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_advca_v300.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :

******************************************************************************/
#include "drv_advca_internal.h"
#include "drv_advca_external.h"
#include "asm/io.h"
#define ADVCA_MAGIC_NUM1 (0xa843dcad)
#define ADVCA_MAGIC_NUM2 (0x12f02456)

CA_KEYLADDER_INFO_S g_CSA2Info = {0};
CA_KEYLADDER_INFO_S g_CSA3Info = {0};
CA_KEYLADDER_INFO_S g_R2RInfo  = {0};
CA_KEYLADDER_INFO_S g_SPInfo   = {0};
CA_KEYLADDER_INFO_S g_MiscInfo = {0};
CA_KEYLADDER_INFO_S g_DCASInfo = {0};
CA_KEYLADDER_INFO_S g_TAInfo   = {0};
CA_KEYLADDER_INFO_S g_GDRMInfo = {0};

static HI_BOOL g_IsSWPKKeyLadderOpen = HI_FALSE;

HI_U32 HAL_ADVCA_V300_Open(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKLadder = HI_UNF_ADVCA_KEYLADDER_BUTT;

    memset(&g_CSA2Info, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_CSA2Info.OpenFlag = HI_TRUE;
    g_CSA2Info.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    ret = HAL_ADVCA_V300_GetCWLadderLevel(&enKLadder);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HAL_ADVCA_V300_GetCWLadderLevel failed, ret: 0x%x", ret);
        return HI_FAILURE;
    }
    g_CSA2Info.MaxLevel = enKLadder + 1;

    memset(&g_CSA3Info, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_CSA3Info.OpenFlag = HI_TRUE;
    g_CSA3Info.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    ret = HAL_ADVCA_V300_GetCSA3LadderLevel(&enKLadder);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HAL_ADVCA_V300_GetCSA3LadderLevel failed, ret: 0x%x", ret);
        return HI_FAILURE;
    }
    g_CSA3Info.MaxLevel = enKLadder + 1;

    memset(&g_R2RInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_R2RInfo.OpenFlag = HI_TRUE;
    g_R2RInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    ret = HAL_ADVCA_V300_GetR2RLadderLevel(&enKLadder);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HAL_ADVCA_V300_GetR2RLadderLevel failed, ret: 0x%x", ret);
        return HI_FAILURE;
    }
    g_R2RInfo.MaxLevel = enKLadder + 1;

    memset(&g_SPInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_SPInfo.OpenFlag = HI_FALSE;
    g_SPInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    g_SPInfo.enDscMode = HI_UNF_ADVCA_SP_DSC_MODE_PAYLOAD_AES_CBC_IDSA;
    ret = HAL_ADVCA_V300_GetSPLadderLevel(&enKLadder);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HAL_ADVCA_V300_GetSPLadderLevel failed, ret: 0x%x", ret);
        return HI_FAILURE;
    }
    g_SPInfo.MaxLevel = enKLadder + 1;

    memset(&g_MiscInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_MiscInfo.OpenFlag = HI_FALSE;
    g_MiscInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    ret = HAL_ADVCA_V300_GetMiscKlLevel(&enKLadder);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("call HAL_ADVCA_V300_GetMiscKlLevel failed, ret: 0x%x", ret);
        return HI_FAILURE;
    }
    g_MiscInfo.MaxLevel = enKLadder + 1;

    memset(&g_DCASInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_DCASInfo.OpenFlag = HI_FALSE;
    g_DCASInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    g_DCASInfo.MaxLevel = 0x03;

    memset(&g_TAInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_TAInfo.OpenFlag = HI_FALSE;
    g_TAInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
    g_TAInfo.MaxLevel = 0x03;    //Fixed to 3 level in Transformation Algorithm keyladder

    memset(&g_GDRMInfo, 0x0, sizeof (CA_KEYLADDER_INFO_S));
    g_GDRMInfo.OpenFlag = HI_FALSE;
    g_GDRMInfo.MaxLevel = 0x03;

    return HI_SUCCESS;
}

HI_U32 HAL_ADVCA_V300_Close(HI_VOID)
{
    memset(&g_CSA2Info, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_CSA3Info, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_R2RInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_SPInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_MiscInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_DCASInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_TAInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    memset(&g_GDRMInfo, 0, sizeof(CA_KEYLADDER_INFO_S));

    return HI_SUCCESS;
}

static HI_S32 HAL_ADVCA_V300_WaitStat(HI_VOID)
{
    HI_U32 cnt = 0;
    CA_V300_CA_STATE_U CAStatus;
    HI_S32 errState = 0;

    /* wait for KeyLadder calc done */
    // corform one value when test, about 500
    while (cnt < 50)
    {
        CAStatus.u32 = 0;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        if (CAStatus.bits.klad_busy == 0)
        {
            HI_INFO_CA("ca_decrypted finished, cnt:%d\n", cnt);
            break;
        }
        ca_msleep(10);
        cnt++;
    }

    if (cnt >= 50)
    {
        HI_ERR_CA("\n Error CA handle too long !!!!!!!!!!! \n");
        return HI_FAILURE;      /* time out */
    }

    /* not support 'last_key_not_rdy' */
    errState = CAStatus.bits.err_state;
    return errState;
}


HI_S32 HAL_ADVCA_V300_StatHardCwSel(HI_U32 *pu32Lock)
{
    HI_S32 Ret = HI_SUCCESS;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    //Get the OTP bit "ts_csa2_hardonly_en"
    Ret = DRV_CA_OTP_V200_GetTSCsa2HardonlyEn(pu32Lock);

    return Ret;
}


HI_S32 HAL_ADVCA_V300_StatR2RHardKey(HI_U32 *pu32Lock)
{
    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }
    //R2R decryption ladder lock.
    //*pbLock = g_pCaReg->CaCtrlProc.Bits.R2rHKeyLock;
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_StatTdesLock(HI_U32 *pu32Lock)
{
    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }
    //Whether to lock the multicipher to enable it to use the TDES rather than the DES.
    //*pbLock = g_pCaReg->CaCtrlProc.Bits.TdesLock;
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetCWLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if ((enSel != HI_UNF_ADVCA_KEYLADDER_LEV3) && (enSel != HI_UNF_ADVCA_KEYLADDER_LEV2))
    {
        HI_ERR_CA("CW ladder err, sel = %d ! \n", enSel);
        return HI_ERR_CA_INVALID_PARA;
    }

    enKeyLevel = (CA_OTP_KEY_LEVEL_E)enSel;
    Ret = DRV_CA_OTP_V200_SetCSA2LadderLevel(enKeyLevel);
    if(Ret == HI_SUCCESS)
    {
        g_CSA2Info.MaxLevel = enSel;
    }

    return Ret;
}

HI_S32 HAL_ADVCA_V300_GetCWLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if (NULL == penSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetCSA2LadderLevel(&enKeyLevel);
    *penSel = (HI_UNF_ADVCA_KEYLADDER_LEV_E)enKeyLevel;

    return Ret;
}

HI_S32 HAL_ADVCA_V300_SetR2RLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if ((enSel != HI_UNF_ADVCA_KEYLADDER_LEV3) && (enSel != HI_UNF_ADVCA_KEYLADDER_LEV2))
    {
        HI_ERR_CA("CW ladder err, sel = %d ! \n", enSel);
        return HI_ERR_CA_INVALID_PARA;
    }

    enKeyLevel = (CA_OTP_KEY_LEVEL_E)enSel;
    Ret = DRV_CA_OTP_V200_SetR2RLadderLevel(enKeyLevel);
    if(Ret == HI_SUCCESS)
    {
        g_R2RInfo.MaxLevel = enSel;
    }

    return Ret;
}

HI_S32 HAL_ADVCA_V300_GetR2RLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if (NULL == penSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetR2RLadderLevel(&enKeyLevel);
    *penSel = (HI_UNF_ADVCA_KEYLADDER_LEV_E)enKeyLevel;

    return Ret;
}

HI_S32 HAL_ADVCA_V300_SetSPLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if ((enSel < HI_UNF_ADVCA_KEYLADDER_LEV2) || (enSel >= HI_UNF_ADVCA_KEYLADDER_BUTT))
    {
        HI_ERR_CA("CW ladder err, sel = %d ! \n", enSel);
        return HI_ERR_CA_INVALID_PARA;
    }

    enKeyLevel = (CA_OTP_KEY_LEVEL_E)enSel;
    Ret = DRV_CA_OTP_V200_SetSPLadderLevel(enKeyLevel);
    if(Ret == HI_SUCCESS)
    {
        g_SPInfo.MaxLevel = enSel;
    }
    return Ret;
}

HI_S32 HAL_ADVCA_V300_GetSPLadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if (NULL == penSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetSPLadderLevel(&enKeyLevel);
    *penSel = (HI_UNF_ADVCA_KEYLADDER_LEV_E)enKeyLevel;

    return Ret;
}

HI_S32 HAL_ADVCA_V300_SetCSA3LadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if ((enSel != HI_UNF_ADVCA_KEYLADDER_LEV3) && (enSel != HI_UNF_ADVCA_KEYLADDER_LEV2))
    {
        HI_ERR_CA("CW ladder err, sel = %d ! \n", enSel);
        return HI_ERR_CA_INVALID_PARA;
    }

    enKeyLevel = (CA_OTP_KEY_LEVEL_E)enSel;
    Ret = DRV_CA_OTP_V200_SetCSA3LadderLevel(enKeyLevel);
    if(Ret == HI_SUCCESS)
    {
        g_CSA3Info.MaxLevel = enSel;
    }    return Ret;
}

HI_S32 HAL_ADVCA_V300_GetCSA3LadderLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_OTP_KEY_LEVEL_E enKeyLevel;

    if (NULL == penSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetCSA3LadderLevel(&enKeyLevel);
    *penSel = (HI_UNF_ADVCA_KEYLADDER_LEV_E)enKeyLevel;

    return Ret;
}


HI_S32 HAL_ADVCA_V300_SetCWAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E type)
{
    if ((type != HI_UNF_ADVCA_ALG_TYPE_TDES) && (type != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid CSA2 Keyladder Algorithm (%d)!\n", type);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_CSA2Info.Alg = type;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetCWAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *pAlgType)
{
    if(pAlgType == NULL)
    {
        HI_ERR_CA("pAlgType: NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    *pAlgType = g_CSA2Info.Alg;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetCSA3Algorithm(HI_UNF_ADVCA_ALG_TYPE_E type)
{
    if ((type != HI_UNF_ADVCA_ALG_TYPE_TDES) && (type != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid CSA3 Keyladder Algorithm (%d)!\n", type);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_CSA3Info.Alg = type;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetCSA3Algorithm(HI_UNF_ADVCA_ALG_TYPE_E *pAlgType)
{
    if(pAlgType == NULL)
    {
        HI_ERR_CA("pAlgType: NULL pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    *pAlgType = g_CSA3Info.Alg;
    return HI_SUCCESS;
}


HI_S32 HAL_ADVCA_V300_SetR2RAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E type)
{
    if ((type != HI_UNF_ADVCA_ALG_TYPE_TDES) && (type != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("R2R Algorithm Error = %d ! \n", type);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_R2RInfo.Alg = type;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetR2RAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *type)
{
    if (NULL == type)
    {
	    HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    *type = g_R2RInfo.Alg;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetSPAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    if ((enType != HI_UNF_ADVCA_ALG_TYPE_TDES) && (enType != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid SP Keyladder algorithm (%d)!\n", enType);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_SPInfo.Alg = enType;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetSPAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType)
{
    if (NULL == penType)
    {
	    HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    *penType = g_SPInfo.Alg;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    if ((enType != HI_UNF_ADVCA_ALG_TYPE_TDES) && (enType != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid Misc Keyladder algorithm (%d)!\n", enType);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_MiscInfo.Alg = enType;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType)
{
    if( NULL == penType)
    {
        HI_ERR_CA("Invalid param ,NULL pointer !\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    *penType = g_MiscInfo.Alg;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enSel)
{
    HI_U32 Ret = HI_SUCCESS;
    Ret = DRV_CA_OTP_V200_SetMiscKlLevel(enSel);
    if(Ret == HI_SUCCESS)
    {
        g_MiscInfo.MaxLevel = enSel;
    }
    return Ret;
}

HI_S32 HAL_ADVCA_V300_GetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    if(NULL == penSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    return DRV_CA_OTP_V200_GetMiscKlLevel(penSel);
}

HI_S32 HAL_ADVCA_V300_SetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E eDscMode)
{
    if (eDscMode >= HI_UNF_ADVCA_SP_DSC_MODE_BUTT)
    {
        HI_ERR_CA("Invaild SP Dsc Mode (%d )!\n", eDscMode);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_SPInfo.enDscMode = eDscMode;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetSPDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E *penType)
{
    if (NULL == penType)
    {
	    HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    *penType = g_SPInfo.enDscMode;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetMiscDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E eDscMode)
{
    if (eDscMode >= HI_UNF_ADVCA_SP_DSC_MODE_BUTT)
    {
        HI_ERR_CA("Invaild SP Dsc Mode (%d )!\n", eDscMode);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_MiscInfo.enDscMode = eDscMode;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetMiscDscMode(HI_UNF_ADVCA_SP_DSC_MODE_E *penType)
{
    if (NULL == penType)
    {
	    HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    *penType = g_MiscInfo.enDscMode;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptCw(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bEvenOrOdd)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_CSA2_CTRL_U      CSA2Ctrl;
    CA_OTP_VENDOR_TYPE_E enCaVendorType = CA_OTP_VENDOR_NONE;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLev;

    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetVendorId(&enCaVendorType);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CA vendor ID.\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_GetCWLadderLevel(&enKeyladderLev);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CW ladder level.\n");
        return HI_FAILURE;
    }

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel) //For Irdeto MSR2.2 chipset
    {
        HAL_ADVCA_V300_DecTAKeyLadder(HI_UNF_ADVCA_KEYLADDER_LEV3, pu32DataIn, 16);
    }
    else
    {
        /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
        //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);
    }

    //(3)   配置寄存器CSA2_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    CSA2Ctrl.u32 = 0;
    CSA2Ctrl.bits.dsc_mode = 0;
    CSA2Ctrl.bits.level_sel = enLevel;
    CSA2Ctrl.bits.tdes_aes_sel = g_CSA2Info.Alg;
    CSA2Ctrl.bits.EvenOrOdd = bEvenOrOdd;  //0：当前是偶密钥；1：当前是奇密钥
    CSA2Ctrl.bits.key_addr = AddrID & 0x7F; //Demux DescambleKey ID
    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel) //last level
    {
        CSA2Ctrl.bits.ta_kl_flag = 1; //for Irdeto MSR2.2
    }
    DRV_ADVCA_WriteReg(CA_V300_CSA2_CTRL, CSA2Ctrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (DVB_SYMMETRIC_KEY_ERROR == Ret)
    {
        HI_ERR_CA("Cw key ladder error: symmetric key usage\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);
        return HI_FAILURE;
    }
    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);
        memcpy(g_CSA2Info.SessionKey[enLevel], pu32DataIn, 16);
        g_CSA2Info.SessionKey_sec[enLevel] = tv.tv_sec;

        g_CSA2Info.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

        //0：当前是偶密钥；1：当前是奇密钥
        if(g_CSA2Info.MaxLevel == (enLevel + 1))
        {
            if(bEvenOrOdd == 0x00)
            {
                memcpy(g_CSA2Info.LastEvenKey, pu32DataIn, 16);
                g_CSA2Info.LastEvenKey_secTime = tv.tv_sec;
            }
            else
            {
                memcpy(g_CSA2Info.LastOddKey, pu32DataIn, 16);
                g_CSA2Info.LastOddKey_secTime = tv.tv_sec;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptCsa3(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bEvenOrOdd)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_CSA3_CTRL_U CSA3Ctrl;
    CA_OTP_VENDOR_TYPE_E enCaVendorType = CA_OTP_VENDOR_NONE;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLev;

    /* para check*/
    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetVendorId(&enCaVendorType);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CA vendor ID.\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_GetCSA3LadderLevel(&enKeyladderLev);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CW ladder level.\n");
        return HI_FAILURE;
    }

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }


    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel) //For Irdeto MSR2.2
    {
        HAL_ADVCA_V300_DecTAKeyLadder(HI_UNF_ADVCA_KEYLADDER_LEV3, pu32DataIn, 16);
    }
    else
    {
        /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
        //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);
    }

    //(3)   配置寄存器CSA3_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    CSA3Ctrl.u32 = 0;
    CSA3Ctrl.bits.dsc_mode = 0x10;
    CSA3Ctrl.bits.level_sel = enLevel;
    CSA3Ctrl.bits.tdes_aes_sel = g_CSA3Info.Alg;
    CSA3Ctrl.bits.EvenOrOdd = bEvenOrOdd;
    CSA3Ctrl.bits.key_addr = AddrID & 0x7F; //Demux DescambleKey ID
    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel) //last level
    {
        CSA3Ctrl.bits.ta_kl_flag = 1; //for Irdeto MSR2.2
    }
    DRV_ADVCA_WriteReg(CA_V300_CSA3_CTRL, CSA3Ctrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (DVB_SYMMETRIC_KEY_ERROR == Ret)
    {
        HI_ERR_CA("CSA3 key ladder error: symmetric key usage\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);
        return HI_FAILURE;
    }
    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);
        memcpy(g_CSA3Info.SessionKey[enLevel], pu32DataIn, 16);
        g_CSA3Info.SessionKey_sec[enLevel] = tv.tv_sec;

        g_CSA3Info.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

        //0：当前是偶密钥；1：当前是奇密钥
        if(g_CSA3Info.MaxLevel == (enLevel + 1))
        {
            if(bEvenOrOdd == 0x00)
            {
                memcpy(g_CSA3Info.LastEvenKey, pu32DataIn, 16);
                g_CSA3Info.LastEvenKey_secTime = tv.tv_sec;
            }
            else
            {
                memcpy(g_CSA3Info.LastOddKey, pu32DataIn, 16);
                g_CSA3Info.LastOddKey_secTime = tv.tv_sec;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_CryptR2R(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bIsDeCrypt)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_R2R_CTRL_U R2RCtrl;

    /* para check*/
    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    if(g_IsSWPKKeyLadderOpen)
    {
        HAL_ADVCA_V300_DecryptSWPK(pu32DataIn, AddrID);
        return HI_SUCCESS;
    }

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
    //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);

    //(3)   配置寄存器R2R_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    R2RCtrl.u32 = 0;
    R2RCtrl.bits.level_sel = enLevel;
    R2RCtrl.bits.mc_alg_sel = 0;//Multicipher Algorithm. default setting
    R2RCtrl.bits.tdes_aes_sel = g_R2RInfo.Alg;
    R2RCtrl.bits.key_addr = AddrID & 0xFF; //Demux DescambleKey ID
    DRV_ADVCA_WriteReg(CA_V300_R2R_CTRL, R2RCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (R2R_SP_SYMMETRIC_KEY_ERROR == Ret)
    {
        HI_ERR_CA("R2R key ladder error: symmetric key usage\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }


    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);
        memcpy(g_R2RInfo.SessionKey[enLevel], pu32DataIn, 16);
        g_R2RInfo.SessionKey_sec[enLevel] = tv.tv_sec;

        g_R2RInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

    }
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptSP(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                                HI_U32 *pu32DataIn,
                                HI_U32 AddrID,
                                HI_BOOL bEvenOrOdd,
                                DRV_ADVCA_CA_TARGET_E enCwTarget)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_SP_CTRL_U unSPCtrl;
    CA_OTP_VENDOR_TYPE_E enCaVendorType = CA_OTP_VENDOR_NONE;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLev;

    /* para check*/
    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    Ret = DRV_CA_OTP_V200_GetVendorId(&enCaVendorType);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CA vendor ID.\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_GetSPLadderLevel(&enKeyladderLev);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CW ladder level.\n");
        return HI_FAILURE;
    }

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel) //For Irdeto MSR2.2
    {
        HAL_ADVCA_V300_DecTAKeyLadder(HI_UNF_ADVCA_KEYLADDER_LEV3, pu32DataIn, 16);
    }
    else
    {
        /*2  put SPE_RootKey（CK2）to CA register(DATA input) */
        //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);
    }

    //(3)   配置寄存器SP_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    unSPCtrl.u32 = 0;
    if(HI_UNF_ADVCA_KEYLADDER_LEV5 == enLevel)
    {
        unSPCtrl.bits.level_sel_5 = 1;
    }
    else
    {
        unSPCtrl.bits.level_sel = enLevel;
    }

    unSPCtrl.bits.tdes_aes_sel = g_SPInfo.Alg;
    if(enCwTarget == DRV_ADVCA_CA_TARGET_DEMUX)
    {
        if (0 == (g_SPInfo.enDscMode & 0x4000))
        {
            unSPCtrl.bits.raw_mode = 0;          //payload模式
            unSPCtrl.bits.key_addr = ((AddrID & 0x7F) << 1) + bEvenOrOdd; //Demux DescambleKey ID + even_or_odd
            unSPCtrl.bits.dsc_mode = g_SPInfo.enDscMode;  //set demux DscMode
        }
        else
        {
            unSPCtrl.bits.raw_mode = 1;          //raw模式
            unSPCtrl.bits.key_addr = AddrID & 0xFF; //multicipher channel ID
        }
    }
    else
    {
        unSPCtrl.bits.raw_mode = 1;          //raw模式
        unSPCtrl.bits.key_addr = AddrID & 0xFF; //multicipher channel ID
    }

    if (enCaVendorType == CA_OTP_VENDOR_IRDETO && g_TAInfo.OpenFlag == HI_TRUE
        && enKeyladderLev == enLevel)
    {
        unSPCtrl.bits.ta_kl_flag = 1; //for Irdeto MSR2.2
    }

    DRV_ADVCA_WriteReg(CA_V300_SP_CTRL, unSPCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (R2R_SP_SYMMETRIC_KEY_ERROR == Ret)
    {
        HI_ERR_CA("SP key ladder error: symmetric key usage\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);
        memcpy(g_SPInfo.SessionKey[enLevel], pu32DataIn, 16);
        g_SPInfo.SessionKey_sec[enLevel] = tv.tv_sec;

        g_SPInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

        //0：当前是偶密钥；1：当前是奇密钥
        if(g_SPInfo.MaxLevel == (enLevel + 1))
        {
            if(bEvenOrOdd == 0x00)
            {
                memcpy(g_SPInfo.LastEvenKey, pu32DataIn, 16);
                g_SPInfo.LastEvenKey_secTime = tv.tv_sec;
            }
            else
            {
                memcpy(g_SPInfo.LastOddKey, pu32DataIn, 16);
                g_SPInfo.LastOddKey_secTime = tv.tv_sec;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptLpk(HI_U32 *pEncryptLpk)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_LP_CTRL_U LPCtrl;

    /* Only use in Conax CA
       Use R2R Root Key as the top key.
       Set First Level LPK protect key, Algorithm:TDES
       After decramble, result same in parameter寄存器。    0x50~0x5C
    */

    /* 0.if support to LPK */


    /* para check*/
    if (HI_NULL == pEncryptLpk)
    {
       HI_ERR_CA("HI_NULL == pEncryptLpk\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will not be changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
    //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    //HI_INFO_CA("ca_DecryptLpk:0x%x, 0x%x, 0x%x, 0x%x\n", pEncryptLpk[0], pEncryptLpk[1], pEncryptLpk[2], pEncryptLpk[3]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pEncryptLpk[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pEncryptLpk[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pEncryptLpk[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pEncryptLpk[3]);

    //(3)   配置寄存器LP_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    LPCtrl.u32 = 0;
    LPCtrl.bits.level_sel = 0x00;//00：当前是1级会话密钥
    DRV_ADVCA_WriteReg(CA_V300_LP_CTRL, LPCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptLPKProtectData(HI_U32 *pEncryptData,HI_U32 *pClearData)
{
    HI_U32 index;
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_LP_CTRL_U      LPCtrl;

    /*  Only use in Conax CA
        Use R2R Root Key as the top key.
        Set Second Level LPK protect key, Algorithm:TDES
        Must invoke after DecryptLpk.
        Use parameter寄存器。   0x50~0x5C as the key, Algorithm:TDES
        Input is pEncryptData, Output is pClearData
    */

    if ((HI_NULL == pEncryptData) || (HI_NULL == pClearData))
    {
        HI_ERR_CA("HI_NULL == pEncryptData, pClearData\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will not be changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /*2  put R2R_RootKey（CK2）to CA register(DATA input) */
    //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    //HI_INFO_CA("DecryptLPKPrtotectData:0x%x, 0x%x\n", pEncryptData[0], pEncryptData[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pEncryptData[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pEncryptData[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pEncryptData[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, (pEncryptData[1] + 1));
    //(3)   配置寄存器LP_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    LPCtrl.u32 = 0;
    LPCtrl.bits.level_sel = 0x01;//01：当前是2级会话密钥
    DRV_ADVCA_WriteReg(CA_V300_LP_CTRL, LPCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);
        return HI_FAILURE;
    }

    for(index = 0; index < 8; index +=4)//Data Result is 128bit(16Bytes) data
    {
        pClearData[(index / 4)] = DRV_ADVCA_ReadReg(CA_V300_LP_PARAMETER_BASE + index);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_DAVCA_V300_EncryptDevicekey(HI_U32 *pDeviceKey, HI_U32 *pEncryptDeviceKey)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_GDRM_CTRL_U unGDRMCtrl;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    if (HI_NULL == pDeviceKey || HI_NULL == pEncryptDeviceKey)
    {
       HI_ERR_CA("HI_NULL == pDataIn || HI_NULL == pEncryptDeviceKey\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */

    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pDeviceKey[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pDeviceKey[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pDeviceKey[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pDeviceKey[3]);

    unGDRMCtrl.u32 = 0;
    unGDRMCtrl.bits.level_sel = 0x0;
    unGDRMCtrl.bits.decryption = 0;

    DRV_ADVCA_WriteReg(CA_V300_GDRM_CTRL, unGDRMCtrl.u32); /* CAStatus.bits.klad_busy will be set to 1 */

    /* Now Wait, until CAStatus.bits.klad_busy == 0 */
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);
        return HI_FAILURE;
    }

    pEncryptDeviceKey[0] = DRV_ADVCA_ReadReg(CA_V300_GDRM_ENC_REST0);
    pEncryptDeviceKey[1] = DRV_ADVCA_ReadReg(CA_V300_GDRM_ENC_REST1);
    pEncryptDeviceKey[2] = DRV_ADVCA_ReadReg(CA_V300_GDRM_ENC_REST2);
    pEncryptDeviceKey[3] = DRV_ADVCA_ReadReg(CA_V300_GDRM_ENC_REST3);

    //Just to store it for /proc message
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);

        memcpy(g_GDRMInfo.SessionKey[0], (HI_U8 *)pDeviceKey, 16);
        memcpy(g_GDRMInfo.SessionKey[1], (HI_U8 *)pEncryptDeviceKey, 16);
        g_GDRMInfo.SessionKey_sec[0] = tv.tv_sec;
        g_GDRMInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_GDRM_CTRL);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_EncryptSwpk(HI_U32 *pClearSwpk,HI_U32 *pEncryptSwpk)
{
    HI_U32 index;
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    /* Key Ladder internal use TDES.
    Just Need to input data. */

    if ((HI_NULL == pClearSwpk) || (HI_NULL == pClearSwpk))
    {
       HI_ERR_CA("HI_NULL == pClearSwpk, pClearSwpk\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /*2  put R2R_RootKey（CK2）to CA register(DATA input) */
    //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
#if defined (CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    //STB_ROOT key not have secure attr
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN0, pClearSwpk[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN1, pClearSwpk[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN2, pClearSwpk[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN3, pClearSwpk[3]);
#else
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pClearSwpk[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pClearSwpk[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pClearSwpk[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pClearSwpk[3]);
#endif
    //(3)   配置寄存器BL_CTRL_ENC，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_ENC, 0x01);//0x1c Set Register

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    for(index = 0; index < 16; index += 4) //Data Result is 128bit(16Bytes) data
    {
        pEncryptSwpk[(index / 4)] = DRV_ADVCA_ReadReg(CA_V300_BLK_ENC_RSLT + index);
    }
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DCASOpen(HI_UNF_CIPHER_ALG_E enAlg)
{
    switch (enAlg)
    {
        case HI_UNF_CIPHER_ALG_3DES:
            g_DCASInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;
            break;
        case HI_UNF_CIPHER_ALG_AES:
            g_DCASInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_AES;
            break;
        default:
            HI_ERR_CA("not support algorithm: %d\n", enAlg);
            return HI_FAILURE;
    }

    if (g_DCASInfo.OpenFlag != HI_TRUE)
    {
        g_DCASInfo.OpenFlag = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DCASClose(HI_VOID)
{
    if(g_DCASInfo.OpenFlag == HI_TRUE)
    {
        g_DCASInfo.OpenFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptDCAS(HI_U32 enLevel, HI_U32 *pu32DataIn, HI_U32 AddrID, HI_BOOL bEvenOrOdd, HI_U32 *pDataOut, HI_UNF_ADVCA_CA_TARGET_E enKlTarget)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_DCAS_CTRL_U DCASCtrl;

    if(g_DCASInfo.OpenFlag != HI_TRUE)
    {
        HI_ERR_CA("Error: DCAS is not Open\n");
        return HI_FAILURE;
    }

    /* para check*/
    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    HI_INFO_CA("input: 0x%08x %08x %08x %08x\n", pu32DataIn[0], pu32DataIn[1], pu32DataIn[2], pu32DataIn[3]);
    /*2  put EncryptData to CA register(DATA input) */
    //(2)   配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);

    //(3)   配置寄存器DCAS_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    DCASCtrl.u32 = 0;
    DCASCtrl.bits.dsc_code_mc_alg_sel = 0;
    DCASCtrl.bits.level_sel = enLevel;
    if(enLevel == 0x04) //To Demux:0100：当前是第3级会话密钥,payload模式SP密钥；
    {
        if(enKlTarget == HI_UNF_ADVCA_CA_TARGET_DEMUX)
        {
            //设置第3级密钥的话，必须设置为0x04: 0100：当前是第3级会话密钥
            //0100：当前是第3级会话密钥,payload模式SP密钥；
            DCASCtrl.bits.level_sel = 0x04;
            DCASCtrl.bits.even_or_odd = bEvenOrOdd;  //Odd/Even Key
            /* Demux DescambleKey ID + even_or_odd */
            DCASCtrl.bits.key_addr = AddrID & 0x7F; //Demux DescambleKey ID
        }
        else
        {
            //0101：当前是第3级会话密钥,raw模式SP密钥；
            DCASCtrl.bits.level_sel = 0x05;
            /* multicipher channel ID */
            DCASCtrl.bits.key_addr = AddrID & 0xFF; //Demux DescambleKey ID
        }
    }
    else if(enLevel == 0x05)  //To R2R,0101：当前是第3级会话密钥,raw模式SP密钥；
    {
        //R2R；0011：当前是第3级会话密钥,R2R密钥
        DCASCtrl.bits.level_sel = 0x05;
        /* multicipher channel ID */
        DCASCtrl.bits.even_or_odd = (AddrID & 0x01);
        DCASCtrl.bits.key_addr = (AddrID >> 1) & 0xFF;
    }

    if(g_DCASInfo.Alg == HI_UNF_ADVCA_ALG_TYPE_TDES)
    {
        DCASCtrl.bits.tdes_aes_sel = 0;//0:tdes, 1:aes
    }
    else
    {
        DCASCtrl.bits.tdes_aes_sel = 1;//0:tdes, 1:aes
    }

    DRV_ADVCA_WriteReg(CA_V300_DCAS_CTRL, DCASCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (DCAS_KL_DISABLE_ERROR == Ret)
    {
        HI_ERR_CA("DCAS key ladder error: dcas_kl_disable error\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    if(DCASCtrl.bits.level_sel == 0x09) //DCAS reply DA-nonce
    {
        if (HI_NULL == pDataOut)
        {
           HI_ERR_CA("HI_NULL == pDataOut\n");
           return HI_ERR_CA_INVALID_PARA;
        }
        pDataOut[0] = DRV_ADVCA_ReadReg(CA_V300_DA_NOUCE);
        pDataOut[1] = DRV_ADVCA_ReadReg(CA_V300_DA_NOUCE+4);
        pDataOut[2] = DRV_ADVCA_ReadReg(CA_V300_DA_NOUCE+8);
        pDataOut[3] = DRV_ADVCA_ReadReg(CA_V300_DA_NOUCE+12);
    }

    //Just to store it for /proc message
    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);

        memcpy(g_DCASInfo.SessionKey[enLevel], (HI_U8 *)pu32DataIn, 16);
        g_DCASInfo.SessionKey_sec[enLevel] = tv.tv_sec;
        //HI_INFO_CA("DCAS enLevel:%d\n", enLevel);
        g_DCASInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

        //0：当前是偶密钥；1：当前是奇密钥
        if ((g_DCASInfo.MaxLevel == (enLevel + 1))
        || (enLevel == 0x04) || (enLevel == 0x05))
        {
            if(bEvenOrOdd == 0x00)
            {
                memcpy(g_DCASInfo.LastEvenKey, pu32DataIn, 16);
                g_DCASInfo.LastEvenKey_secTime = tv.tv_sec;
            }
            else
            {
                memcpy(g_DCASInfo.LastOddKey, pu32DataIn, 16);
                g_DCASInfo.LastOddKey_secTime = tv.tv_sec;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SWPKKeyLadderOpen(void)
{
    g_IsSWPKKeyLadderOpen = HI_TRUE;
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SWPKKeyLadderClose(void)
{
    g_IsSWPKKeyLadderOpen = HI_FALSE;
    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptSWPK(HI_U32 *pu32DataIn,HI_U32 AddrID)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    /* Key Ladder internal use TDES.
       Just Need to input data. */

    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pClearSwpk, pClearSwpk\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */
#if defined (CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    //STB_ROOT key not have secure attr
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_NOMAL_DIN3, pu32DataIn[3]);
#else
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);
#endif


    /* Config GDRM_CTRL, and then CA_STATE.klad_busy would be set */
    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_DEC, (AddrID & 0x3F) << 8);//0x1c Set Register
/*    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_DEC, 0x01);*/  //0x1c Set Register

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_CryptGDRM(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                        HI_U32 *pu32DataIn,
                        HI_U32 AddrID,
                        HI_BOOL bIsDeCrypt,
                        HI_UNF_ADVCA_CA_TARGET_E enKlTarget)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_GDRM_CTRL_U unGDRMCtrl;
    HI_U32 looptime = 0;

    if (HI_NULL == pu32DataIn)
    {
        HI_ERR_CA("HI_NULL == pDataIn\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    /*
        'level_sel: '会话密钥交换级数。
        00：当前是1级会话密钥；AES ECB
        01：当前是2级会话密钥；AES ECB
        10：当前是3级会话密钥byte0~byte15；AES CBC
        11：当前是3级会话密钥高byte16~byte31；AES CBC。
    */
    switch (enLevel)
    {
        case HI_UNF_ADVCA_KEYLADDER_LEV1:
        case HI_UNF_ADVCA_KEYLADDER_LEV2:
            // do one time
            looptime =1;
            break;

        case HI_UNF_ADVCA_KEYLADDER_LEV3:
            // do 2 times
            // index 0: level_sel = 3;select pu32DataIn 0~15;
            // index 0: level_sel = 4;select pu32DataIn 16~31;
            looptime = 2;
            break;

        default:
            return HI_FAILURE;
    }

    while (looptime > 0)
    {
        /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
           The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
        unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
        if(unConfigStatus.bits.st_vld != 1)
        {
            HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
            return HI_FAILURE;
        }

        Ret = HAL_ADVCA_V300_WaitStat();
        if (HI_FAILURE == Ret)
        {
            HI_ERR_CA("Keyladder is busy now!\n");
            return HI_ERR_CA_WAIT_TIMEOUT;
        }
        if( (enLevel == HI_UNF_ADVCA_KEYLADDER_LEV3) && (looptime == 1) )
        {
            /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[4]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[5]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[6]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[7]);
        }
        else
        {
            /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
            DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);
        }

        /* Config GDRM_CTRL, and then CA_STATE.klad_busy would be set */
        unGDRMCtrl.u32 = 0;
        if(enLevel == HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            if(looptime == 2)
            {
                unGDRMCtrl.bits.level_sel = 0x02;
            }
            else  //loop == 1;
            {
                unGDRMCtrl.bits.level_sel = 0x03;
            }
        }
        else
        {
            unGDRMCtrl.bits.level_sel = enLevel;
        }
        /* defaultTarget : Multicipher Algorithm. */
        unGDRMCtrl.bits.target_sel = enKlTarget;
        unGDRMCtrl.bits.decryption = bIsDeCrypt;

        if (HI_UNF_ADVCA_CA_TARGET_MULTICIPHER == enKlTarget)
        {
            /* multicipher channel ID */
            unGDRMCtrl.bits.even_or_odd = (AddrID & 0x7F) & 0x01; // first 1 bit
            unGDRMCtrl.bits.key_addr = (AddrID & 0x7F) >> 1;      // last 7 bits
        }
        else
        {
            unGDRMCtrl.bits.key_addr = AddrID & 0xFF;
        }

        DRV_ADVCA_WriteReg(CA_V300_GDRM_CTRL, unGDRMCtrl.u32); /* CAStatus.bits.klad_busy will be set to 1 */

        /* Now Wait, until CAStatus.bits.klad_busy == 0 */
        Ret = HAL_ADVCA_V300_WaitStat();
        if (HI_FAILURE == Ret)
        {
            HI_ERR_CA("key ladder timeout\n");
            return HI_ERR_CA_WAIT_TIMEOUT;
        }
        else if (HI_SUCCESS != Ret)
        {
            CA_V300_CA_STATE_U CAStatus;
            CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
            HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

            return HI_FAILURE;
        }

        if (looptime > 0)
        {
            looptime --;
        }
    }

    if(enLevel < 4)
    {
        struct timeval tv = {0};

        g_GDRMInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_GDRM_CTRL);

        do_gettimeofday(&tv);
        if ((enLevel == HI_UNF_ADVCA_KEYLADDER_LEV1) || (enLevel == HI_UNF_ADVCA_KEYLADDER_LEV2))
        {
            memcpy(g_GDRMInfo.SessionKey[enLevel], (HI_U8 *)pu32DataIn, 16);
            g_GDRMInfo.SessionKey_sec[enLevel] = tv.tv_sec;
        }
        else if (enLevel == HI_UNF_ADVCA_KEYLADDER_LEV3)
        {
            HI_U32 GG_Flag = 0;
            memcpy(g_GDRMInfo.SessionKey[enLevel], (HI_U8 *)pu32DataIn, 32);

            g_GDRMInfo.SessionKey_sec[enLevel] = tv.tv_sec;
            GG_Flag = DRV_ADVCA_ReadReg(CA_V300_GDRM_FLAG);

            memcpy(g_GDRMInfo.GG_Flag, &GG_Flag, 4);
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetGDRMFlag(HI_U32 *pGDRMFlag)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    if (HI_NULL == pGDRMFlag)
    {
       HI_ERR_CA("HI_NULL == pGDRMFlag\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    pGDRMFlag[0] = DRV_ADVCA_ReadReg(CA_V300_GDRM_FLAG);

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptMisc(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                            HI_U32 *pu32DataIn,
                            HI_U32 AddrID,
                            HI_BOOL bEvenOrOdd,
                            DRV_ADVCA_CA_TARGET_E enCwTarget)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_MISC_CTRL_U unMiscCtrl;

    if (HI_NULL == pu32DataIn)
    {
       HI_ERR_CA("HI_NULL == pDataIn\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if(unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);

    /* Config GDRM_CTRL, and then CA_STATE.klad_busy would be set */
    unMiscCtrl.u32 = 0;
    if(enLevel == HI_UNF_ADVCA_KEYLADDER_LEV5)
    {
        unMiscCtrl.bits.level_sel_5 = 1;
    }
    else
    {
        unMiscCtrl.bits.level_sel = enLevel;
    }
    unMiscCtrl.bits.tdes_aes_sel = g_MiscInfo.Alg;
    unMiscCtrl.bits.target_sel = enCwTarget;
    if (0 == unMiscCtrl.bits.target_sel)
    {
        /* Demux DescambleKey ID + even_or_odd */
        unMiscCtrl.bits.key_addr = ((AddrID & 0x7F) << 1) + bEvenOrOdd;
        unMiscCtrl.bits.dsc_mode = g_MiscInfo.enDscMode;  //set demux DscMode
    }
    else
    {
        /* multicipher channel ID */
        unMiscCtrl.bits.key_addr = AddrID & 0xFF;
    }

    /* CAStatus.bits.klad_busy will be set to 1 */
    DRV_ADVCA_WriteReg(CA_V300_MISC_CTRL, unMiscCtrl.u32);

    /* Now Wait until CAStatus.bits.klad_busy == 0 */
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (MISC_KL_LEVEL_ERROR == Ret)
    {
        HI_ERR_CA("MISC key ladder error: keyladder level is 2 in otp, but start 3rd level actually.\n");
        return HI_FAILURE;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }
    if(enLevel < 16)
    {
        struct timeval tv = {0};
        do_gettimeofday(&tv);
        memcpy(g_MiscInfo.SessionKey[enLevel], pu32DataIn, 16);
        g_MiscInfo.SessionKey_sec[enLevel] = tv.tv_sec;

        g_MiscInfo.Keyladder_Ready = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);

        //0：当前是偶密钥；1：当前是奇密钥
        if(g_MiscInfo.MaxLevel == (enLevel + 1))
        {
            if(bEvenOrOdd == 0x00)
            {
                memcpy(g_MiscInfo.LastEvenKey, pu32DataIn, 16);
                g_MiscInfo.LastEvenKey_secTime = tv.tv_sec;
            }
            else
            {
                memcpy(g_MiscInfo.LastOddKey, pu32DataIn, 16);
                g_MiscInfo.LastOddKey_secTime = tv.tv_sec;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetSecretKeyChecksumFlag(HI_U32 *pu32Checksum)
{
    HI_UNF_ADVCA_CHECKSUM_FLAG_U unChecksum;

    if( NULL == pu32Checksum)
    {
        HI_ERR_CA("ERROR! Null pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    unChecksum.u32 = 0;
    unChecksum.u32 = DRV_ADVCA_ReadReg(CA_V300_CHECKSUM_FLAG);
    *pu32Checksum = unChecksum.u32;

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetCAStates(HI_U32 *pu32State)
{
    if(NULL == pu32State)
    {
        HI_ERR_CA("ERROR! Null pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    DRV_ADVCA_WriteReg(CA_V300_CA_STATE, *pu32State);

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_ResetOTP(HI_VOID)
{
    OTP_V200_CRG_CTRL_U OtpV200CrgCtrl;

    OtpV200CrgCtrl.u32 = 0;

    /*Set the reset value of OTP: 0xF8A220C0 bit[10]*/
    OtpV200CrgCtrl.u32 = DRV_ADVCA_ReadReg(OTP_V200_CRG_ADDR);
    OtpV200CrgCtrl.bits.otp_srst_req = 1;
    DRV_ADVCA_WriteReg(OTP_V200_CRG_ADDR, OtpV200CrgCtrl.u32);

    ca_msleep(1);

    OtpV200CrgCtrl.bits.otp_srst_req = 0;
    DRV_ADVCA_WriteReg(OTP_V200_CRG_ADDR, OtpV200CrgCtrl.u32);
    return HI_SUCCESS;
}



HI_S32 HAL_ADVCA_V300_CryptbyStbRootkey(HI_U32 AddrID)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_STB_CTRL_U unStbCtrl;

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if (unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    //(3)   配置寄存器STB KEY CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    unStbCtrl.u32 = 0;
    unStbCtrl.bits.key_addr = AddrID & 0xFF;
    DRV_ADVCA_WriteReg(CA_V300_STB_KEY_CTRL, unStbCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_CryptHCA(HI_U32 AddrID)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_CAUK_CTRL_U unCaukCtrl;

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if (unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    //(3)   配置寄存器CAUK_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    unCaukCtrl.u32 = 0;
    unCaukCtrl.bits.key_addr = AddrID & 0xFF;
    DRV_ADVCA_WriteReg(CA_V300_CAUK_CTRL, unCaukCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        CA_V300_CA_STATE_U CAStatus;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        HI_ERR_CA("key ladder error state: %x\n", CAStatus.bits.err_state);

        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecTAKeyLadder(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel, HI_U32 *pu32InData, HI_U32 u32InDataLen)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_TA_CTRL_U unTaCtrl;
    HI_U32 u32Loop, u32Count;

    if (HI_NULL == pu32InData)
    {
       HI_ERR_CA("HI_NULL == pu32InData\n");
       return HI_ERR_CA_INVALID_PARA;
    }

    /* Read CONIFG_STATE.st_vld first, we can operate registers once CONIFG_STATE.st_vld set to 1.
       The value of CONIFG_STATE.st_vld will be not changed when powered on after it set to 1. */
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if (unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    u32Count = u32InDataLen / 16;
    for (u32Loop = 0; u32Loop < u32Count; u32Loop++)
    {
        /* Config the input data for crypto : CA_DIN0, CA_DIN1, CA_DIN2, CA_DIN3 */
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32InData[0 + u32Loop * 4]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32InData[1 + u32Loop * 4]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32InData[2 + u32Loop * 4]);
        DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32InData[3 + u32Loop * 4]);

        /* Config TA_CTRL, and then CA_STATE.klad_busy would be set */
        unTaCtrl.u32 = 0;
        unTaCtrl.bits.level_sel = enLevel;

        if (enLevel == HI_UNF_ADVCA_KEYLADDER_LEV2)
        {
            unTaCtrl.bits.dec_lut_alg_sel = HI_UNF_ADVCA_ALG_TYPE_AES;
        }

        if (enLevel == HI_UNF_ADVCA_KEYLADDER_LEV2 && ((u32Loop + 1) == u32Count))
        {
            unTaCtrl.bits.last_time = 1; //last time, should set it to 1.
        }
        else
        {
            unTaCtrl.bits.last_time = 0;
        }
        unTaCtrl.bits.lut_alg_sel = g_TAInfo.Alg;

        /* CAStatus.bits.klad_busy will be set to 1 */
        DRV_ADVCA_WriteReg(CA_V300_TA_KL_CTRL, unTaCtrl.u32);

        /* Now Wait until CAStatus.bits.klad_busy == 0 */
        Ret = HAL_ADVCA_V300_WaitStat();
        if (HI_FAILURE == Ret)
        {
            HI_ERR_CA("Key ladder timeout\n");
            return HI_ERR_CA_WAIT_TIMEOUT;
        }
    }

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetTaAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    if ((enType != HI_UNF_ADVCA_ALG_TYPE_TDES) && (enType != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid TA Keyladder algorithm (%d)!\n", enType);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_TAInfo.Alg = enType;

    return HI_SUCCESS;
}
static HI_VOID VMX_ADVCA_Swith(CA_KEYLADDER_ATTR_S *pDeststKeyladderAttr, CA_KEYLADDER_ATTR_S *pSrcstKeyladderAttr)
{
    HI_INFO_CA("enter VMX_ADVCA_Swith\n");
    memset(pDeststKeyladderAttr, 0, sizeof(CA_KEYLADDER_ATTR_S));

    switch(pSrcstKeyladderAttr->enKeyLadderType)
    {
    case HI_UNF_ADVCA_KEYLADDER_MISC:
        pDeststKeyladderAttr->enKeyLadderType = HI_UNF_ADVCA_KEYLADDER_SP;

        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable      = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.bEnable;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType    = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enAlgType;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode    = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enDscMode;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enStage      = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage;
        memcpy(pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, 16);
        break;
    case HI_UNF_ADVCA_KEYLADDER_SP:
        pDeststKeyladderAttr->enKeyLadderType = HI_UNF_ADVCA_KEYLADDER_MISC;

        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr   = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.bEnable      = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enAlgType    = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enDscMode    = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode;
        pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage      = pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.enStage;
        memcpy(pDeststKeyladderAttr->stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, pSrcstKeyladderAttr->stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, 16);
        break;
    default:
        ;
    }

    HI_INFO_CA("quit VMX_ADVCA_Swith\n");
    return ;
}

static HI_S32 checkVMXAdvanced(HI_BOOL *pbVMXAdvanced)
{
    HI_S32 ret = 0;
    CA_OTP_VENDOR_TYPE_E enCaVendorType = CA_OTP_VENDOR_NONE;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enValue = HI_UNF_ADVCA_KEYLADDER_BUTT;

    ret = DRV_CA_OTP_V200_GetVendorId(&enCaVendorType);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get the CA vendor ID.\n");
        return HI_FAILURE;
    }

    ret = HAL_ADVCA_V300_GetMiscKlLevel(&enValue);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_CA("Failed to get Misck Level, ret=%d\n", ret);
        return HI_FAILURE;
    }

    if ((CA_OTP_VENDOR_VERIMATRIX == enCaVendorType) && (HI_UNF_ADVCA_KEYLADDER_LEV5 == enValue))
    {
        *pbVMXAdvanced = HI_TRUE;
    }
    else
    {
        *pbVMXAdvanced = HI_FALSE;
    }

    return HI_SUCCESS;
}
HI_S32 HAL_ADVCA_V300_SetKeyladderAttr(CA_KEYLADDER_ATTR_S *pstKeyladderAttr)
{
    HI_UNF_ADVCA_KEYLADDER_TYPE_E            enKeyladderType;
    HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_E       enCsa2KeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_E       enCsa3KeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_E        enR2RKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_LP_ATTR_E         enLPKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_E         enSPKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_E       enMiscKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_TA_ATTR_E         enTaKeyladderAttr;
#if !defined(CHIP_TYPE_hi3798mv100) \
 && !defined(CHIP_TYPE_hi3796mv100) \
 && !defined(CHIP_TYPE_hi3716dv100) \
 && defined(HI_ADVCA_TYPE_VERIMATRIX)
    HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_E       enGDRMKeyladderAttr;
#endif

    HI_UNF_ADVCA_ALG_TYPE_E                  enKeyladderAlg;
    HI_UNF_ADVCA_SP_DSC_MODE_E               enSPDscMode;
    HI_UNF_ADVCA_KEYLADDER_LEV_E             enKeyladderLevel;
    CA_CRYPTPM_S stCryptParam = {0};
    CA_LOADLPK_S stLoadLpk;
    HI_S32 Ret = HI_SUCCESS;
    HI_U8 *pu8TmpBuf = NULL;
    HI_U32 u32TmpBufLen;
    HI_BOOL bVMXAdvanced = HI_FALSE;

    CA_KEYLADDER_ATTR_S TempstKeyladderAttr = {0};

    if(pstKeyladderAttr == NULL)
    {
        HI_ERR_CA("Invalid param, pstKeyladderAttr = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    Ret = checkVMXAdvanced(&bVMXAdvanced);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("checkVMXAdvanced error\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == bVMXAdvanced)
    {
        VMX_ADVCA_Swith(&TempstKeyladderAttr, pstKeyladderAttr);
    }
    else
    {
        memcpy(&TempstKeyladderAttr, pstKeyladderAttr, sizeof(CA_KEYLADDER_ATTR_S));
    }

    enKeyladderType = TempstKeyladderAttr.enKeyLadderType;
    switch(enKeyladderType)
    {
    case HI_UNF_ADVCA_KEYLADDER_CSA2:
        enCsa2KeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA2KlAttr.enCsa2KlAttr;
        if (enCsa2KeyladderAttr == HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA2KlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetCWAlgorithm(enKeyladderAlg);
        }
        else if (enCsa2KeyladderAttr == HI_UNF_ADVCA_KEYLADDER_CSA2_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA2KlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA2KlAttr.u8SessionKey, 16);
            Ret = HAL_ADVCA_V300_DecryptCw(stCryptParam.ladder, stCryptParam.pDin, 0, 0);
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enCsa2KeyladderAttr = %d\n", enCsa2KeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_CSA3:
        enCsa3KeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA3KlAttr.enCsa3KlAttr;
        if (enCsa3KeyladderAttr == HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA3KlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetCSA3Algorithm(enKeyladderAlg);
        }
        else if (enCsa3KeyladderAttr == HI_UNF_ADVCA_KEYLADDER_CSA3_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA3KlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stCSA3KlAttr.u8SessionKey, 16);
            Ret = HAL_ADVCA_V300_DecryptCsa3(stCryptParam.ladder, stCryptParam.pDin, 0, 0);
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enCsa3KeyladderAttr = %d\n", enCsa3KeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_R2R:
        enR2RKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stR2RKlAttr.enR2RKlAttr;
        if(enR2RKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stR2RKlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetR2RAlgorithm(enKeyladderAlg);
        }
        else if(enR2RKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stR2RKlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stR2RKlAttr.u8SessionKey, 16);
            Ret = HAL_ADVCA_V300_CryptR2R(stCryptParam.ladder, stCryptParam.pDin, 0, 1);
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enR2RKeyladderAttr = %d\n", enR2RKeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_SP:
        enSPKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.enSPKlAttr;
        if(enSPKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetSPAlgorithm(enKeyladderAlg);
        }
        else if(enSPKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_SP_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.u8SessionKey, 16);
            Ret = HAL_ADVCA_V300_DecryptSP(stCryptParam.ladder, stCryptParam.pDin, 0, 0, 0);
        }
        else if(enSPKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_SP_ATTR_DSC_MODE)
        {
            enSPDscMode =  TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode;
            Ret = HAL_ADVCA_V300_SetSPDscMode(enSPDscMode);
        }
        else if(enSPKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_SP_ATTR_ENABLE)
        {
            g_SPInfo.OpenFlag = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.bEnable;
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enR2RKeyladderAttr = %d\n", enSPKeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_MISC:
        enMiscKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stMiscKlAttr.enMiscKlAttr;
        if(enMiscKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stMiscKlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetMiscAlgorithm(enKeyladderAlg);
        }
        else if(enMiscKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stMiscKlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stMiscKlAttr.u8SessionKey, 16);
            Ret = HAL_ADVCA_V300_DecryptMisc(stCryptParam.ladder, stCryptParam.pDin, 0, 0, 0);
        }
        else if(enMiscKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_DSC_MODE)
        {
            enSPDscMode =  TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stSPKlAttr.enDscMode;
            Ret = HAL_ADVCA_V300_SetMiscDscMode(enSPDscMode);
        }
        else if(enMiscKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_ENABLE)
        {
            g_MiscInfo.OpenFlag = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stMiscKlAttr.bEnable;
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enMiscKeyladderAttr = %d\n", enMiscKeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_LP:
        enLPKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stLpKlAttr.enLPKlAttr;
        if (enLPKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_LP_ATTR_LOAD_LPK)
        {
            memset(&stLoadLpk, 0x00, sizeof(CA_LOADLPK_S));
            memcpy(stLoadLpk.EncryptLpk, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stLpKlAttr.u8Lpk, 16);
            /* Decrypt LPK */
            Ret = HAL_ADVCA_V300_DecryptLpk(stLoadLpk.EncryptLpk);
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enLPKeyladderAttr = %d\n", enLPKeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    case HI_UNF_ADVCA_KEYLADDER_TA:
        enTaKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.enTAKlAttr;
        if (enTaKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_TA_ATTR_ENABLE)
        {
            g_TAInfo.OpenFlag = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.bEnable;
        }
        else if (enTaKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_TA_ATTR_ALG)
        {
            enKeyladderAlg = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.enAlgType;
            Ret = HAL_ADVCA_V300_SetTaAlgorithm(enKeyladderAlg);
        }
        else if (enTaKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_TA_ATTR_SESSION_KEY)
        {
            stCryptParam.ladder = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.enStage;
            memcpy(stCryptParam.pDin, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.au8SessionKey, 16);
            Ret = HAL_ADVCA_V300_DecTAKeyLadder(stCryptParam.ladder, stCryptParam.pDin, 16);
        }
        else if (enTaKeyladderAttr == HI_UNF_ADVCA_KEYLADDER_TA_ATTR_LOAD_TRANDATA)
        {
            enKeyladderLevel = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.enStage;
            pu8TmpBuf = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.pu8TranData;
            u32TmpBufLen = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stTAKlAttr.u32TranDataLen;
            Ret = HAL_ADVCA_V300_DecTAKeyLadder(enKeyladderLevel, (HI_U32*)pu8TmpBuf, u32TmpBufLen);
        }
        else
        {
            HI_ERR_CA("Invalid keyladder attribute, enTaKeyladderAttr = %d\n", enTaKeyladderAttr);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;
#if !defined(CHIP_TYPE_hi3798mv100) \
 && !defined(CHIP_TYPE_hi3796mv100) \
 && !defined(CHIP_TYPE_hi3716dv100) \
 && defined(HI_ADVCA_TYPE_VERIMATRIX)
        case HI_UNF_ADVCA_KEYLADDER_GDRM:
        {
            HI_U32 u32SessionKey[8] = {0};
            HI_U32 u32Output[4]     = {0};
            HI_U32 u32KeyAddr       = 0;

            enGDRMKeyladderAttr = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.enGDRMKlAttr;
            if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENABLE == enGDRMKeyladderAttr)
            {
                g_GDRMInfo.OpenFlag = TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.bEnable;
            }
            else if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_ENCRYPT == enGDRMKeyladderAttr)
            {
                memcpy(u32SessionKey, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.au8SessionKey, 16);
                Ret = HAL_DAVCA_V300_EncryptDevicekey(u32SessionKey,
                                                      u32Output);
                memcpy(pstKeyladderAttr->stKeyladderAttr.unKlAttr.stGDRMAttr.au8Output, u32Output, 16);
            }
            else if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_SESSION_KEY == enGDRMKeyladderAttr)
            {
                if (HI_UNF_ADVCA_KEYLADDER_LEV3 == TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.enStage)
                {
                    memcpy(u32SessionKey, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.au8SessionKey, 32);
                    u32KeyAddr = HI_HANDLE_GET_CHNID(TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.hCipherHandle);
                }
                else
                {
                    memcpy(u32SessionKey, TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.au8SessionKey, 16);
                    u32KeyAddr = 0;
                }
                Ret = HAL_ADVCA_V300_CryptGDRM(TempstKeyladderAttr.stKeyladderAttr.unKlAttr.stGDRMAttr.enStage, u32SessionKey, u32KeyAddr, 1, 1);
            }
            else if (HI_UNF_ADVCA_KEYLADDER_GDRM_ATTR_GETFLAG == enGDRMKeyladderAttr)
            {
                Ret = HAL_ADVCA_V300_GetGDRMFlag(u32Output);
                memcpy(pstKeyladderAttr->stKeyladderAttr.unKlAttr.stGDRMAttr.au8Output, u32Output, 4);
            }
            else
            {
                HI_ERR_CA("Invalid keyladder attribute, enGDRMKeyladderAttr = %d\n", enGDRMKeyladderAttr);
            }
        }
        break;
#endif
    default:
        HI_ERR_CA("Invalid keyladder type, enKeyladderType = %d\n", enKeyladderType);
        Ret = HI_ERR_CA_INVALID_PARA;
        break;
    }

    return Ret;
}

HI_S32 HAL_ADVCA_V300_GenChipConfCmac(HI_U8 *pu8ChipConfBitm, HI_U8 *pu8ChipConfCmac)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_CFG_CBC_MAC_U unCfgCbcMacCtrl;

    if ((HI_NULL == pu8ChipConfBitm) || (HI_NULL == pu8ChipConfCmac))
    {
        HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }
    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if (unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, *((HI_U32*)pu8ChipConfBitm + 0));
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, *((HI_U32*)pu8ChipConfBitm + 1));
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, *((HI_U32*)pu8ChipConfBitm + 2));
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, *((HI_U32*)pu8ChipConfBitm + 3));

    //(2)   配置寄存器CFG_CBC_MAC，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    unCfgCbcMacCtrl.u32 = 0;
    unCfgCbcMacCtrl.bits.start = 1;
    DRV_ADVCA_WriteReg(CA_V300_CFG_CMAC_CTRL, unCfgCbcMacCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    *(HI_U32*)pu8ChipConfCmac = DRV_ADVCA_ReadReg(CA_V300_CFG_CMAC_OUT);
    *(HI_U32*)(pu8ChipConfCmac + 0x04) = DRV_ADVCA_ReadReg(CA_V300_CFG_CMAC_OUT  + 0x04);
    *(HI_U32*)(pu8ChipConfCmac + 0x08) = DRV_ADVCA_ReadReg(CA_V300_CFG_CMAC_OUT + 0x08);
    *(HI_U32*)(pu8ChipConfCmac + 0x0C) = DRV_ADVCA_ReadReg(CA_V300_CFG_CMAC_OUT + 0x0C);

    return Ret;
}

HI_S32 HAL_ADVCA_V300_SetIrdetoCsa3ActCode(HI_U8 *pu8Csa3ActCode)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_CFG_IVRK_CTRL_U unCfgIvRkCtrl;

    if (HI_NULL == pu8Csa3ActCode)
    {
        HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
    unConfigStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CONFIG_STATE);
    if (unConfigStatus.bits.st_vld != 1)
    {
        HI_ERR_CA("Error: ConfigStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, *(HI_U32*)pu8Csa3ActCode);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, *((HI_U32*)pu8Csa3ActCode + 1));
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, *((HI_U32*)pu8Csa3ActCode + 2));
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, *((HI_U32*)pu8Csa3ActCode + 3));

    //(2)   配置寄存器CFG_IVRK_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    unCfgIvRkCtrl.u32 = 0;
    unCfgIvRkCtrl.bits.alg_sel = g_CSA3Info.Alg;
    DRV_ADVCA_WriteReg(CA_V300_IVRK_CTRL, unCfgIvRkCtrl.u32); //CAStatus.bits.klad_busy will be set to 1

    //Now Wait until CAStatus.bits.klad_busy == 0
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    return Ret;
}

HI_S32 HAL_ADVCA_V300_SetCavendorOpt(CA_VENDOR_OPT_S *pstCavendorOpt)
{
    HI_S32 Ret = HI_SUCCESS;
    HI_UNF_ADVCA_VENDORID_E enCaVendor;
    HI_UNF_ADVCA_CA_VENDOR_OPT_E enCaVendorOpt;
    HI_U8 *pu8TmpBuf = NULL;

    if (HI_NULL == pstCavendorOpt)
    {
        HI_ERR_CA("invalid param\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    enCaVendor = pstCavendorOpt->enCaVendor;
    enCaVendorOpt = pstCavendorOpt->stCaVendorOpt.enCaVendorOpt;
    switch(enCaVendor)
    {
    case HI_UNF_ADVCA_IRDETO:

        if (enCaVendorOpt == HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_CHIP_CONF_CMAC)
        {
            Ret = HAL_ADVCA_V300_GenChipConfCmac(pstCavendorOpt->stCaVendorOpt.unCaVendorOpt.stIrdetoChipConfCmac.au8ChipConfBitm,
                                                                                                        pstCavendorOpt->stCaVendorOpt.unCaVendorOpt.stIrdetoChipConfCmac.au8ChipConfCmac);
        }
        else if (enCaVendorOpt == HI_UNF_ADVCA_CA_VENDOR_OPT_IRDETO_ACTIVATION_CODE)
        {
            pu8TmpBuf = pstCavendorOpt->stCaVendorOpt.unCaVendorOpt.stIrdetoCsa3ActCode.au8CSA3ActCode;
            Ret = HAL_ADVCA_V300_SetIrdetoCsa3ActCode(pu8TmpBuf);
        }
        else
        {
            HI_ERR_CA("Invalid CA vendor attribute, enCaVendorOpt = %d\n", enCaVendorOpt);
            Ret = HI_ERR_CA_INVALID_PARA;
        }
        break;

    default:
        HI_ERR_CA("Invalid CA vendor type, enCaVendor = %d\n", enCaVendor);
        break;
    }

    return Ret;
}

