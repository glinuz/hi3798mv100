/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hiota_acquisition.c
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
#include "com_buffer.h"
#include "com_parse.h"
#include "hiota_parse.h"
#include "download_ota.h"
#include "hiota_acquisition.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Macro Definition *********************************/

/*************************** Structure Definition *****************************/



typedef struct tagHIOTA_ACQUISITION_S
{
    HI_U8*  pu8SectionBuffer;
    HI_U32  u32SectionBufLen;
    PROT_PROCESS_CALLBACK pfnProcessCallback;
    PROT_DATA_CALLBACK    pfnDataCallback;
} HIOTA_ACQUISITION_S;

static HI_S32               s_u32InitCount = 0;
static HIOTA_ACQUISITION_S  s_stAcquisition;

/*************************** API Declaration **********************************/


static HI_S32 loadGetDownloadInfo(HIOTA_UPGRADE_INFO_S* pstUpgradeInfo)
{
    HI_U32 u32SectionLen = 0;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));
    HI_SET_MASK(stFilterAttr, 0, HIOTA_D_DOWNLOAD_TABID);

    s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
        return s32Ret;
    }

    s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer,
                                  s_stAcquisition.u32SectionBufLen,
                                  &u32SectionLen);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
        return s32Ret;
    }

    s32Ret = HIOTA_ParseDownloadControlSection(s_stAcquisition.pu8SectionBuffer, u32SectionLen, pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIOTA_ParseDownloadControlSection, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

static HI_S32 loadGetPartitionInfo(HIOTA_UPGRADE_INFO_S* pstUpgradeInfo)
{
    HI_U8  i = 0;
    HI_U32 u32SectionLen = 0;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HIOTA_PARTITION_S* pstPartition = HI_NULL_PTR;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));

    HI_SET_MASK(stFilterAttr, 0, pstUpgradeInfo->u8DownloadTableId);
    HI_SET_MASK(stFilterAttr, 1, 0x00);
    HI_SET_MASK(stFilterAttr, 2, 0x00);
    HI_SET_MASK(stFilterAttr, 6, HIOTA_D_PARTITION_HEADTAG);

    for (i = 0; i < pstUpgradeInfo->u8PartitionCount; i++)
    {
        pstPartition = &pstUpgradeInfo->astPartition[i];
        HI_SET_MASK(stFilterAttr, 4, pstPartition->u8PartitionNumber);

        s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
            return s32Ret;
        }

        s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer,
                                      s_stAcquisition.u32SectionBufLen,
                                      &u32SectionLen);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
            return s32Ret;
        }

        s32Ret = HIOTA_ParsePartitionControlSection(s_stAcquisition.pu8SectionBuffer, u32SectionLen, pstPartition);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HIOTA_ParsePartitionControlSection, s32Ret);
            return s32Ret;
        }
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ACQ_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProcessCallback
)
{
    HI_U8* pu8SectionBuffer = HI_NULL_PTR;
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

    pu8SectionBuffer = COM_MemMalloc(HIOTA_D_LEN_SECTION * sizeof(HI_U8));
    if (HI_NULL_PTR == pu8SectionBuffer)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    s32Ret = DOWNLOAD_OTA_Init(pstLoaderParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_Init, s32Ret);

        COM_MemFreeSetNull(pu8SectionBuffer);
        return s32Ret;
    }

    COM_MemSet(&s_stAcquisition, 0x00, sizeof(s_stAcquisition));
    s_stAcquisition.u32SectionBufLen = HIOTA_D_LEN_SECTION;
    s_stAcquisition.pu8SectionBuffer = pu8SectionBuffer;
    s_stAcquisition.pfnDataCallback  = pfnDataCallback;
    s_stAcquisition.pfnProcessCallback = pfnProcessCallback;

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ACQ_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK(DOWNLOAD_OTA_DeInit());

    COM_MemFreeSetNull(s_stAcquisition.pu8SectionBuffer);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ACQ_GetUpgradeInfo(HIOTA_UPGRADE_INFO_S*  pstUpgradeInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstUpgradeInfo);

    s32Ret = loadGetDownloadInfo(pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(loadGetDownloadInfo, s32Ret);
        return s32Ret;
    }

    s32Ret = loadGetPartitionInfo(pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(loadGetPartitionInfo, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIOTA_ACQ_GetUpgradeData
(
    HI_U8*                  pu8MemBuff,
    HI_U32                  u32MemSize,
    HIOTA_UPGRADE_INFO_S*   pstUpgradeInfo
)
{
    HI_U32 i = 0;
    HI_U32 u32SectionLen = 0;
    HI_U32 u32Index = 0;
    HI_U32 u32ImageNum = 0;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HIOTA_DATAGRAM_INFO_S stDatagram;
    COM_BUFFER_DATABLOCK_S astData[HIOTA_D_MAXNUM_PARTITION];
    HI_U32 au32CalculateCRC[HIOTA_D_MAXNUM_PARTITION];
    COM_BUFFER_INITPARAM_S stBufferInitParam;
    COM_BUFFER_STATUS_E enDLStatus = COM_BUFFER_E_STATUS_NORMAL;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pu8MemBuff);
    COM_CHECK_PARAM(HI_NULL_PTR == pstUpgradeInfo);
    COM_CHECK_PARAM(0 == u32MemSize);

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));
    HI_SET_MASK(stFilterAttr, 0,  pstUpgradeInfo->u8DownloadTableId);
    HI_SET_MASK(stFilterAttr, 6, (pstUpgradeInfo->u32MagicNum >> 24) & 0xFF);
    HI_SET_MASK(stFilterAttr, 7, (pstUpgradeInfo->u32MagicNum >> 16) & 0xFF);
    HI_SET_MASK(stFilterAttr, 8, (pstUpgradeInfo->u32MagicNum >> 8) & 0xFF);
    HI_SET_MASK(stFilterAttr, 9, (pstUpgradeInfo->u32MagicNum) & 0xFF);

    s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stBufferInitParam, 0x00, sizeof(stBufferInitParam));
    stBufferInitParam.pu8MemBuff   = pu8MemBuff;
    stBufferInitParam.u32MemSize   = u32MemSize;
    stBufferInitParam.u32BlockSize = HIOTA_D_LEN_DATAGRAM;
    stBufferInitParam.u32ModuleNum = pstUpgradeInfo->u8PartitionCount;

    for (i = 0; i < pstUpgradeInfo->u8PartitionCount; i++)
    {
        stBufferInitParam.astModuleBuf[i].u32ModuleID   = pstUpgradeInfo->astPartition[i].u8PartitionNumber;
        stBufferInitParam.astModuleBuf[i].u32ModuleSize = pstUpgradeInfo->astPartition[i].u32PartTotalSize;
    }

    s32Ret = COM_BUFFER_Init(&stBufferInitParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_BUFFER_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(au32CalculateCRC, 0xFF, sizeof(au32CalculateCRC));

    while (COM_BUFFER_E_STATUS_COMPLETE != enDLStatus)
    {
        s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer,
                                      s_stAcquisition.u32SectionBufLen,
                                      &u32SectionLen);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
            COM_CHECK(COM_BUFFER_DeInit());
            return s32Ret;
        }

        COM_MemSet(&stDatagram, 0x00, sizeof(stDatagram));
        s32Ret = HIOTA_ParseDatagramSection(s_stAcquisition.pu8SectionBuffer, u32SectionLen, &stDatagram);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(HIOTA_ParseDatagramSection, s32Ret);
            COM_CHECK(COM_BUFFER_DeInit());
            return s32Ret;
        }

        u32Index = HIOTA_GetPartitionIndex(stDatagram.u8PartitionNumber);
        if (u32Index >= pstUpgradeInfo->u8PartitionCount)
        {
            COM_CHECK(COM_BUFFER_DeInit());

            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        s32Ret = COM_BUFFER_PutData(u32Index, stDatagram.u32SectionIndex, stDatagram.pu8Data, stDatagram.u16DataLength);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(COM_BUFFER_PutData, s32Ret);
            COM_CHECK(COM_BUFFER_DeInit());
            return s32Ret;
        }

        s32Ret = COM_BUFFER_GetStatus(&enDLStatus);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(COM_BUFFER_GetStatus, s32Ret);
            COM_CHECK(COM_BUFFER_DeInit());
            return s32Ret;
        }

        if (HI_NULL_PTR != s_stAcquisition.pfnProcessCallback)
        {
            COM_CHECK(s_stAcquisition.pfnProcessCallback(COM_BUFFER_GetSize(), pstUpgradeInfo->u32DownloadDataTotalsize));
        }

        if ((COM_BUFFER_E_STATUS_NOMEM == enDLStatus)
            || (COM_BUFFER_E_STATUS_COMPLETE == enDLStatus))
        {
            s32Ret = COM_BUFFER_GetData(astData, HIOTA_D_MAXNUM_PARTITION, &u32ImageNum);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(COM_BUFFER_GetData, s32Ret);
                COM_CHECK(COM_BUFFER_DeInit());
                return s32Ret;
            }

            for (i = 0; i < u32ImageNum; i++)
            {
                u32Index = HIOTA_GetPartitionIndex(astData[i].u32ModuleID);

                if (u32Index >= pstUpgradeInfo->u8PartitionCount)
                {
                    COM_CHECK(COM_BUFFER_DeInit());
                    COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                    return COM_ERR_IMAGE_ILLEGAL;
                }

                au32CalculateCRC[u32Index] = COM_CRC32(au32CalculateCRC[u32Index], astData[i].pu8DataBuff, astData[i].u32DataSize);

                if (HI_TRUE == astData[i].bLastData)
                {
                    if (au32CalculateCRC[u32Index] != pstUpgradeInfo->astPartition[u32Index].u32DownloadCRC32)
                    {
                        COM_CHECK(COM_BUFFER_DeInit());
                        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                        return COM_ERR_IMAGE_ILLEGAL;
                    }
                }
            }

            s32Ret = s_stAcquisition.pfnDataCallback(astData, u32ImageNum);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(s_stAcquisition.pfnDataCallback, s32Ret);

                COM_CHECK(COM_BUFFER_DeInit());
                return s32Ret;
            }
        }
    }

    COM_CHECK(COM_BUFFER_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;

}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


