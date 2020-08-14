/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : com_debug.c
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef HI_LOADER_APPLOADER

HI_S32 COM_LogSystemInit(HI_VOID)
{
    HI_CHAR* pcEnv = HI_NULL_PTR;
    HI_LOG_LEVEL_E enLogLevel = HI_LOG_LEVEL_INFO;

    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_SYS_Init();
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    s32Ret = HI_MODULE_Register(HI_ID_LOADER, "HI_LOADER");
    if (HI_SUCCESS != s32Ret)
    {
        HI_SYS_DeInit();
        return s32Ret;
    }

    pcEnv = (HI_CHAR*)getenv("loglevel");
    if (HI_NULL_PTR != pcEnv)
    {
        HI_S32 s32Value = atoi(pcEnv);

        if (HI_LOG_LEVEL_BUTT > s32Value)
        {
            enLogLevel = (HI_LOG_LEVEL_E)s32Value;
        }
    }

    s32Ret = HI_SYS_SetLogLevel(HI_ID_LOADER, enLogLevel);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_SYS_SetLogLevel, s32Ret);

        HI_MODULE_UnRegister(HI_ID_LOADER);
        HI_SYS_DeInit();
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 COM_LogSystemDeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_MODULE_UnRegister(HI_ID_LOADER);
    COM_CheckFuncErr(HI_MODULE_UnRegister, s32Ret);

    s32Ret = HI_SYS_DeInit();
    COM_CheckFuncErr(HI_SYS_DeInit, s32Ret);

    return HI_SUCCESS;
}

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

