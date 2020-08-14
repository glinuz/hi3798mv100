/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : dcas_ssd.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "dcas_debug.h"
#include "dcas_config.h"
#include "dcas_verify.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct tagSSD_PARTITITON_INFO
{
    HI_CHAR*        pcPartitionName;
    HI_U64          u64FlashStartAddr;
    HI_BOOL         bSigned;

} SSD_PARTITITON_INFO_S;


SSD_PARTITITON_INFO_S s_astPartitionInfo[] =
{
    {"boot",            0, HI_TRUE},        /** Linux */
    {"fastboot",        0, HI_TRUE},        /** Android */
    {"bootargs",        0, HI_TRUE},
    {"stbinfo",         0, HI_FALSE},       /** Android */
    {"deviceinfo",      0, HI_FALSE},
    {"softwareinfo",    0, HI_FALSE},
    {"loaderdb",        0, HI_FALSE},
    {"loaderdbbk",      0, HI_FALSE},
    {"loader",          0, HI_TRUE},
    {"loaderbak",       0, HI_TRUE},
    {"baseparam",       0, HI_FALSE},
    {"pqparam",         0, HI_FALSE},
    {"logo",            0, HI_TRUE},
    {"logobak",         0, HI_TRUE},
    {"fastplay",        0, HI_FALSE},
    {"fastplaybak",     0, HI_FALSE},
    {"kernel",          0, HI_TRUE},        /** Android */
    {"misc",            0, HI_FALSE},       /** Android */
    {"cadata",          0, HI_FALSE},       /** Android */
    {"shatable",        0, HI_TRUE},        /** Android */
    {"system",          0, HI_FALSE},       /** Android */
    {"userdata",        0, HI_FALSE},       /** Android */
    {"cache",           0, HI_FALSE},       /** Android */
    {"private",         0, HI_FALSE},       /** Android */
    {"sdcard",          0, HI_FALSE},       /** Android */
    {"trustedcore",     0, HI_FALSE},       /** Linux */
    {"rootfs",          0, HI_TRUE}         /** Linux */
};

static SSD_PARTITITON_INFO_S* ssdGetPartitionInfo(HI_U64 u64FlashStartAddr)
{
    HI_U32 i = 0;
    HI_U32 u32ImageCount = 0;

    SSA_DBG_FuncEnter();

    u32ImageCount = sizeof(s_astPartitionInfo) / sizeof(SSD_PARTITITON_INFO_S);

    for (i = 0; i < u32ImageCount; i++)
    {
        if (u64FlashStartAddr == s_astPartitionInfo[i].u64FlashStartAddr)
        {
            SSA_DBG_FuncExit();
            return &s_astPartitionInfo[i];
        }
    }

    SSA_DBG_FuncExit();
    return HI_NULL_PTR;
}

