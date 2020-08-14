/******************************************************************************

Copyright (C), 2005-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : cmd_otp.c
Version       : Initial
Author        : Hisilicon hisecurity group
Created       : 2014-09-19
Last Modified :
Description   :  
Function List :
History       :
******************************************************************************/
#ifndef HI_ADVCA_FUNCTION_RELEASE

#include "hi_reg_common.h"
#if defined (CHIP_TYPE_hi3716mv310)
#include "hi_boot_common.h"
#else
#include "hi_common.h"
#endif
#include "command.h"
#include "otp.h"
#include "hi_unf_otp.h"

#ifndef HI_MINIBOOT_SUPPORT
#include "common.h"
#include <linux/string.h>
#endif

#ifndef HI_ADVCA_TYPE_VERIMATRIX
#ifdef OTP_CMD_TEST
HI_S32 OTP_Get_CustomerKey_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 i = 0;
    HI_U8 u32CustomerKey[16];

    HI_UNF_OTP_Init();
    
    memset(u32CustomerKey,0,sizeof(u32CustomerKey));
    HI_UNF_OTP_Get_CustomerKey(u32CustomerKey,16);
    
    HI_INFO_OTP("customer = \n");
    for(i = 0;i<16;i++)
    {
        HI_INFO_OTP("0x%x ",u32CustomerKey[i]);
    }
    HI_INFO_OTP("\n");
    HI_INFO_OTP("get customerkey success\n");
    
    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getcustomerkey,2,1,OTP_Get_CustomerKey_test,"otp_getcustomerkey ","");

HI_S32 OTP_Set_StbPrivData_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 i = 0;
    HI_U8 StbPrivData[16] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10};

    HI_UNF_OTP_Init();     
    for(i = 0; i < 16; i++)
    {
        HI_UNF_OTP_SetStbPrivData(i, StbPrivData[i]);
    }

    HI_INFO_OTP("Set StbPrivData success\n");
    return HI_SUCCESS;
}
U_BOOT_CMD(otp_setstbprivdata,2,1,OTP_Set_StbPrivData_test,"StbPrivData ","");

HI_S32 OTP_Get_StbPrivData_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 i = 0;
    HI_U8 StbPrivData[16];

    HI_INFO_OTP("StbPrivData:\n");

    HI_UNF_OTP_Init();

    for(i = 0;i<16;i++)
    {
        HI_UNF_OTP_GetStbPrivData(i, &(StbPrivData[i]));
        HI_INFO_OTP("0x%x ",StbPrivData[i]);
    }
    HI_INFO_OTP("\n");
    HI_INFO_OTP("get StbPrivData success\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getstbprivdata,2,1,OTP_Get_StbPrivData_test,"otp_getstbprivdata ","");


#if 0
HI_S32 OTP_SetStbPrivData_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U8 i = 0;
    HI_U32 u32TmpValue = 0x12;
    HI_U8 StbPrivData[16];

    HI_UNF_OTP_Init();

    HI_INFO_OTP("Set StbPrivData byte 0 to 0x12 \n");
    
    OTP_SetByte(OTP_V200_STB_PRIV_DATA_ADDR, u32TmpValue);
 
    HI_INFO_OTP("set StbPrivData success\n");
    return HI_SUCCESS;
}
U_BOOT_CMD(setStbPrivData,2,1,OTP_SetStbPrivData_test,"setStbPrivData ","");
#endif

HI_S32 OTP_ReadAll_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    int i;

    HI_UNF_OTP_Init();

    for (i = 0; i < 0x200; i++)
    {
        if (!(i & 0x3))
            HI_INFO_OTP("\n%04x ", i * 4);

        HI_INFO_OTP("%08x ", HI_OTP_Read(i * 4));
    }
    HI_INFO_OTP("\n");
    return HI_SUCCESS;
}
U_BOOT_CMD(otpreadall,1,1,OTP_ReadAll_test,"read otp ,for example otpreadall ","");

HI_S32 OTP_WriteAddress_test(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    HI_U32  addr = 0;
    HI_U32  value = 0;

    HI_UNF_OTP_Init();

    addr = simple_strtoul(argv[1], NULL, 16);
    value = simple_strtoul(argv[2], NULL, 16);
    OTP_SetByte( addr, value);
    return HI_SUCCESS;
}
U_BOOT_CMD(otpwrite,3,1,OTP_WriteAddress_test,"write otp ,for example otpwrite adddress value ","");

HI_S32 OTP_BurnToSecureChipset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	HI_S32 ret = HI_SUCCESS;
	
    ret = HI_UNF_OTP_BurnToSecureChipset();
    if(HI_SUCCESS != ret)
    {
		HI_INFO_OTP("Burn to secure chipset failed, ret: 0x%x\n", ret);
		return HI_FAILURE;
    }

	HI_INFO_OTP("Burn to secure chipset success.\n");

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_burntoecurechipset, 3, 1, OTP_BurnToSecureChipset,"Burn to secure chipset, please be careful !!!","");

#if 0
HI_S32 OTP_GetIDWordLockFlag(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	HI_S32 ret = HI_SUCCESS;
	HI_BOOL bFlag = HI_FALSE;

    ret = HI_UNF_OTP_GetIDWordLockFlag(&bFlag);
    if(HI_SUCCESS != ret)
    {
		HI_INFO_OTP("Get id_word failed, ret: 0x%x\n", ret);
		return HI_FAILURE;
    }

    HI_INFO_OTP("ID_WORD lock flag; 0x%x\n", bFlag);

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_getidwordlockglag, 3, 1, OTP_GetIDWordLockFlag,"Get ID_WORD lock flag","");

HI_S32 OTP_LockIDWord(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	HI_S32 ret = HI_SUCCESS;

	ret = HI_UNF_OTP_LockIDWord();
    if(HI_SUCCESS != ret)
    {
		HI_INFO_OTP("Lock ID_WORD failed, ret: 0x%x\n", ret);
		return HI_FAILURE;
    }

	HI_INFO_OTP("Lock ID_WORD success, ret: 0x%x\n", ret);

    return HI_SUCCESS;
}
U_BOOT_CMD(otp_lockidword, 3, 1, OTP_LockIDWord,"Locd ID_WORD, please be careful !!!","");
#endif

#endif /* #ifdef OTP_CMD_TEST */
#endif /* #ifndef HI_ADVCA_TYPE_VERIMATRIX */

#endif /* #ifndef HI_ADVCA_FUNCTION_RELEASE */

