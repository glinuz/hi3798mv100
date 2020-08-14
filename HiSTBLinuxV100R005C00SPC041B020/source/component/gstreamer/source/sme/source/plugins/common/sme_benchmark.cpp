/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : utils_benchmark.c
  版 本 号   : 初稿
  作    者   : 刘荣亮(l00180035)
  生成日期   : 2011年12月30日
  最近修改   :
  功能描述   : benchamark
  函数列表   :
  修改历史   :
  1.日    期   : 2011年12月30日
    作    者   : 刘荣亮(l00180035)
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "osal_str.h"
#include "osal_mem.h"
#include "sme_log_adpt.h"
#include "sme_macro.h"
#include "sme_benchmark.h"
#include "sme_player_impl_gst.h"
#ifdef _SUPPORT_SME_DVBPLAYER
#include "sme_dvbplayer_impl.h"
#endif
#include "hi_common.h"
#include "hi_type.h"
#include "gstsme.h"


#define SMEPLAYERDIR           "smeplayer"
#define DVBPLAYERDIR           "dvbplayer"
#define CONTROLERENTRYNAME     "control"
#define COMMONENTRYNAME        "common"
#ifdef _SUPPORT_SMEPLAYER
#define SMEDEMUXENTRYNAME      "sme_demux"
#endif
#define HWTSDEMUXENTRYNAME     "hwts_demux"
#define VDECENTRYNAME          "video_decoder"
#define ADECENTRYNAME          "audio_decoder"
#define VSINKENTRYNAME         "video_sink"
#define ASINKENTRYNAME         "audio_sink"


//commod of proc
#define CMD_DUMP_SINK_DATA          "DumpSinkData"
#define CMD_DUMP_SRC_DATA           "DumpSrcData"
#define CMD_DUMP_DEMUX_AUDIO_DATA   "DumpDemuxAudioData"
#define CMD_DUMP_DEMUX_VIDEO_DATA   "DumpDemuxVideoData"
#define CMD_HWTSDMXTUNERID          "HWTSDmxTunerID"
#define CMD_DUMPSTREM_STORE_PATH    "DumpStorePath"
#define CMD_DUMP_START              "start"
#define CMD_DUMP_STOP               "stop"
#define CMD_VSINK_AVSYNC            "VSinkAVSync"
#define CMD_HELP_NAME               "help"
#define CMD_SET_LOG_LEVEL           "SetLogLevel"
#define CMD_FLUSH_LOG               "flushlog"

#define LINUX_DEFAULT_DUMP_DATA_PATH     "/mnt/"
#define ANDROID_DEFAULT_DUMP_DATA_PATH   "/data/"

/* BEGIN: Added for DTS2016042502439, 2016/4/28 */
//lint -e1055   //can not find symbol 'strlen'
//lint -e1778   //folder name and filename need not to define as constants
//lint -e1773   //folder name and filename are not decleared as constants, so discarding constant is not a problem
//lint -e818    //prompt pu8Argv could be define as contants, but we can't modify hisi interface
/* END: Added for DTS2016042502439, 2016/4/28 */

const V_CHAR *PlayerStateName[] ={
    "PLAYING",
    "PAUSE",
    "BUFFERING",
    "STOP",
    "ERROR"
};

const V_CHAR *DvbPlayerStateName[] ={
    "PLAYING",
    "PAUSE",
    "STOP",
    "ERROR"
};

const V_CHAR *PlayerLoop[] ={
    "Player is not loop",
    "Player is loop"
};

static HI_PROC_ENTRY_S stControlEntry;
static HI_PROC_ENTRY_S stCommonEntry;
#ifdef _SUPPORT_SMEPLAYER
static HI_PROC_ENTRY_S stSmeDemuxEntry;
#endif
static HI_PROC_ENTRY_S stTsDemuxEntry;
static HI_PROC_ENTRY_S stVdecEntry;
static HI_PROC_ENTRY_S stAdecEntry;
static HI_PROC_ENTRY_S stVsinkEntry;
static HI_PROC_ENTRY_S stAsinkEntry;
static HI_CHAR *cDirectoryName = NULL;

HI_S32 ProcCommonShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcCommonCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
#ifdef _SUPPORT_SMEPLAYER
static HI_S32 ProcSmeDemuxHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcSmeDemuxShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcSmeDemuxCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
#endif
HI_S32 ProcSmeHWTSDemuxShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcSmeHWTSDemuxCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
static HI_S32 ProcVdecHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcVdecShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcVdecCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
static HI_S32 ProcAdecHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcAdecShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcAdecCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
static HI_S32 ProcVsinkHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcVsinkShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcVsinkCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
static HI_S32 ProcAsinkHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcAsinkShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcAsinkCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
static HI_S32 ProcControlHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf);
HI_S32 ProcControlShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData);
HI_S32 ProcControlCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData);
V_UINT32 _ConvertTime( struct timeval time_sequence);



