/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_otp_v200.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <asm/delay.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/memory.h>
#include <linux/delay.h>
#include "hi_type.h"
#include "drv_otp.h"
#include "drv_otp_common.h"
#include "drv_otp_reg_v200.h"
#include "drv_otp_v200.h"
#ifdef SDK_OTP_ARCH_VERSION_V3
#include "hi_error_mpi.h"
#include "drv_otp_ext.h"
#include "hi_reg_common.h"
#else
#include "otp_drv.h"
#endif

#define OTP_CUSTOMER_KEY_ADDR    0x2c0
#define OTP_STB_PRIV_DATA_ADDR    0x2b0

static HI_U16 OTP_CalculateCRC16 (const HI_U8 *pu8Data, HI_U32 u32DataLen)
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
        HI_ERR_OTP("Input param error, null pointer!\n");
        return crc_value;
    }

    if (u32DataLen == 0)
    {
        HI_ERR_OTP("Input param error, length = 0!\n");
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


HI_S32 OTP_V200_Set_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i;
    HI_U8 *pu8CustomerKey = NULL;
    HI_U32 pu32CustomerKey[4] = {0};

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    for(i = 0; i < 4; i++)
    {
        pu32CustomerKey[i] = HAL_OTP_V200_Read(OTP_CUSTOMER_KEY_ADDR + i * 4);
        if (0x0 != pu32CustomerKey[i])
        {
            break;
        }
    }

    if (i >= 4)
    {
        pu8CustomerKey = (HI_U8*)(pCustomerKey->u32CustomerKey);
        for(i = 0; i < 16; i++)
        {
            ret = HAL_OTP_V200_WriteByte(OTP_CUSTOMER_KEY_ADDR + i, pu8CustomerKey[i]);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_OTP("ERROR: Set Customer Key Error!\n");
                return ret;
            }
        }
    }
    else
    {
        HI_ERR_OTP("ERROR: Customer Key already set!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V200_Get_CustomerKey(OTP_CUSTOMER_KEY_S *pCustomerKey)
{
    HI_U32 i;
    HI_U32 *pu32CustomerKey = NULL;

    if(NULL == pCustomerKey)
    {
        return HI_FAILURE;
    }

    pu32CustomerKey = (HI_U32*)(pCustomerKey->u32CustomerKey);
    for(i = 0; i < 4; i++)
    {
        pu32CustomerKey[i] = HAL_OTP_V200_Read(OTP_CUSTOMER_KEY_ADDR + i * 4);
    }

    return HI_SUCCESS;
}

HI_S32 OTP_V200_Get_DDPLUS_Flag(HI_BOOL *pu32DDPlusFlag)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32DDPlusFlag)
    {
        return HI_FAILURE;
    }

    PV_1.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_PV_1);

    if (1 == PV_1.bits.dolby_flag)  /*0:support DDPLUS(default); 1: do not support DDPLUS*/
    {
        *pu32DDPlusFlag = HI_FALSE;
    }
    else
    {
        *pu32DDPlusFlag = HI_TRUE;
    }

    return ret;
}

HI_S32 OTP_V200_Get_DTS_Flag(HI_BOOL *pu32DTSFlag)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_PV_1_U PV_1;

    if (NULL == pu32DTSFlag)
    {
        return HI_FAILURE;
    }

    PV_1.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_PV_1);

    if (1 == PV_1.bits.dts_flag)  /*0:do not support DTS(default); 1: support DTS*/
    {
        *pu32DTSFlag = HI_TRUE;
    }
    else
    {
        *pu32DTSFlag = HI_FALSE;
    }

    return ret;
}


HI_S32 OTP_V200_Set_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    return HAL_OTP_V200_WriteByte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset, pStbPrivData->u8Data);
}

