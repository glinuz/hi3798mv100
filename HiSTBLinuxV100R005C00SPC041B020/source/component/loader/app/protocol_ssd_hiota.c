/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_ssd_hiota.c
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
#include "ssd_parse.h"
#include "hiota_parse.h"
#include "hiota_acquisition.h"
#include "protocol_ssd_hiota.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /*  __cplusplus */
#endif /*  __cplusplus */


typedef struct tagADVCA_HIOTA_CTRLBLOCK_S
{
    HI_U32               u32FileSize;
    HI_U8*               pu8FileData;

    HI_U8*               pu8ImageBuff;

    HIOTA_UPGRADE_INFO_S stUpdateInfo;
    HIFILE_DOCUMENT_S    stDocument;
    PROT_DATA_CALLBACK   pfnDataCallback;

} ADVCA_HIOTA_CTRLBLOCK_S;

static HI_S32                   s_u32InitCount = 0;
static ADVCA_HIOTA_CTRLBLOCK_S  s_stCtrlBlock;


static HI_S32 advcaHiOTADataCallback(COM_BUFFER_DATABLOCK_S* pstData, HI_U32 u32Number)
{
    s_stCtrlBlock.u32FileSize = pstData->u32DataSize;
    s_stCtrlBlock.pu8FileData = pstData->pu8DataBuff;

    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIOTA_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pfnDataCallback);
    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    if (0 < s_u32InitCount)
    {
        s_u32InitCount ++;

        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    s32Ret = HIOTA_ACQ_Init(pstLoaderParam, advcaHiOTADataCallback, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&s_stCtrlBlock, 0x00, sizeof(s_stCtrlBlock));

    s_stCtrlBlock.pfnDataCallback = pfnDataCallback;

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIOTA_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    if (HI_NULL_PTR != s_stCtrlBlock.pu8ImageBuff)
    {
        COM_MemFreeSetNull(s_stCtrlBlock.pu8ImageBuff);
    }

    COM_CHECK(HIOTA_ACQ_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIOTA_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    s32Ret = HIOTA_ACQ_GetUpgradeInfo(&s_stCtrlBlock.stUpdateInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_GetUpgradeInfo, s32Ret);
        return s32Ret;
    }

    pstVersionInfo->u32ManufacturerID    = s_stCtrlBlock.stUpdateInfo.u32ManufacturerID;
    pstVersionInfo->u32HardwareVersion   = s_stCtrlBlock.stUpdateInfo.u32HardwareVersion;
    pstVersionInfo->u32SoftwareVersion   = s_stCtrlBlock.stUpdateInfo.u32SoftwareVersion;
    pstVersionInfo->u32SerialNumberStart = s_stCtrlBlock.stUpdateInfo.u32SerialNumberStart;
    pstVersionInfo->u32SerialNumberEnd   = s_stCtrlBlock.stUpdateInfo.u32SerialNumberEnd;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIOTA_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
)
{
    HI_U16 i = 0;
    HI_U8* pu8ImageBuff = HI_NULL_PTR;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;
    HIOTA_UPGRADE_INFO_S* pstUpdateInfo = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    pstUpdateInfo = &s_stCtrlBlock.stUpdateInfo;

    pu8ImageBuff = COM_MemMalloc(pstUpdateInfo->u32DownloadDataTotalsize);
    if (HI_NULL_PTR == pu8ImageBuff)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8ImageBuff, 0x00, pstUpdateInfo->u32DownloadDataTotalsize);
    s32Ret = HIOTA_ACQ_GetUpgradeData(pu8ImageBuff,
                                      pstUpdateInfo->u32DownloadDataTotalsize,
                                      pstUpdateInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ACQ_GetUpgradeData, s32Ret);
        COM_MemFreeSetNull(pu8ImageBuff);
        return s32Ret;
    }

    if ((pstUpdateInfo->u32DownloadDataTotalsize != s_stCtrlBlock.u32FileSize)
        || (HI_NULL_PTR == s_stCtrlBlock.pu8FileData))
    {
        COM_PrintErrCode(COM_ERR_UNKNOWNED);
        COM_MemFreeSetNull(pu8ImageBuff);
        return COM_ERR_UNKNOWNED;
    }

    s32Ret = ADVCA_ParseFileData(s_stCtrlBlock.pu8FileData, s_stCtrlBlock.u32FileSize, &s_stCtrlBlock.stDocument);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ADVCA_ParseFileData, s32Ret);
        COM_MemFreeSetNull(pu8ImageBuff);
        return s32Ret;
    }

    pstManuInfo = &s_stCtrlBlock.stDocument.astManuInfo[0];

    if (u32BufNum < pstManuInfo->u16FlashMapNum)
    {
        COM_MemFreeSetNull(pu8ImageBuff);
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    if ((pstManuInfo->u32ManufacturerID != pstUpdateInfo->u32ManufacturerID)
        || (pstManuInfo->u32HardwareVersion != pstUpdateInfo->u32HardwareVersion)
        || (pstManuInfo->u32SoftwareVersion != pstUpdateInfo->u32SoftwareVersion)
        || (pstManuInfo->u32SerialNumberStart != pstUpdateInfo->u32SerialNumberStart)
        || (pstManuInfo->u32SerialNumberEnd != pstUpdateInfo->u32SerialNumberEnd))
    {
        COM_MemFreeSetNull(pu8ImageBuff);
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
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

    s_stCtrlBlock.pu8ImageBuff = pu8ImageBuff;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIOTA_Process(HI_VOID)
{
    HI_U8 i = 0;
    COM_BUFFER_DATABLOCK_S stPartData;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    pstManuInfo = &s_stCtrlBlock.stDocument.astManuInfo[0];

    for (i = 0; i < pstManuInfo->u16FlashMapNum; i++)
    {
        if (HI_TRUE != pstManuInfo->astFlashMap[i].bNeedUpgrade)
        {
            continue;
        }

        COM_MemSet(&stPartData, 0x00, sizeof(stPartData));
        stPartData.u32ModuleID = pstManuInfo->astFlashMap[i].u32ImageID;
        stPartData.u32DataSize = pstManuInfo->astFlashMap[i].u32ImageLength;
        stPartData.pu8DataBuff = pstManuInfo->astFlashMap[i].pu8ImageData;
        stPartData.bLastData   = HI_TRUE;

        s32Ret = s_stCtrlBlock.pfnDataCallback(&stPartData, 1);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(s_stCtrlBlock.pfnDataCallback, s32Ret);
            return s32Ret;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /*  __cplusplus */
#endif /*  __cplusplus */

