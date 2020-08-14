/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : download.c
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
#include "download.h"

#ifdef HI_LOADER_MODE_USB
#include "download_fs.h"
#endif

#ifdef HI_LOADER_MODE_FS
#include "download_fs.h"
#endif

#ifdef HI_LOADER_MODE_IP
#include "download_ip.h"
#endif

#ifdef HI_LOADER_MODE_OTA
#include "download_ota.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef HI_S32 (*DOWNLOAD_FN_INIT)(HI_LOADER_PARAM_S* pstLoaderParam);
typedef HI_S32 (*DOWNLOAD_FN_DEINIT)(HI_VOID);
typedef HI_S32 (*DOWNLOAD_FN_GETDATA)(HI_VOID* pDataBuffer, HI_U32 u32BufferSize, HI_U32* pu32OutSize);
typedef HI_S32 (*DOWNLOAD_FN_GETFILESIZE)(HI_U32* pu32FileSize);

typedef struct tagDOWNLOAD_ROUTER_S
{
    HI_LOADER_UPGRADE_TYPE_E    enUpgradeType;
    DOWNLOAD_FN_INIT            pfnInit;
    DOWNLOAD_FN_DEINIT          pfnDeInit;
    DOWNLOAD_FN_GETDATA         pfnGetData;
    DOWNLOAD_FN_GETFILESIZE     pfnGetFileSize;

} DOWNLOAD_ROUTER_S;


static DOWNLOAD_ROUTER_S s_stRouterPool[] =
{
#ifdef HI_LOADER_MODE_IP
    {
        HI_LOADER_UPGRADE_TYPE_IP,
        DOWNLOAD_IP_Init,
        DOWNLOAD_IP_DeInit,
        DOWNLOAD_IP_Getdata,
        DOWNLOAD_IP_GetFileSize
    },
#endif
#ifdef HI_LOADER_MODE_USB
    {
        HI_LOADER_UPGRADE_TYPE_USB,
        DOWNLOAD_FS_Init,
        DOWNLOAD_FS_DeInit,
        DOWNLOAD_FS_Getdata,
        DOWNLOAD_FS_GetFileSize
    },
#endif
#ifdef HI_LOADER_MODE_FS
    {
        HI_LOADER_UPGRADE_TYPE_FS,
        DOWNLOAD_FS_Init,
        DOWNLOAD_FS_DeInit,
        DOWNLOAD_FS_Getdata,
        DOWNLOAD_FS_GetFileSize
    },
#endif
#ifdef HI_LOADER_MODE_OTA
    {
        HI_LOADER_UPGRADE_TYPE_OTA,
        DOWNLOAD_OTA_Init,
        DOWNLOAD_OTA_DeInit,
        DOWNLOAD_OTA_GetData,
        HI_NULL_PTR
    },
#endif
    {
        HI_LOADER_UPGRADE_TYPE_BUTT,
        HI_NULL_PTR,
        HI_NULL_PTR,
        HI_NULL_PTR,
        HI_NULL_PTR
    }
};

static DOWNLOAD_ROUTER_S* s_pstRouterIns = HI_NULL_PTR;

HI_S32 DOWNLOAD_Init(HI_LOADER_PARAM_S* pstLoaderParam)
{
    HI_U32 i = 0;
    HI_U32 u32RoutNum = 0;
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pstLoaderParam);

    s_pstRouterIns = HI_NULL_PTR;

    u32RoutNum = sizeof(s_stRouterPool) / sizeof(DOWNLOAD_ROUTER_S) - 1;

    for (i = 0; i < u32RoutNum; i++)
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

    s32Ret = s_pstRouterIns->pfnInit(pstLoaderParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnInit, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_DeInit(HI_VOID)
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

HI_S32 DOWNLOAD_Getdata(HI_VOID* pDataBuffer, HI_U32 u32BufferSize, HI_U32* pu32OutSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pDataBuffer);
    COM_CHECK_PARAM(HI_NULL_PTR == pu32OutSize);
    COM_CHECK_PARAM(0 == u32BufferSize);

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnGetData))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnGetData(pDataBuffer, u32BufferSize, pu32OutSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnGetData, s32Ret);
        return s32Ret;
    }

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 DOWNLOAD_GetFileSize(HI_U32* pu32FileSize)
{
    HI_S32 s32Ret = HI_FAILURE;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pu32FileSize);

    if ((HI_NULL_PTR == s_pstRouterIns)
        || (HI_NULL_PTR == s_pstRouterIns->pfnGetData))
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return COM_ERR_UNAVAILABLE;
    }

    s32Ret = s_pstRouterIns->pfnGetFileSize(pu32FileSize);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(s_pstRouterIns->pfnGetFileSize, s32Ret);
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