static HI_S32 ssdGetPartitionStartAddr(HI_CHAR* pcPartitionName, HI_U64* pu64FlashStartAddr)
{
    HI_Flash_InterInfo_S stFlashInfo;
    HI_HANDLE hFlashHandle = HI_INVALID_HANDLE;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    hFlashHandle = HI_Flash_OpenByName(pcPartitionName);
    if (HI_INVALID_HANDLE == hFlashHandle)
    {
        SSA_PrintFuncErr(HI_Flash_OpenByName, SSA_ERR_NOEXIST);
        return SSA_ERR_NOEXIST;
    }

    memset(&stFlashInfo, 0x00, sizeof(stFlashInfo));
    s32Ret = HI_Flash_GetInfo(hFlashHandle, &stFlashInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(HI_Flash_GetInfo, s32Ret);

        HI_Flash_Close(hFlashHandle);
        hFlashHandle = HI_NULL_PTR;

        return s32Ret;
    }

    HI_Flash_Close(hFlashHandle);
    hFlashHandle = HI_INVALID_HANDLE;

    *pu64FlashStartAddr = stFlashInfo.OpenAddr;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssdAcquirePartitionInfo(HI_VOID)
{
    HI_U32 i = 0;
    HI_U32 u32ImageCount = 0;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    u32ImageCount = sizeof(s_astPartitionInfo) / sizeof(SSD_PARTITITON_INFO_S);

    for (i = 0; i < u32ImageCount; i++)
    {
        s32Ret = ssdGetPartitionStartAddr(s_astPartitionInfo[i].pcPartitionName, &s_astPartitionInfo[i].u64FlashStartAddr);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(ssdGetPartitionStartAddr, s32Ret);
            continue;
        }
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 ssdVerifyImageData(HI_U8* pu8Data, HI_U32 u32Size)
{
    HI_U32 u32Index = 0;
    HI_U32 u32LeftSize = u32Size;
    HI_U8* pu8TailHead = pu8Data;
    HI_U8* pu8CurrData = pu8Data;
    HI_U8* pu8CurrTail = HI_NULL_PTR;
    HI_U8* pu8PrevTail = HI_NULL_PTR;
    SSA_SECTIONTAIL_S* pstTailHead = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pu8Data);
    SSA_CHECK_PARAM(0 == u32Size);

    while (u32LeftSize >= SSA_DCAS_FIX_PAGESIZE)
    {
        if (SSA_CHECK_TAILSECTION(pu8TailHead))
        {
            pstTailHead = (SSA_SECTIONTAIL_S*)pu8TailHead;
            break;
        }

        u32LeftSize -= SSA_DCAS_FIX_PAGESIZE;
        pu8TailHead += SSA_DCAS_FIX_PAGESIZE;
    }

    if (HI_NULL_PTR == pstTailHead)
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    if (0 == pstTailHead->u32Totalsection)
    {
        SSA_PrintErrCode(SSA_ERR_ILLEGAL_IMAGE);
        return SSA_ERR_ILLEGAL_IMAGE;
    }

    pu8CurrTail = pu8TailHead;

    for (u32Index = 0; u32Index < pstTailHead->u32Totalsection; u32Index ++)
    {
        s32Ret = SSA_VerifySectionTail(pu8PrevTail, pu8CurrTail);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_VerifySectionTail, s32Ret);
            return s32Ret;
        }

        s32Ret = SSA_VerifySectionData(pu8CurrData, pu8TailHead);
        if (HI_SUCCESS != s32Ret)
        {
            SSA_PrintFuncErr(SSA_VerifySectionData, s32Ret);
            return s32Ret;
        }

        pu8CurrData = pu8CurrData + pstTailHead->u32SectionSize;
        pu8CurrTail = pu8CurrTail + SSA_DCAS_FIX_PAGESIZE;
        pu8PrevTail = pu8CurrTail;
    }

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DCAS_SSD_Init(HI_VOID)
{
    HI_U8* pu8Buffer = HI_NULL_PTR;
    SSA_FLASH_OPENPARAM_S stDataParam;
    HI_S32 s32Ret = HI_SUCCESS;

    SSA_DBG_FuncEnter();

    s32Ret = ssdAcquirePartitionInfo();
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssdAcquirePartitionInfo, s32Ret);
        return s32Ret;
    }

    pu8Buffer = malloc(sizeof(HI_U8) * SSA_DCAS_LEN_RSAPUBLICKEY);
    if (HI_NULL_PTR == pu8Buffer)
    {
        SSA_PrintErrCode(SSA_ERR_MEM);
        return SSA_ERR_MEM;
    }

    memset(&stDataParam, 0x00, sizeof(stDataParam));
    stDataParam.pcPartitionName = SSA_PARTNAME_BOOT;
    stDataParam.enFlashType     = HI_FLASH_TYPE_BUTT;
    s32Ret = SSA_ConfigExternKey(&stDataParam, pu8Buffer, SSA_DCAS_LEN_RSAPUBLICKEY);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(SSA_ConfigExternKey, s32Ret);

        free(pu8Buffer);
        pu8Buffer = HI_NULL_PTR;

        return s32Ret;
    }

    free(pu8Buffer);
    pu8Buffer = HI_NULL_PTR;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DCAS_SSD_DeInit(HI_VOID)
{
    SSA_DBG_FuncEnter();

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DCAS_SSD_WholeImageVerfy
(
    HI_U8*      pu8ImageData,
    HI_U32      u32ImageSize,
    HI_U8**     ppu8DataAddr,
    HI_U32*     pu32DataSize
)
{
    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    SSA_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);

    *ppu8DataAddr = pu8ImageData;
    *pu32DataSize = u32ImageSize;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DCAS_SSD_PartitionImageVerfy
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
    SSD_PARTITITON_INFO_S* pstPartition = HI_NULL_PTR;
    HI_S32 s32Ret = HI_FAILURE;

    SSA_DBG_FuncEnter();

    SSA_CHECK_PARAM(HI_NULL_PTR == pu8ImageData);
    SSA_CHECK_PARAM(HI_NULL_PTR == ppu8DataAddr);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu32DataSize);
    SSA_CHECK_PARAM(HI_NULL_PTR == pu32ImageVersion);
    SSA_CHECK_PARAM(HI_NULL_PTR == pbNeedUpgrade);

    pstPartition = ssdGetPartitionInfo(u64FlashStartAddr);

    if ((HI_NULL_PTR == pstPartition)
        || (HI_TRUE != pstPartition->bSigned))
    {
        *ppu8DataAddr       = pu8ImageData;
        *pu32DataSize       = u32ImageSize;
        *pu32ImageVersion   = 0;
        *pbNeedUpgrade      = HI_TRUE;

        SSA_DBG_FuncExit();
        return HI_SUCCESS;
    }

    s32Ret = ssdVerifyImageData(pu8ImageData, u32ImageSize);
    if (HI_SUCCESS != s32Ret)
    {
        SSA_PrintFuncErr(ssdVerifyImageData, s32Ret);
        return s32Ret;
    }

    *ppu8DataAddr       = pu8ImageData;
    *pu32DataSize       = u32ImageSize;
    *pu32ImageVersion   = 0;
    *pbNeedUpgrade      = HI_TRUE;

    SSA_DBG_FuncExit();
    return HI_SUCCESS;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

