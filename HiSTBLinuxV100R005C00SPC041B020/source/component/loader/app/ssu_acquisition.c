/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ssu_acquisition.c
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
#include "ssu_parse.h"
#include "download_ota.h"
#include "protocol_common.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


/*************************** Macro Definition *********************************/

/*************************** Structure Definition *****************************/

typedef struct tagSSU_DLBLOCK_S
{
    HI_U32                  u32SectionBufLen;
    HI_U8*                  pu8SectionBuffer;
    PROT_PROCESS_CALLBACK   pfnProcessCallback;
    PROT_DATA_CALLBACK      pfnDataCallback;
} SSU_DLBLOCK_S;

static HI_S32           s_u32InitCount = 0;
static SSU_DLBLOCK_S    s_stAcquisition;

/*************************** API Declaration **********************************/

static HI_S32 ssuGetIndexByModuleID(HI_U16 u16ModuleId, SSU_DII_S* pstDII, HI_U32* pu32Index)
{
    HI_U32 i = 0;

    for (i = 0; i < pstDII->u16NumberOfModules; i++)
    {
        if (u16ModuleId == pstDII->pstModules[i].u16ModuleID)
        {
            *pu32Index = i;
            return HI_SUCCESS;
        }
    }

    return COM_ERR_NOEXIST;
}

HI_S32 SSU_ACQ_Init
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

    pu8SectionBuffer = COM_MemMalloc(SSU_D_DSMCC_MAXSECTIONLEN * sizeof(HI_U8));
    if (HI_NULL_PTR == pu8SectionBuffer)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8SectionBuffer, 0x00, SSU_D_DSMCC_MAXSECTIONLEN * sizeof(HI_U8));
    s32Ret = DOWNLOAD_OTA_Init(pstLoaderParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_Init, s32Ret);
        COM_MemFreeSetNull(pu8SectionBuffer);
        return s32Ret;
    }

    COM_MemSet(&s_stAcquisition, 0x00, sizeof(s_stAcquisition));
    s_stAcquisition.u32SectionBufLen = SSU_D_DSMCC_MAXSECTIONLEN;
    s_stAcquisition.pu8SectionBuffer = pu8SectionBuffer;
    s_stAcquisition.pfnDataCallback = pfnDataCallback;
    s_stAcquisition.pfnProcessCallback = pfnProcessCallback;

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ACQ_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK(DOWNLOAD_OTA_DeInit());

    if (HI_NULL_PTR != s_stAcquisition.pu8SectionBuffer)
    {
        COM_MemFreeSetNull(s_stAcquisition.pu8SectionBuffer);
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ACQ_GetDSIInfo(SSU_DSI_S* pstDSI)
{
    HI_U32 u32SectionLen = 0;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstDSI);

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));
    HI_SET_MASK(stFilterAttr, 0, (SSU_D_DSMCC_DSITABID));
    HI_SET_MASK(stFilterAttr, 6, (SSU_D_DSMCC_PROTOCOL));
    HI_SET_MASK(stFilterAttr, 7, (SSU_D_DSMCC_TYPE));
    HI_SET_MASK(stFilterAttr, 8, (SSU_D_DSMCC_DSIMSGID >> 8) & 0xFF);
    HI_SET_MASK(stFilterAttr, 9, (SSU_D_DSMCC_DSIMSGID) & 0xFF);

    s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
        return s32Ret;
    }

    s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer, s_stAcquisition.u32SectionBufLen, &u32SectionLen);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
        return s32Ret;
    }

    s32Ret = SSU_ParseDSI(s_stAcquisition.pu8SectionBuffer, (HI_U16)u32SectionLen, pstDSI);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ParseDSI, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;

}

HI_S32 SSU_ACQ_GetDIIInfo(SSU_GII_S* pstGII, SSU_DII_S* pstDII)
{
    HI_U32 u32SectionLen = 0;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstGII);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDII);

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));
    HI_SET_MASK(stFilterAttr,  0, (SSU_D_DSMCC_DIITABID));
    HI_SET_MASK(stFilterAttr,  6, (SSU_D_DSMCC_PROTOCOL));
    HI_SET_MASK(stFilterAttr,  7, (SSU_D_DSMCC_TYPE));
    HI_SET_MASK(stFilterAttr,  8, (SSU_D_DSMCC_DIIMSGID >> 8) & 0xFF);
    HI_SET_MASK(stFilterAttr,  9, (SSU_D_DSMCC_DIIMSGID) & 0xFF);
    HI_SET_MASK(stFilterAttr, 10, (pstGII->u32GroupID >> 24) & 0xFF);
    HI_SET_MASK(stFilterAttr, 11, (pstGII->u32GroupID >> 16) & 0xFF);
    HI_SET_MASK(stFilterAttr, 12, (pstGII->u32GroupID >> 8) & 0xFF);
    HI_SET_MASK(stFilterAttr, 13, (pstGII->u32GroupID) & 0xFF);

    s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
        return s32Ret;
    }

    s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer, s_stAcquisition.u32SectionBufLen, &u32SectionLen);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
        return s32Ret;
    }

    s32Ret = SSU_ParseDII(s_stAcquisition.pu8SectionBuffer, (HI_U16)u32SectionLen, pstDII);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ParseDII, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 SSU_ACQ_GetUpgradeData
