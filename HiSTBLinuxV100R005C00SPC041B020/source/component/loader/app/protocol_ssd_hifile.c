/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_ssd_hifile.c
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
#include "download.h"
#include "ssd_parse.h"
#include "protocol_ssd_hifile.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Macro Definition *********************************/

#define HIFIEL_MAXNUM_PERREAD       (1048576)   /**< Maximum length per read (1M)*/

/*************************** Structure Definition *****************************/

typedef struct tagADVCA_HIFILE_CTRLBLOCK_S
{
    HI_U32               u32FileSize;
    HI_U8*               pu8FileData;
    HIFILE_DOCUMENT_S    stDocument;
    PROT_DATA_CALLBACK   pfnDataCallback;

} ADVCA_HIFILE_CTRLBLOCK_S;

static HI_S32                   s_u32InitCount = 0;
static ADVCA_HIFILE_CTRLBLOCK_S s_stCtrlBlock;

/*************************** API Declaration **********************************/
static HI_S32 hifileReadFile
(
    HI_U8*                  pu8FileData,
    HI_U32                  u32FileSize,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_U32 u64OutSize = 0;
    HI_U32 u64ReadSize = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    while (u64ReadSize < u32FileSize)
    {
        s32Ret = DOWNLOAD_Getdata(pu8FileData, HIFIEL_MAXNUM_PERREAD, &u64OutSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
            return s32Ret;
        }

        if (0 == u64OutSize)
        {
            COM_PrintErrCode(COM_ERR_FILE);
            return COM_ERR_FILE;
        }

        u64ReadSize += u64OutSize;
        pu8FileData += u64OutSize;

        if (HI_NULL_PTR != pfnProgressCallback)
        {
            s32Ret = pfnProgressCallback(u64ReadSize, u32FileSize);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(pfnProgressCallback, s32Ret);
                return s32Ret;
            }
        }
    }

    if (u32FileSize != u64ReadSize)
    {
        COM_PrintErrCode(COM_ERR_FILE);
        return COM_ERR_FILE;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 hifileGetFileData
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    HI_U8**                 ppu8FileData,
    HI_U32*                 pu32FileSize,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_U32 u32FileSize = 0;
    HI_U8* pu8FileData = HI_NULL_PTR;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    s32Ret = DOWNLOAD_Init(pstLoaderParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_Init, s32Ret);
        return s32Ret;
    }

    s32Ret = DOWNLOAD_GetFileSize(&u32FileSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_GetFileSize, s32Ret);
        COM_CHECK(DOWNLOAD_DeInit());
        return s32Ret;
    }

    pu8FileData = COM_MemMalloc(u32FileSize);
    if (HI_NULL_PTR == pu8FileData)
    {
        COM_CHECK(DOWNLOAD_DeInit());

        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8FileData, 0x00, u32FileSize);
    s32Ret = hifileReadFile(pu8FileData, u32FileSize, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hifileReadFile, s32Ret);

        COM_MemFreeSetNull(pu8FileData);
        COM_CHECK(DOWNLOAD_DeInit());

        return s32Ret;
    }

    COM_CHECK(DOWNLOAD_DeInit());

    *ppu8FileData = pu8FileData;
    *pu32FileSize = u32FileSize;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIFILE_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_U32 u32FileSize = 0;
    HI_U8* pu8FileData = HI_NULL_PTR;

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

    COM_MemSet(&s_stCtrlBlock, 0x00, sizeof(s_stCtrlBlock));
    s32Ret = hifileGetFileData(pstLoaderParam, &pu8FileData, &u32FileSize, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hifileGetFileData, s32Ret);
        return s32Ret;
    }

    s32Ret = ADVCA_ParseFileData(pu8FileData, u32FileSize, &s_stCtrlBlock.stDocument);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(ADVCA_ParseFileData, s32Ret);
        COM_MemFreeSetNull(pu8FileData);
        return s32Ret;
    }

    s_stCtrlBlock.pu8FileData = pu8FileData;
    s_stCtrlBlock.u32FileSize = u32FileSize;
    s_stCtrlBlock.pfnDataCallback = pfnDataCallback;

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIFILE_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_MemFreeSetNull(s_stCtrlBlock.pu8FileData);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIFILE_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    COM_CHECK_INITED(s_u32InitCount);

    pstManuInfo = &s_stCtrlBlock.stDocument.astManuInfo[0];
    pstVersionInfo->u32ManufacturerID  = pstManuInfo->u32ManufacturerID;
    pstVersionInfo->u32HardwareVersion = pstManuInfo->u32HardwareVersion;
    pstVersionInfo->u32SoftwareVersion = pstManuInfo->u32SoftwareVersion;
    pstVersionInfo->u32SerialNumberStart = pstManuInfo->u32SerialNumberStart;
    pstVersionInfo->u32SerialNumberEnd = pstManuInfo->u32SerialNumberEnd;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSD_HIFILE_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
)
{
    HI_U8 i = 0;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    COM_CHECK_INITED(s_u32InitCount);

    pstManuInfo = &s_stCtrlBlock.stDocument.astManuInfo[0];
    COM_CHECK_PARAM(u32BufNum < pstManuInfo->u16FlashMapNum);

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

HI_S32 PROT_SSD_HIFILE_Process(HI_VOID)
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
#endif /* __cplusplus */
#endif /* __cplusplus */

