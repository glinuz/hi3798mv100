/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_hiota.c
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

#include "com_osal.h"
#include "com_debug.h"
#include "hiota_parse.h"
#include "hiota_acquisition.h"
#include "protocol_hiota.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static HI_S32               s_u32InitCount = 0;
static HIOTA_UPGRADE_INFO_S s_stUpgradeInfo;


HI_S32 PROT_HIOTA_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);
    COM_CHECK_PARAM(HI_NULL_PTR == pfnDataCallback);

    if (0 < s_u32InitCount)
    {
        s_u32InitCount ++;

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    s32Ret = HIOTA_ACQ_Init(pstLoaderParam, pfnDataCallback, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&s_stUpgradeInfo, 0x00, sizeof(s_stUpgradeInfo));
    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIOTA_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_CHECK(HIOTA_ACQ_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIOTA_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    s32Ret = HIOTA_ACQ_GetUpgradeInfo(&s_stUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_GetUpgradeInfo, s32Ret);
        return s32Ret;
    }

    pstVersionInfo->u32ManufacturerID    = s_stUpgradeInfo.u32ManufacturerID;
    pstVersionInfo->u32HardwareVersion   = s_stUpgradeInfo.u32HardwareVersion;
    pstVersionInfo->u32SoftwareVersion   = s_stUpgradeInfo.u32SoftwareVersion;
    pstVersionInfo->u32SerialNumberStart = s_stUpgradeInfo.u32SerialNumberStart;
    pstVersionInfo->u32SerialNumberEnd   = s_stUpgradeInfo.u32SerialNumberEnd;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIOTA_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
)
{
    HI_U8 i = 0;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    if (s_stUpgradeInfo.u8PartitionCount > u32BufNum)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    for (i = 0; i < s_stUpgradeInfo.u8PartitionCount; i++)
    {
        pstBuffer[i].u32PartitionId     = s_stUpgradeInfo.astPartition[i].u8PartitionNumber;
        pstBuffer[i].u32FlashType       = s_stUpgradeInfo.astPartition[i].u32FlashType;
        pstBuffer[i].u32FlashIndex      = s_stUpgradeInfo.astPartition[i].u32FlashIndex;
        pstBuffer[i].u64FlashStartAddr  = s_stUpgradeInfo.astPartition[i].u64FlashStartAddr;
        pstBuffer[i].u64FlashEndAddr    = s_stUpgradeInfo.astPartition[i].u64FlashEndAddr;
        pstBuffer[i].u32ImageSize       = s_stUpgradeInfo.astPartition[i].u32DownloadSize;
    }

    *pu32PartNum = s_stUpgradeInfo.u8PartitionCount;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIOTA_Process(HI_VOID)
{
    HI_U32 u32UsableMem = 0;
    HI_U8* pu8MemBuff = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    pu8MemBuff = COM_MemGetUsable(s_stUpgradeInfo.u32DownloadDataTotalsize, &u32UsableMem);
    if (HI_NULL_PTR == pu8MemBuff)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    s32Ret = HIOTA_ACQ_GetUpgradeData(pu8MemBuff, u32UsableMem, &s_stUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_GetUpgradeData, s32Ret);

        COM_MemFreeUsable(pu8MemBuff);
        pu8MemBuff = HI_NULL_PTR;

        return s32Ret;
    }

    COM_MemFreeUsable(pu8MemBuff);
    pu8MemBuff = HI_NULL_PTR;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