HI_S32 OTP_V200_Get_StbPrivData(OTP_STB_PRIV_DATA_S *pStbPrivData)
{
    if(NULL == pStbPrivData)
    {
        return HI_FAILURE;
    }

    pStbPrivData->u8Data = HAL_OTP_V200_ReadByte(OTP_STB_PRIV_DATA_ADDR + pStbPrivData->u32Offset);

    return HI_SUCCESS;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_SetHdcpRootKey(HI_U8 *pu8Key)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    OTP_V200_ONE_WAY_0_U unChsumRd;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == pu8Key)
    {
        return HI_FAILURE;
    }

    /* check if key locked */
    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.HDCP_RootKey_lock)
    {
        HI_ERR_OTP("HDCPKEY set ERROR! secret key lock!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_HDCP_RootKey)
    {
        HI_ERR_OTP("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < OTP_HDCP_ROOT_KEY_LEN; i++)
    {
        HAL_OTP_V200_WriteByte(OTP_V200_INTERNAL_HDCP_ROOTKEY_0 + i, pu8Key[i]);
    }

    /* set checksum */
    u16CRCValue = OTP_CalculateCRC16(pu8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = HAL_OTP_V200_WriteByte(OTP_V200_INTERNAL_CHECKSUM_HDCP_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Fail to write checksum!\n");
        return ret;
    }

    /* check if checksum_read_disable set or not */
    unChsumRd.u32 = 0;
    unChsumRd.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNEL_ONEWAY_0);
    if(0 == unChsumRd.bits.checksum_read_disable)
    {
        u8CheckSumCmp = HAL_OTP_V200_ReadByte(OTP_V200_INTERNAL_CHECKSUM_HDCP_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_OTP("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_OTP("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_HDCP_RootKey = 1;
    ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Fail to write checksum lock!\n");
        return ret;
    }

    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_HDCP_RootKey )
    {
        HI_ERR_OTP("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetHdcpRootKey(HI_U8 *pu8Key)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pu8Key)
    {
        return HI_FAILURE;
    }

    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.HDCP_RootKey_lock)
    {
        HI_ERR_OTP("HDCP Root KEY get ERROR! Hdcp root key lock!\n");
        return HI_FAILURE;
    }

    /* read from OTP */
    for (i = 0; i < OTP_HDCP_ROOT_KEY_LEN; i++)
    {
        pu8Key[i] = HAL_OTP_V200_ReadByte(OTP_V200_INTERNAL_HDCP_ROOTKEY_0 + i);
    }

    return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_SetHdcpRootKeyLock(HI_VOID)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    DataLock_0.u32 = 0;
    DataLock_0.bits.HDCP_RootKey_lock = 1;
    ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("set hdcpRootkey lock error!\n");
    }
    return ret;
}

/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetHdcpRootKeyLock(HI_BOOL *pBLock)
{
    HI_S32    ret = HI_SUCCESS;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pBLock)
    {
        return HI_FAILURE;
    }

    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    *pBLock = DataLock_0.bits.HDCP_RootKey_lock;

    return ret;
}
HI_S32 OTP_V200_SetSTBRootKey(HI_U8 u8Key[16])
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    OTP_V200_INTERNAL_CHECKSUMLOCK_U ChecksumLock;
    OTP_V200_ONE_WAY_0_U unChsumRd;
    HI_U16 u16CRCValue = 0;
    HI_U8 u8CheckSum = 0;
    HI_U8 u8CheckSumCmp = 0;

    if (NULL == u8Key)
    {
        return HI_FAILURE;
    }

    /* check if key locked */
    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.STB_RootKey_lock)
    {
        HI_ERR_OTP("STB root key set ERROR! Is locked!\n");
        return HI_FAILURE;
    }

    /* check if checksum locked */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 == ChecksumLock.bits.locker_STB_RootKey)
    {
        HI_ERR_OTP("Error! Checksum Locked before set key!\n");
        return HI_FAILURE;
    }

    /* write to OTP */
    for (i = 0; i < OTP_STB_ROOT_KEY_LEN; i++)
    {
        HAL_OTP_V200_WriteByte(OTP_V200_INTERNAL_STB_ROOTKEY + i, u8Key[i]);
    }

    /* set checksum */
    u16CRCValue = OTP_CalculateCRC16(u8Key, 16);
    u8CheckSum = u16CRCValue & 0xff;
    ret = HAL_OTP_V200_WriteByte(OTP_V200_INTERNAL_CHECKSUM_STB_ROOT_KEY, u8CheckSum);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Fail to write checksum!\n");
        return ret;
    }

    /* check if checksum_read_disable set or not */
    unChsumRd.u32 = 0;
    unChsumRd.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNEL_ONEWAY_0);
    if(0 == unChsumRd.bits.checksum_read_disable)
    {
        u8CheckSumCmp = HAL_OTP_V200_ReadByte(OTP_V200_INTERNAL_CHECKSUM_STB_ROOT_KEY);
        if (u8CheckSumCmp != u8CheckSum)
        {
            HI_ERR_OTP("Fail to write checksum!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_OTP("checksum_read_disable is set to 1, can not verify chechsum write correct or not.\n");
    }

    /* lock checksum */
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    ChecksumLock.bits.locker_STB_RootKey = 1;
    ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_CHECKSUMLOCK, ChecksumLock.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Fail to write checksum lock!\n");
        return ret;
    }
    ChecksumLock.u32 = 0;
    ChecksumLock.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_CHECKSUMLOCK);
    if( 1 != ChecksumLock.bits.locker_STB_RootKey )
    {
        HI_ERR_OTP("Fail to write checksum Lock!\n");
        return HI_FAILURE;
    }

    return ret;
}



