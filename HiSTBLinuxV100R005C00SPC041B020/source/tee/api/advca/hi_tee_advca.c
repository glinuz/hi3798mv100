/******************************************************************************

Copyright (C), 2004-2013, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_tee_advca.c
Version       : Initial
Author        : Hisilicon hisecurity team
Created       : 2013-12-28
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#include "hi_tee_advca.h"
#include "sre_syscalls_chip.h"
#include "tee_drv_advca_ioctl.h"

#define TZ_ERR_ADVCA(fmt...)    uart_printf_func(fmt)
#define TZ_INFO_ADVCA(fmt...)   uart_printf_func(fmt)

HI_S32 HI_TEE_ADVCA_Init()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_INIT, HI_NULL);
}

HI_S32 HI_TEE_ADVCA_DeInit()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_DEINIT, HI_NULL);
}

HI_S32 HI_TEE_ADVCA_GetOtpFuse(HI_TEE_ADVCA_OTP_FUSE_E enOtpFuse, HI_TEE_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_OTP_ATTR_S stOtpAttr;

    if (enOtpFuse <= HI_TEE_ADVCA_OTP_NULL || enOtpFuse >= HI_TEE_ADVCA_OTP_FUSE_BUTT)
    {
        TZ_ERR_ADVCA("Invalid parameter, enOtpFuse = %d\n", enOtpFuse);
        return HI_FAILURE;
    }

    if (pstOtpFuseAttr == NULL)
    {
        TZ_ERR_ADVCA("Invalid parameter, pstOtpFuseAttr = NULL\n");
        return HI_FAILURE;
    }

    memset(&stOtpAttr, 0x0, sizeof (stOtpAttr));
    stOtpAttr.enOtpFuseId = enOtpFuse;
    memcpy(&stOtpAttr.stOtpAttr, pstOtpFuseAttr, sizeof (HI_TEE_ADVCA_OTP_ATTR_S));

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_GET_OTP_FUSE, &stOtpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_GetOtpFuse failed:%d\n", s32Ret);
        return s32Ret;
    }

    memcpy(pstOtpFuseAttr, &stOtpAttr.stOtpAttr, sizeof(HI_TEE_ADVCA_OTP_ATTR_S));

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_SetOtpFuse(HI_TEE_ADVCA_OTP_FUSE_E enOtpFuse, HI_TEE_ADVCA_OTP_ATTR_S *pstOtpFuseAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_OTP_ATTR_S stOtpAttr;

    if (enOtpFuse >= HI_TEE_ADVCA_OTP_FUSE_BUTT)
    {
        TZ_ERR_ADVCA("Invalid parameter, enOtpFuse = %d\n", enOtpFuse);
        return HI_FAILURE;
    }

    if (pstOtpFuseAttr == NULL)
    {
        TZ_ERR_ADVCA("Invalid parameter, pstOtpFuseAttr = NULL\n");
        return HI_FAILURE;
    }

    memset(&stOtpAttr, 0x0, sizeof (TEE_ADVCA_OTP_ATTR_S));
    stOtpAttr.enOtpFuseId = enOtpFuse;
    memcpy(&stOtpAttr.stOtpAttr, pstOtpFuseAttr, sizeof(HI_TEE_ADVCA_OTP_ATTR_S));

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_SET_OTP_FUSE, &stOtpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_SetOtpFuse failed:%d\n", s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_SetKeyLadderAttr(HI_TEE_ADVCA_KEYLADDER_TYPE_E enKeyLadderType, HI_TEE_ADVCA_KEYLADDER_ATTR_S *pstKeyladderAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_KEYLADDER_ATTR_S stKeyladderAttr;

    if (enKeyLadderType >= HI_TEE_ADVCA_KEYLADDER_TYPE_BUTT)
    {
        TZ_ERR_ADVCA("Invalid parameter, enKeyLadderType = %d\n", enKeyLadderType);
        return HI_FAILURE;
    }

    if (pstKeyladderAttr == HI_NULL)
    {
        TZ_ERR_ADVCA("Invalid parameter, pstKeyladderAttr = NULL\n");
        return HI_FAILURE;
    }

    memset(&stKeyladderAttr, 0, sizeof(stKeyladderAttr));
    stKeyladderAttr.enKeyLadderType = enKeyLadderType;
    memcpy(&stKeyladderAttr.stKeyladderAttr, pstKeyladderAttr, sizeof(HI_TEE_ADVCA_KEYLADDER_ATTR_S));
    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_SET_KEYLADDER_ATTR, &stKeyladderAttr);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_SetKeyLadderAttr failed:%d\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_DCASClose()
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_DCAS_CLOSE, HI_NULL);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_DCASClose failed:%d\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_DCASOpen(HI_TEE_ADVCA_ALG_TYPE_E enAlg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_TEE_ADVCA_ALG_TYPE_E enAlgType;

    enAlgType = enAlg;

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_DCAS_OPEN, &enAlgType);
    if (HI_SUCCESS  != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_DCASOpen failed\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_GetDCASChipId(HI_U32 *pu32MSBID, HI_U32 *pu32LSBID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_DCAS_CHIPID_S stDcasChipID;

    if ((pu32MSBID == HI_NULL) || (pu32LSBID == HI_NULL))
    {
        TZ_ERR_ADVCA("Invalid parameter\n");
        return HI_FAILURE;
    }
    memset(&stDcasChipID, 0x0, sizeof (stDcasChipID));

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_DCAS_GET_CHIPID, &stDcasChipID);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_GetDCASChipId failed\n", s32Ret);
        return s32Ret;
    }

    *pu32MSBID = stDcasChipID.u32MSBID;
    *pu32LSBID = stDcasChipID.u32LSBID;

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_SetDCASSessionKey(HI_TEE_ADVCA_DCAS_KEYLADDER_LEV_E enDCASLevel, HI_U8 au8Key[16], HI_U8 au8Output[16])
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_DCAS_SESSIONKEY_S stDcasSessionKey;

    if ((HI_NULL == au8Key) || (HI_NULL == au8Output))
    {        
        TZ_ERR_ADVCA("Dcas param is invalid!\n");
        return HI_FAILURE;
    }

    if(enDCASLevel >= HI_TEE_ADVCA_DCAS_KEYLADDER_BUTT)
    {
        TZ_ERR_ADVCA("enDCASLevel >= HI_UNF_ADVCA_DCAS_KEYLADDER_BUTT, invalid.\n");        
        return HI_FAILURE;
    }

    memset(&stDcasSessionKey, 0x0, sizeof(stDcasSessionKey));
    stDcasSessionKey.level = enDCASLevel;
    memcpy(stDcasSessionKey.pDin, au8Key, 16);

    s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_DCAS_SET_SESSIONKEY, &stDcasSessionKey);

    memcpy(au8Output, stDcasSessionKey.pDout, 16);
    if (HI_SUCCESS != s32Ret)
    {
        TZ_ERR_ADVCA("HI_TEE_ADVCA_SetDCASSessionKey failed:%d\n", s32Ret);
        return s32Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_CalculteAES_CMAC(HI_U8 *buffer, HI_U32 Length, HI_U8 Key[16], HI_U8 MAC[16])
{
    TZ_ERR_ADVCA("NOT supported\n");
    return HI_FAILURE;
}

HI_S32 HI_TEE_ADVCA_CaVendorOperation(HI_TEE_ADVCA_VENDORID_E enCaVendor, HI_TEE_ADVCA_CA_VENDOR_OPT_S *pstCaVendorOpt)
{
    HI_S32 s32Ret = HI_SUCCESS;
    TEE_ADVCA_VENDOR_OPT_S enCaVendorOpt;

    if (enCaVendor >= HI_TEE_ADVCA_VENDORIDE_BUTT)
    {
        TZ_ERR_ADVCA("Invalid parameter, enCaVendor = %d\n", enCaVendor);
        return HI_FAILURE;
    }

    if (HI_NULL == pstCaVendorOpt)
    {
        TZ_ERR_ADVCA("Invalid parameter, pstCaVendorOpt = NULL\n");
        return HI_FAILURE;
    }

    memset(&enCaVendorOpt, 0x0, sizeof (enCaVendorOpt));
    enCaVendorOpt.enVendorID = enCaVendor;
    memcpy(&(enCaVendorOpt.stVendorOpt), pstCaVendorOpt, sizeof (HI_TEE_ADVCA_CA_VENDOR_OPT_S));

    if (HI_TEE_ADVCA_VERIMATRIX == enCaVendor)
    {
        s32Ret = __TEE_ADVCA_Ioctl(CMD_ADVCA_CA_VENDOR_OPT, &enCaVendorOpt);
    }
    else
    {
        TZ_ERR_ADVCA("Not support vendor type:%d\n", enCaVendor);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_TEE_ADVCA_VMXMiscOpen()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_OPEN, HI_NULL);
}

HI_S32 HI_TEE_ADVCA_VMXMiscClose()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_CLOSE, HI_NULL);
}

HI_S32 HI_TEE_ADVCA_SetVMXMiscSessionKey(HI_TEE_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    TEE_ADVCA_CRYPTPM_S stCrypt;

    if (HI_TEE_ADVCA_KEYLADDER_BUTT <= enStage || HI_TEE_ADVCA_KEYLADDER_LEV1 > enStage)
    {
        TZ_ERR_ADVCA("Keyladder level is invalid:%d\n", enStage);
        return HI_FAILURE;
    }

    if (HI_NULL == pu8Key)
    {
        TZ_ERR_ADVCA("The session key to set is null\n");
        return HI_FAILURE;
    }

    if (u32KeyLen != 16)
    {
        TZ_ERR_ADVCA("The key length is invalid:%d\n", u32KeyLen);
        return HI_FAILURE;
    }

    memset(&stCrypt, 0x0, sizeof (stCrypt));
    stCrypt.ladder = enStage;
    stCrypt.u32KeyLen = u32KeyLen;
    memcpy(stCrypt.pDin, pu8Key, u32KeyLen);

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_SET_SESSIONKEY, &stCrypt);
}

HI_S32 HI_TEE_ADVCA_SetVMXMiscAlg(HI_TEE_ADVCA_ALG_TYPE_E enType)
{
    if (HI_TEE_ADVCA_ALG_TYPE_TDES > enType || HI_TEE_ADVCA_ALG_TYPE_BUTT <= enType)
    {
        TZ_ERR_ADVCA("ALG type is invalid:%d\n", enType);
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_SET_ALG, &enType);
}

HI_S32 HI_TEE_ADVCA_SetVMXMiscDscMode(HI_TEE_ADVCA_SP_DSC_MODE_E enDscMode)
{
    if (HI_TEE_ADVCA_SP_DSC_MODE_PAYLOAD_CSA2 > enDscMode || HI_TEE_ADVCA_SP_DSC_MODE_BUTT <= enDscMode)
    {
        TZ_ERR_ADVCA("DSC mode is invalid:%d\n", enDscMode);
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_SET_DSCMODE, &enDscMode);
}

HI_S32 HI_TEE_ADVCA_GetVMXMiscKlLevel(HI_TEE_ADVCA_KEYLADDER_LEV_E *penLevel)
{
    if (HI_NULL == penLevel)
    {
        TZ_ERR_ADVCA("penLevel is null\n");
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_MISC_GET_LEVEL, penLevel);
}

HI_S32 HI_TEE_ADVCA_VMXSPOpen()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_OPEN, HI_NULL);
}

HI_S32 HI_TEE_ADVCA_VMXSPClose()
{
    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_CLOSE, HI_NULL);
}


HI_S32 HI_TEE_ADVCA_SetVMXSPSessionKey(HI_TEE_ADVCA_KEYLADDER_LEV_E enStage, HI_U8 *pu8Key, HI_U32 u32KeyLen)
{
    TEE_ADVCA_CRYPTPM_S stCrypt;

    if (HI_TEE_ADVCA_KEYLADDER_BUTT <= enStage || HI_TEE_ADVCA_KEYLADDER_LEV1 > enStage)
    {
        TZ_ERR_ADVCA("Keyladder level is invalid:%d\n", enStage);
        return HI_FAILURE;
    }

    if (HI_NULL == pu8Key)
    {
        TZ_ERR_ADVCA("The session key to set is null\n");
        return HI_FAILURE;
    }

    if (u32KeyLen != 16)
    {
        TZ_ERR_ADVCA("The key length is invalid:%d\n", u32KeyLen);
        return HI_FAILURE;
    }

    memset(&stCrypt, 0x0, sizeof (stCrypt));
    stCrypt.ladder = enStage;
    stCrypt.u32KeyLen = u32KeyLen;
    memcpy(stCrypt.pDin, pu8Key, u32KeyLen);

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_SET_SESSIONKEY, &stCrypt);
}

HI_S32 HI_TEE_ADVCA_SetVMXSPAlg(HI_TEE_ADVCA_ALG_TYPE_E enType)
{
    if (HI_TEE_ADVCA_ALG_TYPE_TDES > enType || HI_TEE_ADVCA_ALG_TYPE_BUTT <= enType)
    {
        TZ_ERR_ADVCA("ALG type is invalid:%d\n", enType);
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_SET_ALG, &enType);
}

HI_S32 HI_TEE_ADVCA_SetVMXSPDscMode(HI_TEE_ADVCA_SP_DSC_MODE_E enDscMode)
{
    if (HI_TEE_ADVCA_SP_DSC_MODE_PAYLOAD_CSA2 > enDscMode || HI_TEE_ADVCA_SP_DSC_MODE_BUTT <= enDscMode)
    {
        TZ_ERR_ADVCA("DSC mode is invalid:%d\n", enDscMode);
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_SET_DSCMODE, &enDscMode);
}

HI_S32 HI_TEE_ADVCA_GetVMXSPKlLevel(HI_TEE_ADVCA_KEYLADDER_LEV_E *penLevel)
{
    if (HI_NULL == penLevel)
    {
        TZ_ERR_ADVCA("penLevel is null\n");
        return HI_FAILURE;
    }

    return __TEE_ADVCA_Ioctl(CMD_ADVCA_SP_GET_LEVEL, penLevel);
}
