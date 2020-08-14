/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol_ssu.c
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
#include "ssu_parse.h"
#include "ssu_acquisition.h"
#include "protocol_ssu.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static HI_S32               s_u32InitCount = 0;
static SSU_DSI_S            s_stDSIInfo;
static SSU_DII_S            s_stDIIInfo;


HI_S32 PROT_SSU_Init
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

    s32Ret = SSU_ACQ_Init(pstLoaderParam, pfnDataCallback, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ACQ_Init, s32Ret);
        return s32Ret;
    }

    COM_MemSet(&s_stDSIInfo, 0x00, sizeof(s_stDSIInfo));
    COM_MemSet(&s_stDIIInfo, 0x00, sizeof(s_stDIIInfo));

    s_u32InitCount ++;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSU_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    s_u32InitCount --;

    if (0 != s_u32InitCount)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    SSU_FreeDSI(&s_stDSIInfo);
    SSU_FreeDII(&s_stDIIInfo);

    COM_CHECK(SSU_ACQ_DeInit());

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSU_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    SSU_GII_S* pstGroup = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    s32Ret = SSU_ACQ_GetDSIInfo(&s_stDSIInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ACQ_GetDSIInfo, s32Ret);
        return s32Ret;
    }

    pstGroup = &s_stDSIInfo.pstGroups[0];
    pstVersionInfo->u32ManufacturerID    = pstGroup->u32ManufacturerID;
    pstVersionInfo->u32HardwareVersion   = pstGroup->u32HardwareVersion;
    pstVersionInfo->u32SoftwareVersion   = pstGroup->u32SoftwareVersion;
    pstVersionInfo->u32SerialNumberStart = pstGroup->u32SerialNumberStart;
    pstVersionInfo->u32SerialNumberEnd   = pstGroup->u32SerialNumberEnd;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSU_GetPartitionInfo
(
    PROT_PARTITION_INFO_S*   pstBuffer,
    HI_U32                   u32BufNum,
    HI_U32*                  pu32PartNum
)
{
    HI_U16 i = 0;
    SSU_GII_S* pstGroup = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    COM_CHECK_PARAM(HI_NULL_PTR == pstBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    pstGroup = &s_stDSIInfo.pstGroups[0];
    s32Ret = SSU_ACQ_GetDIIInfo(pstGroup, &s_stDIIInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ACQ_GetDIIInfo, s32Ret);
        return s32Ret;
    }

    if (u32BufNum < s_stDIIInfo.u16NumberOfModules)
    {
        COM_PrintErrCode(COM_ERR_OVERFLOW);
        return COM_ERR_OVERFLOW;
    }

    for (i = 0; i < s_stDIIInfo.u16NumberOfModules; i++)
    {
        pstBuffer[i].u32PartitionId     = s_stDIIInfo.pstModules[i].u16ModuleID;
        pstBuffer[i].u32FlashType       = s_stDIIInfo.pstModules[i].u32FlashType;
        pstBuffer[i].u32FlashIndex      = s_stDIIInfo.pstModules[i].u32FlashIndex;
        pstBuffer[i].u64FlashStartAddr  = s_stDIIInfo.pstModules[i].u64FlashStartAddr;
        pstBuffer[i].u64FlashEndAddr    = s_stDIIInfo.pstModules[i].u64FlashEndAddr;
        pstBuffer[i].u32ImageSize       = s_stDIIInfo.pstModules[i].u32ModuleSize;
    }

    *pu32PartNum = s_stDIIInfo.u16NumberOfModules;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 PROT_SSU_Process(HI_VOID)
{
    HI_U32 u32UsableMem = 0;
    HI_U8* pu8MemBuff = HI_NULL_PTR;
    SSU_GII_S* pstGroup = HI_NULL_PTR;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_INITED(s_u32InitCount);

    pstGroup = &s_stDSIInfo.pstGroups[0];
    pu8MemBuff = COM_MemGetUsable(pstGroup->u32GroupSize, &u32UsableMem);
    if (HI_NULL_PTR == pu8MemBuff)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        return COM_ERR_MEM;
    }

    s32Ret = SSU_ACQ_GetUpgradeData(pu8MemBuff, u32UsableMem, pstGroup, &s_stDIIInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(SSU_ACQ_GetUpgradeData, s32Ret);
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

