/******************************************************************************
Copyright (C), 2001-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : vmx_ssd.c
Version       : Initial Draft
Author        : Hisilicon hisecurity team
Created       : 2014/09/23
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmx_ssd.h"
#include "vmx_auth.h"
#include "hi_flash.h"
#include "com_debug.h"
#include "com_osal.h"

#define VMX_SSD_HEADER_LEN      (16)
#define VMX_SSD_SIG_LEN         (256)

/** SSD CA Vendor definition*/
typedef enum tagVMX_SSD_TYPE_E
{
    LOADER_SSD_TYPE_NONE = 0,
    LOADER_SSD_TYPE_VMX_ADVANCE,
    LOADER_SSD_TYPE_VMX_ULTRA,
    LOADER_SSD_TYPE_BUTT

} VMX_SSD_TYPE_E;

typedef struct tagVMX_PARTITITON_INFO
{
    HI_CHAR*        pcPartitionName;
    HI_U64          u64FlashStartAddr;
    VMX_SSD_TYPE_E  enSSDType;

} VMX_PARTITITON_INFO_S;


VMX_PARTITITON_INFO_S s_astPartitionInfo[] =
{
    {"boot",            0, LOADER_SSD_TYPE_VMX_ADVANCE},        /** Linux */
    {"fastboot",        0, LOADER_SSD_TYPE_VMX_ADVANCE},        /** Android */
    {"bootargs",        0, LOADER_SSD_TYPE_VMX_ADVANCE},
    {"stbinfo",         0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"deviceinfo",      0, LOADER_SSD_TYPE_NONE},
    {"softwareinfo",    0, LOADER_SSD_TYPE_NONE},
    {"loaderdb",        0, LOADER_SSD_TYPE_NONE},
    {"loaderdbbk",      0, LOADER_SSD_TYPE_NONE},
    {"loader",          0, LOADER_SSD_TYPE_VMX_ADVANCE},
    {"loaderbak",       0, LOADER_SSD_TYPE_VMX_ADVANCE},
    {"baseparam",       0, LOADER_SSD_TYPE_NONE},
    {"pqparam",         0, LOADER_SSD_TYPE_NONE},
    {"logo",            0, LOADER_SSD_TYPE_NONE},
    {"logobak",         0, LOADER_SSD_TYPE_NONE},
    {"fastplay",        0, LOADER_SSD_TYPE_NONE},
    {"fastplaybak",     0, LOADER_SSD_TYPE_NONE},
    {"kernel",          0, LOADER_SSD_TYPE_VMX_ADVANCE},        /** Android */
    {"misc",            0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"cadata",          0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"shatable",        0, LOADER_SSD_TYPE_VMX_ADVANCE},        /** Android */
    {"system",          0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"userdata",        0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"cache",           0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"private",         0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"sdcard",          0, LOADER_SSD_TYPE_NONE},               /** Android */
    {"trustedcore",     0, LOADER_SSD_TYPE_NONE},               /** Linux */
    {"rootfs",          0, LOADER_SSD_TYPE_VMX_ADVANCE}         /** Linux */
};

HI_VOID vmxSSDPrintPartitionInfo(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 u32ImageCount = 0;

    u32ImageCount = sizeof(s_astPartitionInfo) / sizeof(VMX_PARTITITON_INFO_S);

    for (i = 0; i < u32ImageCount; i++)
    {
        COM_ERROR("PartitionName: %s SSDType: %u FlashStartAddr:%llu\n",
                  s_astPartitionInfo[i].pcPartitionName,
                  s_astPartitionInfo[i].enSSDType,
                  s_astPartitionInfo[i].u64FlashStartAddr);
    }

    return;
}

HI_S32 vmxSSDGetPartitionStartAddr(HI_CHAR* pcPartitionName, HI_U64* pu64FlashStartAddr)
{
    HI_Flash_InterInfo_S stFlashInfo;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    hFlashHandle = HI_Flash_OpenByName(pcPartitionName);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        COM_PrintFuncErr(HI_Flash_OpenByName, COM_ERR_NOEXIST);
        return COM_ERR_NOEXIST;
    }

    COM_MemSet(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        HI_Flash_Close(hFlashHandle);
        return s32Ret;
    }

    HI_Flash_Close(hFlashHandle);
    hFlashHandle = HI_INVALID_HANDLE;

    *pu64FlashStartAddr = stFlashInfo.OpenAddr;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

