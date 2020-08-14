/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : protocol.c
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
#include "protocol.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef HI_ADVCA_SUPPORT

#if defined HI_LOADER_PROTOCOL_HISI_OTA
#include "protocol_ssd_hiota.h"
#elif defined HI_LOADER_PROTOCOL_SSU_OTA
#include "protocol_ssd_ssu.h"
#endif

#ifdef HI_LOADER_PROTOCOL_HISI_FILE
#include "protocol_ssd_hifile.h"
#endif

#else

#if defined HI_LOADER_PROTOCOL_HISI_OTA
#include "protocol_hiota.h"
#elif defined HI_LOADER_PROTOCOL_SSU_OTA
#include "protocol_ssu.h"
#endif

#ifdef HI_LOADER_PROTOCOL_HISI_FILE
#include "protocol_hifile.h"
#endif

#endif


typedef struct tagPROTOCOL_ROUTER_S
{
    HI_LOADER_UPGRADE_TYPE_E enUpgradeType;
    PROT_FN_INIT             pfnInit;
    PROT_FN_DEINIT           pfnDeInit;
    PROT_FN_GETVERSIONINFO   pfnGetVersionInfo;
    PROT_FN_GETPARTITIONINFO pfnGetPartitionInfo;
    PROT_FN_PROCESS          pfnProcess;

} PROTOCOL_ROUTER_S;

static PROTOCOL_ROUTER_S s_stRouterPool[] =
{
#ifdef HI_ADVCA_SUPPORT

#if defined HI_LOADER_MODE_OTA
#if defined HI_LOADER_PROTOCOL_HISI_OTA
    {
        HI_LOADER_UPGRADE_TYPE_OTA,
        PROT_SSD_HIOTA_Init,
        PROT_SSD_HIOTA_DeInit,
        PROT_SSD_HIOTA_GetVersionInfo,
        PROT_SSD_HIOTA_GetPartitionInfo,
        PROT_SSD_HIOTA_Process
    },
#elif defined HI_LOADER_PROTOCOL_SSU_OTA
    {
        HI_LOADER_UPGRADE_TYPE_OTA,
        PROT_SSD_SSU_Init,
        PROT_SSD_SSU_DeInit,
        PROT_SSD_SSU_GetVersionInfo,
        PROT_SSD_SSU_GetPartitionInfo,
        PROT_SSD_SSU_Process
    },
#endif
#endif

#ifdef HI_LOADER_MODE_USB
#ifdef HI_LOADER_PROTOCOL_HISI_FILE
    {
        HI_LOADER_UPGRADE_TYPE_USB,
        PROT_SSD_HIFILE_Init,
        PROT_SSD_HIFILE_DeInit,
        PROT_SSD_HIFILE_GetVersionInfo,
        PROT_SSD_HIFILE_GetPartitionInfo,
        PROT_SSD_HIFILE_Process
    },
#endif
#endif

#ifdef HI_LOADER_MODE_FS
#ifdef HI_LOADER_PROTOCOL_HISI_FILE
    {
        HI_LOADER_UPGRADE_TYPE_FS,
        PROT_SSD_HIFILE_Init,
        PROT_SSD_HIFILE_DeInit,
        PROT_SSD_HIFILE_GetVersionInfo,
        PROT_SSD_HIFILE_GetPartitionInfo,
        PROT_SSD_HIFILE_Process
    },
#endif
#endif

#else

#if defined HI_LOADER_MODE_OTA
#if defined HI_LOADER_PROTOCOL_HISI_OTA
    {
        HI_LOADER_UPGRADE_TYPE_OTA,
        PROT_HIOTA_Init,
        PROT_HIOTA_DeInit,
        PROT_HIOTA_GetVersionInfo,
        PROT_HIOTA_GetPartitionInfo,
        PROT_HIOTA_Process
    },
#elif defined HI_LOADER_PROTOCOL_SSU_OTA
    {
        HI_LOADER_UPGRADE_TYPE_OTA,
        PROT_SSU_Init,
        PROT_SSU_DeInit,
        PROT_SSU_GetVersionInfo,
        PROT_SSU_GetPartitionInfo,
        PROT_SSU_Process
    },
#endif
#endif

#ifdef HI_LOADER_MODE_USB
#ifdef HI_LOADER_PROTOCOL_HISI_FILE
    {
        HI_LOADER_UPGRADE_TYPE_USB,
        PROT_HIFILE_Init,
        PROT_HIFILE_DeInit,
        PROT_HIFILE_GetVersionInfo,
        PROT_HIFILE_GetPartitionInfo,
        PROT_HIFILE_Process
    },
#endif
#endif

#ifdef HI_LOADER_MODE_FS
#ifdef HI_LOADER_PROTOCOL_HISI_FILE
    {
        HI_LOADER_UPGRADE_TYPE_FS,
        PROT_HIFILE_Init,
        PROT_HIFILE_DeInit,
        PROT_HIFILE_GetVersionInfo,
        PROT_HIFILE_GetPartitionInfo,
        PROT_HIFILE_Process
    },
#endif
#endif

#ifdef HI_LOADER_MODE_IP
#ifdef HI_LOADER_PROTOCOL_HISI_FILE
    {
        HI_LOADER_UPGRADE_TYPE_IP,
        PROT_HIFILE_Init,
        PROT_HIFILE_DeInit,
        PROT_HIFILE_GetVersionInfo,
        PROT_HIFILE_GetPartitionInfo,
        PROT_HIFILE_Process
    },
#endif
#endif

#endif
};

static PROTOCOL_ROUTER_S* s_pstRouterIns = HI_NULL_PTR;

HI_S32 LOADER_PROTOCOL_Init
(
    HI_LOADER_PARAM_S*      pstLoaderParam,
    PROT_DATA_CALLBACK      pfnDataCallback,
    PROT_PROCESS_CALLBACK   pfnProgressCallback
)
{
    HI_U32 i = 0;
    HI_U32 u32ProtNum = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);
    COM_CHECK_PARAM(HI_NULL_PTR == pfnDataCallback);

    if (HI_NULL_PTR != s_pstRouterIns)
    {
        COM_DBG_FuncExit();
        return HI_SUCCESS;
    }

    u32ProtNum = sizeof(s_stRouterPool) / sizeof(PROTOCOL_ROUTER_S);

    for (i = 0; i < u32ProtNum; i++)
    {
        if (pstLoaderParam->enUpgradeType == s_stRouterPool[i].enUpgradeType)
        {
            s_pstRouterIns = &s_stRouterPool[i];
        }
    }

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnInit))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnInit(pstLoaderParam, pfnDataCallback, pfnProgressCallback);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnInit, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LOADER_PROTOCOL_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnDeInit))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnDeInit();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnDeInit, s32Ret);
        return s32Ret;
    }

    s_pstRouterIns = HI_NULL_PTR;

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LOADER_PROTOCOL_GetVersionInfo(PROT_VERSION_INFO_S* pstVersionInfo)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstVersionInfo);

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnGetVersionInfo))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnGetVersionInfo(pstVersionInfo);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnGetVersionInfo, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LOADER_PROTOCOL_GetPartitionInfo(PROT_PARTITION_INFO_S* pstPartInfo, HI_U32 u32BufNum, HI_U32* pu32PartNum)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstPartInfo);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32PartNum);
    COM_CHECK_PARAM(0 == u32BufNum);

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnGetPartitionInfo))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnGetPartitionInfo(pstPartInfo, u32BufNum, pu32PartNum);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnGetPartitionInfo, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 LOADER_PROTOCOL_Process(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnProcess))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnProcess();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnProcess, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