SmeBenchMark::SmeBenchMark()
:m_pPlayBinHandler(NULL)
,m_pImplHandler(NULL)
,m_ePlayerType(E_SME_PLAYER)
{
    E_SME_MEM_SECURE_RET eMemSecRet;
    V_INT32 macro_len, struct_len;

    eMemSecRet = VOS_Memset_S(&m_stPlayerControlPara, sizeof(m_stPlayerControlPara), 0, sizeof(m_stPlayerControlPara));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stPlayerControlPara Failed"));
    m_stPlayerControlPara.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stCommonInfo, sizeof(m_stCommonInfo), 0, sizeof(m_stCommonInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stCommonInfo Failed"));
    m_stCommonInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stSmeDemuxInfo, sizeof(m_stSmeDemuxInfo), 0, sizeof(m_stSmeDemuxInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stSmeDemuxInfo Failed"));
    m_stSmeDemuxInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stVdecInfo, sizeof(m_stVdecInfo), 0, sizeof(m_stVdecInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stVdecInfo Failed"));
    m_stVdecInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stAdecInfo, sizeof(m_stAdecInfo), 0, sizeof(m_stAdecInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stAdecInfo Failed"));
    m_stAdecInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stVsinkInfo, sizeof(m_stVsinkInfo), 0, sizeof(m_stVsinkInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stVsinkInfo Failed"));
    m_stVsinkInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stAsinkInfo, sizeof(m_stAsinkInfo), 0, sizeof(m_stAsinkInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stAsinkInfo Failed"));
    m_stAsinkInfo.pBenchmarkHandle = this;

    eMemSecRet = VOS_Memset_S(&m_stTsDemuxInfo, sizeof(m_stTsDemuxInfo), 0, sizeof(m_stTsDemuxInfo));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S m_stTsDemuxInfo Failed"));
    m_stTsDemuxInfo.pBenchmarkHandle = this;

#if defined (__LINUX__)
    macro_len = strlen(LINUX_DEFAULT_DUMP_DATA_PATH);
    struct_len = sizeof(m_stPlayerControlPara.cDumpDataStorePath);
    eMemSecRet = VOS_Memcpy_S (m_stPlayerControlPara.cDumpDataStorePath,
            (V_UINT32)(DUMP_FILE_STORE_PATH_LENGTH - 1),
            LINUX_DEFAULT_DUMP_DATA_PATH,
            MIN((V_UINT32)macro_len, (V_UINT32)struct_len));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S LINUX dump data path Failed"));
#endif
/* BEGIN: Added for DTS2016042502439, 2016/4/28 */
#if defined (__ANDROID__)
    macro_len = strlen(ANDROID_DEFAULT_DUMP_DATA_PATH);
    struct_len = sizeof(m_stPlayerControlPara.cDumpDataStorePath);
    eMemSecRet = VOS_Memcpy_S(m_stPlayerControlPara.cDumpDataStorePath,
               (V_UINT32)(DUMP_FILE_STORE_PATH_LENGTH - 1),
               ANDROID_DEFAULT_DUMP_DATA_PATH,
               MIN((V_UINT32)macro_len, (V_UINT32)struct_len));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S ANDROID dump data path Failed"));
#endif
/* END: Added for DTS2016042502439, 2016/4/28 */
}

SmeBenchMark::~SmeBenchMark()
{
    /* BEGIN: Added for DTS2016042502439, 2016/4/28 */
    if (m_pPlayBinHandler)
        m_pPlayBinHandler = NULL;
    if (m_pImplHandler)
        m_pImplHandler = NULL;
    /* END: Added for DTS2016042502439, 2016/4/28 */
}


V_INT32 SmeBenchMark::SME_CreateProcDir()
{
    ICS_LOGI("Begin create player directory ...");
    HI_S32 s32Ret;

    if (m_ePlayerType)
        cDirectoryName = g_strdup(DVBPLAYERDIR);
    else
        cDirectoryName = g_strdup(SMEPLAYERDIR);
    HI_SYS_Init();

    s32Ret = HI_MODULE_Register(HI_ID_USR_START, cDirectoryName);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_MODULE_Register directory failed !"));

    /* Will be added at /proc/hisi/ directory */
    /* TODO:多实例 */
    s32Ret = HI_PROC_AddDir(cDirectoryName);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddDir directory failed !"));

    /* Will be added at /proc/hisi/${DIRNAME} directory */
    stControlEntry.pszDirectory = cDirectoryName;
    stControlEntry.pszEntryName = (HI_CHAR *)CONTROLERENTRYNAME;
    stControlEntry.pfnShowProc = ProcControlShow;
    stControlEntry.pfnCmdProc = ProcControlCmd;
    stControlEntry.pPrivData = (HI_VOID*)&m_stPlayerControlPara;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stControlEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry control file failed !"));

    stCommonEntry.pszDirectory = cDirectoryName;
    stCommonEntry.pszEntryName = (HI_CHAR *)COMMONENTRYNAME;
    stCommonEntry.pfnShowProc = ProcCommonShow;
    stCommonEntry.pfnCmdProc = ProcCommonCmd;
    stCommonEntry.pPrivData = (HI_VOID*)&m_stCommonInfo;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stCommonEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry common file failed !"));

    if (m_ePlayerType) {
        stTsDemuxEntry.pszDirectory = cDirectoryName;
        stTsDemuxEntry.pszEntryName = (HI_CHAR *)HWTSDEMUXENTRYNAME;
        stTsDemuxEntry.pfnShowProc = ProcSmeHWTSDemuxShow;
        stTsDemuxEntry.pfnCmdProc = ProcSmeHWTSDemuxCmd;
        stTsDemuxEntry.pPrivData = (HI_VOID*)&m_stTsDemuxInfo;
        s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stTsDemuxEntry);
        UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry tsdemux file failed !"));
    }
#ifdef _SUPPORT_SMEPLAYER
    else
    {
        stSmeDemuxEntry.pszDirectory = cDirectoryName;
        stSmeDemuxEntry.pszEntryName = (HI_CHAR *)SMEDEMUXENTRYNAME;
        stSmeDemuxEntry.pfnShowProc = ProcSmeDemuxShow;
        stSmeDemuxEntry.pfnCmdProc = ProcSmeDemuxCmd;
        stSmeDemuxEntry.pPrivData = (HI_VOID*)&m_stSmeDemuxInfo;
        s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stSmeDemuxEntry);
        UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry smedemux file failed !"));
    }
#endif
    stVdecEntry.pszDirectory = cDirectoryName;
    stVdecEntry.pszEntryName = (HI_CHAR *)VDECENTRYNAME;
    stVdecEntry.pfnShowProc = ProcVdecShow;
    stVdecEntry.pfnCmdProc = ProcVdecCmd;
    stVdecEntry.pPrivData = (HI_VOID*)&m_stVdecInfo;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stVdecEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry video decoder file failed !"));

    stAdecEntry.pszDirectory = cDirectoryName;
    stAdecEntry.pszEntryName = (HI_CHAR *)ADECENTRYNAME;
    stAdecEntry.pfnShowProc = ProcAdecShow;
    stAdecEntry.pfnCmdProc = ProcAdecCmd;
    stAdecEntry.pPrivData = (HI_VOID*)&m_stAdecInfo;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stAdecEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry video decoder file failed !"));

    stVsinkEntry.pszDirectory = cDirectoryName;
    stVsinkEntry.pszEntryName = (HI_CHAR *)VSINKENTRYNAME;
    stVsinkEntry.pfnShowProc = ProcVsinkShow;
    stVsinkEntry.pfnCmdProc = ProcVsinkCmd;
    stVsinkEntry.pPrivData = (HI_VOID*)&m_stVsinkInfo;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stVsinkEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry video sink file failed !"));

    stAsinkEntry.pszDirectory = cDirectoryName;
    stAsinkEntry.pszEntryName = (HI_CHAR *)ASINKENTRYNAME;
    stAsinkEntry.pfnShowProc = ProcAsinkShow;
    stAsinkEntry.pfnCmdProc = ProcAsinkCmd;
    stAsinkEntry.pPrivData = (HI_VOID*)&m_stAsinkInfo;
    s32Ret = HI_PROC_AddEntry(HI_ID_USR_START, &stAsinkEntry);
    UTILS_MLOGW_IF(HI_SUCCESS != s32Ret, ("HI_PROC_AddEntry audio sink file failed !"));

    ICS_LOGI("End create player directory ...");

    return HI_SUCCESS;
}

V_VOID SmeBenchMark::SME_DestroyBenchMark() const
{
    ICS_LOGI("Begin destroy benchmark...");

    if(cDirectoryName)
    {
        HI_PROC_RemoveEntry(HI_ID_USR_START, &stAsinkEntry);
        ICS_LOGI("Remove proc entry audio_sink successfully. \n");
        HI_PROC_RemoveEntry(HI_ID_USR_START, &stVsinkEntry);
        ICS_LOGI("Remove proc entry video_sink successfully. \n");
        HI_PROC_RemoveEntry(HI_ID_USR_START, &stAdecEntry);
        ICS_LOGI("Remove proc entry audio_decoder successfully. \n");
        HI_PROC_RemoveEntry(HI_ID_USR_START, &stVdecEntry);
        ICS_LOGI("Remove proc entry video_decoder successfully. \n");

        if (m_ePlayerType)
        {
           HI_PROC_RemoveEntry(HI_ID_USR_START, &stTsDemuxEntry);
           ICS_LOGI("Remove proc entry hwts_demux successfully. \n");
        }
#ifdef _SUPPORT_SMEPLAYER
        else
        {
            HI_PROC_RemoveEntry(HI_ID_USR_START, &stSmeDemuxEntry);
            ICS_LOGI("Remove proc entry sme_demux successfully. \n");
        }
#endif

        HI_PROC_RemoveEntry(HI_ID_USR_START, &stCommonEntry);
        ICS_LOGI("Remove proc entry common successfully. \n");

        HI_PROC_RemoveEntry(HI_ID_USR_START, &stControlEntry);
        ICS_LOGI("Remove proc entry control successfully. \n");

        HI_PROC_RemoveDir(cDirectoryName);
        ICS_LOGI("Remove directory %s successfully. \n", cDirectoryName);

        HI_MODULE_UnRegister(HI_ID_USR_START);
        HI_SYS_DeInit();
        g_free(cDirectoryName);
        cDirectoryName = NULL;
    }
    ICS_LOGI("End destroy benchmark...");
}

V_INT32 SmeBenchMark::SME_CreateBenchMark(V_VOID* pPlaybinHandle, V_VOID *pImplHandle, E_PLAYER_TYPE ePlayerType)
{
    ICS_LOGI("Begin create benchmark ...");
    V_INT32 u32Ret = E_SME_PLAYER_ERROR_NONE;

    if (pPlaybinHandle == NULL || pImplHandle == NULL)
        return -1;

    //hls切台，多实例TODO
    if (m_pPlayBinHandler != NULL && m_pImplHandler != NULL) {
        if (m_pImplHandler == pImplHandle && m_pPlayBinHandler != pPlaybinHandle) {
            m_pPlayBinHandler = pPlaybinHandle;
            return u32Ret;
        }
    }

    m_pPlayBinHandler = pPlaybinHandle;
    m_pImplHandler = pImplHandle;
    m_ePlayerType = ePlayerType;

    u32Ret = SME_CreateProcDir();

    ICS_LOGI("End create benchmark.");

    return u32Ret;
}

HI_S32 ProcCommonShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_PROC_COMMON_INFO *pstCommonShowInfo;
#ifdef _SUPPORT_SMEPLAYER
    V_UINT32 u32TrackIdx = 0;
    V_UINT32 u32Err;
#endif
    SmeBenchMark* bench_handle;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    /* BEGIN: Modified for DTS2016042502439, 2016/5/10 */
    pstCommonShowInfo = (ST_PROC_COMMON_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstCommonShowInfo->pBenchmarkHandle);
    /* END: Modified for DTS2016042502439, 2016/5/10 */

    if (bench_handle->m_ePlayerType)
    {
        #ifdef _SUPPORT_SME_DVBPLAYER
        ((SmeDvbPlayerImpl*)(bench_handle->m_pImplHandler))->UpdateDvbProcInfo((ST_PROC_COMMON_INFO *)pPrivData);
        HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Common Info______________________ \n\n");
        HI_PROC_Printf(pstBuf, "PlyaerState:            %s\n", DvbPlayerStateName[pstCommonShowInfo->DvbPlayerState]);
        HI_PROC_Printf(pstBuf, "Resolution-Width:       %lu\n", pstCommonShowInfo->resolution_width);
        HI_PROC_Printf(pstBuf, "Resolution-Height:      %lu\n", pstCommonShowInfo->resolution_height);
        HI_PROC_Printf(pstBuf, "StartTimeMs:            %llu ms\n", pstCommonShowInfo->start_cost_time);
        HI_PROC_Printf(pstBuf, "FccTimeMs:              %llu ms\n", pstCommonShowInfo->fcc_cost_time);
        HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Common Info______________________ \n\n");
        #endif
        return HI_SUCCESS;
    }
    else
    {
    #ifdef _SUPPORT_SMEPLAYER
        u32Err = ((SmePlayerImplGst*)(bench_handle->m_pImplHandler))->UpdateProcControlInfo((ST_PROC_COMMON_INFO *)pPrivData);
        if (u32Err == HI_SUCCESS)
        {
            HI_PROC_Printf(pstBuf, "Print in Controller process %d, %s\n\n", (HI_S32)getpid(), __FUNCTION__);
            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Fragment Info______________________ \n\n");
            if ((*pstCommonShowInfo->fragment_info.i8ManifestUri == '\0') && (*pstCommonShowInfo->fragment_info.i8FragmentUri == '\0'))
                HI_PROC_Printf(pstBuf, "ManifestUrl:            %s\n", pstCommonShowInfo->unique_url);
            else
            {
                HI_PROC_Printf(pstBuf, "ManifestUrl:            %s\n", pstCommonShowInfo->fragment_info.i8ManifestUri);
                HI_PROC_Printf(pstBuf, "FragmentUrl:            %s\n", pstCommonShowInfo->fragment_info.i8FragmentUri);
            }
            HI_PROC_Printf(pstBuf, "FragmentSize:           %.2f MB\n", (V_DOUBLE)(pstCommonShowInfo->fragment_info.u64FragmentSize)/1000000);
            /* BEGIN: Added for DTS2016042509158, 2016/4/27 */
            HI_PROC_Printf(pstBuf, "CurAudioBitrate:        %u\n", pstCommonShowInfo->current_bitrate.audio);
            HI_PROC_Printf(pstBuf, "CurVideoBitrate:        %u\n", pstCommonShowInfo->current_bitrate.video);
            /* END: Added for DTS2016042509158 by, 2016/4/27 */
            HI_PROC_Printf(pstBuf, "FragmentDloadTimeMs:    %u ms\n", (V_DOUBLE)pstCommonShowInfo->fragment_info.u64DownloadTotalTime/1000000);
            HI_PROC_Printf(pstBuf, "FileDurationMs:         %dms\n", pstCommonShowInfo->file_duration);
            HI_PROC_Printf(pstBuf, "CurrentPositionMs:      %dms\n", pstCommonShowInfo->curposition);
            HI_PROC_Printf(pstBuf, "DownloadRate:           %.2f MBps\n", (V_DOUBLE)pstCommonShowInfo->download_rate/(1000000 * 8));
            HI_PROC_Printf(pstBuf, "Resolution-Width:       %lu\n", pstCommonShowInfo->resolution_width);
            HI_PROC_Printf(pstBuf, "Resolution-Height:      %lu\n", pstCommonShowInfo->resolution_height);
            /* BEGIN: Added for DTS2016042509158, 2016/4/27 */
            HI_PROC_Printf(pstBuf, "Note:CurAudioBitrate and CurVideoBitrate have values only when playing HLS or DASH streams,\n");
            HI_PROC_Printf(pstBuf, "at other times the value is 0; Besides, CurAudioBitrate and CurVideoBitrate have equal value \n");
            HI_PROC_Printf(pstBuf, "when playing a interleaved audio and video streaming.\n");
            /* END: Added for DTS2016042509158, 2016/4/27 */
            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Fragment Info______________________ \n\n");

            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Buffer Status______________________ \n\n");
            HI_PROC_Printf(pstBuf, "MaxBufferTimeMs:        %u\n", pstCommonShowInfo->buffer_status.u32MaxBufferTime);
            HI_PROC_Printf(pstBuf, "WaterMark-HighPercent:  %u\n", pstCommonShowInfo->buffer_status.u32HighPercent);
            HI_PROC_Printf(pstBuf, "WaterMark-LowPercent:   %u\n", pstCommonShowInfo->buffer_status.u32LowPercent);
            HI_PROC_Printf(pstBuf, "VideoBufferSize:        %u\n", pstCommonShowInfo->buffer_status.stVBufferStatus.u32BufferSize);
            HI_PROC_Printf(pstBuf, "VideoBufferTimeMs:      %llu\n", pstCommonShowInfo->buffer_status.stVBufferStatus.u64BufferPlayingTime);
            HI_PROC_Printf(pstBuf, "VideoLastBufferPts:     %llu\n", pstCommonShowInfo->buffer_status.stVBufferStatus.u64LastBufferPTS);
            HI_PROC_Printf(pstBuf, "AudioBufferSize:        %lu\n", pstCommonShowInfo->buffer_status.stABufferStatus.u32BufferSize);
            HI_PROC_Printf(pstBuf, "AudioBufferTimeMs:      %llu\n", pstCommonShowInfo->buffer_status.stABufferStatus.u64BufferPlayingTime);
            HI_PROC_Printf(pstBuf, "AudioLastBufferPts:     %lu\n", pstCommonShowInfo->buffer_status.stABufferStatus.u64LastBufferPTS);
            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Buffer Status______________________ \n\n");

            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Video/Audio Track_____________________ \n\n");
            HI_PROC_Printf(pstBuf, "VideoType:              %s\n", pstCommonShowInfo->video_track_info.cVideoType);
            HI_PROC_Printf(pstBuf, "VideoTrackId:           %u\n", pstCommonShowInfo->video_track_info.u32TrackId);
            HI_PROC_Printf(pstBuf, "AudioTrackCnt:          %u\n", pstCommonShowInfo->audio_track_info.u32TrackNum +
                                                                pstCommonShowInfo->audio_track_info.u32UnSupportedTrackNum);
            ST_SINGLE_AUDIO_TRACK_INFO *pSingleAudTrcInfo;
            for (u32TrackIdx = 0; u32TrackIdx < pstCommonShowInfo->audio_track_info.u32TrackNum; u32TrackIdx++)
            {
                pSingleAudTrcInfo = &pstCommonShowInfo->audio_track_info.stSingleAudioTrackInfo[u32TrackIdx];

                HI_PROC_Printf(pstBuf, "%s ", pstCommonShowInfo->audio_track_info.u32CurrentTrackId == u32TrackIdx ? "*" : " ");
                HI_PROC_Printf(pstBuf, "AudioTrackId:           %d,\n", pSingleAudTrcInfo->u32TrackId);
                HI_PROC_Printf(pstBuf, "AudioType:              %s,\n", pSingleAudTrcInfo->cAudioType);
                HI_PROC_Printf(pstBuf, "AudioBitrate:           %d,\n", pSingleAudTrcInfo->u32Bitrate);
                HI_PROC_Printf(pstBuf, "AudioSampleRate:        %d,\n", pSingleAudTrcInfo->u32SampleRate);
                HI_PROC_Printf(pstBuf, "AudioChannels:          %d,\n", pSingleAudTrcInfo->u32ChannelNum);
                HI_PROC_Printf(pstBuf, "AudioLanguage:          %s \n", pSingleAudTrcInfo->cLang);
            }

            for (u32TrackIdx = 0; u32TrackIdx < pstCommonShowInfo->audio_track_info.u32UnSupportedTrackNum; u32TrackIdx++)
            {
                pSingleAudTrcInfo = &pstCommonShowInfo->audio_track_info.stUnSprtSingleAudioTrackInfo[u32TrackIdx];
                HI_PROC_Printf(pstBuf, "# AudioTrackId:     %d,\n", pSingleAudTrcInfo->u32TrackId);
                HI_PROC_Printf(pstBuf, "AudioType:          %s,\n", pSingleAudTrcInfo->cAudioType);
                HI_PROC_Printf(pstBuf, "AudioBitrate:       %d,\n", pSingleAudTrcInfo->u32Bitrate);
                HI_PROC_Printf(pstBuf, "AudioSampleRate:    %d,\n", pSingleAudTrcInfo->u32SampleRate);
                HI_PROC_Printf(pstBuf, "AudioChannels:      %d,\n", pSingleAudTrcInfo->u32ChannelNum);
                HI_PROC_Printf(pstBuf, "AudioLanguage:      %s \n", pSingleAudTrcInfo->cLang);
            }
            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Video/Audio Track_____________________ \n\n");

            HI_PROC_Printf(pstBuf, "_________________________SME-INFO:Player Performance Time________________ \n\n");
            HI_PROC_Printf(pstBuf, "PrepareTimeMs:          %llu ms\n", pstCommonShowInfo->prepare_cost_time);
            HI_PROC_Printf(pstBuf, "StartTimeMs:            %llu ms\n", pstCommonShowInfo->start_cost_time);
            HI_PROC_Printf(pstBuf, "SeekTimeMs:             %llu ms\n", pstCommonShowInfo->seek_cost_time);
            HI_PROC_Printf(pstBuf, "ResetTimeMs:            %llu ms\n", pstCommonShowInfo->reset_cost_time);
            HI_PROC_Printf(pstBuf, "FccTimeMs:              %llu ms\n", pstCommonShowInfo->fcc_cost_time);
            HI_PROC_Printf(pstBuf, "QuitTimeMs:             %llu ms\n", pstCommonShowInfo->quit_cost_time);
            HI_PROC_Printf(pstBuf, "/_________________________SME-INFO:Player Performance Time________________ \n\n");

            HI_PROC_Printf(pstBuf, "__________________________SME-INFO:Player Status__________________________ \n\n");
            HI_PROC_Printf(pstBuf, "PlyaerState:            %s\n", PlayerStateName[pstCommonShowInfo->PlayerState]);
            HI_PROC_Printf(pstBuf, "IsPlayerLoop:           %s\n", PlayerLoop[pstCommonShowInfo->IsPlayerLoop]);
            HI_PROC_Printf(pstBuf, "__________________________SME-INFO:Player Status__________________________ \n\n");
            return HI_SUCCESS;
        }
        else
            return HI_FAILURE;
    #endif
        HI_PROC_Printf(pstBuf, "Not support command\n");
        return HI_SUCCESS;
    }
}

HI_S32 ProcCommonCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("Common cmd PrivData is NULL !"));

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    return HI_SUCCESS;
}

#ifdef _SUPPORT_SMEPLAYER
static HI_S32 ProcSmeDemuxHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************SmePlayer sme demux help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/smeplayer/sme_demux \n", CMD_DUMP_DEMUX_AUDIO_DATA,CMD_DUMP_START);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/smeplayer/sme_demux \n", CMD_DUMP_DEMUX_AUDIO_DATA,CMD_DUMP_STOP);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/smeplayer/sme_demux \n", CMD_DUMP_DEMUX_VIDEO_DATA,CMD_DUMP_START);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/smeplayer/sme_demux \n", CMD_DUMP_DEMUX_VIDEO_DATA,CMD_DUMP_STOP);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/smeplayer/sme_demux \n", CMD_HELP_NAME);
    HI_PROC_Printf(pstBuf, "************************SmePlayer Help Info End*************************\n");

    return HI_SUCCESS;
}

