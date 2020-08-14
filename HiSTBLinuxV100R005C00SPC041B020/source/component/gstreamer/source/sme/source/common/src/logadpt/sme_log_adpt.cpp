/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_log_adpt.cpp
 * @brief   sme log adpt
 * @author
 * @date    2014/3/29
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/29
 * Author :
 * Desc   : Created file
 *
******************************************************************************/
#include "osal_str.h"
#include "sme_log_adpt.h"
#include "sme_log.h"
#include "sme_macro.h"


//lint -e40 //不识别系统类型va_list
//lint -e10 //可变参数...
//lint -e601 //va_list不识别导致变量类型不识别
//lint -e1784 //Extern 'C'
//lint -e1055  //va_start va_end不识别
//lint -e808
//lint -e1013
//lint -e49
//lint -e522
//lint -esym(530, stVarArgs)
/*lint -e838*/ // initial
//lint -e754   //message of _tag_DebugMessage is not used

#define SME_LOG_ADPT_BUF_SIZE 2048
#define SME_GST_LOG_BUF_SIZE ICS_LOG_BUF_SIZE


typedef struct _tag_DebugMessage
{
  gchar *message;
  const gchar *format;
  va_list arguments;
}MyDebugMessage;


//内部接口
static V_VOID SME_DefaultLog(IN V_VOID* pvCtx, IN const V_CHAR* pszTag,
    IN V_INT32 i32Level,
    IN const V_CHAR* pszFunc,
    IN const V_CHAR* pszFile,
    IN V_UINT32 u32Line,
    const V_CHAR* pszFmt,
    va_list stValist)
{
    char acBuf[SME_LOG_ADPT_BUF_SIZE] = {0,};
    V_INT32 i32PrintStrRes = 0;
    (V_VOID)pvCtx;

    i32PrintStrRes = VOS_Vsnprintf_S(acBuf, sizeof(acBuf) - 1,
        sizeof(acBuf) - 2, pszFmt, stValist);
    if(i32PrintStrRes <= 0)
    {
        ICS_Log(ICS_LOG_WARNING, pszTag, pszFile, pszFunc, u32Line, "%s",
            "SME_DefaultLog VOS_Vsnprintf_S failed !");
    }

    switch(i32Level)
    {
        case E_SME_PLAYER_LOG_LEVEL_VERBOSE:
            ICS_Log(ICS_LOG_VERBOSE, pszTag, pszFile, pszFunc, u32Line, "%s", acBuf);
            break;
        case E_SME_PLAYER_LOG_LEVEL_DEBUG:
            ICS_Log(ICS_LOG_DEBUG, pszTag, pszFile,pszFunc,  u32Line, "%s", acBuf);
            break;
        case E_SME_PLAYER_LOG_LEVEL_INFO:
            ICS_Log(ICS_LOG_INFO, pszTag, pszFile, pszFunc, u32Line, "%s", acBuf);
            break;
        case E_SME_PLAYER_LOG_LEVEL_WARNING:
            ICS_Log(ICS_LOG_WARNING, pszTag, pszFile, pszFunc, u32Line, "%s", acBuf);
            break;
        case E_SME_PLAYER_LOG_LEVEL_ERROR:
            ICS_Log(ICS_LOG_ERROR, pszTag, pszFile, pszFunc, u32Line, "%s", acBuf);
            break;
        default:
            ICS_Log(ICS_LOG_DEBUG, pszTag, pszFile, pszFunc, u32Line, "%s", acBuf);
            break;
    }

    return ;
}

//外部接口
V_VOID SME_LogAdpt(IN const ST_SME_LOG_CB* pstLogCtx,
    IN const V_CHAR* pszTag,
    IN V_INT32 i32Level,
    IN const V_CHAR* pszFunc,
    IN const V_CHAR* pszFile,
    IN V_UINT32 u32Line,
    IN const V_CHAR* pszFmt,...)
{
    va_list stVarArgs;

    va_start (stVarArgs, pszFmt);
    if(NULL != pstLogCtx && NULL != pstLogCtx->pfCb)
    {
        pstLogCtx->pfCb(pstLogCtx->pvCtx, pszTag, i32Level, pszFunc, pszFile, u32Line, pszFmt, stVarArgs);
    }
    else
    {
        SME_DefaultLog(NULL, pszTag, i32Level, pszFunc, pszFile, u32Line, pszFmt, stVarArgs);
    }

    va_end (stVarArgs);

    return;
}

E_ICS_LOG_LEVEL
SMEDVBLogLevelToICSLogLevel(IN E_SME_DVBPLAYER_LOG_LEVEL eLevel)
{
    E_ICS_LOG_LEVEL eRet = ICS_LOG_WARNING;

    switch(eLevel)
    {
        case E_SME_DVBPLAYER_LOG_LEVEL_VERBOSE:
            eRet = ICS_LOG_VERBOSE;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_DEBUG:
            eRet = ICS_LOG_DEBUG;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_INFO:
            eRet = ICS_LOG_INFO;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_WARNING:
            eRet = ICS_LOG_WARNING;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_ERROR:
            eRet = ICS_LOG_ERROR;
            break;
        default:
            break;
    }

    return eRet;
}

