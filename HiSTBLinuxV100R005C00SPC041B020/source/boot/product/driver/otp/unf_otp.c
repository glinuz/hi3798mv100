/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : unf_otp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-16
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "hi_type.h"
#include "exports.h"
#include "otp.h"
#include "hi_debug.h"
#include "hi_unf_otp.h"

extern HI_BOOL g_bOTPInit;

#define CHECK_OTP_Init()\
do{\
    if (HI_FALSE == g_bOTPInit )\
    {\
        HI_ERR_OTP("OTP is not init.\n");\
        return HI_FAILURE;\
    }\
}while(0)

HI_S32 HI_UNF_OTP_Init(HI_VOID)
{
    (HI_VOID)OTP_Init();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_OTP_DeInit(HI_VOID)
{
    (HI_VOID)OTP_DeInit();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_OTP_Get_CustomerKey(HI_U8 *pKey, HI_U32 u32KeyLen)
{
    HI_S32 Ret = HI_SUCCESS;
    OTP_CUSTOMER_KEY_S *pstOtpCustomerKey = HI_NULL;

    if (HI_NULL == pKey)
    {
        HI_ERR_OTP("null ptr for otp read\n");
        return HI_FAILURE;
    }

    if (OTP_CUSTOMER_KEY_LEN !=  u32KeyLen)
    {
       HI_ERR_OTP("invalid customer key length: 0x%x, should be 0x%x\n", u32KeyLen, OTP_CUSTOMER_KEY_LEN);
       return HI_FAILURE;
    }

    (HI_VOID)HI_UNF_OTP_Init();

    CHECK_OTP_Init();
    pstOtpCustomerKey = (OTP_CUSTOMER_KEY_S*)pKey;

    Ret = OTP_V200_getCustomerKey(pstOtpCustomerKey);

    return Ret;
}


HI_S32 HI_UNF_OTP_SetStbPrivData(HI_U32 u32Offset, HI_U8 u8Data)
{
    OTP_STB_PRIV_DATA_S OtpStbPrivData;
    HI_S32 Ret;

    (HI_VOID)HI_UNF_OTP_Init();

    OtpStbPrivData.u32Offset = u32Offset;
    OtpStbPrivData.u8Data = u8Data;

    Ret = OTP_V200_Set_StbPrivData(&OtpStbPrivData);
    return Ret;

}

HI_S32 HI_UNF_OTP_GetStbPrivData(HI_U32 u32Offset, HI_U8 *pu8Data)
{
    OTP_STB_PRIV_DATA_S OtpStbPrivData;
    HI_S32 Ret = 0;

    (HI_VOID)HI_UNF_OTP_Init();

    OtpStbPrivData.u32Offset = u32Offset;

    Ret = OTP_V200_Get_StbPrivData(&OtpStbPrivData);
    *pu8Data = OtpStbPrivData.u8Data;

    return Ret;
}

HI_S32 HI_UNF_OTP_GetIDWordLockFlag(HI_BOOL *pbLockFlag)
{
    HI_U32 value = 0;

    if(NULL == pbLockFlag)
    {
        HI_ERR_OTP("Null ptr for read ID Word Lock Flag\n");
        return HI_FAILURE;
    }

    (HI_VOID)HI_UNF_OTP_Init();

    value = HI_OTP_Read(OTP_INTERNAL_DATALOCK_0); //aligned by 4 bytes
    if(1 == ( (value >> 10) & 0x1 ) )           //0X11[2]
    {
        *pbLockFlag = HI_TRUE;
    }
    else
    {
        *pbLockFlag = HI_FALSE;
    }

    return HI_SUCCESS;
}

/*
    Lock ID_WORD_lock: 0x11[2]
    Lock secure_chip_flag_lock¡êo0xc[0]
*/
HI_S32 HI_UNF_OTP_LockIDWord(HI_VOID)
{
    HI_S32 ret = 0;
    HI_U32 u32Value = 0;
    HI_U8 u8Value;

    (HI_VOID)HI_UNF_OTP_Init();

    /* ID_WORD_lock */
    u32Value = HI_OTP_Read(OTP_INTERNAL_DATALOCK_0); //aligned by 4 bytes

    /* Check ID_WORD locked or not */
    u8Value = (u32Value >> 8) & 0x04;              //0X11[2]
    if(0 == u8Value)
    {
        /* Lock ID_WORD */
        u8Value = ((u32Value >> 8) | 0x04) & 0xff;     //0X11[2]
        ret = HI_OTP_WriteByte(OTP_INTERNAL_DATALOCK_0 + 1, u8Value);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_OTP("Write id word failed!\n");
            return HI_FAILURE;
        }
    }

    /* secure_chip_flag_lock */
    u32Value = HI_OTP_Read(OTP_INTERNAL_PVLOCK_1); //aligned by 4 bytes

    /* Check secure_chip_flag locked or not */
    u8Value = (u32Value & 0x1);                     //0xc[0]
    if(0 == u8Value)
    {
        /* Lock secure_chip_flag */
        u8Value = (u32Value | 0x1) & 0xff;          //0xc[0]
        ret = HI_OTP_WriteByte(OTP_INTERNAL_PVLOCK_1, u8Value);
        if(HI_SUCCESS != ret)
        {
            HI_ERR_OTP("Write secure_chip_flag_lock failed!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

/*
    echo write 0x04 0x01 > /proc/msp/otp //secure_chip_flag
    echo write 0x19 0x08 > /proc/msp/otp  //right_ctrl_en
    echo write 0xa8 0x53 > /proc/msp/otp  //Advca ID_WORD
    echo write 0xa9 0xe9 > /proc/msp/otp  //Advca ID_WORD
    echo write 0xaa 0xdb > /proc/msp/otp  //Advca ID_WORD
    echo write 0xab 0x6e > /proc/msp/otp  //Advca ID_WORD
    echo write 0xad 0x81 > /proc/msp/otp  //scs_en_bak:            STBM will write it at last
    echo write 0xae 0x42 > /proc/msp/otp  //jtag_mode_bak:         STBM will write it at last
    echo write 0xaf 0xff > /proc/msp/otp  //right_ctrl_en_bak:     STBM will write it at last
    echo write 0x1c 0x11 > /proc/msp/otp  //self_boot_disable_bak: STBM will write it at last
*/
HI_S32 HI_UNF_OTP_BurnToSecureChipset(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_BOOL bIsIDWordLocked = HI_FALSE;

    (HI_VOID)HI_UNF_OTP_Init();

    ret = HI_UNF_OTP_GetIDWordLockFlag(&bIsIDWordLocked);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Get IDWord Lock Flag error!\n");
        return HI_FAILURE;
    }

    u32Value = HI_OTP_Read(OTP_ADVCA_ID_WORD_ADDR);

    if ((HI_TRUE == bIsIDWordLocked) && (ADVCA_ID_WORD == u32Value))
    {
        return HI_SUCCESS;
    }

    if ((HI_TRUE != bIsIDWordLocked) && (ADVCA_ID_WORD == u32Value))
    {
        return HI_UNF_OTP_LockIDWord();
    }

    if ((HI_TRUE == bIsIDWordLocked) && (ADVCA_ID_WORD != u32Value))
    {
        HI_ERR_OTP("ID_WORD has already been loacked to noaml chipset\n");
        return HI_FAILURE;
    }

    /* echo write 0x04 0x01 > /proc/msp/otp //secure_chip_flag */
    ret = HI_OTP_WriteByte(OTP_INTERNAL_PV_1, 0x01);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set secure_chip_flag failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /* echo write 0x19 0x08 > /proc/msp/otp  //right_ctrl_en */
    ret = HI_OTP_WriteByte(OTP_RIGHT_CTRL_EN_ADDR, 0x08);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set right_ctrl_en failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /*
        echo write 0xa8 0x53 > /proc/msp/otp  //Advca ID_WORD
        echo write 0xa9 0xe9 > /proc/msp/otp  //Advca ID_WORD
        echo write 0xaa 0xdb > /proc/msp/otp  //Advca ID_WORD
        echo write 0xab 0x6e > /proc/msp/otp  //Advca ID_WORD
    */
    ret = HI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR, 0x53);
    ret |= HI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 1, 0xe9);
    ret |= HI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 2, 0xdb);
    ret |= HI_OTP_WriteByte(OTP_ADVCA_ID_WORD_ADDR + 3, 0x6e);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set advca ID_WORD failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /* echo write 0xad 0x81 > /proc/msp/otp  //scs_en_bak */
    ret = HI_OTP_WriteByte(OTP_SCS_EN_BAK_ADDR, 0x81);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set scs_en_bak failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /* echo write 0xae 0x42 > /proc/msp/otp  //jtag_mode_bak */
    ret = HI_OTP_WriteByte(OTP_JTAG_MODE_BAK_ADDR, 0x42);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set jtag_mode_bak failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /* echo write 0xaf 0xff > /proc/msp/otp  //right_ctrl_en_bak */
    ret = HI_OTP_WriteByte(OTP_RIGHT_CTRL_EN_BAK_ADDR, 0xff);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set right_ctrl_en_bak failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    /* echo write 0x1c 0x11 > /proc/msp/otp  //self_boot_disable_bak */
    ret = HI_OTP_WriteByte(OTP_SELF_BOOT_DIABLE_BAK_ADDR, 0x11);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Set self_boot_disable_bak failed, ret: 0x%x!\n", ret);
        return HI_FAILURE;
    }

    return HI_UNF_OTP_LockIDWord();
}

