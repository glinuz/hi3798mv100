/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_hifile.c
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

#include "com_debug.h"
#include "com_osal.h"
#include "hifile_parse.h"
#include "hifile_acquisition.h"
#include "protocol_hifile.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static HI_S32               s_u32InitCount = 0;
static HIFILE_DOCUMENT_S    s_stUpdateInfo;

HI_S32 PROT_HIFILE_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    if (0 < s_u32InitCount)
    {
        s_u32InitCount ++;

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_CHECK_PARAM(HI_NULL_PTR == pfnDataCallback);
    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    s32Ret = HIFILE_ACQ_Init(pstLoaderParam, pfnDataCallback, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ACQ_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&s_stUpdateInfo, 0x00, sizeof(s_stUpdateInfo));

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIFILE_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_CHECK(HIFILE_ACQ_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIFILE_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    s32Ret = HIFILE_ACQ_GetUpgradeInfo(&s_stUpdateInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ACQ_GetUpgradeInfo, s32Ret);
        return s32Ret;
    }

    pstManuInfo = &s_stUpdateInfo.astManuInfo[0];
    pstVersionInfo->u32ManufacturerID = pstManuInfo->u32ManufacturerID;
    pstVersionInfo->u32HardwareVersion = pstManuInfo->u32HardwareVersion;
    pstVersionInfo->u32SoftwareVersion = pstManuInfo->u32SoftwareVersion;
    pstVersionInfo->u32SerialNumberStart = pstManuInfo->u32SerialNumberStart;
    pstVersionInfo->u32SerialNumberEnd = pstManuInfo->u32SerialNumberEnd;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIFILE_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
)
{
    HI_U8 i = 0;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    pstManuInfo = &s_stUpdateInfo.astManuInfo[0];

    if (u32BufNum < pstManuInfo->u16FlashMapNum)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    for (i = 0; i < pstManuInfo->u16FlashMapNum; i++)
    {
        pstBuffer[i].u32PartitionId     = pstManuInfo->astFlashMap[i].u32ImageID;
        pstBuffer[i].u32FlashType       = pstManuInfo->astFlashMap[i].u32FlashType;
        pstBuffer[i].u32FlashIndex      = pstManuInfo->astFlashMap[i].u32FlashIndex;
        pstBuffer[i].u64FlashStartAddr  = pstManuInfo->astFlashMap[i].u64PartitionStartAddr;
        pstBuffer[i].u64FlashEndAddr    = pstManuInfo->astFlashMap[i].u64PartitionEndAddr;
        pstBuffer[i].u32ImageSize       = pstManuInfo->astFlashMap[i].u32ImageLength;
    }

    *pu32PartNum = pstManuInfo->u16FlashMapNum;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_HIFILE_Process(HI_VOID)
{
    HI_U32 u32UsableMem = 0;
    HI_U8* pu8MemBuff = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    pu8MemBuff = COM_MemGetUsable(s_stUpdateInfo.u32FileLen, &u32UsableMem);
    if (HI_NULL_PTR == pu8MemBuff)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    s32Ret = HIFILE_ACQ_GetUpgradeData(pu8MemBuff, u32UsableMem, &s_stUpdateInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ACQ_GetUpgradeData, s32Ret);
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