HI_S32 ProcSmeDemuxShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_SMEDEMUX_PROC_INFO *pstSmeDemuxProcInfo;
    SmeBenchMark* bench_handle;
    ST_SMEDEMUX_SHOW_INFO *pstSmeDemuxShowInfo;
    ST_SMEDEMUX_CMD_INFO *pstSmeDemuxCmdInfo;
    V_UINT32 u32CovertTime;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstSmeDemuxProcInfo = (ST_SMEDEMUX_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstSmeDemuxProcInfo->pBenchmarkHandle);
    pstSmeDemuxProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    pstSmeDemuxShowInfo = &(pstSmeDemuxProcInfo->stShowInfo);

    g_object_set(bench_handle->m_pPlayBinHandler,
        GST_SME_BENCH_MARK_DEMUX_INFO, pPrivData, NULL);

    HI_PROC_Printf(pstBuf, "Print in SmeDemux process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer sme demux proc info_______________\n");

    u32CovertTime = _ConvertTime(pstSmeDemuxShowInfo->sme_demux_created_time);
    HI_PROC_Printf(pstBuf, "CreateDemuxTimeMs:          %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstSmeDemuxShowInfo->first_output_i_frame_time);
    HI_PROC_Printf(pstBuf, "DemuxFirstPlayTimeMs:       %ums\n", u32CovertTime);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer sme demux proc info_______________\n");

    pstSmeDemuxCmdInfo = &(pstSmeDemuxProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer sme demux control info_______________\n");

    HI_PROC_Printf(pstBuf, "DumpDemuxAudioStream:       %d\n", pstSmeDemuxCmdInfo->bDumpSrcAudioStream);
    HI_PROC_Printf(pstBuf, "DumpDemuxVideoStream:       %d\n", pstSmeDemuxCmdInfo->bDumpSrcVideoStream);
    HI_PROC_Printf(pstBuf, "DumpStorePath:              %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer sme demux control info_________________\n\n");

    return HI_SUCCESS;
}

HI_S32 ProcSmeDemuxCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_SMEDEMUX_PROC_INFO *pstSmeDemuxProcInfo;
    ST_SMEDEMUX_PROC_INFO  SmeDemuxCmd;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstSmeDemuxProcInfo = (ST_SMEDEMUX_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstSmeDemuxProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(SmeDemuxCmd.stCmdInfo),
                              sizeof(ST_SMEDEMUX_CMD_INFO),
                              0xFF,
                              sizeof(ST_SMEDEMUX_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S SmeDemuxCmd Failed"));

    eMemSecRet = VOS_Memset_S(SmeDemuxCmd.stCmdInfo.cDumpFileStorePath,
                                             DUMP_FILE_STORE_PATH_LENGTH,
                                             0,
                                             DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S sme demux init dump audio path Failed"));

    SmeDemuxCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;

    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcSmeDemuxHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
            if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_DEMUX_AUDIO_DATA, strlen(CMD_DUMP_DEMUX_AUDIO_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstSmeDemuxProcInfo->stCmdInfo.bDumpSrcAudioStream = 1;
                   SmeDemuxCmd.stCmdInfo.bDumpSrcAudioStream = 1;

                   eMemSecRet= VOS_Memcpy_S(SmeDemuxCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S sme demux dump audio data store path Failed"));
                   HI_PROC_Printf(pstBuf, "Start dump audio data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   SmeDemuxCmd.stCmdInfo.bDumpSrcAudioStream = 0;
                   pstSmeDemuxProcInfo->stCmdInfo.bDumpSrcAudioStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump audio data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_DEMUX_INFO, &SmeDemuxCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_DEMUX_VIDEO_DATA, strlen(CMD_DUMP_DEMUX_VIDEO_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstSmeDemuxProcInfo->stCmdInfo.bDumpSrcVideoStream = 1;
                   SmeDemuxCmd.stCmdInfo.bDumpSrcVideoStream = 1;

                   eMemSecRet= VOS_Memcpy_S(SmeDemuxCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S sme demux dump video data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump video data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   SmeDemuxCmd.stCmdInfo.bDumpSrcVideoStream = 0;
                   pstSmeDemuxProcInfo->stCmdInfo.bDumpSrcVideoStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump video data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_DEMUX_INFO, &SmeDemuxCmd, NULL);
            }
           break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }
    return HI_SUCCESS;
}
#endif

static HI_S32 ProcSmeHWTSDemuxHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************DVBPlayer hwtsdemux help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/dvbplayer/hwts_demux \n", CMD_DUMP_DEMUX_AUDIO_DATA,CMD_DUMP_START);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/dvbplayer/hwts_demux \n", CMD_DUMP_DEMUX_AUDIO_DATA,CMD_DUMP_STOP);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/dvbplayer/hwts_demux \n", CMD_DUMP_DEMUX_VIDEO_DATA,CMD_DUMP_START);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/dvbplayer/hwts_demux \n", CMD_DUMP_DEMUX_VIDEO_DATA,CMD_DUMP_STOP);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/dvbplayer/hwts_demux \n", CMD_HWTSDMXTUNERID, "[id]:demuxid in decimal format,example:32 for public");
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/dvbplayer/hwts_demux \n", CMD_HELP_NAME);
    HI_PROC_Printf(pstBuf, "************************DVBPlayer hwtsdemux Help Info End*************************\n");

    return HI_SUCCESS;
}

HI_S32 ProcSmeHWTSDemuxShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_SMEHWTSDEMUX_PROC_INFO *pstSmeHWTSDemuxProcInfo;
    SmeBenchMark* bench_handle;
    ST_TSDEMUX_SHOW_INFO *pstSmeHWTSDemuxShowInfo;
    ST_TSDEMUX_CMD_INFO *pstSmeHWTSDemuxCmdInfo;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstSmeHWTSDemuxProcInfo = (ST_SMEHWTSDEMUX_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstSmeHWTSDemuxProcInfo->pBenchmarkHandle);
    pstSmeHWTSDemuxProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    pstSmeHWTSDemuxShowInfo = &(pstSmeHWTSDemuxProcInfo->stShowInfo);

    g_object_set(bench_handle->m_pPlayBinHandler,
        GST_SME_BENCH_MARK_HWTSDEMUX_INFO, pPrivData, NULL);

    HI_PROC_Printf(pstBuf, "Print in SmeHWTSDemux process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "_____________________________DVBPlayer sme hwtsdemux proc info_______________\n");
    HI_PROC_Printf(pstBuf, "audio pid:                %#x\n", pstSmeHWTSDemuxShowInfo->u32Apid);
    HI_PROC_Printf(pstBuf, "audio codec name:         %s\n", pstSmeHWTSDemuxShowInfo->acACodecName);
    HI_PROC_Printf(pstBuf, "video pid:                %#x\n", pstSmeHWTSDemuxShowInfo->u32Vpid);
    HI_PROC_Printf(pstBuf, "video codec name:         %s\n", pstSmeHWTSDemuxShowInfo->acVCodecName);
    HI_PROC_Printf(pstBuf, "video is need descramble: %d\n", pstSmeHWTSDemuxShowInfo->i32VideoIsCa);
    HI_PROC_Printf(pstBuf, "audio is need descramble: %d\n", pstSmeHWTSDemuxShowInfo->i32AudioIsCa);
    HI_PROC_Printf(pstBuf, "is Pcr Sync:              %d\n", pstSmeHWTSDemuxShowInfo->i32PcrSync);
    HI_PROC_Printf(pstBuf, "used hwtsDemux tuner ID:  %d\n", pstSmeHWTSDemuxShowInfo->u32TunerID);
    HI_PROC_Printf(pstBuf, "_____________________________DVBPlayer sme hwtsdemux proc info_______________\n");

    pstSmeHWTSDemuxCmdInfo = &(pstSmeHWTSDemuxProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________DVBPlayer sme hwtsdemux control info_______________\n");

    HI_PROC_Printf(pstBuf, "DumpDemuxAudioStream:       %d\n", pstSmeHWTSDemuxCmdInfo->bDumpSrcAudioStream);
    HI_PROC_Printf(pstBuf, "DumpDemuxVideoStream:       %d\n", pstSmeHWTSDemuxCmdInfo->bDumpSrcVideoStream);
    HI_PROC_Printf(pstBuf, "SetHWTSTunerID:             %d\n", pstSmeHWTSDemuxCmdInfo->u32TunerID);
    HI_PROC_Printf(pstBuf, "DumpStorePath:              %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);
    HI_PROC_Printf(pstBuf, "_____________________________DVBPlayer sme hwtsdemux control info_________________\n\n");

    return HI_SUCCESS;
}

HI_S32 ProcSmeHWTSDemuxCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_SMEHWTSDEMUX_PROC_INFO *pstSmeHWTSDemuxProcInfo;
    ST_SMEHWTSDEMUX_PROC_INFO stSmeHWTSDemuxProcCmd;
    ST_SMEHWTSDEMUX_PROC_INFO* pstSmeHWTSDemuxProcCmdGet;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstSmeHWTSDemuxProcInfo = (ST_SMEHWTSDEMUX_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstSmeHWTSDemuxProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(stSmeHWTSDemuxProcCmd.stCmdInfo),
                              sizeof(ST_TSDEMUX_CMD_INFO),
                              0,
                              sizeof(ST_TSDEMUX_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S stSmeHWTSDemuxProcCmd Failed"));

    eMemSecRet = VOS_Memset_S(stSmeHWTSDemuxProcCmd.stCmdInfo.cDumpFileStorePath,
                                             DUMP_FILE_STORE_PATH_LENGTH,
                                             0,
                                             DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S tsdmx data store path Failed"));

    stSmeHWTSDemuxProcCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;
    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcSmeHWTSDemuxHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
            if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_DEMUX_AUDIO_DATA, strlen(CMD_DUMP_DEMUX_AUDIO_DATA)))
            {
                g_object_get(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &pstSmeHWTSDemuxProcCmdGet, NULL);
                if(NULL != pstSmeHWTSDemuxProcCmdGet)
                {
                    stSmeHWTSDemuxProcCmd.stCmdInfo = pstSmeHWTSDemuxProcCmdGet->stCmdInfo;
                    if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                    {
                       pstSmeHWTSDemuxProcInfo->stCmdInfo.bDumpSrcAudioStream = 1;
                       stSmeHWTSDemuxProcCmd.stCmdInfo.bDumpSrcAudioStream = 1;
                       eMemSecRet= VOS_Memcpy_S(stSmeHWTSDemuxProcCmd.stCmdInfo.cDumpFileStorePath,
                                  DUMP_FILE_STORE_PATH_LENGTH - 1,
                                  bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                                  (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath) + 1));
                       UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S sme demux dump audio data store path Failed"));
                       HI_PROC_Printf(pstBuf, "Start dump audio data.path:%s\n", stSmeHWTSDemuxProcCmd.stCmdInfo.cDumpFileStorePath);
                    }
                    else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                    {
                       stSmeHWTSDemuxProcCmd.stCmdInfo.bDumpSrcAudioStream = 0;
                       pstSmeHWTSDemuxProcInfo->stCmdInfo.bDumpSrcAudioStream = 0;
                       HI_PROC_Printf(pstBuf, "Stop dump audio data\n");
                    }
                    g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &stSmeHWTSDemuxProcCmd, NULL);
                }
                else
                {
                    ICS_LOGW("get hwtsdemux info failed !");
                }
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_DEMUX_VIDEO_DATA, strlen(CMD_DUMP_DEMUX_VIDEO_DATA)))
            {
                g_object_get(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &pstSmeHWTSDemuxProcCmdGet, NULL);
                if(NULL != pstSmeHWTSDemuxProcCmdGet)
                {
                    stSmeHWTSDemuxProcCmd.stCmdInfo = pstSmeHWTSDemuxProcCmdGet->stCmdInfo;
                    if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                    {
                       pstSmeHWTSDemuxProcInfo->stCmdInfo.bDumpSrcVideoStream = 1;
                       stSmeHWTSDemuxProcCmd.stCmdInfo.bDumpSrcVideoStream = 1;

                       eMemSecRet= VOS_Memcpy_S(stSmeHWTSDemuxProcCmd.stCmdInfo.cDumpFileStorePath,
                                  DUMP_FILE_STORE_PATH_LENGTH - 1,
                                  bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                                  (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath) + 1));
                       UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S sme demux dump video data store path Failed"));

                       HI_PROC_Printf(pstBuf, "Start dump video data,path:%s,ctrlpath:%s\n",
                            stSmeHWTSDemuxProcCmd.stCmdInfo.cDumpFileStorePath,
                            bench_handle->m_stPlayerControlPara.cDumpDataStorePath);
                    }
                    else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                    {
                       stSmeHWTSDemuxProcCmd.stCmdInfo.bDumpSrcVideoStream = 0;
                       pstSmeHWTSDemuxProcInfo->stCmdInfo.bDumpSrcVideoStream = 0;
                       HI_PROC_Printf(pstBuf, "Stop dump video data\n");
                    }
                    g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &stSmeHWTSDemuxProcCmd, NULL);
                }
                else
                {
                    ICS_LOGW("get hwtsdemux info failed !");
                }
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HWTSDMXTUNERID, strlen(CMD_HWTSDMXTUNERID)))
            {
                g_object_get(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &pstSmeHWTSDemuxProcCmdGet, NULL);
                if(NULL != pstSmeHWTSDemuxProcCmdGet)
                {
                    stSmeHWTSDemuxProcCmd.stCmdInfo = pstSmeHWTSDemuxProcCmdGet->stCmdInfo;
                    if(NULL != pu8Argv[1])
                    {
                        stSmeHWTSDemuxProcCmd.stCmdInfo.u32TunerID = (V_UINT32)atoi((const V_CHAR*)pu8Argv[1]);
                    }
                    else
                    {
                        stSmeHWTSDemuxProcCmd.stCmdInfo.u32TunerID = 0;
                    }
                    pstSmeHWTSDemuxProcInfo->stCmdInfo.u32TunerID = stSmeHWTSDemuxProcCmd.stCmdInfo.u32TunerID;
                    g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_HWTSDEMUX_INFO, &stSmeHWTSDemuxProcCmd, NULL);
                }
                else
                {
                    ICS_LOGW("get hwtsdemux info failed !");
                }
            }
            break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }
    return HI_SUCCESS;
}

