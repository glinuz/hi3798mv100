/**************************************************************************//**

  Copyright (C), 2001-2016, Huawei Tech. Co., Ltd.

 ******************************************************************************
/ * @file    sme_init_gst.cpp
 * @brief   init gstreamer
 * @author
 * @date    2017/7/15
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2017/7/15
 * Author :
 * Desc   : Created file
 *
******************************************************************************/

#include "sme_init.h"
#include "osal_str.h"
#include "osal_thread.h"


#include "gst.h"
#include "sme_log_adpt.h"
#include "sme_log.h"
#include "sme_macro.h"

//lint -e10   //va_list
//lint -e717 //do{}while(0)
//lint -e774   //Boolean within 'if' always evaluates to False
//lint -esym(438, pobjSmePlayerGstWrPipe, pppArgv, pszGstUri, pszGstSubUri)  //not used

#define M_SME_PLAYER_NAME "SmePlayerGst"

#if !defined (__LINUX__)
#define M_SME_GST_PLUGIN_PATH "--gst-plugin-path=/system/lib/gstplugins"
#else
#define M_SME_GST_PLUGIN_PATH "--gst-plugin-path=/usr/lib/gstplugins"
#endif
#define M_SME_GST_DISABLE_FORK "--gst-disable-registry-fork"

static const V_CHAR* g_spszGstArgs[]={
    M_SME_GST_DISABLE_FORK,
    M_SME_GST_PLUGIN_PATH
};

static void EnableLog(GstDebugLevel level, GstLogFunction func)
{
    gst_debug_add_log_function (func, NULL, NULL);
    gst_debug_set_default_threshold (level);

    return;
}
static ST_VOS_THD_MUTEX g_sstInitMutex = VOS_PTHREAD_MUTEX_INITIALIZER;
static V_BOOL g_sbIsInited = ICS_FALSE;

V_UINT32 SmeInit(IN E_SME_PLAYER_LOG_LEVEL eLevel)
{

    V_UINT32    u32Ret = static_cast<V_UINT32>(E_SME_PLAYER_ERROR_NONE);
    gchar***    pppArgv = NULL;

#ifdef SME_RLS
    eLevel = E_SME_PLAYER_LOG_LEVEL_ERROR;
#endif

    VOS_ThdMutexLock(&g_sstInitMutex);
    if(g_sbIsInited){
        VOS_ThdMutexUnLock(&g_sstInitMutex);
        ICS_LOGE("already inited !");
        return ICS_SUCCESS;
    }
    ICS_SetLogLevel(SMELogLevelToICSLogLevel(eLevel));

    ICS_LOGE("SmeInit in, eLevel=%d", eLevel);

    do
    {
        V_INT32 argc = sizeof(g_spszGstArgs)/sizeof(g_spszGstArgs[0]) + 1;
        ICS_LOGD("InitGst argc=%d", argc);
        pppArgv = (gchar***)malloc(sizeof(gchar**));
        UTILS_MLOGE_BRK_VAL_IF(NULL == pppArgv, u32Ret, E_SME_PLAYER_ERROR_NOMEM,
            ("malloc failed"));

        *pppArgv = (gchar**)malloc((guint)argc * sizeof(gchar*));
        UTILS_MLOGE_BRK_VAL_IF(NULL == *pppArgv, u32Ret, E_SME_PLAYER_ERROR_NOMEM,
            ("malloc failed"));

        (*pppArgv)[0] = const_cast<gchar*>(M_SME_PLAYER_NAME);
        for(V_INT32 j = 1; j < argc; j++)
        {
            (*pppArgv)[j] = const_cast<gchar*>(g_spszGstArgs[j-1]);
        }

        if(!gst_is_initialized())
        {
            EnableLog(SmeLogLevelToGstLogLevel(eLevel), SmePlayerGstLogMap);
            gst_init (&argc, pppArgv);
        }
        g_sbIsInited = ICS_TRUE;
    }while(ICS_FALSE);
    VOS_ThdMutexUnLock(&g_sstInitMutex);

    if(pppArgv != NULL)
    {
        UTILS_MSAFEFREE(*pppArgv);
        UTILS_MSAFEFREE(pppArgv);
    }
    ICS_LOGE("SmeInit out,u32Ret=%u", u32Ret);
    return u32Ret;
}

V_VOID SmeDeInit()
{
    ;//do nothing.
}



