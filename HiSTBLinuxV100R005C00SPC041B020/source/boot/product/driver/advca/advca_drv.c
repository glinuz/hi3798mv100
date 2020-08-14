/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : advca_drv.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2013-08-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "advca_reg.h"
#include "otp.h"
#include "advca_drv.h"
#include "otp.h"
#include "hi_unf_otp.h"
#ifdef HI_MINIBOOT_SUPPORT
#include "delay.h"
#endif
#define BOOT_FROM_OTP_BIT 		(1)
#define BOOT_FROM_CHIPSET_PIN 	(0)

static HI_UNF_ADVCA_ALG_TYPE_E g_R2RAlgType = HI_UNF_ADVCA_ALG_TYPE_TDES;
static HI_UNF_ADVCA_KEYLADDER_LEV_E g_R2RLadderSel = HI_UNF_ADVCA_KEYLADDER_LEV2;
HI_U32 DRV_ADVCA_ReadReg(HI_U32 addr);
HI_VOID DRV_ADVCA_WriteReg(HI_U32 addr, HI_U32 val);
HI_S32 s_CA_CheckVmxAdvanced(HI_BOOL *pbVmxAdvanced);

extern HI_S32 HI_OTP_Write(HI_U32 addr, HI_U32 u32Data);
/*========================interface of CAV300======================================*/
HI_VOID DRV_ADVCA_WriteReg(HI_U32 addr, HI_U32 val)
{
    ca_wtReg(addr, val);
	return;
}

HI_U32 DRV_ADVCA_ReadReg(HI_U32 addr)
{
	HI_U32 val;
    ca_rdReg(addr, val);
	return val;
}

HI_VOID DRV_ADVCA_Wait(HI_U32 us)
{
	return;
}

static HI_S32 HAL_ADVCA_V300_WaitStat(HI_VOID)
{
    HI_U32 cnt = 0;
    CA_V300_CA_STATE_U CAStatus;
    HI_S32 errState = 0;

    /* wait for KeyLadder calc done */
    // corform one value when test, about 500
    while (cnt < 1000)
    {
        CAStatus.u32 = 0;
        CAStatus.u32 = DRV_ADVCA_ReadReg(CA_V300_CA_STATE);
        if (CAStatus.bits.klad_busy == 0)
        {
            //HI_INFO_CA("ca_decrypted finished, cnt:%d\n", cnt);
            break;
        }
        udelay(1000);
        cnt++;
    }

    if (cnt >= 100)
    {
        HI_ERR_CA("\n Error CA handle too long !!!!!!!!!!! \n");
        return HI_FAILURE;      /* time out */
    }

    /* success */
    errState = CAStatus.bits.err_state;
    return errState;
}

HI_S32 CA_GetR2RLadder(HI_UNF_ADVCA_KEYLADDER_LEV_E *penSel)
{
    HI_S32 Ret = HI_SUCCESS;

    if (HI_NULL == penSel)
    {
        HI_ERR_CA("pointer parameter is NULL.\n");
        return HI_ERR_CA_INVALID_PARA;
    }
        
    Ret = OTP_GetR2RLadderLevel((OTP_KEY_LEVEL_E *)&g_R2RLadderSel);
    *penSel = g_R2RLadderSel;

    return Ret;
}
// limited, only aes
HI_S32 CA_SetR2RAlg(HI_UNF_ADVCA_ALG_TYPE_E type)
{
    if ((type != HI_UNF_ADVCA_ALG_TYPE_TDES) && (type != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("R2R Algorithm Error = %d ! \n", type);
        return HI_ERR_CA_INVALID_PARA;
    }
    g_R2RAlgType = type;

    return HI_SUCCESS;
}
HI_S32 CA_CryptR2R(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,HI_U32 *pu32DataIn,HI_U32 AddrID,HI_BOOL bIsDeCrypt)
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
    //(1)	先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
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

    /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
    //(2)	配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);

    //(3)	配置寄存器R2R_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    R2RCtrl.u32 = 0;
    R2RCtrl.bits.level_sel = enLevel;
    R2RCtrl.bits.mc_alg_sel = 0;//Multicipher Algorithm. default setting
    R2RCtrl.bits.tdes_aes_sel = g_R2RAlgType;
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
        return HI_FAILURE;
    }
    DRV_ADVCA_Wait(100);
    return HI_SUCCESS;
}