static HI_S32 ProcVdecHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************Video decoder help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_decoder \n", CMD_DUMP_SINK_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_decoder \n", CMD_DUMP_SINK_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_decoder \n", CMD_DUMP_SRC_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_decoder \n", CMD_DUMP_SRC_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/%s/video_decoder \n", CMD_HELP_NAME,cDirectoryName);
    HI_PROC_Printf(pstBuf, "************************Video decoder help Info Begin*************************\n");

    return HI_SUCCESS;
}

HI_S32 ProcVdecShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    SmeBenchMark* bench_handle;
    ST_VDEC_SHOW_INFO *pstVideoDecShowInfo;
    ST_VDEC_CMD_INFO *pstVideoDecCmdInfo;
    ST_VIDEODEC_PROC_INFO *pstVideoProcInfo;
    V_UINT32 u32CovertTime;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstVideoProcInfo = (ST_VIDEODEC_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstVideoProcInfo->pBenchmarkHandle);
    pstVideoProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    pstVideoDecShowInfo = &(pstVideoProcInfo->stShowInfo);

    g_object_set(bench_handle->m_pPlayBinHandler,
            GST_SME_BENCH_MARK_VDEC_INFO, pPrivData, NULL);

    HI_PROC_Printf(pstBuf, "Print in Vdec process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video decoder proc info_______________\n");

    u32CovertTime = _ConvertTime(pstVideoDecShowInfo->vdec_create_time);
    HI_PROC_Printf(pstBuf, "CreateVDecTimeMs:            %ums\n",  u32CovertTime);
    HI_PROC_Printf(pstBuf, "FileFPS:                     %lu\n", pstVideoDecShowInfo->file_frame_rate);
    HI_PROC_Printf(pstBuf, "CurFPS:                      %lu\n", pstVideoDecShowInfo->cur_frame_rate);
    HI_PROC_Printf(pstBuf, "VDecErrorCnt:                %lu\n", pstVideoDecShowInfo->vdec_err_cnt);
    HI_PROC_Printf(pstBuf, "IFramesNum:                  %lu\n", pstVideoDecShowInfo->Iframe_cnt);
    u32CovertTime = _ConvertTime(pstVideoDecShowInfo->get_first_I_frame);
    HI_PROC_Printf(pstBuf, "VdecFirstPlayTimeMs:         %ums\n", u32CovertTime);
    HI_PROC_Printf(pstBuf, "VdecAnalyzeFirstIframeTimeM: %lld ms\n", pstVideoDecShowInfo->fir_Iframe_vdecT);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video decoder proc info_______________\n");

    pstVideoDecCmdInfo = &(pstVideoProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video decoder control info_______________\n");

    HI_PROC_Printf(pstBuf, "DumpSinkStream:      %d\n", pstVideoDecCmdInfo->bDumpSinkStream);
    HI_PROC_Printf(pstBuf, "DumpSrcStream:       %d\n", pstVideoDecCmdInfo->bDumpSrcStream);
    HI_PROC_Printf(pstBuf, "DumpStorePath:       %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video decoder control info_________________\n\n");

    return HI_SUCCESS;
}