GstDebugLevel
SmeDVBLogLevelToGstLogLevel(E_SME_DVBPLAYER_LOG_LEVEL eLevel)
{
    GstDebugLevel eGstLevel = GST_LEVEL_NONE;
    switch(eLevel)
    {
        case E_SME_DVBPLAYER_LOG_LEVEL_VERBOSE:
            eGstLevel = GST_LEVEL_LOG;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_DEBUG:
            eGstLevel = GST_LEVEL_DEBUG;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_INFO:
            eGstLevel = GST_LEVEL_INFO;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_WARNING:
            eGstLevel = GST_LEVEL_WARNING;
            break;
        case E_SME_DVBPLAYER_LOG_LEVEL_ERROR:
            eGstLevel = GST_LEVEL_ERROR;
            break;
        default:
            break;
    }

    return eGstLevel;
}

E_ICS_LOG_LEVEL SMELogLevelToICSLogLevel(IN E_SME_PLAYER_LOG_LEVEL eLevel)
{
    E_ICS_LOG_LEVEL eRet = ICS_LOG_WARNING;

    switch(eLevel)
    {
        case E_SME_PLAYER_LOG_LEVEL_VERBOSE:
            eRet = ICS_LOG_VERBOSE;
            break;
        case E_SME_PLAYER_LOG_LEVEL_DEBUG:
            eRet = ICS_LOG_DEBUG;
            break;
        case E_SME_PLAYER_LOG_LEVEL_INFO:
            eRet = ICS_LOG_INFO;
            break;
        case E_SME_PLAYER_LOG_LEVEL_WARNING:
            eRet = ICS_LOG_WARNING;
            break;
        case E_SME_PLAYER_LOG_LEVEL_ERROR:
            eRet = ICS_LOG_ERROR;
            break;
        default:
            break;
    }

    return eRet;
}

E_SME_PLAYER_LOG_LEVEL ICSLogLevelToSMELogLevel(IN E_ICS_LOG_LEVEL eLevel)
{
    E_SME_PLAYER_LOG_LEVEL eRet = E_SME_PLAYER_LOG_LEVEL_INFO;

    switch(eLevel)
    {
        case ICS_LOG_VERBOSE:
            eRet = E_SME_PLAYER_LOG_LEVEL_VERBOSE;
            break;
        case ICS_LOG_DEBUG:
            eRet = E_SME_PLAYER_LOG_LEVEL_DEBUG;
            break;
        case ICS_LOG_INFO:
            eRet = E_SME_PLAYER_LOG_LEVEL_INFO;
            break;
        case ICS_LOG_WARNING:
            eRet = E_SME_PLAYER_LOG_LEVEL_WARNING;
            break;
        case ICS_LOG_ERROR:
            eRet = E_SME_PLAYER_LOG_LEVEL_ERROR;
            break;
        default:
            break;
    }

    return eRet;
}


GstDebugLevel SmeLogLevelToGstLogLevel(E_SME_PLAYER_LOG_LEVEL eLevel)
{
    GstDebugLevel eGstLevel = GST_LEVEL_NONE;
    switch(eLevel)
    {
        case E_SME_PLAYER_LOG_LEVEL_VERBOSE:
            eGstLevel = GST_LEVEL_LOG;
            break;
        case E_SME_PLAYER_LOG_LEVEL_DEBUG:
            eGstLevel = GST_LEVEL_DEBUG;
            break;
        case E_SME_PLAYER_LOG_LEVEL_INFO:
            eGstLevel = GST_LEVEL_INFO;
            break;
        case E_SME_PLAYER_LOG_LEVEL_WARNING:
            eGstLevel = GST_LEVEL_WARNING;
            break;
        case E_SME_PLAYER_LOG_LEVEL_ERROR:
            eGstLevel = GST_LEVEL_ERROR;
            break;
        default:
            break;
    }

    return eGstLevel;
}

void SmePlayerGstLogMap(GstDebugCategory * category,
    GstDebugLevel      level,
    const gchar      * file,
    const gchar      * function,
    gint               line,
    GObject          * object,
    GstDebugMessage  * message,
    gpointer           user_data)
{
    char acBuf[SME_GST_LOG_BUF_SIZE] = {0,};

    UTILS_M_INVALID_ARGS(category);
    UTILS_M_INVALID_ARGS(object);
    UTILS_M_INVALID_ARGS(user_data);

    if(NULL != message)
    {
        MyDebugMessage* pstLogMsg = (MyDebugMessage*)message;
        if(NULL != pstLogMsg->format)
        {
            if (-1 == VOS_Vsnprintf_S(acBuf, SME_GST_LOG_BUF_SIZE, sizeof(acBuf) - 1, pstLogMsg->format,
                pstLogMsg->arguments))
            {
                ICS_LOGE("SECURE: VOS_Vsnprintf_S return failed.");
            }
        }
    }

    switch(level)
    {
        case GST_LEVEL_LOG:
            ICS_Log(ICS_LOG_VERBOSE, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
        case GST_LEVEL_DEBUG:
            ICS_Log(ICS_LOG_DEBUG, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
        case GST_LEVEL_INFO:
            ICS_Log(ICS_LOG_INFO, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
        case GST_LEVEL_WARNING:
            ICS_Log(ICS_LOG_WARNING, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
        case GST_LEVEL_ERROR:
            ICS_Log(ICS_LOG_ERROR, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
        default:
            ICS_Log(ICS_LOG_VERBOSE, MOD_NAME, file, function,
                static_cast<V_UINT32>(line), "%s", acBuf);
            break;
    }

    return;
}