static CA_KEYLADDER_INFO_S g_MiscInfo = {0};
static CA_KEYLADDER_INFO_S g_CSA2Info = {0};
static CA_KEYLADDER_INFO_S g_CSA3Info = {0};
static CA_KEYLADDER_INFO_S g_R2RInfo  = {0};
static CA_KEYLADDER_INFO_S g_SPInfo   = {0};


HI_S32 CA_GetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penValue)
{
    HI_U32 addr = 0x4;
	OTP_INTERNAL_PV_1_U PV_1;

    if ( NULL == penValue)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = HI_OTP_Read(addr);

    if( (0x00 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV2;
    }
    else if( (0x01 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV3;
    }
    else if( (0x00 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV4;
    }
    else if( (0x01 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV5;
    }

    return HI_SUCCESS;
}

HI_S32 CA_MISCKeyLadder_Open(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bVmxAdvanced = HI_FALSE;
	ret = s_CA_CheckVmxAdvanced(&bVmxAdvanced);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("s_CA_CheckVmxAdvanced failed.\n");
        return HI_FAILURE;
    }
    if(HI_TRUE == bVmxAdvanced)
    {
        g_SPInfo.OpenFlag = HI_TRUE;
    }
    else
    {
        g_MiscInfo.OpenFlag = HI_TRUE;
    }
    return HI_SUCCESS;
}

HI_S32 CA_MISCKeyLadder_Close(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bVmxAdvanced = HI_FALSE;
    ret = s_CA_CheckVmxAdvanced(&bVmxAdvanced);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("s_CA_CheckVmxAdvanced failed.\n");
        return HI_FAILURE;
    }
    if(HI_TRUE == bVmxAdvanced)
    {
        g_SPInfo.OpenFlag = HI_FALSE;
    }
    else
    {
	g_MiscInfo.OpenFlag = HI_FALSE;
    }
    return HI_SUCCESS;
}

HI_S32 CA_SetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E enType)
{
    HI_S32 ret = HI_SUCCESS;
    HI_BOOL bVmxAdvanced = 0;
    if ((enType != HI_UNF_ADVCA_ALG_TYPE_TDES) && (enType != HI_UNF_ADVCA_ALG_TYPE_AES))
    {
        HI_ERR_CA("Invalid Misc Keyladder algorithm (%d)!\n", enType);
        return HI_ERR_CA_INVALID_PARA;
    }
    ret = s_CA_CheckVmxAdvanced(&bVmxAdvanced);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("s_CA_CheckVmxAdvanced failed.\n");
        return HI_FAILURE;
    }
    if(HI_TRUE == bVmxAdvanced)
    {
        g_SPInfo.Alg = enType;
    }
    else
    {
    g_MiscInfo.Alg = enType;
    }

    return HI_SUCCESS;
}

HI_S32 CA_V300_GetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType)
{
    if( NULL == penType)
    {
        HI_ERR_CA("Invalid param ,NULL pointer !\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    *penType = g_MiscInfo.Alg;

    return HI_SUCCESS;
}

HI_S32 CA_DecryptMisc(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
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
    if (HI_SUCCESS != Ret)
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
        return HI_FAILURE;
    }

    DRV_ADVCA_Wait(100);

    return HI_SUCCESS;
}


HI_S32 HAL_ADVCA_V300_DecryptMisc(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                            HI_U32 *pu32DataIn,
                            HI_U32 AddrID,
                            HI_BOOL bEvenOrOdd,
                            DRV_ADVCA_CA_TARGET_E enCwTarget)
{
	return CA_DecryptMisc(enLevel, pu32DataIn, AddrID, bEvenOrOdd, enCwTarget);
}

HI_S32 CA_DecryptSP(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                            HI_U32 *pu32DataIn,
                            HI_U32 AddrID,
                            HI_BOOL bEvenOrOdd,
                            DRV_ADVCA_CA_TARGET_E enCwTarget)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;
    CA_V300_SP_CTRL_U unSPCtrl;

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
    if (HI_SUCCESS != Ret)
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
    unSPCtrl.u32 = 0;
    if(enLevel == HI_UNF_ADVCA_KEYLADDER_LEV5)
    {
        unSPCtrl.bits.level_sel_5 = 1;
    }
    else
    {
        unSPCtrl.bits.level_sel = enLevel;
    }
    unSPCtrl.bits.tdes_aes_sel = g_SPInfo.Alg;

    /* multicipher channel ID */
	unSPCtrl.bits.raw_mode = 1;
    unSPCtrl.bits.key_addr = AddrID & 0xFF;

    /* CAStatus.bits.klad_busy will be set to 1 */
    DRV_ADVCA_WriteReg(CA_V300_SP_CTRL, unSPCtrl.u32);

    /* Now Wait until CAStatus.bits.klad_busy == 0 */
    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("Key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
    	HI_ERR_CA("SP Key ladder error, ret:0x%x\n", Ret);
        return HI_FAILURE;
    }

    DRV_ADVCA_Wait(100);

    return HI_SUCCESS;
}