HI_S32 ProcVdecCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_VIDEODEC_PROC_INFO *pstVdecProcInfo;
    ST_VIDEODEC_PROC_INFO  VdecCmd;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(pPrivData == NULL, HI_FAILURE, ("PrivData is NULL !"));

    pstVdecProcInfo = (ST_VIDEODEC_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstVdecProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(VdecCmd.stCmdInfo),
                              sizeof(ST_VDEC_CMD_INFO),
                              0xFF,
                              sizeof(ST_VDEC_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S vdec init cmd info Failed"));

    eMemSecRet = VOS_Memset_S(VdecCmd.stCmdInfo.cDumpFileStorePath,
                                           DUMP_FILE_STORE_PATH_LENGTH,
                                           0,
                                           DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S vdec data store path Failed"));
    VdecCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;

    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcVdecHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
            if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SRC_DATA, strlen(CMD_DUMP_SRC_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstVdecProcInfo->stCmdInfo.bDumpSrcStream = 1;
                   VdecCmd.stCmdInfo.bDumpSrcStream = 1;

                   eMemSecRet= VOS_Memcpy_S(VdecCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S vdec create src data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump vdec src data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   VdecCmd.stCmdInfo.bDumpSrcStream = 0;
                   pstVdecProcInfo->stCmdInfo.bDumpSrcStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump vdec src data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_VDEC_INFO, &VdecCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SINK_DATA, strlen(CMD_DUMP_SINK_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstVdecProcInfo->stCmdInfo.bDumpSinkStream = 1;
                   VdecCmd.stCmdInfo.bDumpSinkStream = 1;

                   eMemSecRet= VOS_Memcpy_S(VdecCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S vdec create sink data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump vdec sink data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   VdecCmd.stCmdInfo.bDumpSinkStream = 0;
                   pstVdecProcInfo->stCmdInfo.bDumpSinkStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump vdec sink data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_VDEC_INFO, &VdecCmd, NULL);
            }
           break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }
    return HI_SUCCESS;
}

static HI_S32 ProcAdecHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************Audio decoder help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_decoder \n", CMD_DUMP_SINK_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_decoder \n", CMD_DUMP_SINK_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_decoder \n", CMD_DUMP_SRC_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_decoder \n", CMD_DUMP_SRC_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/%s/audio_decoder \n", CMD_HELP_NAME,cDirectoryName);
    HI_PROC_Printf(pstBuf, "************************Audio decoder help Info End*************************\n");

    return HI_SUCCESS;
}


HI_S32 ProcAdecShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_AUDIODEC_PROC_INFO *pstAudioProcInfo;
    SmeBenchMark* bench_handle;
    ST_ADEC_SHOW_INFO *pstAudioDecShowInfo;
    ST_ADEC_CMD_INFO *pstAudioDecCmdInfo;
    V_UINT32 u32CovertTime;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("PrivData is NULL !"));

    pstAudioProcInfo = (ST_AUDIODEC_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstAudioProcInfo->pBenchmarkHandle);
    pstAudioProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    g_object_set(bench_handle->m_pPlayBinHandler,
            GST_SME_BENCH_MARK_ADEC_INFO, pPrivData, NULL);

    pstAudioDecShowInfo = &(pstAudioProcInfo->stShowInfo);

    HI_PROC_Printf(pstBuf, "Print in Adec process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio decoder proc info_______________\n");

    u32CovertTime = _ConvertTime(pstAudioDecShowInfo->CreateAdecTime);
    HI_PROC_Printf(pstBuf, "CreateAdecTimeMs:           %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstAudioDecShowInfo->ADecGetFirstIFrameTime);
    HI_PROC_Printf(pstBuf, "ADecGetFirstIFrameTimeMs:   %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstAudioDecShowInfo->ADecFirstPlayTime);
    HI_PROC_Printf(pstBuf, "ADecFirstPlayTimeMs:        %ums\n", u32CovertTime);
    HI_PROC_Printf(pstBuf, "MaxBufferSize:              %u\n", pstAudioDecShowInfo->MaxBufferSize);
    HI_PROC_Printf(pstBuf, "ADecErrorCnt:               %u\n", pstAudioDecShowInfo->ADecErrorCnt);
    HI_PROC_Printf(pstBuf, "ADecErrorInfo:              %s\n", pstAudioDecShowInfo->ADecErrorInfo);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio decoder proc info_______________\n");


    pstAudioDecCmdInfo = &(pstAudioProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio decoder control info_______________\n");
    HI_PROC_Printf(pstBuf, "DumpSinkStream:      %d\n", pstAudioDecCmdInfo->bDumpSinkStream);
    HI_PROC_Printf(pstBuf, "DumpSrcStream:       %d\n", pstAudioDecCmdInfo->bDumpSrcStream);
    HI_PROC_Printf(pstBuf, "DumpStorePath:       %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio decoder control info_________________\n\n");
    return HI_SUCCESS;
}

HI_S32 ProcAdecCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_AUDIODEC_PROC_INFO *pstAudioProcInfo;
    ST_AUDIODEC_PROC_INFO  AdecCmd;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("PrivData is NULL !"));

    pstAudioProcInfo = (ST_AUDIODEC_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstAudioProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(AdecCmd.stCmdInfo),
                              sizeof(ST_ADEC_CMD_INFO),
                              0xFF,
                              sizeof(ST_ADEC_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S pstAudioProcInfo Failed"));

    eMemSecRet = VOS_Memset_S(AdecCmd.stCmdInfo.cDumpFileStorePath,
                                             DUMP_FILE_STORE_PATH_LENGTH,
                                             0,
                                             DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S adec data store path Failed"));
    AdecCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;

    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcAdecHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SINK_DATA, strlen(CMD_DUMP_SINK_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   AdecCmd.stCmdInfo.bDumpSinkStream = 1;
                   pstAudioProcInfo->stCmdInfo.bDumpSinkStream = 1;

                   eMemSecRet = VOS_Memcpy_S(AdecCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S adec create sink dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump audio decoder sink data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   AdecCmd.stCmdInfo.bDumpSinkStream = 0;
                   pstAudioProcInfo->stCmdInfo.bDumpSinkStream = 0;//store cmd
                   HI_PROC_Printf(pstBuf, "Stop dump audio decoder sink data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_ADEC_INFO, &AdecCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SRC_DATA, strlen(CMD_DUMP_SRC_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstAudioProcInfo->stCmdInfo.bDumpSrcStream = 1;
                   AdecCmd.stCmdInfo.bDumpSrcStream = 1;

                   eMemSecRet= VOS_Memcpy_S(AdecCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S adec create src dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump audio decoder src data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   AdecCmd.stCmdInfo.bDumpSrcStream = 0;
                   pstAudioProcInfo->stCmdInfo.bDumpSrcStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump audio decoder src data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_ADEC_INFO, &AdecCmd, NULL);
            }
           break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }

    return HI_SUCCESS;
}

static HI_S32 ProcVsinkHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************Video sink help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_sink \n", CMD_DUMP_SINK_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_sink \n", CMD_DUMP_SINK_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_sink \n", CMD_DUMP_SRC_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/video_sink \n", CMD_DUMP_SRC_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/%s/video_sink \n", CMD_HELP_NAME,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s [0 or 1] >/proc/hisi/%s/video_sink \n", CMD_VSINK_AVSYNC,cDirectoryName);
    HI_PROC_Printf(pstBuf, "disable avsync:echo %s 0 >/proc/hisi/%s/video_sink \n", CMD_VSINK_AVSYNC,cDirectoryName);
    HI_PROC_Printf(pstBuf, "enable avsync:echo %s 1 >/proc/hisi/%s/video_sink \n", CMD_VSINK_AVSYNC,cDirectoryName);
    HI_PROC_Printf(pstBuf, "************************Video sink help Info End*************************\n");

    return HI_SUCCESS;
}

HI_S32 ProcVsinkShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_VIDEOSINK_PROC_INFO *pstVideoSinkProcInfo;
    SmeBenchMark* bench_handle;
    ST_VSINK_SHOW_INFO *pstVideoSinkShowInfo;
    ST_VSINK_CMD_INFO *pstVideoSinkCmdInfo;
    V_UINT32 u32CovertTime;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("PrivData is NULL !"));

    pstVideoSinkProcInfo = (ST_VIDEOSINK_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstVideoSinkProcInfo->pBenchmarkHandle);
    pstVideoSinkProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    pstVideoSinkShowInfo = &(pstVideoSinkProcInfo->stShowInfo);

    g_object_set(bench_handle->m_pPlayBinHandler,
            GST_SME_BENCH_MARK_VSINK_INFO, pPrivData, NULL);

    HI_PROC_Printf(pstBuf, "Print in Vsink process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video sink proc info_______________\n");

    u32CovertTime = _ConvertTime(pstVideoSinkShowInfo->VideoSinkCreatedTimeMs);
    HI_PROC_Printf(pstBuf, "VsinkCreatedTimeMs:         %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstVideoSinkShowInfo->VideoSinkFirstFrameTimeMs);
    HI_PROC_Printf(pstBuf, "VSinkGetFirstFrameTimeMs:   %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstVideoSinkShowInfo->VideoFirstPlayTimeMs);
    HI_PROC_Printf(pstBuf, "VideoFirstPlayTimeMs:       %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstVideoSinkShowInfo->VideoFinalPlayTimeMs);
    HI_PROC_Printf(pstBuf, "VideoFinalPlayTimeMs:       %ums\n", u32CovertTime);
    HI_PROC_Printf(pstBuf, "VideoPts:                   %lld\n", pstVideoSinkShowInfo->VideoPts);
    HI_PROC_Printf(pstBuf, "DroppedVideoFrameCntAtSync: %u\n", pstVideoSinkShowInfo->DroppedVideoFrameCntAtSync);
    HI_PROC_Printf(pstBuf, "VideoFrameIsDelay:          %d\n", pstVideoSinkShowInfo->VideoFrameIsDelay);
    HI_PROC_Printf(pstBuf, "IsSync:                     %d\n", pstVideoSinkShowInfo->IsSync);
    HI_PROC_Printf(pstBuf, "ClockForSync:               %lld\n", pstVideoSinkShowInfo->ClockForSync);

    HI_PROC_Printf(pstBuf, "video sink avsync info:\n");
    HI_PROC_Printf(pstBuf, "element basetime:         %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64BaseTime);
    HI_PROC_Printf(pstBuf, "clock time:               %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64ClockTime);
    HI_PROC_Printf(pstBuf, "clock running-time:       %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64ClockRunningTime);
    HI_PROC_Printf(pstBuf, "segment.start:            %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64SegStart);
    HI_PROC_Printf(pstBuf, "segment.stop:             %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64SegStop);
    HI_PROC_Printf(pstBuf, "segment.base:             %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64Segbase);
    HI_PROC_Printf(pstBuf, "segment.dbRate:           %f\n",      pstVideoSinkShowInfo->stAVSyncInfo.dbRate);
    HI_PROC_Printf(pstBuf, "Buffer.Pts:               %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64BufferPts);
    HI_PROC_Printf(pstBuf, "Buffer.Duration:          %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64Duration);
    HI_PROC_Printf(pstBuf, "Buffer.RunningTime:       %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64BufferRunningTime);
    HI_PROC_Printf(pstBuf, "Buffer.CurRenderDelay:    %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64CurRenderDelay);
    HI_PROC_Printf(pstBuf, "MaxLatency:               %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64MaxLatency);
    HI_PROC_Printf(pstBuf, "diff:                     %lld ms\n", pstVideoSinkShowInfo->stAVSyncInfo.i64Diff);
    HI_PROC_Printf(pstBuf, "diff=C.runningtime-B.runningtime,diff>0 means buffer is late,late<0 means buffer is early!\n");
    HI_PROC_Printf(pstBuf, "if disable avsync, diff = 0\n");

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video sink proc info_______________\n");

    pstVideoSinkCmdInfo = &(pstVideoSinkProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video sink control info_______________\n");
    HI_PROC_Printf(pstBuf, "DumpSinkStream:      %d\n", pstVideoSinkCmdInfo->bDumpSinkStream);
    HI_PROC_Printf(pstBuf, "DumpSrcStream:       %d\n", pstVideoSinkCmdInfo->bDumpSrcStream);
    HI_PROC_Printf(pstBuf, "DumpStorePath:       %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer video sink control info_________________\n\n");
    return HI_SUCCESS;
}

HI_S32 ProcVsinkCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_VIDEOSINK_PROC_INFO *pstVsinkProcInfo;
    ST_VIDEOSINK_PROC_INFO  VsinkCmd;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("PrivData is NULL !"));

    pstVsinkProcInfo = (ST_VIDEOSINK_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstVsinkProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(VsinkCmd.stCmdInfo),
                              sizeof(ST_VSINK_CMD_INFO),
                              0xFF,
                              sizeof(ST_VSINK_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S vsink cmd info failed !"));

    eMemSecRet = VOS_Memset_S(VsinkCmd.stCmdInfo.cDumpFileStorePath,
                                                 DUMP_FILE_STORE_PATH_LENGTH,
                                                 0,
                                                 DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S vsink data store path Failed"));

    VsinkCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;

    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcVsinkHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
            if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SINK_DATA, strlen(CMD_DUMP_SINK_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   VsinkCmd.stCmdInfo.bDumpSinkStream = 1;
                   pstVsinkProcInfo->stCmdInfo.bDumpSinkStream = 1;

                   eMemSecRet = VOS_Memcpy_S(VsinkCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S vsink create sink dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump vsink sink data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   VsinkCmd.stCmdInfo.bDumpSinkStream = 0;
                   pstVsinkProcInfo->stCmdInfo.bDumpSinkStream = 0;//store cmd
                   HI_PROC_Printf(pstBuf, "Stop dump vsink sink data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_VSINK_INFO, &VsinkCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SRC_DATA, strlen(CMD_DUMP_SRC_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstVsinkProcInfo->stCmdInfo.bDumpSrcStream = 1;
                   VsinkCmd.stCmdInfo.bDumpSrcStream = 1;

                   eMemSecRet= VOS_Memcpy_S(VsinkCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S vsink create src dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump vsink src data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   VsinkCmd.stCmdInfo.bDumpSrcStream = 0;
                   pstVsinkProcInfo->stCmdInfo.bDumpSrcStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump vsink src data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_VSINK_INFO, &VsinkCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_VSINK_AVSYNC , strlen(CMD_VSINK_AVSYNC)))
            {
                if(NULL != pu8Argv[1])
                {
                    VsinkCmd.stCmdInfo.i8EnableAVSync= (V_INT8)atoi((const V_CHAR*)pu8Argv[1]);
                }
                pstVsinkProcInfo->stCmdInfo.i8EnableAVSync = VsinkCmd.stCmdInfo.i8EnableAVSync;
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_VSINK_INFO, &VsinkCmd, NULL);
            }
            break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }

    return HI_SUCCESS;
}

static HI_S32 ProcAsinkHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************Audio sink help Info Begin*************************\n\n");
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_sink \n", CMD_DUMP_SINK_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_sink \n", CMD_DUMP_SINK_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_sink \n", CMD_DUMP_SRC_DATA,CMD_DUMP_START,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s %s>/proc/hisi/%s/audio_sink \n", CMD_DUMP_SRC_DATA,CMD_DUMP_STOP,cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/%s/audio_sink \n", CMD_HELP_NAME,cDirectoryName);
    HI_PROC_Printf(pstBuf, "************************Audio sink help Info End*************************\n");
    return HI_SUCCESS;
}

static const char *audio_channel_mode_to_str(A_TRACK_CHANNEL_MODE_E mode)
{
    const char * str;
    switch (mode)
    {
        case A_TRACK_MODE_STEREO:
            str = "A_TRACK_MODE_STEREO";
            break;
        case A_TRACK_MODE_DOUBLE_MONO:
            str = "A_TRACK_MODE_DOUBLE_MONO";
            break;
        case A_TRACK_MODE_DOUBLE_LEFT:
            str = "A_TRACK_MODE_DOUBLE_LEFT";
            break;
        case A_TRACK_MODE_DOUBLE_RIGHT:
            str = "A_TRACK_MODE_DOUBLE_RIGHT";
            break;
        case A_TRACK_MODE_EXCHANGE:
            str = "A_TRACK_MODE_EXCHANGE";
            break;
        case A_TRACK_MODE_ONLY_RIGHT:
            str = "A_TRACK_MODE_ONLY_RIGHT";
            break;
        case A_TRACK_MODE_ONLY_LEFT:
            str = "A_TRACK_MODE_ONLY_LEFT";
            break;
        case A_TRACK_MODE_MUTED:
            str = "A_TRACK_MODE_MUTED";
            break;
        default:
            str = "unknown";
            break;
    }
    return str;
}


HI_S32 ProcAsinkShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_AUDIOSINK_PROC_INFO *pstAudioSinkProcInfo;
    SmeBenchMark* bench_handle;
    ST_ASINK_SHOW_INFO *pstAudioSinkShowInfo;
    ST_ASINK_CMD_INFO *pstAudioSinkCmdInfo;
    V_UINT32 u32CovertTime;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pPrivData is NULL !"));

    pstAudioSinkProcInfo = (ST_AUDIOSINK_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstAudioSinkProcInfo->pBenchmarkHandle);
    pstAudioSinkProcInfo->eProcCmdType = E_SME_PROC_CMD_SHOW_INFO;
    pstAudioSinkShowInfo = &(pstAudioSinkProcInfo->stShowInfo);

    g_object_set(bench_handle->m_pPlayBinHandler,
            GST_SME_BENCH_MARK_ASINK_INFO, pPrivData, NULL);

    HI_PROC_Printf(pstBuf, "Print in Asink process %d, %s\n", (HI_S32)getpid(), __FUNCTION__);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio sink proc info_______________\n");

    u32CovertTime = _ConvertTime(pstAudioSinkShowInfo->CreateASinkTime);
    HI_PROC_Printf(pstBuf, "CreateAsinkTimeMs:      %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstAudioSinkShowInfo->ASinkFirstPlayTime);
    HI_PROC_Printf(pstBuf, "ASinkFirstPlayTimeMs:   %ums\n", u32CovertTime);
    u32CovertTime = _ConvertTime(pstAudioSinkShowInfo->ASinkFinalPlayTime);
    HI_PROC_Printf(pstBuf, "ASinkOutputPts:         %lld\n", pstAudioSinkShowInfo->ASinkOutputPTS);
    HI_PROC_Printf(pstBuf, "ASinkFinalPlayTimeMs:   %ums\n", u32CovertTime);
    HI_PROC_Printf(pstBuf, "AudioDiscardCnt:        %d\n", pstAudioSinkShowInfo->AudioDiscardCnt);
    HI_PROC_Printf(pstBuf, "ClockForSync:           %lld\n", pstAudioSinkShowInfo->ClockForSync);
    HI_PROC_Printf(pstBuf, "AudioRunningTimeMs:     %llu\n", pstAudioSinkShowInfo->AudioRunningTime);
    HI_PROC_Printf(pstBuf, "AudioFrameIsDelay:      %d\n", pstAudioSinkShowInfo->AudioFrameIsDelay);
    HI_PROC_Printf(pstBuf, "AudioMute:              %u\n", pstAudioSinkShowInfo->AudioMute);
    HI_PROC_Printf(pstBuf, "AudioVolume:            %u\n", pstAudioSinkShowInfo->AudioVolume);
    HI_PROC_Printf(pstBuf, "AudioChannelMode:       %s\n", audio_channel_mode_to_str(pstAudioSinkShowInfo->AudioChannelMode));

    HI_PROC_Printf(pstBuf, "audio sink avsync info:\n");
    HI_PROC_Printf(pstBuf, "element basetime:         %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64BaseTime);
    HI_PROC_Printf(pstBuf, "clock time:               %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64ClockTime);
    HI_PROC_Printf(pstBuf, "clock running-time:       %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64ClockRunningTime);
    HI_PROC_Printf(pstBuf, "segment.start:            %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64SegStart);
    HI_PROC_Printf(pstBuf, "segment.stop:             %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64SegStop);
    HI_PROC_Printf(pstBuf, "segment.base:             %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64Segbase);
    HI_PROC_Printf(pstBuf, "segment.dbRate:           %f\n",      pstAudioSinkShowInfo->stAVSyncInfo.dbRate);
    HI_PROC_Printf(pstBuf, "Buffer.Pts:               %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64BufferPts);
    HI_PROC_Printf(pstBuf, "Buffer.Duration:          %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64Duration);
    HI_PROC_Printf(pstBuf, "Buffer.RunningTime:       %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64BufferRunningTime);
    HI_PROC_Printf(pstBuf, "Buffer.CurRenderDelay:    %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64CurRenderDelay);
    HI_PROC_Printf(pstBuf, "MaxLatency:               %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64MaxLatency);
    HI_PROC_Printf(pstBuf, "diff:                     %lld ms\n", pstAudioSinkShowInfo->stAVSyncInfo.i64Diff);
    HI_PROC_Printf(pstBuf, "diff=C.runningtime-B.runningtime,diff>0 means buffer is late,late<0 means buffer is early!\n");
    HI_PROC_Printf(pstBuf, "if disable avsync, diff = 0\n");

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio sink proc info_______________\n");

    pstAudioSinkCmdInfo = &(pstAudioSinkProcInfo->stCmdInfo);
    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio sink control info_______________\n");
    HI_PROC_Printf(pstBuf, "DumpSinkStream:      %d\n", pstAudioSinkCmdInfo->bDumpSinkStream);
    HI_PROC_Printf(pstBuf, "DumpSrcStream:       %d\n", pstAudioSinkCmdInfo->bDumpSrcStream);
    HI_PROC_Printf(pstBuf, "DumpStorePath:       %s\n", bench_handle->m_stPlayerControlPara.cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer audio sink control info_________________\n\n");

    return HI_SUCCESS;
}

HI_S32 ProcAsinkCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_AUDIOSINK_PROC_INFO *pstAsinkProcInfo;
    ST_AUDIOSINK_PROC_INFO  AsinkCmd;
    SmeBenchMark* bench_handle;
    E_SME_MEM_SECURE_RET eMemSecRet;

    UTILS_MLOGE_RET_VAL_IF(NULL == pPrivData, HI_FAILURE, ("pPrivData is NULL !"));

    pstAsinkProcInfo = (ST_AUDIOSINK_PROC_INFO *)pPrivData;
    bench_handle = static_cast<SmeBenchMark*>(pstAsinkProcInfo->pBenchmarkHandle);

    for (i=0; i<u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "\tArgv[%d]: %s \n", i, (HI_CHAR*)pu8Argv[i]);
    }

    eMemSecRet = VOS_Memset_S(&(AsinkCmd.stCmdInfo),
                              sizeof(ST_ASINK_CMD_INFO),
                              0xFF,
                              sizeof(ST_ASINK_CMD_INFO));
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S asink cmd info failed !"));

    eMemSecRet = VOS_Memset_S(AsinkCmd.stCmdInfo.cDumpFileStorePath,
                                             DUMP_FILE_STORE_PATH_LENGTH,
                                             0,
                                             DUMP_FILE_STORE_PATH_LENGTH);
    UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S asink data store path Failed"));
    AsinkCmd.eProcCmdType = E_SME_PROC_CMD_SET_CMD;

    switch (u32Argc)
    {
       case 1:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
           {
               ProcAsinkHelpInfo(pstBuf);
           }
           else
           {
               HI_PROC_Printf(pstBuf, "Not support command\n");
           }
           break;
       }
       case 2:
       {
           if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SINK_DATA, strlen(CMD_DUMP_SINK_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   AsinkCmd.stCmdInfo.bDumpSinkStream = 1;
                   pstAsinkProcInfo->stCmdInfo.bDumpSinkStream = 1;

                   eMemSecRet = VOS_Memcpy_S(AsinkCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S asink create sink dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump asink sink data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   AsinkCmd.stCmdInfo.bDumpSinkStream = 0;
                   pstAsinkProcInfo->stCmdInfo.bDumpSinkStream = 0;//store cmd
                   HI_PROC_Printf(pstBuf, "Stop dump asink sink data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_ASINK_INFO, &AsinkCmd, NULL);
            }
            else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMP_SRC_DATA, strlen(CMD_DUMP_SRC_DATA)))
            {
                if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_START, strlen(CMD_DUMP_START)))
                {
                   pstAsinkProcInfo->stCmdInfo.bDumpSrcStream = 1;
                   AsinkCmd.stCmdInfo.bDumpSrcStream = 1;

                   eMemSecRet= VOS_Memcpy_S(AsinkCmd.stCmdInfo.cDumpFileStorePath,
                              DUMP_FILE_STORE_PATH_LENGTH - 1,
                              bench_handle->m_stPlayerControlPara.cDumpDataStorePath,
                              (V_UINT32)(strlen(bench_handle->m_stPlayerControlPara.cDumpDataStorePath)));
                   UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memset_S asink create src dump data store path Failed"));

                   HI_PROC_Printf(pstBuf, "Start dump asink src data\n");
                }
                else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[1], CMD_DUMP_STOP, strlen(CMD_DUMP_STOP)))
                {
                   AsinkCmd.stCmdInfo.bDumpSrcStream = 0;
                   pstAsinkProcInfo->stCmdInfo.bDumpSrcStream = 0;
                   HI_PROC_Printf(pstBuf, "Stop dump asink src data\n");
                }
                g_object_set(bench_handle->m_pPlayBinHandler, GST_SME_BENCH_MARK_ASINK_INFO, &AsinkCmd, NULL);
            }
           break;
       }
       default:
       {
           HI_PROC_Printf(pstBuf, "Not support command\n");
           break;
       }
    }

    return HI_SUCCESS;
}

static HI_S32 ProcControlHelpInfo(HI_PROC_SHOW_BUFFER_S * pstBuf)
{
    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pstBuf is null, help info fail"));

    HI_PROC_Printf(pstBuf, "\n************************Player Help Info Begin*************************\n");
    HI_PROC_Printf(pstBuf, "echo %s = 0|1|2|3|4|5 >/proc/hisi/%s/control \n", CMD_SET_LOG_LEVEL, cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s = path_name > /proc/hisi/%s/control\n", CMD_DUMPSTREM_STORE_PATH, cDirectoryName);
    HI_PROC_Printf(pstBuf, "echo %s >/proc/hisi/%s/control \n", CMD_HELP_NAME, cDirectoryName);
    HI_PROC_Printf(pstBuf, "     example: 'echo DumpStorePath = /mnt > /proc/hisi/%s/control ' \n", cDirectoryName);
    HI_PROC_Printf(pstBuf, "     Log level (0|1|2|3|4|5 = NONE|ERROR|WARNING|INFO|DEBUG|VERBOSE) \n\n");
    HI_PROC_Printf(pstBuf, "echo %s > /proc/hisi/%s/control \n", CMD_FLUSH_LOG, cDirectoryName);
    HI_PROC_Printf(pstBuf, "************************Player Help Info End*************************\n");

    return HI_SUCCESS;
}

HI_S32 ProcControlShow(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{
    ST_SME_PLAYER_CONTROL_PARA *pstControlProcInfo;
    E_SME_PLAYER_LOG_LEVEL sme_loglevel;
    E_ICS_LOG_LEVEL ics_loglevel;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pPrivData is NULL !"));

    pstControlProcInfo = (ST_SME_PLAYER_CONTROL_PARA*)pPrivData;
    ics_loglevel = ICS_GetLogLevel();
    sme_loglevel = ICSLogLevelToSMELogLevel(ics_loglevel);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer Control Info Begin_______________\n");
    HI_PROC_Printf(pstBuf, "LogLevel                     :%d (0|1|2|3|4|5 = NONE|ERROR|WARNING|INFO|DEBUG|VERBOSE) \n",sme_loglevel);
    HI_PROC_Printf(pstBuf, "DumpStorePath                :%s \n", pstControlProcInfo->cDumpDataStorePath);

    HI_PROC_Printf(pstBuf, "_____________________________SmePlayer Control Info End_________________\n\n");

    return HI_SUCCESS;
}

HI_S32 ProcControlCmd(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
    HI_U32 i;
    ST_SME_PLAYER_CONTROL_PARA *pstControlProcInfo;
    V_UINT32 u32LogLevel = E_SME_PLAYER_LOG_LEVEL_INFO;
    E_SME_MEM_SECURE_RET eMemSecRet = E_SME_MEM_OK;
    V_UINT32 u32PathLen, u32ArgvLen;

    UTILS_MLOGE_RET_VAL_IF(NULL == pstBuf, HI_FAILURE, ("pPrivData is NULL !"));
    pstControlProcInfo = (ST_SME_PLAYER_CONTROL_PARA*)pPrivData;

    for (i = 0; i < u32Argc; i++)
    {
        HI_PROC_Printf(pstBuf, "Arg[%d] = %s\n", i, pu8Argv[i]);
    }

    if (u32Argc >= 3)
    {
        if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_SET_LOG_LEVEL, strlen(CMD_SET_LOG_LEVEL)))
        {
            u32LogLevel = (V_UINT32)(atoi((HI_CHAR const *)pu8Argv[2]));
            ICS_SetLogLevel(SMELogLevelToICSLogLevel(static_cast<E_SME_PLAYER_LOG_LEVEL>(u32LogLevel)));
            gst_debug_set_default_threshold (SmeLogLevelToGstLogLevel(static_cast<E_SME_PLAYER_LOG_LEVEL>(u32LogLevel)));
        }
        else if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_DUMPSTREM_STORE_PATH, strlen(CMD_DUMPSTREM_STORE_PATH)))
        {
            u32ArgvLen = (V_UINT32)(strlen((HI_CHAR const *)pu8Argv[2]));
            u32PathLen = sizeof(pstControlProcInfo->cDumpDataStorePath);

            eMemSecRet = VOS_Memcpy_S (pstControlProcInfo->cDumpDataStorePath,
                                       sizeof(pstControlProcInfo->cDumpDataStorePath) - 1,
                                       (HI_CHAR const *)pu8Argv[2],
                                       MIN((u32ArgvLen + 1), u32PathLen));
            UTILS_MLOGE_IF(eMemSecRet != E_SME_MEM_OK, ("VOS_Memcpy_S Set dump path Failed"));
        }
        else
        {
            HI_PROC_Printf(pstBuf, "Not support command\n");
        }
    }
    else if (u32Argc >= 1)
    {
        if (0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_HELP_NAME, strlen(CMD_HELP_NAME)))
        {
            ProcControlHelpInfo(pstBuf);
        }
        else if(0 == strncasecmp((HI_CHAR const *)pu8Argv[0], CMD_FLUSH_LOG, strlen(CMD_FLUSH_LOG)))
        {
            ICS_FlushLogBuff();
        }
    }
    else
    {
        HI_PROC_Printf(pstBuf, "Parameter is invalid\n");
    }

    return HI_SUCCESS;
}

V_UINT32 _ConvertTime( struct timeval time_sequence)
{
   V_UINT32 u32ConvertRes;
   V_LONG u32Unuse;

   u32ConvertRes = (V_UINT32)(time_sequence.tv_sec *1000) +
                    (V_UINT32)time_sequence.tv_usec/1000;

   u32Unuse = time_sequence.tv_usec;
   time_sequence.tv_usec = 0;
   time_sequence.tv_usec = u32Unuse;

   return u32ConvertRes;
}
