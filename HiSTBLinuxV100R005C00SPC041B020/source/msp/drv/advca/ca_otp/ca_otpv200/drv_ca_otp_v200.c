/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_ca_otp_v200.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   : CA OTPV200 FUNC DEFINE
  Function List :
  History       :
******************************************************************************/
#include "drv_advca_internal.h"
#include "drv_advca_external.h"

extern OTP_EXPORT_FUNC_S *g_pOTPExportFunctionList;

const static HI_U16 rsa_crc_table[16] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
                              0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef};

static HI_U16 DRV_CA_OTP_CalculateCRC16 (const HI_U8 *pu8Data, HI_U32 u32DataLen)
{
    HI_U16 crc_value = 0xff;
    HI_U16 polynomial = 0x8005;
    HI_U16 data_index = 0;
    HI_U32 l = 0;
    HI_BOOL flag = HI_FALSE;
    HI_U8 byte0 = 0;
    HI_U8 au8CrcInput[17];

    if (NULL == pu8Data)
    {
        HI_ERR_CA("Input param error, null pointer!\n");
        return crc_value;
    }

    if (u32DataLen == 0)
    {
        HI_ERR_CA("Input param error, length = 0!\n");
        return crc_value;
    }

    memset(au8CrcInput, 0, sizeof(au8CrcInput));
    au8CrcInput[0] = 0x55;
    memcpy(au8CrcInput + 1, pu8Data, u32DataLen);
    u32DataLen += 1;

    for (data_index = 0; data_index < u32DataLen; data_index++)
    {
        byte0 = au8CrcInput[data_index];
        crc_value ^= byte0 * 256;

        for (l=0; l<8; l++)
        {
            flag = ((crc_value & 0x8000) == 32768);
            crc_value = (crc_value & 0x7FFF)*2;
            if (HI_TRUE == flag)
            {
                crc_value ^= polynomial;
            }
        }
    }

    return crc_value;
}

static HI_U16 DRV_CA_OTP_CalculateRSAPerByte(HI_U8 u8KeyByte, HI_U16 u16Crc)
{
   HI_U8 u8Tmp = 0;
   u8Tmp    = ((HI_U8) (u16Crc / 256)) / 16;
   u16Crc <<= 4;
   u16Crc  ^= (HI_U16) rsa_crc_table[u8Tmp ^ (u8KeyByte / 16)];
   u8Tmp    = ((HI_U8) (u16Crc / 256)) / 16;
   u16Crc <<= 4;
   u16Crc  ^= (HI_U16) rsa_crc_table[u8Tmp ^ (u8KeyByte & 0x0f)];

   return u16Crc;
}

static HI_BOOL isSecureChipset(HI_VOID)
{
    HI_U32 u32IdWord = 0x0;

    u32IdWord = g_pOTPExportFunctionList->HAL_OTP_V200_Read(CA_OTP_V200_INTERNAL_IDWORD);
    if (CA_OTP_V200_NOMAL_IDWORD_VALUE == u32IdWord)
    {
        return HI_FALSE;
    }
    else
    {
        return HI_TRUE;
    }
}

static HI_U16 DRV_CA_OTP_CalculateRSACRC16(const HI_U8 *pu8Data)
{
    HI_U16 crc_value = 0;
    HI_S32 index     = 0;

    for (index = 0; index < 512; index++)
    {
        crc_value = DRV_CA_OTP_CalculateRSAPerByte(pu8Data[index], crc_value);
    }

    return crc_value;
}