HI_S32 HI_UNF_OTP_BurnToNormalChipset(HI_VOID)
{
    HI_S32 s32Ret   = HI_SUCCESS;
    HI_U32 u32Value = 0;
    HI_BOOL bIsIDWordLocked = HI_FALSE;

    (HI_VOID)HI_UNF_OTP_Init();

    u32Value = HI_OTP_Read(OTP_ADVCA_ID_WORD_ADDR);
    if (ADVCA_ID_WORD == u32Value)
    {
        HI_ERR_OTP("It's secure chipset already, can not burn to normal!\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_OTP_GetIDWordLockFlag(&bIsIDWordLocked);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_OTP("Get IDWord Lock Flag error!\n");
        return HI_FAILURE;
    }

    if(HI_TRUE == bIsIDWordLocked)
    {
        return HI_SUCCESS;
    }

    return HI_UNF_OTP_LockIDWord();
}

/* Not an UNF interface */
HI_S32 HI_OTP_WriteByte(HI_U32 addr, HI_U8 u8data)
{
    (HI_VOID)HI_UNF_OTP_Init();

    return OTP_SetByte(addr, u8data);
}

/* Not an UNF interface */
HI_U32 HI_OTP_Read(HI_U32 addr)
{
    (HI_VOID)HI_UNF_OTP_Init();

    return OTP_Read(addr);
}

HI_S32 HI_OTP_Write(HI_U32 addr, HI_U32 u32Data)
{
    (HI_VOID)HI_UNF_OTP_Init();

    return OTP_Write(addr, u32Data);
}

HI_U8 HI_OTP_ReadByte(HI_U32 addr)
{
    (HI_VOID)HI_UNF_OTP_Init();

    return OTP_ReadByte(addr);
}

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
HI_S32 HI_OTP_LockIdWord(HI_VOID)
{
    HI_S32 ret = 0;
    HI_BOOL bLockFlag = HI_FALSE;

    ret = HI_UNF_OTP_GetIDWordLockFlag(&bLockFlag);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_OTP("Get ID_WORD lock flag failed! Ret = %x\n");
        return HI_FAILURE;
    }

    if (bLockFlag == HI_TRUE)
    {
        HI_PRINT("ID_WORD have already been locked\n");
        return HI_SUCCESS;
    }

#ifdef HI_ADVCA_SUPPORT
    /* Check and burn to secure chipset */
    ret = HI_UNF_OTP_BurnToSecureChipset();
    if (HI_SUCCESS != ret)
    {
        HI_PRINT("Burn to secure chipset failed! Ret = %x\n", ret);
        return HI_FAILURE;
    }
    HI_PRINT("Burn to secure chipset success!\n");

#else
    ret = HI_UNF_OTP_BurnToNormalChipset();
    if (HI_SUCCESS != ret)
    {
        HI_PRINT("Burn to nomal chipset failed! Ret = %x\n", ret);
        return HI_FAILURE;
    }
    HI_PRINT("Burn to nomal chipset success!\n");
#endif

    return HI_SUCCESS;
}
#endif

HI_U32 HI_UNF_OTP_Read(HI_U32 addr)
{
    return HI_OTP_Read(addr);
}

HI_U8 HI_UNF_OTP_ReadByte(HI_U32 addr)
{
    return HI_OTP_ReadByte(addr);
}

HI_S32 HI_UNF_OTP_Write(HI_U32 addr, HI_U32 u32Data)
{
    return HI_OTP_Write(addr, u32Data);
}

HI_S32 HI_UNF_OTP_WriteByte(HI_U32 addr, HI_U8 u8data)
{
    return HI_OTP_WriteByte(addr, u8data);
}


