/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_log_adpt.h
 * @brief   支持设置日志回调
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
#ifndef __SME_LOG_ADPT_H__
#define __SME_LOG_ADPT_H__

#include "osal_type.h"
#include "sme_player_type.h"
#include "sme_dvbplayer_type.h"
#include "sme_media_type.h"
#include "sme_log.h"
#include "gst.h"

#ifndef MOD_NAME
#define MOD_NAME "SME_LOG_ADPT"
#endif

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define SME_LOGV_ADPT(pszFmt...) SME_LogAdpt(pstThisLogCtx, MOD_NAME, E_SME_PLAYER_LOG_LEVEL_VERBOSE, \
    __FUNCTION__, __FILE__, __LINE__, ##pszFmt)
#define SME_LOGD_ADPT(pszFmt...) SME_LogAdpt(pstThisLogCtx, MOD_NAME, E_SME_PLAYER_LOG_LEVEL_DEBUG, \
    __FUNCTION__, __FILE__, __LINE__, ##pszFmt)
#define SME_LOGI_ADPT(pszFmt...) SME_LogAdpt(pstThisLogCtx, MOD_NAME, E_SME_PLAYER_LOG_LEVEL_INFO, \
    __FUNCTION__, __FILE__, __LINE__, ##pszFmt)
#define SME_LOGW_ADPT(pszFmt...) SME_LogAdpt(pstThisLogCtx, MOD_NAME, E_SME_PLAYER_LOG_LEVEL_WARNING, \
    __FUNCTION__, __FILE__, __LINE__, ##pszFmt)
#define SME_LOGE_ADPT(pszFmt...) SME_LogAdpt(pstThisLogCtx, MOD_NAME, E_SME_PLAYER_LOG_LEVEL_ERROR,\
    __FUNCTION__, __FILE__, __LINE__, ##pszFmt)

#ifdef ICS_LOG_PRINTV
#undef ICS_LOG_PRINTV
#endif
#define ICS_LOG_PRINTV SME_LOGV_ADPT

#ifdef ICS_LOG_PRINTD
#undef ICS_LOG_PRINTD
#endif
#define ICS_LOG_PRINTD SME_LOGD_ADPT

#ifdef ICS_LOG_PRINTI
#undef ICS_LOG_PRINTI
#endif
#define ICS_LOG_PRINTI SME_LOGI_ADPT

#ifdef ICS_LOG_PRINTW
#undef ICS_LOG_PRINTW
#endif
#define ICS_LOG_PRINTW SME_LOGW_ADPT

#ifdef ICS_LOG_PRINTE
#undef ICS_LOG_PRINTE
#endif
#define ICS_LOG_PRINTE SME_LOGE_ADPT


V_VOID SME_LogAdpt(IN const ST_SME_LOG_CB* pstLogCtx,
    IN const V_CHAR* pszTag,
    IN V_INT32 i32Level,
    IN const V_CHAR* pszFunc,
    IN const V_CHAR* pszFile,
    IN V_UINT32 u32Line,
    IN const V_CHAR* pszFmt,...);

E_ICS_LOG_LEVEL
SMEDVBLogLevelToICSLogLevel(IN E_SME_DVBPLAYER_LOG_LEVEL eLevel);
GstDebugLevel
SmeDVBLogLevelToGstLogLevel(E_SME_DVBPLAYER_LOG_LEVEL eLevel);

E_ICS_LOG_LEVEL SMELogLevelToICSLogLevel(IN E_SME_PLAYER_LOG_LEVEL eLevel);
E_SME_PLAYER_LOG_LEVEL ICSLogLevelToSMELogLevel(IN E_ICS_LOG_LEVEL eLevel);

GstDebugLevel SmeLogLevelToGstLogLevel(E_SME_PLAYER_LOG_LEVEL eLevel);
void SmePlayerGstLogMap(GstDebugCategory * category,
    GstDebugLevel      level,
    const gchar      * file,
    const gchar      * function,
    gint               line,
    GObject          * object,
    GstDebugMessage  * message,
    gpointer           user_data);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SME_LOG_ADPT_H__
