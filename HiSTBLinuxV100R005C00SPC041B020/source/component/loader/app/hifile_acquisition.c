/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hifile_acquisition.c
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
#include "com_parse.h"
#include "com_buffer.h"
#include "download.h"
#include "hifile_parse.h"
#include "hifile_acquisition.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/

#define HIFILE_D_MIN_HEADLEN        (80)        /**< The minimum size of head */
#define HIFILE_D_POS_HEADLEN        (8)         /**< The position of the headlen descriptor*/
#define HIFILE_D_MIN_BLOCKSIZE      (1048576)   /**< The minimum size of the download block (1M)*/
#define HIFILE_D_LEN_DATAHEAD       (8)         /**< The length of the data block's Header*/

/*************************** Structure Definition *****************************/

/*************************** API Declaration **********************************/

typedef struct tagHIFILE_ACQUISITION_S
{
    PROT_PROCESS_CALLBACK pfnProcessCallback;
    PROT_DATA_CALLBACK    pfnDataCallback;

} HIFILE_ACQUISITION_S;

static HI_S32                   s_u32InitCount = 0;
static HIFILE_ACQUISITION_S     s_stAcquisition;

HI_S32 HIFILE_ACQ_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProcessCallback
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

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);
    COM_CHECK_PARAM(HI_NULL_PTR == pfnDataCallback);

    s32Ret = DOWNLOAD_Init(pstLoaderParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&s_stAcquisition, 0x00, sizeof(s_stAcquisition));
    s_stAcquisition.pfnDataCallback = pfnDataCallback;
    s_stAcquisition.pfnProcessCallback = pfnProcessCallback;

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIFILE_ACQ_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    COM_CHECK(DOWNLOAD_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIFILE_ACQ_GetUpgradeInfo(HIFILE_DOCUMENT_S* pstUpgradeInfo)
{
    HI_U32 u32OutSize = 0;
    HI_U32 u32ReadSize = 0;
    HI_U32 u32HeaderLen = 0;
    HI_U32 u32RemainLen = 0;
    HI_U8  au8HeaderBuf[HIFILE_D_MIN_HEADLEN] = {0};
    HI_U8* pu8HeaderBuf = HI_NULL_PTR;
    HI_U8* pu8ReadPos = HI_NULL_PTR;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstUpgradeInfo);

    s32Ret = DOWNLOAD_Getdata(au8HeaderBuf, HIFILE_D_MIN_HEADLEN, &u32OutSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
        return s32Ret;
    }

    if (HIFILE_D_MIN_HEADLEN != u32OutSize)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8ReadPos = au8HeaderBuf + HIFILE_D_POS_HEADLEN;
    HI_READ_32BIT(pu8ReadPos, u32HeaderLen, u32ReadSize);

    if (HIFILE_D_MIN_HEADLEN > u32HeaderLen)
    {
        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
        return COM_ERR_IMAGE_ILLEGAL;
    }

    pu8HeaderBuf = COM_MemMalloc(u32HeaderLen);
    if (HI_NULL_PTR == pu8HeaderBuf)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8HeaderBuf, 0x00, u32HeaderLen);
    COM_MemCpy(pu8HeaderBuf, au8HeaderBuf, HIFILE_D_MIN_HEADLEN);

    pu8ReadPos   = pu8HeaderBuf + HIFILE_D_MIN_HEADLEN;
    u32RemainLen = u32HeaderLen - HIFILE_D_MIN_HEADLEN;

    if (0 < u32RemainLen)
    {
        s32Ret = DOWNLOAD_Getdata(pu8ReadPos, u32RemainLen, &u32OutSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
            COM_MemFreeSetNull(pu8HeaderBuf);
            return s32Ret;
        }

        if (u32RemainLen != u32OutSize)
        {
            COM_MemFreeSetNull(pu8HeaderBuf);
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }
    }

    s32Ret = HIFILE_ParseHeader(pu8HeaderBuf, u32HeaderLen, pstUpgradeInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HIFILE_ParseHeader, s32Ret);
        COM_MemFreeSetNull(pu8HeaderBuf);
        return s32Ret;
    }

    COM_MemFreeSetNull(pu8HeaderBuf);

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 HIFILE_ACQ_GetUpgradeData
(
    HI_U8*                  pu8MemBuff,
    HI_U32                  u32MemSize,
    HIFILE_DOCUMENT_S*      pstUpgradeInfo
)
{
    HI_U8  i = 0;
    HI_U8  u8Index = 0;
    HI_U32 u32SectionIndex = 0;
    HI_U32 u32ReadSize = 0;
    HI_U32 u32GetSize = 0;
    HI_U32 u32OutSize = 0;
    HI_U32 u32ImageNum = 0;
    HI_U32 u32TotalRead = 0;
    HI_U32 u32ImageLength = 0;
    HI_U32 u32BlockSize = 0;
    HI_U32 u32CalculateCRC = 0xffffffffL;
    COM_BUFFER_INITPARAM_S stBufferInitParam;
    HI_U8 au8DataBlock[HIFILE_D_MIN_BLOCKSIZE] = {0};
    COM_BUFFER_DATABLOCK_S astData[HIFILE_D_MAXNUM_FLASHMAP];
    COM_BUFFER_STATUS_E enDLStatus = COM_BUFFER_E_STATUS_NORMAL;
    HI_U8* pu8DataBlock = HI_NULL_PTR;
    HIFILE_FLASHMAP_S* pstFlashMap = HI_NULL_PTR;
    HIFILE_MANUINFO_S* pstManuInfo = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pu8MemBuff);
    COM_CHECK_PARAM(HI_NULL_PTR == pstUpgradeInfo);
    COM_CHECK_PARAM(0 == u32MemSize);

    u32TotalRead = pstUpgradeInfo->u32HeaderLen;
    pstManuInfo = &pstUpgradeInfo->astManuInfo[0];

    COM_MemSet(&stBufferInitParam, 0x00, sizeof(stBufferInitParam));
    stBufferInitParam.pu8MemBuff   = pu8MemBuff;
    stBufferInitParam.u32MemSize   = u32MemSize;
    stBufferInitParam.u32BlockSize = HIFILE_D_MIN_BLOCKSIZE;
    stBufferInitParam.u32ModuleNum = pstManuInfo->u16FlashMapNum;

    for (u8Index = 0; u8Index < pstManuInfo->u16FlashMapNum; u8Index++)
    {
        stBufferInitParam.astModuleBuf[u8Index].u32ModuleID   = pstManuInfo->astFlashMap[u8Index].u32ImageID;
        stBufferInitParam.astModuleBuf[u8Index].u32ModuleSize = pstManuInfo->astFlashMap[u8Index].u32ImageLength;
    }

    s32Ret = COM_BUFFER_Init(&stBufferInitParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(COM_BUFFER_Init, s32Ret);
        return s32Ret;
    }

    u32BlockSize = COM_BUFFER_GetBlockSize();

    for (u8Index = 0; u8Index < pstManuInfo->u16FlashMapNum; u8Index++)
    {
        pstFlashMap = &(pstManuInfo->astFlashMap[u8Index]);

        if (pstFlashMap->u32ImageOffset < u32TotalRead)
        {
            COM_BUFFER_DeInit();
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        if (pstFlashMap->u32ImageOffset > u32TotalRead)
        {
            u32GetSize = pstFlashMap->u32ImageOffset - u32TotalRead;

            if (u32BlockSize < u32GetSize)
            {
                COM_BUFFER_DeInit();
                COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                return COM_ERR_IMAGE_ILLEGAL;
            }

            s32Ret = DOWNLOAD_Getdata(au8DataBlock, u32GetSize, &u32OutSize);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
                COM_BUFFER_DeInit();
                return s32Ret;
            }

            if (u32GetSize != u32OutSize)
            {
                COM_BUFFER_DeInit();
                COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                return COM_ERR_IMAGE_ILLEGAL;
            }
        }

        s32Ret = DOWNLOAD_Getdata(au8DataBlock, HIFILE_D_LEN_DATAHEAD, &u32GetSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
            COM_BUFFER_DeInit();
            return s32Ret;
        }

        if (HIFILE_D_LEN_DATAHEAD != u32GetSize)
        {
            COM_BUFFER_DeInit();
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        pu8DataBlock = au8DataBlock;
        HI_READ_32BIT(pu8DataBlock, u32ImageLength, u32TotalRead);

        if (u32ImageLength != pstFlashMap->u32ImageLength)
        {
            COM_BUFFER_DeInit();
            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
            return COM_ERR_IMAGE_ILLEGAL;
        }

        HI_READ_32BIT(pu8DataBlock, pstFlashMap->u32ImageCRC, u32TotalRead);

        u32ReadSize = 0;
        u32SectionIndex = 0;

        while (u32ReadSize < pstFlashMap->u32ImageLength)
        {
            u32GetSize = pstFlashMap->u32ImageLength - u32ReadSize;

            if (u32GetSize > u32BlockSize)
            {
                u32GetSize = u32BlockSize;
            }

            s32Ret = DOWNLOAD_Getdata(au8DataBlock, u32GetSize, &u32OutSize);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(DOWNLOAD_Getdata, s32Ret);
                COM_BUFFER_DeInit();
                return s32Ret;
            }

            if (u32GetSize != u32OutSize)
            {
                COM_BUFFER_DeInit();
                COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                return COM_ERR_IMAGE_ILLEGAL;
            }

            s32Ret = COM_BUFFER_PutData(u8Index, u32SectionIndex, au8DataBlock, u32GetSize);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(COM_BUFFER_PutData, s32Ret);
                COM_BUFFER_DeInit();
                return s32Ret;
            }

            u32SectionIndex ++;

            u32ReadSize += u32GetSize;

            if (HI_NULL_PTR != s_stAcquisition.pfnProcessCallback)
            {
                COM_CHECK(s_stAcquisition.pfnProcessCallback(COM_BUFFER_GetSize(), pstManuInfo->u32ImageLength));
            }

            s32Ret = COM_BUFFER_GetStatus(&enDLStatus);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(COM_BUFFER_GetStatus, s32Ret);
                COM_BUFFER_DeInit();
                return s32Ret;
            }

            if ((COM_BUFFER_E_STATUS_NOMEM == enDLStatus)
                || (COM_BUFFER_E_STATUS_COMPLETE == enDLStatus))
            {
                s32Ret = COM_BUFFER_GetData(astData, HIFILE_D_MAXNUM_FLASHMAP, &u32ImageNum);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_PrintFuncErr(COM_BUFFER_GetData, s32Ret);
                    COM_BUFFER_DeInit();
                    return s32Ret;
                }

                for (i = 0; i < u32ImageNum; i++)
                {
                    u32CalculateCRC = COM_CRC32(u32CalculateCRC, astData[i].pu8DataBuff, astData[i].u32DataSize);

                    if (HI_TRUE == astData[i].bLastData)
                    {
                        if (u32CalculateCRC != pstManuInfo->astFlashMap[astData[i].u32ModuleID].u32ImageCRC)
                        {
                            COM_BUFFER_DeInit();
                            COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                            return COM_ERR_IMAGE_ILLEGAL;
                        }

                        u32CalculateCRC = 0xffffffffL;
                    }
                }

                s32Ret = s_stAcquisition.pfnDataCallback(astData, u32ImageNum);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_PrintFuncErr(s_stAcquisition.pfnDataCallback, s32Ret);
                    COM_BUFFER_DeInit();

                    return s32Ret;
                }
            }
        }

        u32TotalRead += u32ReadSize;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