HI_S32 CA_SetMISCSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret = 0;
    HI_BOOL bVmxAdvanced = HI_FALSE;

    ret = s_CA_CheckVmxAdvanced(&bVmxAdvanced);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CA("s_CA_CheckVmxAdvanced failed.\n");
        return HI_FAILURE;
    }
    if(HI_TRUE == bVmxAdvanced)
    {
        ret = CA_DecryptSP(enStage,(HI_U32 *)pu8Key,0,0, 0);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_CA("CA_DecryptSP failed, ret:0x%x\n", ret);
            return HI_FAILURE;
        }
    }
    else
    {
        ret = CA_DecryptMisc(enStage,(HI_U32 *)pu8Key,0,0, 0);
        if(HI_SUCCESS != ret)
    {
            HI_ERR_CA("CA_DecryptMisc failed, ret:0x%x\n", ret);
       return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

/* New UNF interface: HI_UNF_ADVCA_EncryptSwpk */
HI_S32 CA_EncryptSwpk(HI_U32 *pClearSwpk,HI_U32 *pEncryptSwpk)
{
    HI_U32 index;
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    /* Key Ladder internal use TDES.
    Just Need to input data. */

    if ((HI_NULL == pClearSwpk) || (HI_NULL == pEncryptSwpk))
    {
       HI_ERR_CA("HI_NULL == pClearSwpk, pEncryptSwpk\n");
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
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
    //(2)	配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pClearSwpk[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pClearSwpk[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pClearSwpk[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pClearSwpk[3]);

    //(3)	配置寄存器BL_CTRL_ENC，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_ENC, 0x01);//0x1c Set Register

    Ret = HAL_ADVCA_V300_WaitStat();
    if (Ret > 0)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }

    for(index = 0; index < 16; index += 4) //Data Result is 128bit(16Bytes) data
    {
        pEncryptSwpk[(index / 4)] = DRV_ADVCA_ReadReg(CA_V300_BLK_ENC_RSLT + index);
    }

    return HI_SUCCESS;
}

/* Called by HAL_Cipher_SetKey */
HI_S32 CA_DecryptSwpk(HI_U32 AddrID, HI_U32 *pEncryptSwpk)
{
    HI_S32 Ret = HI_SUCCESS;
    CA_V300_CONFIG_STATE_U unConfigStatus;

    //Key Ladder internal use TDES.
    //Just Need to input data.
    /* para check*/
    if (HI_NULL == pEncryptSwpk)
    {
       HI_ERR_CA("HI_NULL == pEncryptSwpk\n");
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
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_CA("Keyladder is busy now!\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    /*2  put EncryptData to CA register(DATA input) */
    //(2)	配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pEncryptSwpk[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pEncryptSwpk[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pEncryptSwpk[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pEncryptSwpk[3]);

    //(3)	配置寄存器BL_CTRL_DEC，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_DEC, (AddrID & 0x3F) << 8);//0x1c Set Register
//    DRV_ADVCA_WriteReg(CA_V300_BL_CTRL_DEC, 0x01);//0x1c Set Register

    Ret = HAL_ADVCA_V300_WaitStat();
    if (HI_FAILURE == Ret)
    {
        HI_ERR_CA("key ladder timeout\n");
        return HI_ERR_CA_WAIT_TIMEOUT;
    }
    else if (HI_SUCCESS != Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 CA_CryptHCA(HI_U32 AddrID)
{
        HI_S32 Ret = HI_SUCCESS;
        CA_V300_CONFIG_STATE_U unConfigStatus;
        CA_V300_CAUK_CTRL_U unCaukCtrl;

        //(1)   先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
        //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
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
            return HI_FAILURE;
        }
        DRV_ADVCA_Wait(100);

        return HI_SUCCESS;
}

/*========================interface of CA  Common======================================*/

HI_S32 CA_Init(HI_VOID)
{

    OTP_Init();

    CA_GetR2RLadder(&g_R2RLadderSel);

    memset(&g_SPInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_SPInfo.OpenFlag = HI_FALSE;
    g_SPInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_AES;

    memset(&g_MiscInfo, 0, sizeof(CA_KEYLADDER_INFO_S));
    g_MiscInfo.OpenFlag = HI_FALSE;
    g_MiscInfo.Alg = HI_UNF_ADVCA_ALG_TYPE_TDES;

    return HI_SUCCESS;
}

HI_S32 CA_DeInit(HI_VOID)
{
    OTP_DeInit();

    return HI_SUCCESS;
}

/* Called by HAL_Cipher_SetKey */
HI_S32 CA_DecryptCipher(HI_U32 AddrID, HI_U32 *pDataIn)
{
	HI_S32 ret = 0;

	if(HI_TRUE == g_MiscInfo.OpenFlag)
	{
		ret = CA_DecryptMisc(HI_UNF_ADVCA_KEYLADDER_LEV5, pDataIn, AddrID, 0, 1); //target : multi-cipher
	}
	else if(HI_TRUE == g_SPInfo.OpenFlag)
	{
		ret = CA_DecryptSP(HI_UNF_ADVCA_KEYLADDER_LEV5, pDataIn, AddrID, 0, 1); //target : multi-cipher
	}
	else
	{
		ret = CA_CryptR2R(g_R2RLadderSel, pDataIn, AddrID, 1);		//target : multi-cipher
	}

	return ret;
}

/* New UNF interface: HI_UNF_ADVCA_SetR2RSessionKey */
HI_S32 CA_SetR2RSessionKey(HI_UNF_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key)
{
    HI_S32 ret = 0;
    CA_CRYPTPM_S cryptPm;

    memset(&cryptPm, 0, sizeof(CA_CRYPTPM_S));
    cryptPm.ladder = enStage;
    memcpy(cryptPm.pDin, pu8Key, 16);

    ret = CA_CryptR2R(cryptPm.ladder,cryptPm.pDin,0,1);
    if (ret != HI_SUCCESS)
    {
       HI_ERR_CA("fail to call CA_CryptR2R, ret:0x%x\n", ret);
       return HI_FAILURE;
    }

    return ret;
}

HI_S32 CA_GetBootMode(HI_UNF_ADVCA_FLASH_TYPE_E *penFlashType)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 bootMode = 0;

    if (NULL == penFlashType)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    bootMode = HI_OTP_Read(OTP_INTERNAL_PV_0);
    switch (bootMode & 0x03)
    {
        case 0x00:
		{
            *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
            break;
		}
        case 0x01:
		{
            *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
            break;
        }
        case 0x02:
		{
            *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
            break;
        }
        case 0x03:
		{
            *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
            break;
		}
        default:
		{
            *penFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
            break;
		}
    }

    return ret;
}

static HI_S32 CA_SetBootMode(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType)
{
    HI_S32 ret = HI_SUCCESS;
    OTP_INTERNAL_PV_0_U PV_0;
    OTP_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (enFlashType >= HI_UNF_ADVCA_FLASH_TYPE_BUTT)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = HI_OTP_Read(OTP_INTERNAL_PVLOCK_0);
    if ((1 == PVLOCK_0.bits.boot_mode_sel_1_lock) && (1 == PVLOCK_0.bits.boot_mode_sel_0_lock))
    {
        PV_0.u32 = HI_OTP_Read(OTP_INTERNAL_PV_0);
        if (enFlashType == ((PV_0.bits.boot_mode_sel_1 << 1) + PV_0.bits.boot_mode_sel_0))
        {
            /*if type has been set as the Boot Mode already, HI_SUCCESS should be returned*/
            return HI_SUCCESS;
        }
        else
        {
            /*if Boot Mode has been set, but it's not type, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set boot mode*/
    PV_0.u32 = 0;
    switch (enFlashType)
    {
        case HI_UNF_ADVCA_FLASH_TYPE_SPI:
        {
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_0.bits.boot_mode_sel_0 = 0;
            break;
		}
        case HI_UNF_ADVCA_FLASH_TYPE_NAND:
		{
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_0.bits.boot_mode_sel_0 = 1;
            break;
		}
        case HI_UNF_ADVCA_FLASH_TYPE_SD:
		{
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_0.bits.boot_mode_sel_0 = 0;
            break;
		}
        case HI_UNF_ADVCA_FLASH_TYPE_EMMC:
		{
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_0.bits.boot_mode_sel_0 = 1;
            break;
        }
        default:
		{
            HI_ERR_CA("Not supported flash type 0x%x!\n", enFlashType);
            return HI_ERR_CA_NOT_SUPPORT;
		}
    }

    ret = HI_OTP_Write(OTP_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock boot mode*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.boot_mode_sel_0_lock = 1;
    PVLOCK_0.bits.boot_mode_sel_1_lock = 1;

    ret = HI_OTP_Write(OTP_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

static HI_S32 CA_SetBootSelCtrl(HI_U32 u32BootSel)
{
    HI_S32 ret = HI_SUCCESS;
    OTP_INTERNAL_PV_1_U PV_1;
    OTP_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32BootSel != BOOT_FROM_CHIPSET_PIN) && (u32BootSel != BOOT_FROM_OTP_BIT))
    {
    	HI_ERR_CA("Invalid param!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = HI_OTP_Read(OTP_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.bootsel_ctrl_lock)
    {
        PV_1.u32 = HI_OTP_Read(OTP_INTERNAL_PV_1);
        if (u32BootSel == PV_1.bits.bootsel_ctrl)
        {
            /* value already set to the right one, return HI_SUCCESS */
            return HI_SUCCESS;
        }
        else
        {
            /* set value to another value, return HI_ERR_CA_SETPARAM_AGAIN */
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /* set value */
    if (u32BootSel == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.bootsel_ctrl = 1;
        ret = HI_OTP_Write(OTP_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write bootsel_ctrl!\n");
            return ret;
        }
    }

    /* lock value */
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bootsel_ctrl_lock = 1;
    ret = HI_OTP_Write(OTP_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write bootsel_ctrl_lock!\n");
        return ret;
    }

    return ret;
}
HI_S32 CA_SetRSAKeyLockFlag(HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
    HI_S32 ret = HI_SUCCESS;
	HI_U32 u32Temp = 0;

	if(NULL == pstOtpFuseAttr)
	{
		return HI_ERR_CA_INVALID_PARA;
	}

	if(HI_TRUE == pstOtpFuseAttr->unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked)
	{
		u32Temp = HI_OTP_Read(OTP_INTERNAL_DATALOCK_1);
		if(1 == (u32Temp & 1))
		{
			return HI_SUCCESS;
		}

		u32Temp = u32Temp | 1;
		ret = HI_OTP_Write(OTP_INTERNAL_DATALOCK_1, u32Temp);
		if(HI_SUCCESS != ret)
		{
			HI_ERR_CA("Write rsa key lock flag failed, ret:0x%x\n", ret);
			return HI_ERR_CA_INVALID_PARA;
		}
	}

	return HI_SUCCESS;
}

HI_S32 CA_GetRSAKeyLockFlag(HI_UNF_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
	HI_U8 u8Temp = 0;

	if(NULL == pstOtpFuseAttr)
	{
		return HI_ERR_CA_INVALID_PARA;
	}

	u8Temp = HI_OTP_ReadByte(OTP_INTERNAL_DATALOCK_1);
	if(0 == (u8Temp & 1))
	{
		pstOtpFuseAttr->unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_FALSE;
	}
	else
	{
		pstOtpFuseAttr->unOtpFuseAttr.stRSAKeyLockFlag.bIsLocked = HI_TRUE;
	}

	return HI_SUCCESS;
}

HI_S32 CA_EnableSCSActive(HI_UNF_ADVCA_FLASH_TYPE_E enFlashType)
{
    HI_S32 ret = HI_SUCCESS;
	OTP_ONE_WAY_0_U OneWay;
	OTP_INTERNAL_DATALOCK_0_U unDataLock;
    HI_U8 u8ScsBak = 0;
	HI_BOOL bSCSActiveFlag = HI_FALSE;

    if (enFlashType >= HI_UNF_ADVCA_FLASH_TYPE_BUTT)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

	ret = CA_GetSCSActiveFlag(&bSCSActiveFlag);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Get scs active flag failed.\n");
		return ret;
	}

	if(HI_TRUE == bSCSActiveFlag)
	{
		HI_ERR_CA("SCS has been already enabled before.\n");
		return HI_FAILURE;
	}

	/* write SCS_activation flag */
	OneWay.u32 = 0;
	OneWay.bits.SCS_activation = 1;
	ret = HI_OTP_Write(OTP_INTERNAL_ONEWAY_0, OneWay.u32);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Write scs active flag failed, ret:0x%x.\n", ret);
		return ret;
	}

	/* write scs_en_bak_lock */
	unDataLock.u32 = 0;
	unDataLock.u32 = HI_OTP_Read(OTP_INTERNAL_DATALOCK_0);
	if(1 != unDataLock.bits.scs_en_bak_lock)
	{
		/* write scs_en_bak */
	    u8ScsBak = SCS_BAK_ENABLE;
		ret = HI_OTP_WriteByte(OTP_SCS_EN_BAK_ADDR, u8ScsBak);
		if(HI_SUCCESS != ret)
		{
			HI_ERR_CA("Write scs bak failed, ret:0x%x.\n", ret);
			return ret;
		}

		unDataLock.u32 = 0;
		unDataLock.bits.scs_en_bak_lock = 1;
		ret = HI_OTP_Write(OTP_INTERNAL_DATALOCK_0, unDataLock.u32);
		if(HI_SUCCESS != ret)
		{
			HI_ERR_CA("Write scs_en_bak_lock failed, ret:0x%x.\n", ret);
			return ret;
		}
	}

	/* write boot mode flash type */
	ret = CA_SetBootMode(enFlashType);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Write boot mode failed, ret:0x%x, enFlashType:0x%x.\n", ret, enFlashType);
		return ret;
	}

	/* write bootsel_ctrl */
	ret = CA_SetBootSelCtrl(BOOT_FROM_OTP_BIT);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("Write bootsel_ctrl(1) failed, ret:0x%x\n", ret);
		return ret;
	}

	return HI_SUCCESS;
}

HI_S32 CA_GetSCSActiveFlag(HI_BOOL *pbSCSActiveFlag)
{
    OTP_ONE_WAY_0_U OneWay_0;
    HI_U8 u8ScsBak = 0;

    if (NULL == pbSCSActiveFlag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = HI_OTP_Read(OTP_INTERNAL_ONEWAY_0);
    u8ScsBak = HI_OTP_ReadByte(OTP_SCS_EN_BAK_ADDR);

    if ((1 == OneWay_0.bits.SCS_activation) || (SCS_BAK_DISABLE != u8ScsBak))
    {
        *pbSCSActiveFlag = HI_TRUE;
    }
    else
    {
        *pbSCSActiveFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetCSA2LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_0);
    if (1 == PV_0.bits.csa2_lv_sel)
    {
        *pLevel = CA_OTP_KEY_LEV3;
    }
    else
    {
        *pLevel = CA_OTP_KEY_LEV2;
    }

    return ret;
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

HI_S32 DRV_CA_OTP_V200_GetR2RLadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_0);
    if (1 == PV_0.bits.r2r_lv_sel)
    {
        *pLevel = CA_OTP_KEY_LEV3;
    }
    else
    {
        *pLevel = CA_OTP_KEY_LEV2;
    }

    return ret;
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

HI_S32 DRV_CA_OTP_V200_GetSPLadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_0);
    PV_1.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_1);
    if( (0x00 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = HI_UNF_ADVCA_KEYLADDER_LEV2;
    }
    else if( (0x01 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = HI_UNF_ADVCA_KEYLADDER_LEV3;
    }
    else if( (0x00 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = HI_UNF_ADVCA_KEYLADDER_LEV4;
    }
    else if( (0x01 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = HI_UNF_ADVCA_KEYLADDER_LEV5;
    }
    else
    {
        *pLevel = HI_UNF_ADVCA_KEYLADDER_BUTT;
        ret = HI_FAILURE;
    }

    return ret;
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

HI_S32 DRV_CA_OTP_V200_GetCSA3LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_0);
    if (1 == PV_0.bits.csa3_lv_sel)
    {
        *pLevel = CA_OTP_KEY_LEV3;
    }
    else
    {
        *pLevel = CA_OTP_KEY_LEV2;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSecureChipId(CA_OTP_VENDOR_TYPE_E *pu32SecureChipId)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32SecureChipId;

    if (NULL == pu32SecureChipId)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    u32SecureChipId = HI_OTP_Read(CA_OTP_V200_INTERNAL_CA_VENDOR_ID);
    u32SecureChipId = u32SecureChipId & 0xff;

    switch (u32SecureChipId)
    {
        case 0x0:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_NONE;
            break;
        }
        case 0x1:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_NAGRA;
            break;
        }
        case 0x02:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_IRDETO;
            break;
        }
        case 0x03:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_CONAX;
            break;
        }
        case 0x04:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_NDS;
            break;
        }
        case 0x05:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_SUMA;
            break;
        }
        case 0x06:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_NOVEL;
            break;
        }
        case 0x07:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_VERIMATRIX;
            break;
        }
        case 0x08:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_CTI;
            break;
        }
        case 0x09:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_SAFEVIEW;
            break;
        }
        case 0x0a:
        {
            *pu32SecureChipId = CA_OTP_VERDOR_LATENSE;
            break;
        }
        case 0x0b:
        {
            *pu32SecureChipId = CA_OTP_VERDOR_SH_TELECOM;
            break;
        }
        case 0x0c:
        {
            *pu32SecureChipId = CA_OTP_VERDOR_VIACCESS;
            break;
        }
        default:
        {
            *pu32SecureChipId = CA_OTP_VENDOR_BUTT;
            break;
        }
    }

    return ret;
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

	CA_SetMiscAlgorithm(enType);

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_GetMiscAlgorithm(HI_UNF_ADVCA_ALG_TYPE_E *penType)
{
    if( NULL == penType)
    {
        HI_ERR_CA("Invalid param ,NULL pointer !\n");
        return HI_ERR_CA_INVALID_PARA;
    }

	CA_V300_GetMiscAlgorithm(penType);

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E *penValue)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ( NULL == penValue)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = HI_OTP_Read(CA_OTP_V200_INTERNAL_PV_1);
    if( (0x00 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV2;
    }
    else if( (0x01 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV3;
    }
    else if( (0x00 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV4;
    }
    else if( (0x01 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
    {
        *penValue = HI_UNF_ADVCA_KEYLADDER_LEV5;
    }

    return HI_SUCCESS;
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
    *penType = g_MiscInfo.enDscMode;

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

    //(1)	先读寄存器CONIFG_STATE.st_vld，只有其值为1时，ca的寄存器值才是有效的，才能对寄存器操作。
    //一旦CONIFG_STATE.st_vld变高之后，会一直保持。只需上电之后查询一次。
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

    /*2  put DVB_RootKey（CK2）to CA register(DATA input) */
    //(2)	配置寄存器CA_DIN0，CA_DIN1，CA_DIN2，CA_DIN3，这是解密(加密)的输入数据。
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN0, pu32DataIn[0]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN1, pu32DataIn[1]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN2, pu32DataIn[2]);
    DRV_ADVCA_WriteReg(CA_V300_CA_DIN3, pu32DataIn[3]);

    //(3)	配置寄存器R2R_CTRL，之后逻辑会启动运算，逻辑同时把CA_STATE.klad_busy置高
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
        return HI_FAILURE;
    }
    DRV_ADVCA_Wait(100);

    return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_DecryptSP(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel,
                            HI_U32 *pu32DataIn,
                            HI_U32 AddrID,
                            HI_BOOL bEvenOrOdd,
                            DRV_ADVCA_CA_TARGET_E enCwTarget)
{
	return CA_DecryptSP(enLevel, pu32DataIn, AddrID, bEvenOrOdd, enCwTarget);
}



static HI_U32 VMX_ADVCA_Swith(CA_KEYLADDER_ATTR_S *pDeststKeyladderAttr, CA_KEYLADDER_ATTR_S *pSrcstKeyladderAttr)
{
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
    return HI_SUCCESS;
}

HI_S32 CA_GetCAVendorId(HI_U32 *pu32VendorId)
{
	HI_U32 value = 0;
    HI_U32 addr = 0xac;

	if(NULL == pu32VendorId)
	{
		return HI_FAILURE;
	}

	value = HI_OTP_Read(addr);
	*pu32VendorId = value & 0xff;

	return HI_SUCCESS;
}

HI_S32 s_CA_CheckVmxAdvanced(HI_BOOL *pbVmxAdvanced)
{
    HI_S32 ret = 0;
	HI_U32 u32CAVendorId = 0;
	HI_UNF_ADVCA_KEYLADDER_LEV_E enValue = HI_UNF_ADVCA_KEYLADDER_BUTT;

	(HI_VOID)CA_GetCAVendorId(&u32CAVendorId);
	ret = CA_GetMiscKlLevel(&enValue);
	if(HI_SUCCESS != ret)
	{
		HI_ERR_CA("CA_GetMiscKlLevel failed, ret:0x%x\n", ret);
		return HI_FAILURE;
	}

	if( (0x7 == u32CAVendorId) && (HI_UNF_ADVCA_KEYLADDER_LEV5 == enValue) )
	{
		*pbVmxAdvanced = HI_TRUE;
	}
	else
	{
		*pbVmxAdvanced = HI_FALSE;
	}

	return HI_SUCCESS;
}

HI_S32 HAL_ADVCA_V300_SetKeyladderAttr(CA_KEYLADDER_ATTR_S *pstKeyladderAttr)
{
    HI_UNF_ADVCA_KEYLADDER_TYPE_E            enKeyladderType;
    HI_UNF_ADVCA_KEYLADDER_R2R_ATTR_E        enR2RKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_SP_ATTR_E         enSPKeyladderAttr;
    HI_UNF_ADVCA_KEYLADDER_MISC_ATTR_E       enMiscKeyladderAttr;
    HI_UNF_ADVCA_ALG_TYPE_E                  enKeyladderAlg;
    HI_UNF_ADVCA_SP_DSC_MODE_E               enSPDscMode;
	CA_KEYLADDER_ATTR_S 					 TempstKeyladderAttr;
    CA_CRYPTPM_S stCryptParam;
    HI_S32 Ret = HI_SUCCESS;
	HI_BOOL bVmxAdvanced = HI_TRUE;

    if(pstKeyladderAttr == NULL)
    {
        HI_ERR_CA("Invalid param, pstKeyladderAttr = NULL\n");
        return HI_ERR_CA_INVALID_PARA;
    }

	memset(&TempstKeyladderAttr, 0, sizeof(CA_KEYLADDER_ATTR_S));
    Ret = s_CA_CheckVmxAdvanced(&bVmxAdvanced);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_CA("s_CA_CheckVmxAdvanced.\n");
        return HI_FAILURE;
    }

    if(bVmxAdvanced == HI_TRUE)
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
		case HI_UNF_ADVCA_KEYLADDER_CSA3:
		{
			HI_ERR_CA("Invalid keyladder attribute, enKeyladderType = %d\n", enKeyladderType);
			Ret = HI_ERR_CA_INVALID_PARA;
	        break;
		}
	    case HI_UNF_ADVCA_KEYLADDER_R2R:
		{
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
	    }
	    case HI_UNF_ADVCA_KEYLADDER_SP:
		{
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
	    }
	    case HI_UNF_ADVCA_KEYLADDER_MISC:
		{
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
	    }
	    case HI_UNF_ADVCA_KEYLADDER_LP:
		{
			HI_ERR_CA("Invalid keyladder attribute, enKeyladderType = %d\n", enKeyladderType);
			Ret = HI_ERR_CA_INVALID_PARA;
			break;
	    }
	    case HI_UNF_ADVCA_KEYLADDER_TA:
		{
			HI_ERR_CA("Invalid keyladder attribute, enKeyladderType = %d\n", enKeyladderType);
			Ret = HI_ERR_CA_INVALID_PARA;
	        break;
	    }
	    default:
	    {
	        HI_ERR_CA("Invalid keyladder type, enKeyladderType = %d\n", enKeyladderType);
	        Ret = HI_ERR_CA_INVALID_PARA;
	        break;
	    }
	}

    return Ret;
}