(
    HI_U8*                  pu8MemBuff,
    HI_U32                  u32MemSize,
    SSU_GII_S*              pstGII,
    SSU_DII_S*              pstDII
)
{
    HI_U32 i = 0;
    HI_U32 u32SectionLen = 0;
    HI_U32 u32Index = 0;
    HI_U32 u32ImageNum = 0;
    SSU_DDB_S stDDB;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    COM_BUFFER_INITPARAM_S stBufferInitParam;
    COM_BUFFER_DATABLOCK_S astData[SSU_D_DSMCC_MAXMODULENUM];
    HI_U32 au32CalculateCRC[SSU_D_DSMCC_MAXMODULENUM] = {0};
    COM_BUFFER_STATUS_E enDLStatus = COM_BUFFER_E_STATUS_NORMAL;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pu8MemBuff);
    COM_CHECK_PARAM(HI_NULL_PTR == pstGII);
    COM_CHECK_PARAM(HI_NULL_PTR == pstDII);
    COM_CHECK_PARAM(0 == u32MemSize);

    COM_MemSet(&stFilterAttr, 0x00, sizeof(stFilterAttr));
    COM_MemSet(stFilterAttr.au8Mask, 0xFF, sizeof(stFilterAttr.au8Mask));
    HI_SET_MASK(stFilterAttr,  0, (SSU_D_DSMCC_DDBTABID));
    HI_SET_MASK(stFilterAttr,  1, (pstGII->u32GroupID & 0xFF));
    HI_SET_MASK(stFilterAttr,  6, (SSU_D_DSMCC_PROTOCOL));
    HI_SET_MASK(stFilterAttr,  7, (SSU_D_DSMCC_TYPE));
    HI_SET_MASK(stFilterAttr,  8, (SSU_D_DSMCC_DDBMSGID >> 8) & 0xFF);
    HI_SET_MASK(stFilterAttr,  9, (SSU_D_DSMCC_DDBMSGID) & 0xFF);

    s32Ret = DOWNLOAD_OTA_SetFilter(&stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(DOWNLOAD_OTA_SetFilter, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&stBufferInitParam, 0x00, sizeof(stBufferInitParam));
    stBufferInitParam.pu8MemBuff   = pu8MemBuff;
    stBufferInitParam.u32MemSize   = u32MemSize;
    stBufferInitParam.u32BlockSize = pstDII->u16BlockSize;
    stBufferInitParam.u32ModuleNum = pstDII->u16NumberOfModules;

    for (i = 0; i < pstDII->u16NumberOfModules; i++)
    {
        stBufferInitParam.astModuleBuf[i].u32ModuleID   = pstDII->pstModules[i].u16ModuleID;
        stBufferInitParam.astModuleBuf[i].u32ModuleSize = pstDII->pstModules[i].u32ModuleSize;
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
        s32Ret = DOWNLOAD_OTA_GetData(s_stAcquisition.pu8SectionBuffer, s_stAcquisition.u32SectionBufLen, &u32SectionLen);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(DOWNLOAD_OTA_GetData, s32Ret);
            COM_BUFFER_DeInit();
            return s32Ret;
        }

        COM_MemSet(&stDDB, 0x00, sizeof(stDDB));
        s32Ret = SSU_ParseDDB(s_stAcquisition.pu8SectionBuffer, (HI_U16)u32SectionLen, &stDDB);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(SSU_ParseDDB, s32Ret);
            COM_BUFFER_DeInit();
            return s32Ret;
        }

        s32Ret = ssuGetIndexByModuleID(stDDB.u16ModuleId, pstDII, &u32Index);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(ssuGetIndexByModuleID, s32Ret);
            COM_BUFFER_DeInit();
            return s32Ret;
        }

        s32Ret = COM_BUFFER_PutData(u32Index, stDDB.u16BlockNumber, stDDB.puBlockData, stDDB.u16BlockSize);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(COM_BUFFER_PutData, s32Ret);
            COM_BUFFER_DeInit();
            return s32Ret;
        }

        if (HI_NULL_PTR != s_stAcquisition.pfnProcessCallback)
        {
            COM_CHECK(s_stAcquisition.pfnProcessCallback(COM_BUFFER_GetSize(), pstGII->u32GroupSize));
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
            s32Ret = COM_BUFFER_GetData(astData, SSU_D_DSMCC_MAXMODULENUM, &u32ImageNum);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(COM_BUFFER_GetData, s32Ret);
                COM_BUFFER_DeInit();
                return s32Ret;
            }

            for (i = 0; i < u32ImageNum; i++)
            {
                s32Ret = ssuGetIndexByModuleID((HI_U16)astData[i].u32ModuleID, pstDII, &u32Index);
                if (HI_SUCCESS != s32Ret)
                {
                    COM_BUFFER_DeInit();
                    COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                    return COM_ERR_IMAGE_ILLEGAL;
                }

                au32CalculateCRC[u32Index] = COM_CRC32(au32CalculateCRC[u32Index], astData[i].pu8DataBuff, astData[i].u32DataSize);

                if (HI_TRUE == astData[i].bLastData)
                {
                    if (au32CalculateCRC[u32Index] != pstDII->pstModules[u32Index].u32CRC32)
                    {
                        COM_BUFFER_DeInit();

                        COM_PrintErrCode(COM_ERR_IMAGE_ILLEGAL);
                        return COM_ERR_IMAGE_ILLEGAL;
                    }
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

    COM_BUFFER_DeInit();

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