HI_S32 DRV_CA_OTP_V200_Set_MarketId(HI_U32 u32Id)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 marketId = 0;

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.msid_lock)
    {
        marketId = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_MSID);
        if (marketId == u32Id)
        {
            /*if u32Id has been set as the market Id already, HI_SUCCESS should be returned*/
            return HI_SUCCESS;
        }
        else
        {
            /*if market Id has been set, but it's not u32Id, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    //Write MarketId
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_MSID, u32Id);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    //Set mktsid_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.msid_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Set_UniqueMarketId(HI_U8 *pu8Id)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 i = 0;

    if (NULL == pu8Id)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.msid_lock)
    {
        /*if market Id has been set, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
        return HI_ERR_CA_SETPARAM_AGAIN;
    }

    //Write MarketId
    for ( i = 0 ; i < 4 ; i++ )
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_MSID + i, pu8Id[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    //Set mktsid_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.msid_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_MarketId(HI_U32 *pu32MarketID)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32MarketID)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32MarketID = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_MSID);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Get_UniqueMarketId(HI_U8 *pu8MarketID)
{
    HI_U32    i   = 0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu8MarketID)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    for ( i = 0 ; i < 4 ; i++ )
    {
        pu8MarketID[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_MSID + i);
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Set_STBSN(HI_U32 u32SN)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 stbsn = 0;

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.stbsn0_lock)
    {
        stbsn = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_STB_SN_0);
        if (stbsn == u32SN)
        {
            /*if u32SN has been set as the STBSN already, HI_SUCCESS should be returned*/
            return HI_SUCCESS;
        }
        else
        {
            /*if STBSN has been set, but it's not u32SN, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    //Write STBSN
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_STB_SN_0, u32SN);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    //Set stbsn_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.stbsn0_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Set_UniqueSTBSN(HI_U8 *pu8SN)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 i = 0;

    if (NULL == pu8SN)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.stbsn0_lock)
    {
        /*if STBSN has been set, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
        return HI_ERR_CA_SETPARAM_AGAIN;
    }

    //Write STBSN
    for ( i = 0 ; i < 4 ; i++ )
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_STB_SN_0 + i, pu8SN[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    //Set stbsn_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.stbsn0_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSTBSN(HI_U32 *pu32SN)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32SN)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32SN = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_STB_SN_0);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Get_UniqueSTBSN(HI_U8 *pu8SN)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;

    if (NULL == pu8SN)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    for ( i = 0 ; i < 4 ; i++ )
    {
        pu8SN[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_STB_SN_0 + i);
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Set_UniqueVersionId(HI_U8 *pu8Id)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 i = 0;

    if (NULL == pu8Id)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.version_id_lock)
    {
        /*if version Id has been set, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
        return HI_ERR_CA_SETPARAM_AGAIN;
    }

    //Write VersionId
    for ( i = 0 ; i < 4 ; i++ )
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_VERSIONID + i, pu8Id[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    //not set version id lock
#else
    //Set version_id_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.version_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }
#endif

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Get_UniqueVersionId(HI_U8 *pu8VersionID)
{
    HI_U32    i   = 0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu8VersionID)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    for ( i = 0 ; i < 4 ; i++ )
    {
        pu8VersionID[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_VERSIONID + i);
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockHardCwSel(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ts_csa2_hardonly_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (1 == PV_0.bits.ts_csa2_hardonly_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    PV_0.u32 = 0;
    PV_0.bits.ts_csa2_hardonly_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_csa2_hardonly_en_lock= 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockHardSPESel(HI_VOID)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    HI_S32    ret = HI_SUCCESS;

    PV_0.u32 = 0;
    PV_0.bits.ts_sp_hardonly_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockHardNVSel(HI_VOID)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    HI_S32    ret = HI_SUCCESS;

    PV_0.u32 = 0;
    PV_0.bits.ts_nv_hardonly_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockBootDecEn(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    /* write to OTP */
    PV_0.u32 = 0;
    PV_0.bits.bload_dec_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootDecMode(HI_U32 u32Mode)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((0 != u32Mode) && (1 != u32Mode))
    {
        HI_ERR_CA("HI_ERR_CA_INVALID_PARA\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.bload_mode_sel_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Mode == PV_0.bits.bload_mode_sel)
        {
            /*boot dec mode already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("HI_ERR_CA_INVALID_PARA\n");
            /*set boot dec mode to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (1 == u32Mode)
    {
        /* write to OTP */
        PV_0.u32 = 0;
        PV_0.bits.bload_mode_sel = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock bload_mode_sel*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.bload_mode_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetKlDPAClkSelEn(HI_BOOL bValue)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ((0 != bValue) && (1 != bValue))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (1 == bValue)
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.kl_dpa_clk_sel_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }

        PV_1.bits.kl_dpa_clk_sel_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }
    else
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.kl_dpa_clk_sel_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("Can not set from 1 to 0!\n");
            return HI_ERR_CA_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetKlDPAClkSelEn(HI_BOOL *pbValue)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ( NULL == pbValue)
    {
        HI_ERR_CA("Invalid param, NULL Pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( 1 == PV_1.bits.kl_dpa_clk_sel_en)
    {
        *pbValue = HI_TRUE;
    }
    else
    {
        *pbValue = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetKlDPAFilterClkEn(HI_BOOL bValue)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ((0 != bValue) && (1 != bValue))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (1 == bValue)
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.kl_dpa_filter_clk_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }

        PV_1.bits.kl_dpa_filter_clk_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }
    else
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.kl_dpa_filter_clk_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("Can not set from 1 to 0!\n");
            return HI_ERR_CA_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_GetKlDPAFilterClkEn(HI_BOOL *pbValue)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ( NULL == pbValue )
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( 1 == PV_1.bits.kl_dpa_filter_clk_en)
    {
        *pbValue = HI_TRUE;
    }
    else
    {
        *pbValue = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetMcDPAClkSelEn(HI_BOOL bValue)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ((0 != bValue) && (1 != bValue))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (1 == bValue)
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.mc_dpa_clk_sel_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }

        PV_1.bits.mc_dpa_clk_sel_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }
    else
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.mc_dpa_clk_sel_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("Can not set from 1 to 0!\n");
            return HI_ERR_CA_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetMcDPAClkSelEn(HI_BOOL *pbValue)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ( NULL == pbValue)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( 1 == PV_1.bits.mc_dpa_clk_sel_en)
    {
        *pbValue = HI_TRUE;
    }
    else
    {
        *pbValue = HI_FALSE;
    }

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_SetPvrDPAFilterClkEn(HI_BOOL bValue)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ((0 != bValue) && (1 != bValue))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (1 == bValue)
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.pvr_dpa_filter_clk_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }

        PV_1.bits.pvr_dpa_filter_clk_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }
    else
    {
        /* write to OTP */
        PV_1.u32 = 0;
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if( bValue == PV_1.bits.pvr_dpa_filter_clk_en)
        {
            /*already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("Can not set from 1 to 0!\n");
            return HI_ERR_CA_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetPvrDPAFilterClkEn(HI_BOOL *pbValue)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if ( NULL == pbValue )
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( 1 == PV_1.bits.pvr_dpa_filter_clk_en)
    {
        *pbValue = HI_TRUE;
    }
    else
    {
        *pbValue = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetDDRScrambleEn(HI_U32 u32Mode)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((0 != u32Mode) && (1 != u32Mode))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ddr_scramble_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Mode == PV_0.bits.ddr_scramble_en)
        {
            /*DDR scramble already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set DDR scramble to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (1 == u32Mode)
    {
        /* write to OTP */
        PV_0.u32 = 0;
        PV_0.bits.ddr_scramble_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock ddr_scramble_en*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ddr_scramble_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDDRScrambleEn(HI_U32 *pu32Mode)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Mode)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* write to OTP */
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Mode = PV_0.bits.ddr_scramble_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockR2RHardKey(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;

    //NO use in v300

    return ret;
}

HI_S32 DRV_CA_OTP_V200_DisableNV(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    /* write to OTP */
    PV_0.u32 = 0;
    PV_0.bits.tskl_nv_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_DisableLinkProtect(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    //Set link_prt_disable
    PV_0.u32 = 0;
    PV_0.bits.link_prt_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_DisableRuntimeCheck(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    //Set runtime_check_en
    PV_1.u32 = 0;
    PV_1.bits.runtime_check_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_DisableSelfBoot(HI_U32 u32SelfBoot)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;
    HI_U32 u32Tmp = 0;

    if ((0 != u32SelfBoot) && (1 != u32SelfBoot))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.self_boot_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32SelfBoot == PV_0.bits.self_boot_disable)
        {
            /*self boot already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set self boot to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (u32SelfBoot)
    {
        /*disable self boot*/
        PV_0.u32 = 0;
        PV_0.bits.self_boot_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }

        /*lock self_boot_disable*/
        PVLOCK_0.u32 = 0;
        PVLOCK_0.bits.self_boot_disable_lock = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }

        /* write selfboot_disable_bak */
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK, (HI_U8)SELFBOOT_BAK_DISABLE);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write selfboot_disable_bak!\n");
            return HI_FAILURE;
        }

        u32Tmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK);
        if( SELFBOOT_BAK_DISABLE != u32Tmp )
        {
            HI_ERR_CA("Fail to write selfboot_disable_bak!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        /*lock self_boot_disable*/
        PVLOCK_0.u32 = 0;
        PVLOCK_0.bits.self_boot_disable_lock = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }

        /* write selfboot_disable_bak */
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK, (HI_U8)SELFBOOT_BAK_ENABLE);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write selfboot_disable_bak!\n");
            return HI_FAILURE;
        }

        u32Tmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK);
        if( SELFBOOT_BAK_ENABLE != u32Tmp )
        {
            HI_ERR_CA("Fail to write selfboot_disable_bak!\n");
            return HI_FAILURE;
        }
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSelfBoot(HI_U32 *pu32SelfBoot)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    HI_U32 u32Tmp = 0;

    if (NULL == pu32SelfBoot)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_FALSE == isSecureChipset())
    {
        *pu32SelfBoot = 0;
        return HI_SUCCESS;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32SelfBoot = PV_0.bits.self_boot_disable;

    u32Tmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SELFBOOT_DISABLE_BAK);
    if( ( 1 == *pu32SelfBoot ) || ( SELFBOOT_BAK_ENABLE != u32Tmp ))
    {
        *pu32SelfBoot = 1;
    }
    else
    {
        *pu32SelfBoot = 0;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_DisableJtagWrite(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    //Set jtag_w_disable
    OneWay_0.u32 = 0;
    OneWay_0.bits.jtag_w_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagWriteDisable(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    //Get jtag_w_disable
    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32Disable = OneWay_0.bits.jtag_w_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootMode(CA_OTP_FLASH_TYPE_E type)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0, PV_0_OTP;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1, PV_1_OTP;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (type >= CA_OTP_FLASH_TYPE_BUTT)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /*set boot mode*/
    PV_0.u32 = 0;
    PV_1.u32 = 0;
    switch (type)
    {
#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
        case HI_UNF_ADVCA_FLASH_TYPE_SPI:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_NAND:
            PV_0.bits.boot_mode_sel_0 = 1;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_EMMC:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_1.bits.boot_mode_sel_2 = 1;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_SD:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 1;
            break;
#elif defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
        case HI_UNF_ADVCA_FLASH_TYPE_SPI:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_NAND:
            PV_0.bits.boot_mode_sel_0 = 1;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_EMMC:
            PV_0.bits.boot_mode_sel_0 = 1;
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_1.bits.boot_mode_sel_2 = 1;
            break;
        case HI_UNF_ADVCA_FLASH_TYPE_SD:
            PV_0.bits.boot_mode_sel_0 = 0;
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_1.bits.boot_mode_sel_2 = 0;
            break;
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716cv200)
        case CA_OTP_FLASH_TYPE_SPI:
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_0.bits.boot_mode_sel_0 = 0;
            break;
        case CA_OTP_FLASH_TYPE_NAND :
            PV_0.bits.boot_mode_sel_1 = 0;
            PV_0.bits.boot_mode_sel_0 = 1;
            break;
        case CA_OTP_FLASH_TYPE_SD :
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_0.bits.boot_mode_sel_0 = 0;
            break;
        case CA_OTP_FLASH_TYPE_EMMC:
            PV_0.bits.boot_mode_sel_1 = 1;
            PV_0.bits.boot_mode_sel_0 = 1;
            break;
#else
            HI_ERR_CA("Not supported Chiptype\n");
            return HI_ERR_CA_NOT_SUPPORT;
#endif
        default:
            HI_ERR_CA("Not supported flash type 0x%x!\n", type);
            return HI_ERR_CA_NOT_SUPPORT;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if ((1 == PVLOCK_0.bits.boot_mode_sel_1_lock)
     || (1 == PVLOCK_0.bits.boot_mode_sel_0_lock)
     || (1 == PVLOCK_1.bits.boot_mode_sel_2_lock))
    {
        PV_0_OTP.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        PV_1_OTP.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if((PV_0.bits.boot_mode_sel_1 != PV_0_OTP.bits.boot_mode_sel_1)
         ||(PV_0.bits.boot_mode_sel_0 != PV_0_OTP.bits.boot_mode_sel_0)
         ||(PV_1.bits.boot_mode_sel_2 != PV_1_OTP.bits.boot_mode_sel_2))
        {
            /*if Boot Mode has been set, but it's not type, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
            return HI_ERR_CA_SETPARAM_AGAIN;

        }
        else
        {
            /*if type has been set as the Boot Mode already, HI_SUCCESS should be returned*/
            return HI_SUCCESS;
        }
    }
    ret  = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    ret |= (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock boot mode*/
    PVLOCK_0.u32 = 0;
    PVLOCK_1.u32 = 0;
#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    PVLOCK_0.bits.boot_mode_sel_0_lock = 1;
    PVLOCK_0.bits.boot_mode_sel_1_lock = 1;
    PVLOCK_1.bits.boot_mode_sel_2_lock = 1;
#else
    PVLOCK_0.bits.boot_mode_sel_0_lock = 1;
    PVLOCK_0.bits.boot_mode_sel_1_lock = 1;
    PVLOCK_1.bits.boot_mode_sel_2_lock = 0;
#endif

    ret  = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    ret |= (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716cv200)
static HI_UNF_ADVCA_FLASH_TYPE_E otpTobootmode_hi3716cv200(CA_OTP_V200_INTERNAL_PV_0_U u32PV0)
{
    HI_UNF_ADVCA_FLASH_TYPE_E eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    if ((0 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if ((0 == u32PV0.bits.boot_mode_sel_1) && (1 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if ((1 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else if ((1 == u32PV0.bits.boot_mode_sel_1) && (1 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
        HI_ERR_CA("Invalid Flash Type!\n");
    }

    return eFlashType;
}
#endif


#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
static HI_UNF_ADVCA_FLASH_TYPE_E otpTobootmode_hi3716mv410(CA_OTP_V200_INTERNAL_PV_0_U u32PV0, CA_OTP_V200_INTERNAL_PV_1_U u32PV1)
{
    HI_UNF_ADVCA_FLASH_TYPE_E eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    if ((0 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if ((0 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (1 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if ((0 == u32PV1.bits.boot_mode_sel_2) && (1 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND;
    }
    else if ((1 == u32PV1.bits.boot_mode_sel_2) && (1 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else if ((1 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
        HI_ERR_CA("Invalid Flash Type!\n");
    }

    return eFlashType;
}
#endif

#if defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
static HI_UNF_ADVCA_FLASH_TYPE_E otpTobootmode_hi3798cv200(CA_OTP_V200_INTERNAL_PV_0_U u32PV0, CA_OTP_V200_INTERNAL_PV_1_U u32PV1)
{
    HI_UNF_ADVCA_FLASH_TYPE_E eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    if ((0 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI;
    }
    else if ((0 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (1 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_NAND;
    }
    else if ((1 == u32PV1.bits.boot_mode_sel_2) && (0 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SPI_NAND;
    }
    else if ((0 == u32PV1.bits.boot_mode_sel_2) && (1 == u32PV0.bits.boot_mode_sel_1) && (1 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_EMMC;
    }
    else if ((0 == u32PV1.bits.boot_mode_sel_2) && (1 == u32PV0.bits.boot_mode_sel_1) && (0 == u32PV0.bits.boot_mode_sel_0))
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_SD;
    }
    else
    {
        eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
        HI_ERR_CA("Invalid Flash Type!\n");
    }

    return eFlashType;
}
#endif

HI_S32 DRV_CA_OTP_V200_GetBootMode(HI_UNF_ADVCA_FLASH_TYPE_E *pType)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    HI_UNF_ADVCA_FLASH_TYPE_E eFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;

    if (NULL == pType)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410)
    eFlashType = otpTobootmode_hi3716mv410(PV_0, PV_1);
#elif defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    eFlashType = otpTobootmode_hi3798cv200(PV_0, PV_1);
#elif defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) || defined(CHIP_TYPE_hi3716cv200)
    eFlashType = otpTobootmode_hi3716cv200(PV_0);
#else
    HI_ERR_CA("Not support chiptype!\n");
    return HI_ERR_CA_NOT_SUPPORT;
#endif

    if (HI_UNF_ADVCA_FLASH_TYPE_BUTT == eFlashType)
    {
        HI_ERR_CA("Invalid flash type [sel2:sel1:sel0]:[%d%d%d]!\n", PV_1.bits.boot_mode_sel_2, PV_0.bits.boot_mode_sel_1, PV_0.bits.boot_mode_sel_0);
        return HI_FAILURE;
    }

    *pType = eFlashType;

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_SetSCSActive(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.SCS_activation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    if (1 != DataLock_0.bits.scs_en_bak_lock)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SCS_EN_BAK, SCS_BAK_ENABLE);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }

        DataLock_0.bits.scs_en_bak_lock = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSCSActive(HI_U32 *pu32ActiveFlag)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;

    HI_U32  u32ScsBak = 0;

    if (NULL == pu32ActiveFlag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_FALSE == isSecureChipset())
    {
        *pu32ActiveFlag = 0;
        return HI_SUCCESS;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);

    u32ScsBak = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SCS_EN_BAK);
    if ((1 == OneWay_0.bits.SCS_activation) || (SCS_BAK_DISABLE != u32ScsBak))
    {
        *pu32ActiveFlag = 1;
    }
    else
    {
        *pu32ActiveFlag = 0;
    }

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetJtagProtectMode(CA_OTP_JTAG_MODE_E JtagMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32ProtectBak = 0;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    CA_OTP_VENDOR_TYPE_E enVendorId;

    if (JtagMode >= CA_OTP_JTAG_MODE_BUTT)
    {
        HI_ERR_CA("Jtag mode is invalid:%d\n", JtagMode);
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);

    if (1 == OneWay_0.bits.jtag_prt_mode_1)
    {
        if (CA_OTP_JTAG_MODE_CLOSED == JtagMode) /* CLOSED->CLOSED, do nothing and return HI_SUCCESS */
        {
            return HI_SUCCESS;
        }
        else /* CLOSED->others, not support */
        {
            HI_ERR_CA("Jtag mode is closed, not support to set to :%d\n", JtagMode);
            return HI_ERR_CA_NOT_SUPPORT;
        }
    }

    if (1 == OneWay_0.bits.jtag_prt_mode_0)
    {
        if (CA_OTP_JTAG_MODE_OPEN == JtagMode)          /* PROTECTED->OPEN, not support */
        {
            HI_ERR_CA("Jtag mode is protected, not support to set to :%d\n", JtagMode);
            return HI_ERR_CA_NOT_SUPPORT;
        }
        if (CA_OTP_JTAG_MODE_PROTECT == JtagMode)       /* PROTECTED->PROTECTED, do nothing and return HI_SUCCESS */
        {
            return HI_SUCCESS;
        }
    }

    s32Ret = DRV_CA_OTP_V200_GetVendorId(&enVendorId);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("Get vendor type err !\n");
        return HI_FAILURE;
    }
    switch (JtagMode)
    {
        case CA_OTP_JTAG_MODE_OPEN :
        {
            u32ProtectBak = JTAG_MODE_BAK_OPEN;
            break;
        }
        case CA_OTP_JTAG_MODE_PROTECT :
        {
            if (enVendorId == CA_OTP_VENDOR_IRDETO)
            {
                OneWay_0.bits.SecCpuMode_0 = 1;
                OneWay_0.bits.dbg_port_mode = 0x55;
            }

            OneWay_0.bits.jtag_prt_mode_0 = 1;
            if (CA_OTP_VENDOR_VERIMATRIX == enVendorId)
            {
                u32ProtectBak = JTAG_MODE_BAK_VMX_PROTECT;
            }
            else
            {
                u32ProtectBak = JTAG_MODE_BAK_PROTECT;
            }
            break;
        }
        case CA_OTP_JTAG_MODE_CLOSED:
        {
            if (enVendorId == CA_OTP_VENDOR_IRDETO)
            {
                OneWay_0.bits.SecCpuMode_0 = 1;
                OneWay_0.bits.SecCpuMode_1 = 1;
                OneWay_0.bits.dbg_port_mode = 0xff;
            }

            u32ProtectBak = JTAG_MODE_BAK_CLOSE;
            OneWay_0.bits.jtag_prt_mode_1 = 1;
            break;
        }
        default:
            HI_ERR_CA("Jtag mode is invalid:%d\n", JtagMode);/* default Open Mode */
            return HI_FAILURE;
    }
    s32Ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_JTAG_MODE_BAK, u32ProtectBak);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("Fail to write jtag mode back!\n");
        return HI_FAILURE;
    }

    s32Ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_CA("Fail to write jtag mode!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetJtagProtectMode(CA_OTP_JTAG_MODE_E *pJtagMode)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;
    HI_U32  u32JtagModeBak = 0;

    if (NULL == pJtagMode)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_FALSE == isSecureChipset())
    {
        *pJtagMode = CA_OTP_JTAG_MODE_OPEN;
        return HI_SUCCESS;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);

    u32JtagModeBak = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_JTAG_MODE_BAK);
    if ((OneWay_0.bits.jtag_prt_mode_0 == 0) && (OneWay_0.bits.jtag_prt_mode_1 == 0) && (JTAG_MODE_BAK_OPEN == u32JtagModeBak))
    {
        *pJtagMode = CA_OTP_JTAG_MODE_OPEN;
    }
    else if ((OneWay_0.bits.jtag_prt_mode_0 == 1) && (OneWay_0.bits.jtag_prt_mode_1 == 0))
    {
        *pJtagMode = CA_OTP_JTAG_MODE_PROTECT;
    }
    else
    {
        *pJtagMode = CA_OTP_JTAG_MODE_CLOSED;
    }

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetCSA2LadderLevel(CA_OTP_KEY_LEVEL_E level)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((level <= CA_OTP_KEY_LEV1) || (level >= CA_OTP_KEY_LEV4))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.csa2_lv_sel_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if ((level - 1) == PV_0.bits.csa2_lv_sel)
        {
            /*level already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set level to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set level*/
    if (level == CA_OTP_KEY_LEV3)
    {
        PV_0.u32 = 0;
        PV_0.bits.csa2_lv_sel = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock csa2 level*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.csa2_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCSA2LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
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

HI_S32 DRV_CA_OTP_V200_SetCSA3LadderLevel(CA_OTP_KEY_LEVEL_E level)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((level <= CA_OTP_KEY_LEV1) || (level >= CA_OTP_KEY_LEV_BUTT))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.csa3_lv_sel_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if ((level - 1) == PV_0.bits.csa3_lv_sel)
        {
            /*level already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set level to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set level*/
    if (level == CA_OTP_KEY_LEV3)
    {
        PV_0.u32 = 0;
        PV_0.bits.csa3_lv_sel = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock csa3 level*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.csa3_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCSA3LadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
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

HI_S32 DRV_CA_OTP_V200_SetR2RLadderLevel(CA_OTP_KEY_LEVEL_E level)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((level <= CA_OTP_KEY_LEV1) || (level >= CA_OTP_KEY_LEV_BUTT))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.r2r_lv_sel_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if ((level - 1) == PV_0.bits.r2r_lv_sel)
        {
            /*level already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set level to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set level*/
    if (level == CA_OTP_KEY_LEV3)
    {
        PV_0.u32 = 0;
        PV_0.bits.r2r_lv_sel = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock R2R level*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.r2r_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2RLadderLevel(CA_OTP_KEY_LEVEL_E *pLevel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pLevel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
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

HI_S32 DRV_CA_OTP_V200_SetSPLadderLevel(CA_OTP_KEY_LEVEL_E level)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((level <= CA_OTP_KEY_LEV1) || (level >= CA_OTP_KEY_LEV_BUTT))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if( (1 == PVLOCK_0.bits.sp_lv_sel_lock) || (0x01 == PVLOCK_1.bits.sp_lv_sel_2_lock) )
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

        if (CA_OTP_KEY_LEV2 == level)
        {
            if( (0x00 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if (CA_OTP_KEY_LEV3 == level)
        {
            if( (0x01 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if (CA_OTP_KEY_LEV4 == level)
        {
            if( (0x00 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if (CA_OTP_KEY_LEV5 == level)
        {
            if( (0x01 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
    }

    /*set level*/
    //write OTP control bit
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if (CA_OTP_KEY_LEV2 == level)
    {
        /* write to OTP */
        PV_0.bits.sp_lv_sel = 0x00;
        PV_1.bits.sp_lv_sel_2 = 0x00;
    }
    else if (CA_OTP_KEY_LEV3 == level)
    {
        /* write to OTP */
        PV_0.bits.sp_lv_sel = 0x01;
        PV_1.bits.sp_lv_sel_2 = 0x00;
    }
    else if (CA_OTP_KEY_LEV4 == level)
    {
        /* write to OTP */
        PV_0.bits.sp_lv_sel = 0x00;
        PV_1.bits.sp_lv_sel_2 = 0x01;
    }
    else if (CA_OTP_KEY_LEV5 == level)
    {
        /* write to OTP */
        PV_0.bits.sp_lv_sel = 0x01;
        PV_1.bits.sp_lv_sel_2 = 0x01;
    }
    else
    {
        HI_ERR_CA("Do not support this level:%x\n", level);
        return HI_FAILURE;
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }
    //write OTP lock bit, lock SP level*/
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    PVLOCK_0.bits.sp_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    PVLOCK_1.bits.sp_lv_sel_2_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
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

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( (0x00 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = CA_OTP_KEY_LEV2;
    }
    else if( (0x01 == PV_0.bits.sp_lv_sel) && (0x00 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = CA_OTP_KEY_LEV3;
    }
    else if( (0x00 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = CA_OTP_KEY_LEV4;
    }
    else if( (0x01 == PV_0.bits.sp_lv_sel) && (0x01 == PV_1.bits.sp_lv_sel_2) )
    {
        *pLevel = CA_OTP_KEY_LEV5;
    }
    else
    {
        *pLevel = CA_OTP_KEY_LEV_BUTT;
        ret = HI_FAILURE;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetVendorId(CA_OTP_VENDOR_TYPE_E enVendorType)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U32 u32VendorId = 0;

    if (enVendorType >= CA_OTP_VENDOR_BUTT)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.ca_vendor_id_lock)
    {
        u32VendorId = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CA_VENDOR_ID);
        if (enVendorType == u32VendorId)
        {
            /*vendor type already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set vendor type to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CA_VENDOR_ID, enVendorType);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock vendor type*/
    DataLock_0.u32 = 0;
    DataLock_0.bits.ca_vendor_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetVendorId(CA_OTP_VENDOR_TYPE_E *penVendorType)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32VendorId;

    if (NULL == penVendorType)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    u32VendorId = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CA_VENDOR_ID);
    u32VendorId = u32VendorId & 0xff;

    switch (u32VendorId)
    {
        case 0x0:
        {
            *penVendorType = CA_OTP_VENDOR_NONE;
            break;
        }
        case 0x1:
        {
            *penVendorType = CA_OTP_VENDOR_NAGRA;
            break;
        }
        case 0x02:
        {
            *penVendorType = CA_OTP_VENDOR_IRDETO;
            break;
        }
        case 0x03:
        {
            *penVendorType = CA_OTP_VENDOR_CONAX;
            break;
        }
        case 0x04:
        {
            *penVendorType = CA_OTP_VENDOR_NDS;
            break;
        }
        case 0x05:
        {
            *penVendorType = CA_OTP_VENDOR_SUMA;
            break;
        }
        case 0x06:
        {
            *penVendorType = CA_OTP_VENDOR_NOVEL;
            break;
        }
        case 0x07:
        {
            *penVendorType = CA_OTP_VENDOR_VERIMATRIX;
            break;
        }
        case 0x08:
        {
            *penVendorType = CA_OTP_VENDOR_CTI;
            break;
        }
        case 0x09:
        {
            *penVendorType = CA_OTP_VENDOR_SAFEVIEW;
            break;
        }
        case 0x0a:
        {
            *penVendorType = CA_OTP_VENDOR_LATENSE;
            break;
        }
        case 0x0b:
        {
            *penVendorType = CA_OTP_VENDOR_COMMONCA;
            break;
        }
        case 0x0c:
        {
            *penVendorType = CA_OTP_VENDOR_DCAS;
            break;
        }
        case 0x0e:
        {
            *penVendorType = CA_OTP_VENDOR_PANACCESS;
            break;
        }
        default:
        {
            *penVendorType  = (u32VendorId & 0xff);//CA_OTP_VENDOR_BUTT;
            HI_ERR_CA("Invalid vendor id (%d)!\n", *penVendorType);
            break;
        }
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetVendorIdLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.ca_vendor_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetVendorIdLock(HI_U32 *pu32VendorIdLock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32VendorIdLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32VendorIdLock = DataLock_0.bits.ca_vendor_id_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSecureChipFlag(HI_U32 u32Flag)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Flag != 0) && (u32Flag != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.secure_chip_flag_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Flag == PV_1.bits.secure_chip_flag)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Flag == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.secure_chip_flag = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.secure_chip_flag_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSecureChipFlag(HI_U32 *pu32SecureChipFlag)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32SecureChipFlag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32SecureChipFlag = PV_1.bits.secure_chip_flag;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTZEnable(HI_U32 u32Enable)
{
    HI_S32 ret;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLock_1;

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    PVLock_1.u32 = 0;
    PVLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    if ((1 == PVLock_1.bits.soc_tz_enable_lock) && (1 == PVLock_1.bits.otp_tz_area_enable_lock))
    {
        if ((u32Enable == PV_1.bits.soc_tz_enable) && (u32Enable == PV_1.bits.otp_tz_area_enable))
        {
            return HI_SUCCESS;
        }
        else
        {
            HI_ERR_CA("Soc and otp tz have already been set\n");
            return HI_FAILURE;
        }
    }

    if (1 == u32Enable)
    {
        if(1 != PV_1.bits.soc_tz_enable)
        {
            PV_1.bits.soc_tz_enable = 1;
        }

        if(1 != PV_1.bits.otp_tz_area_enable)
        {
            PV_1.bits.otp_tz_area_enable = 1;
        }

        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return HI_FAILURE;
        }
    }

    //lock value
    PVLock_1.u32 = 0;
    PVLock_1.bits.soc_tz_enable_lock = 1;
    PVLock_1.bits.otp_tz_area_enable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLock_1.u32);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTZEnStatus(HI_BOOL *pbEn)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if(NULL == pbEn)
    {
        HI_ERR_CA("Invalid param, NULL pointer!\n");
        return HI_FAILURE;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if( (1 == PV_1.bits.soc_tz_enable) && (1 == PV_1.bits.otp_tz_area_enable) )
    {
        *pbEn = HI_TRUE;
    }
    else
    {
        *pbEn = HI_FALSE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetCSA2Deactive(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;
    OneWay_0.bits.csa2_deactivation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCSA2Deactive(HI_U32 *pu32CSA2Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    if (NULL == pu32CSA2Deactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32CSA2Deactive = OneWay_0.bits.csa2_deactivation;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2RDeactive(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;
    OneWay_0.bits.r2r_deactivation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }
    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2RDeactive(HI_U32 *pu32R2RDeactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    if (NULL == pu32R2RDeactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32R2RDeactive = OneWay_0.bits.r2r_deactivation;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSPDeactive(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;
    OneWay_0.bits.sp_deactivation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSPDeactive(HI_U32 *pu32SPDeactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    if (NULL == pu32SPDeactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32SPDeactive = OneWay_0.bits.sp_deactivation;

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetCSA2RootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.secret_key_lock)
    {
        HI_ERR_CA("CSA2 key set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_CSA2_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CSA2_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum! addr:0x%x, u8CheckSumCmp:0x%02x != u8CheckSum:0x%02x\n",
                                        CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY,
                                        u8CheckSumCmp,
                                        u8CheckSum);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_CSA2_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_CSA2_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock csa2_root_key */
    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    DataLock_0.bits.secret_key_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCSA3RootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.CSA3_RootKey_lock)
    {
        HI_ERR_CA("CSA3 key set CSA3_RootKey, locked!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_CSA3_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CSA3_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_CSA3_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_CSA3_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock key */
    DataLock_1.u32 = 0;
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    DataLock_1.bits.CSA3_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DataLock_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set CSA3_RootKey_lock!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2RRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.R2R_RootKey_lock)
    {
        HI_ERR_CA("Failed to set R2R_RootKey, locked!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_R2R_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_R2R_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to set R2R_RootKey!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_R2R_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_R2R_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock r2r_root_key */
    DataLock_1.u32 = 0;
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    DataLock_1.bits.R2R_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DataLock_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set R2R_RootKey_lock!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSPRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.SP_RootKey_lock)
    {
        HI_ERR_CA("SP root key set ERROR! Locked!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_SP_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SP_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_SP_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_SP_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock sp_root_key */
    DataLock_1.u32 = 0;
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    DataLock_1.bits.SP_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DataLock_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set SP_RootKey_lock!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    HI_U8 au8ChipId[8] = {0};
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;
    HI_U8 u8ChecksumInput[16];
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu8Key)
    {
        HI_ERR_CA("Get chipid ERROR!\n");
        return HI_FAILURE;
    }

    for(i = 0; i < 8; i++)
    {
        au8ChipId[i] = DRV_CA_OTP_V200_ReadByte(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0 + i);
    }

    /* check if key locked */
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.JTAG_Key_lock)
    {
        HI_ERR_CA("JTAG key set ERROR! Locked!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_JTAGKey_ChipID)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 8; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_JTAG_KEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write JTAG_Key!\n");
            return ret;
        }
    }
    memset(u8ChecksumInput, 0, sizeof(u8ChecksumInput));
    memcpy(u8ChecksumInput, pu8Key, 8);
    memcpy(u8ChecksumInput + 8, au8ChipId, 8);

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(u8ChecksumInput, 16);
    u8CheckSum = u16CRCValue & 0xff;

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }
    u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID);
    if (u8CheckSumCmp != u8CheckSum)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return HI_FAILURE;
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_JTAGKey_ChipID = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write locker_JTAGKey_ChipID!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_JTAGKey_ChipID )
    {
        HI_ERR_CA("Fail to write locker_JTAGKey_ChipID!\n");
        return HI_FAILURE;
    }

    /* lock jtag_key */
    DataLock_1.u32 = 0;
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    DataLock_1.bits.JTAG_Key_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DataLock_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set JTAG_Key_lock!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.secret_key_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRootKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32Lock = DataLock_0.bits.secret_key_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetESCK(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.esck_lock)
    {
        HI_ERR_CA("ESCK set ERROR! esck key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_ESCK)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_ESCK_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_ESCK, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }
    u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_ESCK);
    if (u8CheckSumCmp != u8CheckSum)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return HI_FAILURE;
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_ESCK = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_ESCK )
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /*set esck_lock*/
    DataLock_0.u32 = 0;
    DataLock_0.bits.esck_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetESCKLock(HI_BOOL *pbLock)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 temp = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pbLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    temp = DataLock_0.bits.esck_lock;
    if( 0 == temp )
    {
        *pbLock = HI_FALSE;
    }
    else
    {
        *pbLock = HI_TRUE;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetMISCRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if locked or not */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.misc_rootkey_lock)
    {
        HI_ERR_CA("MISC RootKey set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_MISC_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_MISC_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_MISC_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_MISC_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_MISC_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_MISC_RootKey )
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock misc_rootkey */
    DataLock_0.u32 = 0;
    DataLock_0.bits.misc_rootkey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetMISCRootKeyLock(HI_BOOL *pbLock)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 temp = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pbLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    temp = DataLock_0.bits.misc_rootkey_lock;
    if( 0 == temp )
    {
        *pbLock = HI_FALSE;
    }
    else
    {
        *pbLock = HI_TRUE;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetOEMRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if locked */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.OEM_RootKey_lock)
    {
        HI_ERR_CA("OEM RootKey set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_OEM_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_OEM_ROOTKEY_0 + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_OEM_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_OEM_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_OEM_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_OEM_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock oem_rootkey */
    DataLock_0.u32 = 0;
    DataLock_0.bits.OEM_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetOEMRootKeyLock(HI_BOOL *pbLock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pbLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pbLock = DataLock_0.bits.OEM_RootKey_lock;

    return ret;
}

/* The input address should be 16byte aligned */
HI_S32 DRV_CA_OTP_V200_SetOTPTzData(CA_OTP_TZ_DATA_S *pstInputParam)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_1_U unOTPTZLock;
    HI_U32 i = 0;
    HI_U32 u32Start = 0;
    HI_U32 u32LockArea = 0;
    HI_U32 u32Round = 0;
    HI_U8 u8ReadBuf[128];

    if ( NULL == pstInputParam )
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if locked or not */
    for(i = 0; i < pstInputParam->u32Len; i++)
    {
        u32LockArea = (pstInputParam->u32Addr - CA_OTP_V200_INTERNAL_TZ_OTP_BASE) / 16;
        unOTPTZLock.u32 = 0;
        unOTPTZLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
        if( 0 != ((unOTPTZLock.bits.tz_area_lock >> u32LockArea) & 0x1) )
        {
            HI_ERR_CA("Write otp failed! Already locked!\n");
            return HI_FAILURE;
        }
    }

    /* write to OTP */
    u32Start = pstInputParam->u32Addr;
    u32Round = pstInputParam->u32Len;

    if( (CA_OTP_V200_INTERNAL_TZ_OTP_BASE > u32Start)
     || (CA_OTP_V200_INTERNAL_SEC_OTP_END < (u32Start + u32Round)) )
    {
        HI_ERR_CA("Invalid address or length!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < u32Round; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(u32Start + i, pstInputParam->u8Buf[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* check if write success or not */
    memset(u8ReadBuf, 0x0, sizeof(u8ReadBuf));
    u32Start = pstInputParam->u32Addr;
    u32Round = pstInputParam->u32Len;
    for (i = 0; i < u32Round; i++)
    {
        u8ReadBuf[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(u32Start + i);
    }
    ret = memcmp(u8ReadBuf, pstInputParam->u8Buf, pstInputParam->u32Len);
    if( 0 != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetOTPTzData(CA_OTP_TZ_DATA_S *pstInputParam)
{
    HI_U32 i = 0;
    HI_U32 u32Addr = 0;
    HI_U32 u32Len = 0;
    HI_U8 *pu8ReadBuf;

    if (NULL == pstInputParam)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    pu8ReadBuf = pstInputParam->u8Buf;
    u32Addr = pstInputParam->u32Addr;
    u32Len = pstInputParam->u32Len;

    if( (CA_OTP_V200_INTERNAL_TZ_OTP_BASE > u32Addr)
     || ((CA_OTP_V200_INTERNAL_SEC_OTP_END + 8) < (u32Addr + u32Len)) )
    {
        HI_ERR_CA("Invalid address or length!\n");
        return HI_FAILURE;
    }

    for (i = 0; i < u32Len; i++)
    {
        pu8ReadBuf[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(u32Addr + i);
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_LockOTPTzData(HI_UNF_ADVCA_LOCK_TZ_OTP_PARAM_S *pstInputParam)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_1_U unOTPTZLock;
    HI_U32 i = 0;
    HI_U32 u32Offset = 0;
    HI_U32 u32LockLen = 0;      /* 1bit lock 16bytes */

    if ( NULL == pstInputParam )
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    unOTPTZLock.u32 = 0;
    unOTPTZLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
    u32Offset = (pstInputParam->u32Addr - CA_OTP_V200_INTERNAL_TZ_OTP_BASE) / 16;
    u32LockLen = pstInputParam->u32Len / 16;

    /* if not locked, lock it */
    for( i = u32Offset; i < (u32Offset + u32LockLen); i++)
    {
        if( 0 == ((unOTPTZLock.bits.tz_area_lock >> i) & 0x1) )
        {
            unOTPTZLock.bits.tz_area_lock |= (1 << i);
        }
    }

    /* write lock flag to otp */
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_1, unOTPTZLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write otp lock flag!\n");
        return ret;
    }

    /* check if locked or not */
    unOTPTZLock.u32 = 0;
    unOTPTZLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
    u32Offset = (pstInputParam->u32Addr - CA_OTP_V200_INTERNAL_TZ_OTP_BASE) / 16;
    u32LockLen = pstInputParam->u32Len / 16;
    for( i = u32Offset; i < (u32Offset + u32LockLen); i++)
    {
        if( 0 == ((unOTPTZLock.bits.tz_area_lock >> i) & 0x1) )
        {
            HI_ERR_CA("Write otp lock flag failed!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetRSAKeyE(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.rsa_key_lock)
    {
        HI_ERR_CA("RSA key E set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_RSA_KEY_E_BASE + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRSAKeyE(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.rsa_key_lock)
    {
        HI_ERR_CA("RSA key E get ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* read from OTP */
    for (i = 0; i < 16; i++)
    {
        pu8Key[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_RSA_KEY_E_BASE + i);
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRSAKeyN(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.rsa_key_lock)
    {
        HI_ERR_CA("RSA key N set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 256; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_RSA_KEY_N_BASE + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRSACRC(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    if (1 == DataLock_0.bits.RSA_CRC_lock)
    {
        HI_ERR_CA("RSA_CRC set error! RSA_CRC has already locked!\n");
        return HI_FAILURE;
    }

    u16CRCValue = DRV_CA_OTP_CalculateRSACRC16(pu8Key);
    u8CheckSum = (u16CRCValue & 0xff);

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_RSA_KEY_CRC_BASE, u8CheckSum);
    u8CheckSum = ((u16CRCValue >> 8) & 0xff);

    ret |= (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_RSA_KEY_CRC_BASE + 1, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("RSA_CRC write error!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 DRV_CA_OTP_V200_GetRSAKeyN(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    if (1 == DataLock_1.bits.rsa_key_lock)
    {
        HI_ERR_CA("RSA key N get ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* read from OTP */
    for (i = 0; i < 256; i++)
    {
        pu8Key[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_RSA_KEY_N_BASE + i);
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_LockRSAKey(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

#if defined (CHIP_TYPE_hi3716mv410) || defined (CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
#endif

    //Lock RSAKey
    DataLock_1.u32 = 0;
    DataLock_1.bits.rsa_key_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DataLock_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write RSA key Lock!\n");
        return HI_FAILURE;
    }

#if defined (CHIP_TYPE_hi3716mv410) || defined (CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    //Lock RSA CRC
    DataLock_0.u32 = 0;
    DataLock_0.bits.RSA_CRC_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write RSA CRC Lock!\n");
        return HI_FAILURE;
    }
#endif
    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetRSAKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DataLock_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_1.u32 = 0;
    DataLock_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);
    *pu32Lock = DataLock_1.bits.rsa_key_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetChipId(HI_U32 u32Id)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    chipId = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.ca_chip_id_lock)
    {
        chipId = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0);
        if (chipId == u32Id)
        {
            /*if u32Id has been set as the chip Id already, HI_SUCCESS should be returned*/
            return HI_SUCCESS;
        }
        else
        {
            /*if chip Id has been set, but it's not u32Id, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /* write to OTP */
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0, u32Id);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    //Set ca_chipid_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.ca_chip_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetUniqueChipId(HI_U8 *pu8Id)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu8Id)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.ca_chip_id_lock)
    {
        /*if chip Id has been set, HI_ERR_CA_SETPARAM_AGAIN should be returned*/
        return HI_ERR_CA_SETPARAM_AGAIN;
    }

    /* write to OTP */
    for(i = 0; i < 8; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0 + i, pu8Id[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }


    //Set ca_chipid_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.ca_chip_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetChipId(HI_U32 *pu32ChipID)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32ChipID)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32ChipID = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetUniqueChipId(HI_U8 *pu8ChipID)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu8ChipID)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    pu8ChipID[0] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0);
    pu8ChipID[1] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0 + 1);
    pu8ChipID[2] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0 + 2);
    pu8ChipID[3] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CA_CHIP_ID_0 + 3);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetChipIdLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.ca_chip_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetChipIdLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32Lock = DataLock_0.bits.ca_chip_id_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTdesLock(HI_U32 u32TdesLock)
{
    HI_S32 ret = HI_ERR_CA_NOT_SUPPORT;
    HI_ERR_CA("Do not support in this Version!\n");
    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTdesLock(HI_U32 *pu32TdesLock)
{
    HI_S32 ret = HI_ERR_CA_NOT_SUPPORT;
    HI_ERR_CA("Do not support in this Version!\n");
    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLowPowerDisable(HI_U32 u32Flag)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((0 != u32Flag) && (1 != u32Flag))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ddr_wakeup_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Flag == PV_0.bits.ddr_wakeup_disable)
        {
            /*low power already set to the right value, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set low_power_disable to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (u32Flag)
    {
        /*disable low power*/
        PV_0.u32 = 0;
        PV_0.bits.ddr_wakeup_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock low_power_disable*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ddr_wakeup_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLowPowerDisable(HI_U32 *pu32Flag)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Flag)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Flag = PV_0.bits.ddr_wakeup_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa3En(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;
    OneWay_0.bits.csa3_deactivation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa3En(HI_U32 *pu32CSA3Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    if (NULL == pu32CSA3Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32CSA3Disable = OneWay_0.bits.csa3_deactivation;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLpcEn(HI_U32 u32LpcDisable)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((0 != u32LpcDisable) && (1 != u32LpcDisable))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.lpc_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32LpcDisable == PV_1.bits.lpc_disable)
        {
            /*lpc_disable already set to the right value, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set lpc_disable to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (u32LpcDisable)
    {
        /*disable lpc*/
        PV_1.u32 = 0;
        PV_1.bits.lpc_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock lpc_disable*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.lpc_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLpcEn(HI_U32 *pu32LpcDisable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32LpcDisable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32LpcDisable = PV_1.bits.lpc_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLpcMasterEn(HI_U32 u32LpcMasterDisable)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((0 != u32LpcMasterDisable) && (1 != u32LpcMasterDisable))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.lpc_master_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32LpcMasterDisable == PV_1.bits.lpc_master_disable)
        {
            /*lpc_master_disable already set to the right value, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set lpc_master_disable to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (u32LpcMasterDisable)
    {
        /*disable lpc_master*/
        PV_1.u32 = 0;
        PV_1.bits.lpc_master_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock lpc_master_disable*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.lpc_master_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLpcMasterEn(HI_U32 *pu32LpcMasterDisable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32LpcMasterDisable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32LpcMasterDisable = PV_1.bits.lpc_master_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootSelLock_0(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.boot_mode_sel_0_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootSelLock_0(HI_U32 *pu32BootSelLock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32BootSelLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32BootSelLock = PVLOCK_0.bits.boot_mode_sel_0_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootSelLock_1(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.boot_mode_sel_1_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootSelLock_1(HI_U32 *pu32BootSelLock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32BootSelLock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32BootSelLock = PVLOCK_0.bits.boot_mode_sel_1_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetMktIdLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.msid_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetMktIdLock(HI_U32 *pu32mktIdP)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32mktIdP)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32mktIdP = DataLock_0.bits.msid_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetStbSnLock_0()
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.stbsn0_lock = 0x01;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetStbSnLock_0(HI_U32 *pu32StbSnP)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32StbSnP)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32StbSnP = DataLock_0.bits.stbsn0_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSelfbootLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.self_boot_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSelfbootLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.self_boot_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLowPowerDisableLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ddr_wakeup_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLowPowerDisableLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.ddr_wakeup_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDVBLadderLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.csa2_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDVBLadderLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.csa2_lv_sel_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2RLadderLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.r2r_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2RLadderLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.r2r_lv_sel_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSPLadderLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.sp_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSPLadderLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.sp_lv_sel_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCSA3LadderLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.csa3_lv_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCSA3LadderLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.csa3_lv_sel_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSCWDeactive(HI_U32 u32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Deactive != 0) && (u32Deactive != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.tskl_csa2_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Deactive == PV_0.bits.tskl_csa2_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Deactive == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.tskl_csa2_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_csa2_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCWDeactive(HI_U32 *pu32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Deactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Deactive = PV_0.bits.tskl_csa2_disable;

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetTSCWDeactiveLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_csa2_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCWDeactiveLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.tskl_csa2_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSSPDeactive(HI_U32 u32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Deactive != 0) && (u32Deactive != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.tskl_spe_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Deactive == PV_0.bits.tskl_sp_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Deactive == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.tskl_sp_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_spe_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSSPDeactive(HI_U32 *pu32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Deactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Deactive = PV_0.bits.tskl_sp_disable;

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetTSSPDeactiveLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_spe_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSSPDeactiveLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.tskl_spe_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSCSA3Deactive(HI_U32 u32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Deactive != 0) && (u32Deactive != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.tskl_csa3_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Deactive == PV_0.bits.tskl_csa3_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Deactive == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.tskl_csa3_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_csa3_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCSA3Deactive(HI_U32 *pu32Deactive)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Deactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Deactive = PV_0.bits.tskl_csa3_disable;

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetTSCSA3DeactiveLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_csa3_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCSA3DeactiveLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.tskl_csa3_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDesDisable(HI_U32 u32Disable)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_GetDesDisable(HI_U32 *pu32Disable)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}


HI_S32 DRV_CA_OTP_V200_SetDesDisableLock(HI_VOID)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_GetDesDisableLock(HI_U32 *pu32Lock)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_SetDesHardDisable(HI_U32 u32Disable)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_GetDesHardDisable(HI_U32 *pu32Disable)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}


HI_S32 DRV_CA_OTP_V200_SetDesHardDisableLock(HI_VOID)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_GetDesHardDisableLock(HI_U32 *pu32Lock)
{
    HI_ERR_CA("Not supported!\n");
    return HI_FAILURE;
}

HI_S32 DRV_CA_OTP_V200_SetTsOutDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.ts_out_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.ts_out_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.ts_out_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.ts_out_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTsOutDisable(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32Disable = PV_1.bits.ts_out_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTsOutDisableLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.ts_out_disable_lock= 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTsOutDisableLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.ts_out_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLpcEnLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.lpc_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLpcEnLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.lpc_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLpcMasterEnLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.lpc_master_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLpcMasterEnLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.lpc_master_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSecureChipFlagLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.secure_chip_flag_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSecureChipFlagLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.secure_chip_flag_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootSelCtrl(HI_U32 u32BootSel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32BootSel != 0) && (u32BootSel != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.bootsel_ctrl_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32BootSel == PV_1.bits.bootsel_ctrl)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32BootSel == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.bootsel_ctrl = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bootsel_ctrl_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootSelCtrl(HI_U32 *pu32BootSel)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32BootSel)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32BootSel = PV_1.bits.bootsel_ctrl;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootSelCtrlLock(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bootsel_ctrl_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootSelCtrlLock(HI_U32 *pu32Lock)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.bootsel_ctrl_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCSA3Deactive(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.csa3_deactivation = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCSA3Deactive(HI_U32 *pu32Deactive)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Deactive)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32Deactive = OneWay_0.bits.csa3_deactivation;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagReadDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.jtag_r_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagReadDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32Disable = OneWay_0.bits.jtag_r_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDebugDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.debug_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDebugDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32Disable = OneWay_0.bits.debug_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRigthCtrlEn(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.right_ctrl_en = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRigthCtrlEn(HI_U32 *pu32Enable)
{
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Enable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    *pu32Enable = OneWay_0.bits.right_ctrl_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetLinkProtectDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.link_prt_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Disable == PV_0.bits.link_prt_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.link_prt_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.link_prt_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSKLNVDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.tskl_nv_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Disable == PV_0.bits.tskl_nv_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.tskl_nv_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_nv_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDcasKLDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.dcas_kl_en_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.dcas_kl_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.dcas_kl_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.dcas_kl_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBloadEncDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.bload_enc_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.bload_enc_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.bload_enc_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bload_enc_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBloadEncDisable(HI_U32 *pu32BloadEncDisable)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (pu32BloadEncDisable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32BloadEncDisable = PV_1.bits.bload_enc_disable;

    return HI_SUCCESS;
}



HI_S32 DRV_CA_OTP_V200_SetBloadDecEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.bload_dec_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.bload_dec_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.bload_dec_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.bload_dec_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSCsa2HardonlyEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ts_csa2_hardonly_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.ts_csa2_hardonly_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.ts_csa2_hardonly_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_csa2_hardonly_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSSPEHardonlyEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ts_spe_hardonly_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.ts_sp_hardonly_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.ts_sp_hardonly_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_spe_hardonly_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSNVHardonlyEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ts_nv_hardonly_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.ts_nv_hardonly_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.ts_nv_hardonly_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_nv_hardonly_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTSCSA3HardonlyEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.ts_csa3_hardonly_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.ts_csa3_hardonly_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.ts_csa3_hardonly_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_csa3_hardonly_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetOtpGlobalLockEn(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.otp_globle_lock_en_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.otp_global_lock_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.otp_global_lock_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.otp_globle_lock_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRuntimeCheckEn(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.runtime_check_en_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.runtime_check_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.runtime_check_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.runtime_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetWakeupDDRCheckEn(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.wakeup_ddr_check_en_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.wakeup_ddr_check_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.wakeup_ddr_check_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.wakeup_ddr_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetMiscKlLevel(HI_UNF_ADVCA_KEYLADDER_LEV_E enLevel)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /* read OTP */
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if( (0x01 == PVLOCK_1.bits.misc_lv_sel_lock) || (0x01 == PVLOCK_1.bits.misc_lv_sel_2_lock) )
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if(HI_UNF_ADVCA_KEYLADDER_LEV2 == enLevel)
        {
            if( (0x00 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if(HI_UNF_ADVCA_KEYLADDER_LEV3 == enLevel)
        {
            if( (0x01 == PV_1.bits.misc_lv_sel) && (0x00 == PV_1.bits.misc_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if(HI_UNF_ADVCA_KEYLADDER_LEV4 == enLevel)
        {
            if( (0x00 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
        else if(HI_UNF_ADVCA_KEYLADDER_LEV5 == enLevel)
        {
            if( (0x01 == PV_1.bits.misc_lv_sel) && (0x01 == PV_1.bits.misc_lv_sel_2) )
            {
                return HI_SUCCESS;
            }
            else
            {
                return HI_FAILURE;
            }
        }
    }
    //write OTP control bit
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    if(HI_UNF_ADVCA_KEYLADDER_LEV2 == enLevel)
    {
        PV_1.bits.misc_lv_sel = 0;
        PV_1.bits.misc_lv_sel_2 = 0;
    }
    else if(HI_UNF_ADVCA_KEYLADDER_LEV3 == enLevel)
    {
        PV_1.bits.misc_lv_sel = 1;
        PV_1.bits.misc_lv_sel_2 = 0;
    }
    else if(HI_UNF_ADVCA_KEYLADDER_LEV4 == enLevel)
    {
        PV_1.bits.misc_lv_sel = 0;
        PV_1.bits.misc_lv_sel_2 = 1;
    }
    else if(HI_UNF_ADVCA_KEYLADDER_LEV5 == enLevel)
    {
        PV_1.bits.misc_lv_sel = 1;
        PV_1.bits.misc_lv_sel_2 = 1;
    }
    else
    {
        HI_ERR_CA("Do not support this level:%x\n", enLevel);
        return HI_FAILURE;
    }
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }
    //write OTP lock bit
    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    PVLOCK_1.bits.misc_lv_sel_lock = 1;
    PVLOCK_1.bits.misc_lv_sel_2_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

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
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
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

HI_S32 DRV_CA_OTP_V200_SetVersionIdCheckEn(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.version_id_check_en_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.version_id_check_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.version_id_check_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.version_id_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBlMSIDCheckEn(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.bl_msid_check_en_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.bl_msid_check_en)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.bl_msid_check_en = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bl_msid_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSelftestDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;

    OneWay_0.u32 = 0;

    /* write to OTP */
    OneWay_0.bits.self_test_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, OneWay_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetVersionIdLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    //Set version_id_lock
    DataLock_0.u32 = 0;
    DataLock_0.bits.version_id_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCSA3HardOnlyEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ts_csa3_hardonly_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBloadModeSelLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.bload_mode_sel_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDDRScrambleEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.ddr_scramble_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetOtpGlobalLockEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.otp_globle_lock_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRuntimeCheckEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.runtime_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}
HI_S32 DRV_CA_OTP_V200_SetWakeupDDRCheckEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.wakeup_ddr_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetVersionIdCheckEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.version_id_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBlMSIDCheckEnLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.bl_msid_check_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetVersionIdCheckLock(HI_U32 *pu32VersionIdCheckP)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu32VersionIdCheckP)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    *pu32VersionIdCheckP = DataLock_0.bits.version_id_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBloadDecEn(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.bload_dec_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBloadDecEnLock(HI_U32 *pu32BloadDecEncLock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32BloadDecEncLock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32BloadDecEncLock = PVLOCK_0.bits.bload_dec_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLinkProtectDisableLock(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Disable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Disable = PVLOCK_0.bits.link_prt_disable_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCSA3HardonlyEn(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.ts_csa3_hardonly_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCSA3HardonlyEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Lock = PVLOCK_0.bits.ts_csa3_hardonly_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSSPHardonlyEn(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.ts_sp_hardonly_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSSPHardonlyEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Lock = PVLOCK_0.bits.ts_spe_hardonly_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBlMSIDCheckEn(HI_U32 *pu32BlMSIDCheckEn)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (pu32BlMSIDCheckEn == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }


    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32BlMSIDCheckEn = PV_1.bits.bl_msid_check_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBlMSIDCheckEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    *pu32Lock = PVLOCK_1.bits.bl_msid_check_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetVersionIDCheckEn(HI_U32 *pu32VersionIDCheckEn)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (pu32VersionIDCheckEn == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }


    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32VersionIDCheckEn = PV_1.bits.version_id_check_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetVersionIDCheckEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    *pu32Lock = PVLOCK_1.bits.version_id_check_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetWakeupDDRCheckEn(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (pu32Disable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32Disable = PV_1.bits.wakeup_ddr_check_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetWakeupDDRCheckEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    *pu32Lock = PVLOCK_1.bits.wakeup_ddr_check_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRuntimeCheckEn(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (pu32Disable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32Disable = PV_1.bits.runtime_check_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRuntimeCheckEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    *pu32Lock = PVLOCK_1.bits.runtime_check_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetOtpGlobalLockEn(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.otp_global_lock_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetOtpGlobalLockEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Lock = PVLOCK_0.bits.otp_globle_lock_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDDRScrambleEnLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = 0;
    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);

    *pu32Lock = PVLOCK_0.bits.ddr_scramble_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBloadModeSel(HI_U32 *pu32Mode)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Mode == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Mode = PV_0.bits.bload_mode_sel;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBloadModeSelLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Lock == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Lock = PVLOCK_0.bits.bload_mode_sel_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCsa2HardonlyEn(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.ts_csa2_hardonly_en;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTSCsa2HardonlyEnLock(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    *pu32Enable = PVLOCK_0.bits.ts_csa2_hardonly_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetLinkProtectDisable(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Disable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Disable = PV_0.bits.link_prt_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetDcasKLDisableLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.dcas_kl_en_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDcasKLDisable(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32Disable = PV_1.bits.dcas_kl_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetDcasKLDisableLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.dcas_kl_en_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2rRootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    DATALOCK_1.u32 = 0;
    DATALOCK_1.bits.R2R_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DATALOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2rRootKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_1.u32 = 0;
    DATALOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    *pu32Lock = DATALOCK_1.bits.R2R_RootKey_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSpRootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    DATALOCK_1.u32 = 0;
    DATALOCK_1.bits.SP_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DATALOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSpRootKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_1.u32 = 0;
    DATALOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    *pu32Lock = DATALOCK_1.bits.SP_RootKey_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa3RootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    DATALOCK_1.u32 = 0;
    DATALOCK_1.bits.CSA3_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DATALOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa3RootKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_1.u32 = 0;
    DATALOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    *pu32Lock = DATALOCK_1.bits.CSA3_RootKey_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    DATALOCK_1.u32 = 0;
    DATALOCK_1.bits.JTAG_Key_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_1, DATALOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagKeyLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_1_U DATALOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_1.u32 = 0;
    DATALOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_1);

    *pu32Lock = DATALOCK_1.bits.JTAG_Key_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootRootKeyLockFlag(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_DATA_LOCK_0_U DATALOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    *pu32Lock = DATALOCK_0.bits.Boot_RootKey_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSecStoreRootKeyLockFlag(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_1_U ONEWAY_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    ONEWAY_1.u32 = 0;
    ONEWAY_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);

    *pu32Lock = ONEWAY_1.bits.SEC_STORE_RootKey_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.Boot_RootKey_lock)
    {
        HI_ERR_CA("BootRoot key set ERROR! BootRoot key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_BOOT_RootKey)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_BOOT_ROOTKEY + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_BOOT_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_BOOT_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum! addr:0x%x, u8CheckSumCmp:0x%02x != u8CheckSum:0x%02x\n",
                                        CA_OTP_V200_INTERNAL_CHECKSUM_BOOT_ROOT_KEY,
                                        u8CheckSumCmp,
                                        u8CheckSum);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_BOOT_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_BOOT_RootKey)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock boot_root_key */
    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    DataLock_0.bits.Boot_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetSecStoreRootKey(HI_U8 *pu8Key)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    CA_OTP_V200_ONE_WAY_0_U OneWay_0;
    CA_OTP_V200_ONE_WAY_1_U OneWay_1;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if key locked */
    OneWay_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
    if (1 == OneWay_1.bits.SEC_STORE_RootKey_lock)
    {
        HI_ERR_CA("SecStore Root key set ERROR! SecStore Root key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_SEC_STORE_ROOTKEY)
    {
        HI_ERR_CA("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 16; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SEC_STORE_ROOTKEY + i, pu8Key[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /* set checksum */
    u16CRCValue = DRV_CA_OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_CHECKSUM_SEC_STORE_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum!\n");
        return ret;
    }

    OneWay_0.u32 = 0;
    OneWay_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);
    if(0 == OneWay_0.bits.checksum_read_disable)
    {
        u8CheckSumCmp = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_CHECKSUM_SEC_STORE_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_CA("Fail to write checksum! addr:0x%x, u8CheckSumCmp:0x%02x != u8CheckSum:0x%02x\n",
                                        CA_OTP_V200_INTERNAL_CHECKSUM_SEC_STORE_ROOT_KEY,
                                        u8CheckSumCmp,
                                        u8CheckSum);
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CA("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_SEC_STORE_ROOTKEY = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_SEC_STORE_ROOTKEY)
    {
        HI_ERR_CA("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    /* lock sec store root_key */
    OneWay_1.u32 = 0;
    OneWay_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);

    OneWay_1.bits.SEC_STORE_RootKey_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_1, OneWay_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_SetIdWord(HI_U32 u32Data)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;
    HI_U32    u32IdWord = 0;

    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DATALOCK_0.bits.id_word_lock)
    {
        u32IdWord = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_IDWORD);
        if (u32IdWord == u32Data)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_IDWORD, u32Data);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.id_word_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetIdWord(HI_U32 *pu32Data)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Data)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Data = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_IDWORD);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetIdWordLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.id_word_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetIdWordLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    *pu32Lock = DATALOCK_0.bits.id_word_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa2RootKeyCrc(HI_U32 u32Crc)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurCrc = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if (1 == CHECKSUMLOCK.bits.locker_CSA2_RootKey)
    {
        u32CurCrc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY);
        if (u32CurCrc == u32Crc)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY, u32Crc);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_CSA2_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa2RootKeyCrc(HI_U32 *pu32Crc)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Crc)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Crc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA2_ROOT_KEY);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa2RootKeyCrcLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_CSA2_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa2RootKeyCrcLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);

    *pu32Lock = CHECKSUMLOCK.bits.locker_CSA2_RootKey;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2rRootKeyCrc(HI_U32 u32Crc)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurCrc = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if (1 == CHECKSUMLOCK.bits.locker_R2R_RootKey)
    {
        u32CurCrc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY);
        if (u32CurCrc == u32Crc)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY, u32Crc);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_R2R_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2rRootKeyCrc(HI_U32 *pu32Crc)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Crc)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Crc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_R2R_ROOT_KEY);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetR2rRootKeyCrcLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_R2R_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetR2rRootKeyCrcLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);

    *pu32Lock = CHECKSUMLOCK.bits.locker_R2R_RootKey;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSpRootKeyCrc(HI_U32 u32Crc)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurCrc = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if (1 == CHECKSUMLOCK.bits.locker_SP_RootKey)
    {
        u32CurCrc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY);
        if (u32CurCrc == u32Crc)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY, u32Crc);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_SP_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSpRootKeyCrc(HI_U32 *pu32Crc)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Crc)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Crc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_SP_ROOT_KEY);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetSpRootKeyCrcLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_SP_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSpRootKeyCrcLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);

    *pu32Lock = CHECKSUMLOCK.bits.locker_SP_RootKey;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa3RootKeyCrc(HI_U32 u32Crc)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurCrc = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if (1 == CHECKSUMLOCK.bits.locker_CSA3_RootKey)
    {
        u32CurCrc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY);
        if (u32CurCrc == u32Crc)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY, u32Crc);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_CSA3_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa3RootKeyCrc(HI_U32 *pu32Crc)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Crc)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Crc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_CSA3_ROOT_KEY);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetCsa3RootKeyCrcLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_CSA3_RootKey = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa3RootKeyCrcLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);

    *pu32Lock = CHECKSUMLOCK.bits.locker_CSA3_RootKey;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagKeyCrc(HI_U32 u32Crc)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurCrc = 0;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);
    if (1 == CHECKSUMLOCK.bits.locker_JTAGKey_ChipID)
    {
        u32CurCrc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID);
        if (u32CurCrc == u32Crc)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID, u32Crc);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_JTAGKey_ChipID = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagKeyCrc(HI_U32 *pu32Crc)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Crc)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Crc = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUM_JTAGKEY_CHIPID);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagKeyCrcLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.bits.locker_JTAGKey_ChipID = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK, CHECKSUMLOCK.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagKeyCrcLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_CHECKSUMLOCK_U CHECKSUMLOCK;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CHECKSUMLOCK.u32 = 0;
    CHECKSUMLOCK.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_CHECKSUMLOCK);

    *pu32Lock = CHECKSUMLOCK.bits.locker_JTAGKey_ChipID;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetScsEnBak(HI_U32 u32Bak)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurValue = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DATALOCK_0.bits.scs_en_bak_lock)
    {
        u32CurValue = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SCS_EN_BAK);
        if (u32CurValue == u32Bak)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_SCS_EN_BAK, u32Bak);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.scs_en_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetScsEnBak(HI_U32 *pu32Bak)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Bak)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Bak = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_SCS_EN_BAK);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetScsEnBakLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.scs_en_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetScsEnBakLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    *pu32Lock = DATALOCK_0.bits.scs_en_bak_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagModeBak(HI_U32 u32Bak)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurValue = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DATALOCK_0.bits.jtag_mode_bak_lock)
    {
        u32CurValue = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_JTAG_MODE_BAK);
        if (u32CurValue == u32Bak)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_JTAG_MODE_BAK, u32Bak);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.jtag_mode_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagModeBak(HI_U32 *pu32Bak)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Bak)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Bak = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_JTAG_MODE_BAK);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetJtagModeBakLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.jtag_mode_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetJtagModeBakLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    *pu32Lock = DATALOCK_0.bits.jtag_mode_bak_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRightCtrlEnBak(HI_U32 u32Bak)
{
    HI_S32    ret = HI_SUCCESS;
    HI_U32    u32CurValue = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DATALOCK_0.bits.right_ctrl_en_bak_lock)
    {
        u32CurValue = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK);
        if (u32CurValue == u32Bak)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK, u32Bak);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    u32CurValue = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK);
    if (u32CurValue != u32Bak)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return HI_FAILURE;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.right_ctrl_en_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRightCtrlEnBak(HI_U32 *pu32Bak)
{
    HI_S32    ret = HI_SUCCESS;

    if (NULL == pu32Bak)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    *pu32Bak = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_RIGHT_CTRL_EN_BAK);

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetRightCtrlEnBakLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    DATALOCK_0.u32 = 0;
    DATALOCK_0.bits.right_ctrl_en_bak_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DATALOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetRightCtrlEnBakLock(HI_U32 *pu32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DATALOCK_0;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    DATALOCK_0.u32 = 0;
    DATALOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    *pu32Lock = DATALOCK_0.bits.right_ctrl_en_bak_lock;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetMiscKlDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.misc_kl_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.misc_kl_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.misc_kl_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.misc_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetMiscKlDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32Disable = PV_1.bits.misc_kl_disable;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetMiscKlDisableLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.misc_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetMiscKlDisableLock(HI_U32 *pu32Lock)
{
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.misc_kl_disable_lock;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetGgKlDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.gg_kl_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.gg_kl_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.gg_kl_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.gg_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetGgKlDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = 0;
    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);

    *pu32Disable = PV_1.bits.gg_kl_disable;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetGgKlDisableLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.gg_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetGgKlDisableLock(HI_U32 *pu32Lock)
{
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if (NULL == pu32Lock)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = 0;
    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);

    *pu32Lock = PVLOCK_1.bits.gg_kl_disable_lock;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetBootInfoDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U ONEWAY_0;

    /*lock value*/
    ONEWAY_0.u32 = 0;
    ONEWAY_0.bits.boot_info_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, ONEWAY_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootInfoDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_ONE_WAY_0_U ONEWAY_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    ONEWAY_0.u32 = 0;
    ONEWAY_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);

    *pu32Disable = ONEWAY_0.bits.boot_info_disable;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetUSBDisable(HI_U32 u32Enable)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;
    CA_OTP_V200_CHIPSET_CFG_LOCK_1_U CFGLOCK_1;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CFGLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_1);
    if (1 == CFGLOCK_1.bits.usb2_disable_lock)
    {
        CFG_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_CHIPSET_CONFIGURE_1);
        if (u32Enable == CFG_1.bits.usb2_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            HI_ERR_CA("USB disable has already set another value\n");
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    if (1 == u32Enable)
    {
        /*set value*/
        CFG_1.u32 = 0;
        CFG_1.bits.usb2_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_1, CFG_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    CFGLOCK_1.u32 = 0;
    CFGLOCK_1.bits.usb2_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_1, CFGLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set usb2_disable_lock bit!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetUSBDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CFG_1.u32 = 0;
    CFG_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_CHIPSET_CONFIGURE_1);

    *pu32Disable = CFG_1.bits.usb2_disable;

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_SetUARTDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;
    CA_OTP_V200_CHIPSET_CFG_LOCK_1_U CFGLOCK_0;

    /*set value*/
    CFG_1.u32 = 0;
    CFG_1.bits.uart_disable_0 = 1;
    CFG_1.bits.uart_disable_1 = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_1, CFG_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set UART disable!\n");
    }

    /*lock value*/
    CFGLOCK_0.u32 = 0;
    CFGLOCK_0.bits.uart_disable_0_lock = 1;
    CFGLOCK_0.bits.uart_disable_1_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_1, CFGLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set usb2_disable_lock bit!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetUARTDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CFG_1.u32 = 0;
    CFG_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_CHIPSET_CONFIGURE_1);

    *pu32Disable = CFG_1.bits.uart_disable_0;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetEthernetDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;
    CA_OTP_V200_CHIPSET_CFG_LOCK_1_U CFGLOCK_1;

#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    CA_OTP_V200_CHIPSET_CFG_2_U CFG_2;
    CA_OTP_V200_CHIPSET_CFG_LOCK_2_U CFGLOCK_2;
#endif

    /*set value*/
    CFG_1.u32 = 0;
    CFG_1.bits.sf_disable= 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_1, CFG_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set UART disable!\n");
        return ret;
    }

    /*lock value*/
    CFGLOCK_1.u32 = 0;
    CFGLOCK_1.bits.sf_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_1, CFGLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set usb2_disable_lock bit!\n");
        return ret;
    }

#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    CFG_2.u32 = 0;
    CFG_2.bits.gmii_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_2, CFG_2.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set gmii disable\n");
        return ret;
    }

    /*lock value*/
    CFGLOCK_2.u32 = 0;
    CFGLOCK_2.bits.gmii_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_CHIPSET_CONFIGURE_LOCK_2, CFGLOCK_2.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set gmii disable lock\n");
        return ret;
    }
#endif

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetEthernetDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_CHIPSET_CFG_1_U CFG_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    CFG_1.u32 = 0;
    CFG_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_CHIPSET_CONFIGURE_1);

    *pu32Disable = CFG_1.bits.sf_disable;

    return HI_SUCCESS;
}


HI_S32 DRV_CA_OTP_V200_SetSM4Disable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    /* disable */
    PV_0.u32 = 0;
    PV_0.bits.tskl_nv_disable = 1;
#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3798cv200)  || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    PV_0.bits.tskl_others_disable = 1;
#endif
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /* lock */
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_nv_disable_lock = 1;
#if defined (CHIP_TYPE_hi3716mv420) || defined (CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
    PVLOCK_0.bits.tskl_others_disable_lock = 1;
#endif
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}


HI_S32 DRV_CA_OTP_V200_GetSM4Disable(HI_U32 *pu32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Disable = PV_0.bits.tskl_nv_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTDESDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    /* disable */
    PV_0.u32 = 0;
    PV_0.bits.tskl_tdes_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /* lock */
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_tdes_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTDESDisable(HI_U32 *pu32Disable)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Disable = PV_0.bits.tskl_tdes_disable;

    return ret;
}


HI_S32 DRV_CA_OTP_V200_SetChecksumReadDisable(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_0_U ONEWAY_0;

    /*lock value*/
    ONEWAY_0.u32 = 0;
    ONEWAY_0.bits.checksum_read_disable = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_0, ONEWAY_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetChecksumReadDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_ONE_WAY_0_U ONEWAY_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    ONEWAY_0.u32 = 0;
    ONEWAY_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_0);

    *pu32Disable = ONEWAY_0.bits.checksum_read_disable;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetSelfBootDisableBak(HI_U32 u32Bak)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_1_U ONEWAY_1;

    /*lock value*/
    ONEWAY_1.u32 = 0;
    ONEWAY_1.bits.self_boot_disable_bak = (HI_U8)u32Bak;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_1, ONEWAY_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    ONEWAY_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
    if (ONEWAY_1.bits.self_boot_disable_bak != u32Bak)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return HI_FAILURE;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetSelfBootDisableBak(HI_U32 *pu32Bak)
{
    CA_OTP_V200_ONE_WAY_1_U ONEWAY_1;

    if (NULL == pu32Bak)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    ONEWAY_1.u32 = 0;
    ONEWAY_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);

    *pu32Bak = ONEWAY_1.bits.self_boot_disable_bak;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetTzAreaLock(HI_U32 u32Lock)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_ONE_WAY_1_U ONEWAY_1;

    /*lock value*/
    ONEWAY_1.u32 = 0;
    ONEWAY_1.bits.tz_area_lock = (HI_U8)u32Lock;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_ONEWAY_1, ONEWAY_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
    }

    ONEWAY_1.u32 = 0;
    ONEWAY_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);
    if (ONEWAY_1.bits.tz_area_lock != u32Lock)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return HI_FAILURE;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetTzAreaLock(HI_U32 *pu32Disable)
{
    CA_OTP_V200_ONE_WAY_1_U ONEWAY_1;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    ONEWAY_1.u32 = 0;
    ONEWAY_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_ONEWAY_1);

    *pu32Disable = ONEWAY_1.bits.tz_area_lock;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetBootMode_0(CA_OTP_FLASH_TYPE_E type)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (type > CA_OTP_FLASH_TYPE_NAND)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.boot_mode_sel_0_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (type == PV_0.bits.boot_mode_sel_0)
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
    if (CA_OTP_FLASH_TYPE_NAND == type)
    {
        PV_0.bits.boot_mode_sel_0 = 1;
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock boot mode*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.boot_mode_sel_0_lock = 1;

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootMode_0(CA_OTP_FLASH_TYPE_E *pType)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pType)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pType = PV_0.bits.boot_mode_sel_0;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetBootMode_1(HI_U32 type)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if (type >= CA_OTP_FLASH_TYPE_BUTT)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.boot_mode_sel_1_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (type == (PV_0.bits.boot_mode_sel_1))
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
    if (1 == type)
    {
        PV_0.bits.boot_mode_sel_1 = 1;
    }

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    /*lock boot mode*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.boot_mode_sel_1_lock = 1;

    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetBootMode_1(HI_U32 *pType)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pType)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pType = PV_0.bits.boot_mode_sel_1;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetMiscKLDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.misc_kl_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.misc_kl_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.misc_kl_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.misc_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetGgKLDisable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;
    CA_OTP_V200_INTERNAL_PVLOCK_1_U PVLOCK_1;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_1);
    if (1 == PVLOCK_1.bits.gg_kl_disable_lock)
    {
        PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
        if (u32Disable == PV_1.bits.gg_kl_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_1.u32 = 0;
        PV_1.bits.gg_kl_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_1.u32 = 0;
    PVLOCK_1.bits.gg_kl_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_1, PVLOCK_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetTsklCsa3Disable(HI_U32 u32Disable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PVLOCK_0_U PVLOCK_0;

    if ((u32Disable != 0) && (u32Disable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.tskl_csa3_disable_lock)
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Disable == PV_0.bits.tskl_csa3_disable)
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Disable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.tskl_csa3_disable = 1;
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.tskl_csa3_disable_lock = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_U32 DRV_CA_OTP_V200_Read(HI_U32 addr)
{
    return g_pOTPExportFunctionList->HAL_OTP_V200_Read(addr);
}

HI_U8 DRV_CA_OTP_V200_ReadByte(HI_U32 addr)
{
    return g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte(addr);
}

HI_S32 DRV_CA_OTP_V200_WriteByte(HI_U32 addr, HI_U8 tdata)
{
    return g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte(addr, tdata);
}

HI_S32 DRV_CA_OTP_V200_GetDDRWakeup(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Enable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Enable = PV_0.bits.ddr_wakeup_disable;

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetCsa2KlDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Disable = PV_0.bits.tskl_csa2_disable;
    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetCsa3KlDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Disable = PV_0.bits.tskl_csa3_disable;
    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_GetSpKlDisable(HI_U32 *pu32Disable)
{
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (NULL == pu32Disable)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = 0;
    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);

    *pu32Disable = PV_0.bits.tskl_sp_disable;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_Set_VMXBLFuse(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    //Set vmx_bl_fuse
    PV_1.u32 = 0;
    PV_1.bits.vmx_bl_fuse = 1;
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_1, PV_1.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_Get_VMXBLFuse(HI_U32 *pu32Value)
{
    CA_OTP_V200_INTERNAL_PV_1_U PV_1;

    if(NULL == pu32Value)
    {
        HI_ERR_CA("Invalid parameters!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_1.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_1);
    *pu32Value = PV_1.bits.vmx_bl_fuse;

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetItCsa3Enable(HI_U32 u32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;
    CA_OTP_V200_INTERNAL_PV_0_U PVLOCK_0;

    if ((u32Enable != 0) && (u32Enable != 1))
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PVLOCK_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PVLOCK_0);
    if (1 == PVLOCK_0.bits.mc_rc4_hardonly_en) //0x03[1], used as "itcsa3_enable" in irdeto MSR2.2
    {
        PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
        if (u32Enable == PV_0.bits.mc_rc4_hardonly_en) //0x03[1], used as "itcsa3_enable" in irdeto MSR2.2
        {
            /*value already set to the right one, return HI_SUCCESS*/
            return HI_SUCCESS;
        }
        else
        {
            /*set value to another value, return HI_ERR_CA_SETPARAM_AGAIN*/
            return HI_ERR_CA_SETPARAM_AGAIN;
        }
    }

    /*set value*/
    if (u32Enable == 1)
    {
        PV_0.u32 = 0;
        PV_0.bits.mc_rc4_hardonly_en = 1; //0x03[1], used as "itcsa3_enable" in irdeto MSR2.2
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PV_0, PV_0.u32);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to write OTP!\n");
            return ret;
        }
    }

    /*lock value*/
    PVLOCK_0.u32 = 0;
    PVLOCK_0.bits.mc_rc4_hardonly_en = 1; //0x03[1], used as "itcsa3_enable" in irdeto MSR2.2
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_PVLOCK_0, PVLOCK_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to write OTP!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetItCsa3Enable(HI_U32 *pu32Enable)
{
    HI_S32    ret = HI_SUCCESS;
    CA_OTP_V200_INTERNAL_PV_0_U PV_0;

    if (pu32Enable == NULL)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    PV_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_PV_0);
    *pu32Enable = PV_0.bits.mc_rc4_hardonly_en; //0x03[1], used as "itcsa3_enable" in irdeto MSR2.2

    return ret;
}

HI_S32 DRV_CA_OTP_V200_SetITCSA3IMLB(HI_U8 *pu8IMLB)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0;
    CA_OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu8IMLB)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* check if IMLB locked */
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.stbsn3_lock) //used as ITCSA3_IMLB in Irdeto MSR2.2
    {
        HI_ERR_CA("Failed to set ITCSA3_IMLB, locked!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < 2; i++)
    {
        ret = (g_pOTPExportFunctionList->HAL_OTP_V200_WriteByte)(CA_OTP_V200_INTERNAL_ITCSA3_IMLB + i, pu8IMLB[i]);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_CA("Fail to set ITCSA3_IMLB!\n");
            return ret;
        }
    }

    /* lock ITCSA3_IMLB */
    DataLock_0.u32 = 0;
    DataLock_0.u32 = (g_pOTPExportFunctionList->HAL_OTP_V200_Read)(CA_OTP_V200_INTERNAL_DATALOCK_0);

    DataLock_0.bits.stbsn3_lock = 1; //used as ITCSA3_IMLB in Irdeto MSR2.2
    ret = (g_pOTPExportFunctionList->HAL_OTP_V200_Write)(CA_OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("Fail to set ITCSA3_IMLB_Lock!\n");
        return ret;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetITCSA3IMLB(HI_U8 *pu8IMLB)
{
    HI_U32 i = 0;

    if (NULL == pu8IMLB)
    {
        return HI_ERR_CA_INVALID_PARA;
    }

    /* read from OTP */
    for (i = 0; i < 2; i++)
    {
        pu8IMLB[i] = (g_pOTPExportFunctionList->HAL_OTP_V200_ReadByte)(CA_OTP_V200_INTERNAL_ITCSA3_IMLB + i);
    }

    return HI_SUCCESS;
}

HI_S32 DRV_CA_OTP_V200_SetOtpFuse(CA_OTP_ATTR_S *pstOtpAttr)
{
    HI_S32    ret = HI_SUCCESS;
    HI_BOOL bEnable;
    CA_OTP_FLASH_TYPE_E enFlashType;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLevel;
    HI_UNF_ADVCA_JTAG_MODE_E enJtagPrtMode;
    HI_U8 *pu8TmBuf = NULL;
    //HI_U32 u32ChipId;
    HI_U8 u8TmpBuf[512];
    CA_OTP_VENDOR_TYPE_E enVendorId;
    HI_U32 u32MaketId;

    if(pstOtpAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstOtpAttr == NULL\n");
        return HI_FAILURE;
    }

    switch(pstOtpAttr->enOtpFuseId)
    {
        case HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION:

            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.bEnable;
            enFlashType = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType;
            if(bEnable)
            {
                ret = DRV_CA_OTP_V200_SetSCSActive();
                if(HI_SUCCESS == ret)
                {
                    ret = DRV_CA_OTP_V200_SetBootMode(enFlashType);
                }
                if(HI_SUCCESS == ret)
                {
                    ret = DRV_CA_OTP_V200_SetBootSelCtrl(1);
                }
            }
            break;

        case HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION_ONLY:
            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            if(bEnable)
            {
                ret = DRV_CA_OTP_V200_SetSCSActive();
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE:
            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stBootFlashType.bBootSelCtrl;
            if(bEnable)
            {
                enFlashType = (CA_OTP_FLASH_TYPE_E)pstOtpAttr->stOtpAttr.unOtpFuseAttr.stBootFlashType.enFlashType;
                ret = DRV_CA_OTP_V200_SetBootMode(enFlashType);
                if(HI_SUCCESS == ret)
                {
                    ret = DRV_CA_OTP_V200_SetBootSelCtrl(1);
                }
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION:
            ret = DRV_CA_OTP_V200_SetBloadDecEn(HI_TRUE);
            break;
        case HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION:
            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            ret = DRV_CA_OTP_V200_SetBloadEncDisable(bEnable);
            break;
        case HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION:

            ret = DRV_CA_OTP_V200_DisableSelfBoot(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetLowPowerDisable(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL:

            enKeyladderLevel = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
            ret = HAL_ADVCA_V300_SetCWLadderLevel(enKeyladderLevel);
            break;

        case HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL:

            enKeyladderLevel = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
            ret = HAL_ADVCA_V300_SetR2RLadderLevel(enKeyladderLevel);
            break;

        case HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL:

            enKeyladderLevel = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
            ret = HAL_ADVCA_V300_SetSPLadderLevel(enKeyladderLevel);
            break;

        case HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL:

            enKeyladderLevel = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
            ret = DRV_CA_OTP_V200_SetCSA3LadderLevel(enKeyladderLevel);
            break;

        case HI_UNF_ADVCA_OTP_LP_DEACTIVATION:

            ret = DRV_CA_OTP_V200_DisableLinkProtect();
            break;

        case HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION:

            ret = DRV_CA_OTP_V200_LockHardCwSel();
            break;

        case HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetTSSPEHardonlyEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetTSCSA3HardonlyEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetTSCWDeactive(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetTSSPDeactive(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetTSCSA3Deactive(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetMiscKLDisable(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetGgKLDisable(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION:

            DRV_CA_OTP_V200_SetDcasKLDisable(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetDDRScrambleEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION:
            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            ret = DRV_CA_OTP_V200_SetOtpGlobalLockEn(bEnable);
            break;

        case HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetRuntimeCheckEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetWakeupDDRCheckEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetVersionIdCheckEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION:

            ret = DRV_CA_OTP_V200_SetBlMSIDCheckEn(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_JTAG_MODE:

            enJtagPrtMode = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stJtagPrtMode.enJtagMode;
            ret = DRV_CA_OTP_V200_SetJtagProtectMode(enJtagPrtMode);
            break;

       case  HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION:

            ret = DRV_CA_OTP_V200_SetJtagReadDisable();
            break;

       case  HI_UNF_ADVCA_OTP_CSA2_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetCSA2RootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_R2R_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetR2RRootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_SP_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetSPRootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_CSA3_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetCSA3RootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_MISC_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetMISCRootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_OEM_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetOEMRootKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_JTAG_KEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stJtagKey.u8JtagKey;
            ret = DRV_CA_OTP_V200_SetJtagKey(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_CHIP_ID:
       {
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId;
            ret = DRV_CA_OTP_V200_SetUniqueChipId(pu8TmBuf);
            break;
       }

       case  HI_UNF_ADVCA_OTP_ESCK_ROOTKEY:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetESCK(pu8TmBuf);
            break;

       case  HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID;
            ret = DRV_CA_OTP_V200_GetVendorId(&enVendorId);
            if(ret != HI_SUCCESS)
            {
                return ret;
            }

            if (CA_OTP_VENDOR_NAGRA == enVendorId)
            {
                u32MaketId = *(HI_U32*)pu8TmBuf;
                ret = DRV_CA_OTP_V200_Set_MarketId(u32MaketId);
            }
            else
            {
                memset(u8TmpBuf, 0, sizeof(u8TmpBuf));
                u8TmpBuf[0] = pu8TmBuf[3];
                u8TmpBuf[1] = pu8TmBuf[2];
                u8TmpBuf[2] = pu8TmBuf[1];
                u8TmpBuf[3] = pu8TmBuf[0];
                ret = DRV_CA_OTP_V200_Set_UniqueMarketId(u8TmpBuf);
            }
            break;

       case  HI_UNF_ADVCA_OTP_VERSION_ID:

            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId;
            memset(u8TmpBuf, 0, sizeof(u8TmpBuf));
            u8TmpBuf[0] = pu8TmBuf[3];
            u8TmpBuf[1] = pu8TmBuf[2];
            u8TmpBuf[2] = pu8TmBuf[1];
            u8TmpBuf[3] = pu8TmBuf[0];
            ret = DRV_CA_OTP_V200_Set_UniqueVersionId(u8TmpBuf);
            break;

        case HI_UNF_ADVCA_OTP_MISC_KL_LEVEL_SEL:
            enKeyladderLevel = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel;
            ret = HAL_ADVCA_V300_SetMiscKlLevel(enKeyladderLevel);
            break;
        case HI_UNF_ADVCA_OTP_VMX_BL_FUSE:
            ret = DRV_CA_OTP_V200_Set_VMXBLFuse();
            break;

        case HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION:
            ret = DRV_CA_OTP_V200_SetItCsa3Enable(HI_TRUE);
            break;

        case HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION:
            ret = DRV_CA_OTP_V200_SetBootInfoDisable();
            break;

        case HI_UNF_ADVCA_OTP_ITCSA3_IMLB:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB;
            ret = DRV_CA_OTP_V200_SetITCSA3IMLB(pu8TmBuf);
            break;

        case HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_LockRSAKey();
            break;
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
        case HI_UNF_ADVCA_OTP_USB_DEACTIVATION:
            bEnable = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable;
            ret = DRV_CA_OTP_V200_SetUSBDisable(bEnable);
            break;

        case HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_SetUARTDisable();
            break;

        case HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION:
            ret = DRV_CA_OTP_V200_SetEthernetDisable();
            break;

        case HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION:
            ret = DRV_CA_OTP_V200_SetSM4Disable();
            break;

        case HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION:
            ret = DRV_CA_OTP_V200_SetTDESDisable();
            break;
#endif
        case HI_UNF_ADVCA_OTP_BOOT_ROOTKEY:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetBootRootKey(pu8TmBuf);
            break;
        case HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderRootKey.u8RootKey;
            ret = DRV_CA_OTP_V200_SetSecStoreRootKey(pu8TmBuf);
            break;
        default:
            HI_ERR_CA("Not Support set this otpfuse!\n");
            ret = HI_ERR_CA_NOT_SUPPORT;
            break;
    }

    return ret;
}

HI_S32 DRV_CA_OTP_V200_GetOtpFuse(CA_OTP_ATTR_S *pstOtpAttr)
{
    HI_S32    ret = HI_SUCCESS;
    HI_BOOL bEnable;
    HI_U32  u32BootSel=0;
    HI_UNF_ADVCA_FLASH_TYPE_E enFlashType= HI_UNF_ADVCA_FLASH_TYPE_BUTT;
    HI_UNF_ADVCA_KEYLADDER_LEV_E enKeyladderLevel;
    HI_UNF_ADVCA_JTAG_MODE_E enJtagPrtMode;
    HI_U8 *pu8TmBuf = NULL;
    HI_U32 u32ChipId;
    HI_U8 u8TmpBuf[512] = {0};
    CA_OTP_VENDOR_TYPE_E enVendorId;
    HI_U32 u32MaketId;

    if(pstOtpAttr == NULL)
    {
        HI_ERR_CA("Invalid parameter, pstOtpAttr == NULL\n");
        return HI_FAILURE;
    }

    switch (pstOtpAttr->enOtpFuseId)
    {
        case HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetSCSActive((HI_U32*)&bEnable);
            ret |= DRV_CA_OTP_V200_GetBootSelCtrl(&u32BootSel);
            ret |= DRV_CA_OTP_V200_GetBootMode(&enFlashType);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.bEnable = bEnable;
                if(u32BootSel)  // boot flash type is defined by boot_mode_sel_n in OTP
                {
                    pstOtpAttr->stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType = (HI_UNF_ADVCA_FLASH_TYPE_E)enFlashType;
                }
                else    // boot flash type is defined by chipset pin
                {
                    pstOtpAttr->stOtpAttr.unOtpFuseAttr.stEnableSecureBoot.enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
                }
            }
            break;

        case HI_UNF_ADVCA_OTP_SECURE_BOOT_ACTIVATION_ONLY:
            ret = DRV_CA_OTP_V200_GetSCSActive((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_FLASH_TYPE:
            ret = DRV_CA_OTP_V200_GetBootSelCtrl(&u32BootSel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stBootFlashType.bBootSelCtrl = u32BootSel;
                if(u32BootSel)  // boot flash type is defined by boot_mode_sel_n in OTP
                {
                    ret = DRV_CA_OTP_V200_GetBootMode(&enFlashType);
                    if(ret == HI_SUCCESS)
                    {
                        pstOtpAttr->stOtpAttr.unOtpFuseAttr.stBootFlashType.enFlashType = (HI_UNF_ADVCA_FLASH_TYPE_E)enFlashType;
                    }
                }
                else    // boot flash type is defined by chipset pin
                {
                    pstOtpAttr->stOtpAttr.unOtpFuseAttr.stBootFlashType.enFlashType = HI_UNF_ADVCA_FLASH_TYPE_BUTT;
                }
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_DECRYPTION_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetBloadDecEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_ENCRYPTION_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetBloadEncDisable((HI_U32 *)&bEnable);
            if (HI_SUCCESS == ret)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SELF_BOOT_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetSelfBoot((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_DEACTIVATION:
            DRV_CA_OTP_V200_GetDDRWakeup((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA2_KL_LEVEL_SEL:
            ret = HAL_ADVCA_V300_GetCWLadderLevel(&enKeyladderLevel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            }
            break;

        case HI_UNF_ADVCA_OTP_R2R_KL_LEVEL_SEL:
            ret = HAL_ADVCA_V300_GetR2RLadderLevel(&enKeyladderLevel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            }
            break;

        case HI_UNF_ADVCA_OTP_SP_KL_LEVEL_SEL:
            ret = HAL_ADVCA_V300_GetSPLadderLevel(&enKeyladderLevel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA3_KL_LEVEL_SEL:
            ret = DRV_CA_OTP_V200_GetCSA3LadderLevel((CA_OTP_KEY_LEVEL_E*)&enKeyladderLevel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            }
            break;

        case HI_UNF_ADVCA_OTP_LP_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetLinkProtectDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA2_CW_HARDONLY_ACTIVATION:
            ret = HAL_ADVCA_V300_StatHardCwSel((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SP_CW_HARDONLY_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetTSSPHardonlyEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA3_CW_HARDONLY_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetTSCSA3HardonlyEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA2_KL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetCsa2KlDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SP_KL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetSpKlDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA3_KL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetCsa3KlDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_MISC_KL_DEACTIVATION:
            DRV_CA_OTP_V200_GetMiscKlDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_GOOGLE_KL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetGgKlDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_DCAS_KL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetDcasKLDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_DDR_SCRAMBLE_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetDDRScrambleEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_GLOBAL_LOCK_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetOtpGlobalLockEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_RUNTIME_CHECK_ACTIVATION:

            ret = DRV_CA_OTP_V200_GetRuntimeCheckEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_DDR_WAKEUP_CHECK_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetWakeupDDRCheckEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_VERSION_ID_CHECK_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetVersionIDCheckEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_MSID_CHECK_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetBlMSIDCheckEn((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_JTAG_MODE:
            ret = DRV_CA_OTP_V200_GetJtagProtectMode((CA_OTP_JTAG_MODE_E *)&enJtagPrtMode);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stJtagPrtMode.enJtagMode = enJtagPrtMode;
            }
            break;

        case HI_UNF_ADVCA_OTP_JTAG_READ_DEACTIVATION:
            DRV_CA_OTP_V200_GetJtagReadDisable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CHIP_ID:
            ret = DRV_CA_OTP_V200_GetVendorId(&enVendorId);
            if (ret != HI_SUCCESS)
            {
                return ret;
            }

            if ((CA_OTP_VENDOR_CONAX == enVendorId) || (CA_OTP_VENDOR_NAGRA == enVendorId))
            {
                ret = DRV_CA_OTP_V200_GetChipId(&u32ChipId);
                memcpy(pstOtpAttr->stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId, (HI_U8 *)&u32ChipId, 4);
            }
            else
            {
                HI_U32 index = 0;
                HI_U32 addr = CA_OTP_V200_INTERNAL_CA_CHIP_ID_0;

                for (index = 0; index < 8; index++)
                {
                    u8TmpBuf[index] = DRV_CA_OTP_V200_ReadByte(addr + index);
                }
                memcpy(pstOtpAttr->stOtpAttr.unOtpFuseAttr.stChipId.au8ChipId, u8TmpBuf, 8);
            }

            break;

        case  HI_UNF_ADVCA_OTP_MARKET_SEGMENT_ID:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stMarketId.u8MSID;
            ret = DRV_CA_OTP_V200_GetVendorId(&enVendorId);
            if (ret != HI_SUCCESS)
            {
                return ret;
            }

            if (CA_OTP_VENDOR_NAGRA == enVendorId)
            {
                ret = DRV_CA_OTP_V200_MarketId(&u32MaketId);
                pu8TmBuf[0] = (u32MaketId >> 24) & 0xff;
                pu8TmBuf[1] = (u32MaketId >> 16) & 0xff;
                pu8TmBuf[2] = (u32MaketId >> 8) & 0xff;
                pu8TmBuf[3] = u32MaketId & 0xff;
            }
            else
            {
                ret = DRV_CA_OTP_V200_Get_UniqueMarketId(u8TmpBuf);
                pu8TmBuf[0] = u8TmpBuf[3];
                pu8TmBuf[1] = u8TmpBuf[2];
                pu8TmBuf[2] = u8TmpBuf[1];
                pu8TmBuf[3] = u8TmpBuf[0];
            }
            break;

        case  HI_UNF_ADVCA_OTP_VERSION_ID:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stVersionId.u8VersionId;

            ret = DRV_CA_OTP_V200_Get_UniqueVersionId(u8TmpBuf);
            if (ret == HI_SUCCESS)
            {
                pu8TmBuf[0] = u8TmpBuf[3];
                pu8TmBuf[1] = u8TmpBuf[2];
                pu8TmBuf[2] = u8TmpBuf[1];
                pu8TmBuf[3] = u8TmpBuf[0];
            }
            break;

        case HI_UNF_ADVCA_OTP_MISC_KL_LEVEL_SEL:
            ret = HAL_ADVCA_V300_GetMiscKlLevel(&enKeyladderLevel);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stKeyladderLevSel.enKeyladderLevel = enKeyladderLevel;
            }
            break;
        case HI_UNF_ADVCA_OTP_VMX_BL_FUSE:
            //ret = DRV_CA_OTP_V200_Get_VMXBLFuse();
            break;

        case HI_UNF_ADVCA_OTP_IRDETO_ITCSA3_ACTIVATION:
            ret = DRV_CA_OTP_V200_GetItCsa3Enable((HI_U32*)&bEnable);
            if(ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOTINFO_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetBootInfoDisable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_ITCSA3_IMLB:
            pu8TmBuf = pstOtpAttr->stOtpAttr.unOtpFuseAttr.stItCsa3IMLB.au8ItCsa3IMLB;
            ret = DRV_CA_OTP_V200_GetITCSA3IMLB(pu8TmBuf);
            break;
#if defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420) || defined(CHIP_TYPE_hi3798cv200) || defined(CHIP_TYPE_hi3798mv200) || defined(CHIP_TYPE_hi3798mv200_a)
        case HI_UNF_ADVCA_OTP_USB_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetUSBDisable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SERIAL_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetUARTDisable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_ETHERNET_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetEthernetDisable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SM4_CRYPTO_ENGINE_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetSM4Disable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_TDES_CRYPTO_ENGINE_DEACTIVATION:
            ret = DRV_CA_OTP_V200_GetTDESDisable((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;
#endif
        case HI_UNF_ADVCA_OTP_RSA_KEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetRSAKeyLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_STBSN_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetStbSnLock_0((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_MSID_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetMktIdLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_VERSIONID_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetVersionIdCheckLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_OEM_ROOTKEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetOEMRootKeyLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_R2R_ROOTKEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetR2rRootKeyLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_JTAG_KEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetJtagKeyLock((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_BOOT_ROOTKEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetBootRootKeyLockFlag((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_SEC_STORE_ROOTKEY_LOCK_FLAG:
            ret = DRV_CA_OTP_V200_GetSecStoreRootKeyLockFlag((HI_U32*)&bEnable);
            if (ret == HI_SUCCESS)
            {
                pstOtpAttr->stOtpAttr.unOtpFuseAttr.stDefaultAttr.bEnable = bEnable;
            }
            break;

        case HI_UNF_ADVCA_OTP_CSA2_ROOTKEY:
        case HI_UNF_ADVCA_OTP_R2R_ROOTKEY:
        case HI_UNF_ADVCA_OTP_SP_ROOTKEY:
        case HI_UNF_ADVCA_OTP_CSA3_ROOTKEY:
        case HI_UNF_ADVCA_OTP_MISC_ROOTKEY:
        case HI_UNF_ADVCA_OTP_OEM_ROOTKEY:
        case HI_UNF_ADVCA_OTP_JTAG_KEY:
        case HI_UNF_ADVCA_OTP_ESCK_ROOTKEY:
        case HI_UNF_ADVCA_OTP_TZ_AREA_LOCK_FLAG:
        default:
            HI_ERR_CA("Not Support get this otpfuse!\n");
            ret = HI_ERR_CA_NOT_SUPPORT;
            break;
    }

    return ret;
}

/*--------------------------------------END--------------------------------------*/