/* Not support Hi3716MV300 */
HI_S32 OTP_V200_GetSTBRootKey(HI_U8 u8Key[16])
{
    HI_U32 i = 0;
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == u8Key)
    {
        return HI_FAILURE;
    }

    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.STB_RootKey_lock)
    {
        HI_ERR_OTP("STB root key get ERROR! Is locked!\n");
        return HI_FAILURE;
    }

    /* read from OTP */
    for (i = 0; i < OTP_STB_ROOT_KEY_LEN; i++)
    {
        u8Key[i] = HAL_OTP_V200_ReadByte(OTP_V200_INTERNAL_STB_ROOTKEY + i);
    }

    return HI_SUCCESS;
}
HI_S32 OTP_V200_LockSTBRootKey(HI_VOID)
{
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_S32 ret;
    
    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    if (1 == DataLock_0.bits.STB_RootKey_lock)
    {
        HI_ERR_OTP("STB root key have already been locked!\n");
        return HI_SUCCESS;
    }

    /* lock */
    DataLock_0.u32 = 0;
    DataLock_0.bits.STB_RootKey_lock = 1;
    ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("STB root key have lock error!\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 OTP_V200_GetSTBRootKeyLockFlag(HI_BOOL *pBLock)
{
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;

    if (NULL == pBLock)
    {
        return HI_FAILURE;
    }

    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    *pBLock = DataLock_0.bits.STB_RootKey_lock;

    return HI_SUCCESS;
}




/* RNG REGISTERS AND STRUCTURES */
#if defined (CHIP_TYPE_hi3716mv300)
#define  REG_RNG_BASE_ADDR                     (0x101e5000)
#elif defined (CHIP_TYPE_hi3716mv310) || defined (CHIP_TYPE_hi3716mv320) || defined (CHIP_TYPE_hi3110ev500)
#define  REG_RNG_BASE_ADDR                     (0x10190000)
#else
#define  REG_RNG_BASE_ADDR                     (0xF8005000)
#endif
#define  REG_RNG_NUMBER_ADDR                (REG_RNG_BASE_ADDR + 0x4)
#define  REG_RNG_STAT_ADDR                    (REG_RNG_BASE_ADDR + 0x8)

static HI_S32 HAL_GetRandomNumber(HI_U32 *pu32Rng)
{
    HI_U32 u32RngStat = 0;
    HI_U32 u32RngCtrl = 0;
    HI_BOOL bConfig = HI_FALSE;
    HI_U32 i = 0;

    if(NULL == pu32Rng)
    {
        HI_ERR_OTP("Invalid params!\n");
        return HI_FAILURE;
    }

    /* select rng source: 2 */
    u32RngCtrl = otp_read_reg(REG_RNG_BASE_ADDR);
    if ((0x01 == (u32RngCtrl & 0x03)) || (0x0 == (u32RngCtrl & 0x03))) /* 0x01 and 0x00 is not used */
    {
        u32RngCtrl &= 0xfffffffc;
        u32RngCtrl |= 0x2;        /* select rng source 0x2, but 0x03 is ok too */
        bConfig = HI_TRUE;
    }

    /* config post_process_depth */
    if (0 == (u32RngCtrl & 0x00009000))
    {
        u32RngCtrl |= 0x00009000;
        bConfig = HI_TRUE;
    }

    /* config post_process_enable andd drop_enable */
    if (0 == (u32RngCtrl & 0x000000a0))
    {
        u32RngCtrl |= 0x000000a0;
        bConfig = HI_TRUE;
    }

    if(bConfig)
    {
        otp_write_reg(REG_RNG_BASE_ADDR,u32RngCtrl);
    }
        
    while(1)
    {
        /* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
        u32RngStat = otp_read_reg(REG_RNG_STAT_ADDR);
        if((u32RngStat & 0x7) > 0)
        {
            break;
        }

        msleep(1);
        i++;
        if(i > 3000)
        {
            HI_ERR_OTP("Get random number time out!\n");
            return HI_FAILURE;
        }
    }

    *pu32Rng = otp_read_reg(REG_RNG_NUMBER_ADDR);
    return HI_SUCCESS;
}

#define OTP_LOT_INFO_1_ADDR    0xF0

HI_S32 OTP_V200_DieID_Check(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    OTP_V200_INTERNAL_DATALOCK_0_U DataLock_0;
    HI_BOOL bLockFlag = HI_FALSE;
    HI_BOOL bWritedFlag = HI_FALSE;
    HI_U8 u8Buffer[8]={0};
    HI_U32 *pu32Data0 = (HI_U32 *)&u8Buffer[0];
    HI_U32 *pu32Data1 = (HI_U32 *)&u8Buffer[4];
    HI_U32 i = 0;

    /*get the DieID lock flag :  lot_info_1_lock 0x12[3]*/
    DataLock_0.u32 = HAL_OTP_V200_Read(OTP_V200_INTERNAL_DATALOCK_0);
    bLockFlag = (HI_BOOL)DataLock_0.bits.rsv_data_2_lock;   //lot_info_1_lock for Hi3716CV200 ~ Hi3798MV100
    if(bLockFlag)
    {
        HI_PRINT("DieID is locked!\n");
        return HI_SUCCESS;
    }

    /*Check DieID is writed or not*/
    for(i=0; i<8; i++)
    {
        if(0 != HAL_OTP_V200_ReadByte(OTP_LOT_INFO_1_ADDR+i))
        {
            HI_PRINT("DieID is writed, but not locked!\n");
            bWritedFlag = HI_TRUE;
            break;
        }
    }

    if(HI_TRUE == bWritedFlag) //DieID is writed, should set the lock flag
    {
        /*set DieID lock flag : lot_info_1_lock 0x12[3]*/
        DataLock_0.u32 = 0;
        DataLock_0.bits.rsv_data_2_lock = 1;            //lot_info_1_lock for Hi3716CV200 ~ Hi3798MV100
        s32Ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
        if(HI_SUCCESS != s32Ret )
        {
            HI_PRINT("Write DieID lock flag failed!\n");
        }
        else
        {
            HI_PRINT("Write DieID lock flag successed!\n");
        }
        return s32Ret;
    }

    /*Get random number*/
    memset(u8Buffer, 0, 8);
    while((0 == *pu32Data0)&&(0 == *pu32Data1))
    {
        s32Ret = HAL_GetRandomNumber(pu32Data0);
        s32Ret |= HAL_GetRandomNumber(pu32Data1);
        if(HI_SUCCESS != s32Ret )
        {
            HI_PRINT("Get random number failed!\n");
            return s32Ret;
        }
        msleep(1);
    }
    HI_PRINT("DieID = 0x%llx\n", *((HI_U64*)u8Buffer));

    /*set DieID value*/
    for(i=0; i<8; i++)
    {
        s32Ret |= HAL_OTP_V200_WriteByte(OTP_LOT_INFO_1_ADDR+i, u8Buffer[i]);
    }
    if(HI_SUCCESS != s32Ret )
    {
        HI_PRINT("Write DieID failed!\n");
        return s32Ret;
    }

    /*set DieID lock flag : lot_info_1_lock 0x12[3]*/
    DataLock_0.u32 = 0;
    DataLock_0.bits.rsv_data_2_lock = 1;            //lot_info_1_lock for Hi3716CV200 ~ Hi3798MV100
    s32Ret = HAL_OTP_V200_Write(OTP_V200_INTERNAL_DATALOCK_0, DataLock_0.u32);
    if(HI_SUCCESS != s32Ret )
    {
        HI_PRINT("Write DieID lock flag failed!\n");
        return s32Ret;
    }

    HI_PRINT("Write DieID lock flag successed!\n");
    return HI_SUCCESS;
}

HI_S32 OTP_V200_Reset(HI_VOID)
{
#ifndef CHIP_TYPE_hi3796mv200
    U_PERI_CRG48 unOTPCrg;
/* Reset request */
    unOTPCrg.u32 = g_pstRegCrg->PERI_CRG48.u32;
    unOTPCrg.bits.otp_srst_req = 1;
    g_pstRegCrg->PERI_CRG48.u32 = unOTPCrg.u32;

    otp_wait(1000); /* 1ms */

/* Cancel Reset */
    unOTPCrg.u32 = g_pstRegCrg->PERI_CRG48.u32;
    unOTPCrg.bits.otp_srst_req = 0;
    g_pstRegCrg->PERI_CRG48.u32 = unOTPCrg.u32;
#endif

    return HI_SUCCESS;
}

EXPORT_SYMBOL(OTP_V200_Reset);
/*--------------------------------------END--------------------------------------*/