VMX_PARTITITON_INFO_S* vmxSSDGetPartitionInfo(HI_U64 u64FlashStartAddr)
{
    HI_U32 i = 0;
    HI_U32 u32ImageCount = 0;

    COM_DBG_FuncEnter();

    u32ImageCount = sizeof(s_astPartitionInfo) / sizeof(VMX_PARTITITON_INFO_S);

    for (i = 0; i < u32ImageCount; i++)
    {
        if (u64FlashStartAddr == s_astPartitionInfo[i].u64FlashStartAddr)
        {
            COM_DBG_FuncExit();
            return &s_astPartitionInfo[i];
        }
    }

    COM_DBG_FuncExit();
    return HI_NULL_PTR;
}

HI_S32 vmxSSDGetPartitionInit(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 u32ImageCount = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    u32ImageCount = sizeof(s_astPartitionInfo) / sizeof(VMX_PARTITITON_INFO_S);

    for (i = 0; i < u32ImageCount; i++)
    {
        s32Ret = vmxSSDGetPartitionStartAddr(s_astPartitionInfo[i].pcPartitionName, &s_astPartitionInfo[i].u64FlashStartAddr);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(vmxSSDGetPartitionStartAddr, s32Ret);
            continue;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 vmxSSDGetPartitionDeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 WMX_SSD_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = vmxSSDGetPartitionInit();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(vmxSSDGetPartitionInit, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 WMX_SSD_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK(vmxSSDGetPartitionDeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 WMX_SSD_WholeImageVerfy
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{
    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    COM_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);

    *ppu8DataAddr = pu8ImageData;
    *pu32DataSize = u32ImageSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 WMX_SSD_PartitionImageVerfy
(
    HI_U8*      pu8PartitionName,
    HI_U64      u64FlashStartAddr,
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize,
    HI_U32*     pu32ImageVersion,
    HI_BOOL*    pbNeedUpgrade
)
{
    HI_U32 u32Size = 0;
    HI_UCHAR ucErrorCode = 0;
    HI_U8* pu8Data = HI_NULL_PTR;
    VMX_PARTITITON_INFO_S* pstPartition = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    COM_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32ImageVersion);
    COM_CHECK_PARAM(HI_NULL_PTR == pbNeedUpgrade);

    pstPartition = vmxSSDGetPartitionInfo(u64FlashStartAddr);

    if ((HI_NULL_PTR == pstPartition)
        || (LOADER_SSD_TYPE_NONE == pstPartition->enSSDType)
        || (LOADER_SSD_TYPE_BUTT == pstPartition->enSSDType))
    {
        *ppu8DataAddr = pu8ImageData;
        *pu32DataSize = u32ImageSize;
        *pu32ImageVersion = 0;
        *pbNeedUpgrade = HI_TRUE;

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    u32Size = u32ImageSize - (VMX_SSD_HEADER_LEN + VMX_SSD_SIG_LEN);
    pu8Data = COM_MemMalloc(u32Size);
    if (HI_NULL_PTR == pu8Data)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    memset(pu8Data, 0x00, u32Size);
    s32Ret = verifySignature(pu8ImageData + VMX_SSD_HEADER_LEN,
                             pu8ImageData + VMX_SSD_HEADER_LEN + VMX_SSD_SIG_LEN,
                             pu8Data,
                             u32Size,
                             u32Size,
                             1,
                             &ucErrorCode);

    COM_MemFreeSetNull(pu8Data);

    if (0 == s32Ret)
    {
        if (1 == ucErrorCode)
        {
            COM_PrintFuncErr(verifySignature, s32Ret);
            return HI_FAILURE;
        }
        else if (2 == ucErrorCode)
        {
            *ppu8DataAddr = pu8ImageData;
            *pu32DataSize = u32ImageSize;
            *pu32ImageVersion = 0;
            *pbNeedUpgrade = HI_TRUE;

            COM_DBG_FuncExit();
            return HI_SUCCESS;
        }
    }
    else if (1 == s32Ret)
    {
        *ppu8DataAddr = pu8ImageData;
        *pu32DataSize = u32ImageSize;
        *pu32ImageVersion = 0;
        *pbNeedUpgrade = HI_FALSE;

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    *ppu8DataAddr = pu8ImageData;
    *pu32DataSize = u32ImageSize;
    *pu32ImageVersion = 0;
    *pbNeedUpgrade = HI_TRUE;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


