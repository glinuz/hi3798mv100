#ifndef __PROC_INFO_H__
#define __PROC_INFO_H__

#include <sys/time.h>
//#include <gst/gstclock.h>
//#include <gst/gst.h>
#include "osal_str.h"

#include "sme_player_type.h"
#include "sme_media_type.h"
#include "sme_dvbplayer_type.h"
//#include "sme_player_inf.h"

#define DUMP_FILE_STORE_PATH_LENGTH 256
#define MAX_TIME_LENGTH 64
#define MAX_CODEC_NAME 64

//player type
typedef enum __PlayerType
{
    E_SME_PLAYER = 0,
    E_DVB_PLAYER
}E_PLAYER_TYPE;

//proc cmd type
typedef enum __SmePlayerProcType
{
    E_SME_PROC_CMD_SHOW_INFO = 0,
    E_SME_PROC_CMD_SET_CMD
}E_SME_PROC_CMD_TYPE_E;

typedef struct _SmePlayerControlPara
{
    V_CHAR                                       cDumpDataStorePath[DUMP_FILE_STORE_PATH_LENGTH];
    V_VOID*                                      pBenchmarkHandle;
}ST_SME_PLAYER_CONTROL_PARA;

typedef struct _BenchmarkCommonInfo {
    ST_FRAGMENT_INFO                  fragment_info;
    V_CHAR                            unique_url[URI_MAX_LENGTH];
    V_INT32                           file_duration;
    V_INT32                           curposition;
    V_UINT32                          resolution_width;
    V_UINT32                          resolution_height;
    ST_BUFFER_STATUS                  buffer_status;
    V_UINT32                          download_rate;
    ST_CURRENT_BITRATE                current_bitrate;
    ST_AUDIO_TRACK_INFO               audio_track_info;
    ST_VIDEO_TRACK_INFO               video_track_info;

    V_UINT64                          prepare_cost_time;
    V_UINT64                          start_cost_time;
    V_UINT64                          seek_cost_time;
    V_UINT64                          reset_cost_time;
    V_UINT64                          fcc_cost_time;
    V_UINT64                          quit_cost_time;
    E_SME_PLAYER_STATE                PlayerState;
    E_SME_DVBPLAYER_STATE             DvbPlayerState;
    V_BOOL                            IsPlayerLoop;
    V_VOID*                           pBenchmarkHandle;
}ST_PROC_COMMON_INFO;

typedef struct _BenchmarkVdecInfo
{
    struct timeval     vdec_create_time;
    V_ULONG            file_frame_rate;
    V_ULONG            cur_frame_rate;
    V_ULONG            vdec_err_cnt;
    V_INT64       fir_Iframe_vdecT;
    struct timeval     get_first_I_frame;
    V_ULONG            Iframe_cnt;
}ST_VDEC_SHOW_INFO;

typedef struct _BenchmarkAdecInfo
{
    struct timeval          CreateAdecTime;
    V_UINT32                MaxBufferSize;
    V_UINT32                ADecGetFirstIFrameTimeFlag;
    struct timeval          ADecGetFirstIFrameTime;
    struct timeval          ADecFirstPlayTime;
    V_UINT32                ADecFirstPlayTimeFlag;
    V_UINT32                ADecErrorCnt;
    V_CHAR                  ADecErrorInfo[URI_MAX_LENGTH];
}ST_ADEC_SHOW_INFO;

//for avsync
typedef struct _AVSyncInfo
{
    V_INT64 i64BaseTime;//pipeline base time;
    V_INT64 i64ClockTime;//pipeline clock time;
    V_INT64 i64ClockRunningTime;//pipeline clock running-time;
    V_INT64 i64SegStart;//segment.start:ms
    V_INT64 i64SegStop;//segment.stop:ms
    V_INT64 i64Segbase;//segment.base:ms
    V_DOUBLE dbRate;//segment.rate
    V_INT64 i64BufferPts;//buffer pts:ms
    V_INT64 i64Duration;//buffer duration
    V_INT64 i64BufferRunningTime;//buffer runningtime
    V_INT64 i64MaxLatency;
    V_INT64 i64CurRenderDelay;
    V_INT64 i64Diff;//i64Diff for clock:ms, >0 is late,<0 is early.
}ST_AVSYNC_SHOW_INFO;

typedef struct _BenchmarkVSinkInfo
{
    struct timeval          VideoSinkCreatedTimeMs;
    struct timeval          VideoSinkFirstFrameTimeMs;
    V_INT64                 VideoPts;
    struct timeval          VideoFirstPlayTimeMs;
    struct timeval          VideoFinalPlayTimeMs;
    V_UINT32                DroppedVideoFrameCnt;
    V_UINT32                DroppedVideoFrameCntAtSync;
    V_BOOL                  VideoFrameIsDelay;
    V_BOOL                  IsSync;
    V_INT64                 ClockForSync;
    //for avsync
    ST_AVSYNC_SHOW_INFO     stAVSyncInfo;
}ST_VSINK_SHOW_INFO;

typedef struct _BenchmarkASinkInfo
{
    struct timeval          CreateASinkTime;
    V_INT64                 ASinkOutputPTS;
    V_UINT32                ASinkFirstPlayTimeFlag;
    struct timeval          ASinkFirstPlayTime;
    struct timeval          ASinkFinalPlayTime;
    V_UINT32                AudioFrameLostCnt;
    V_UINT32                AudioDiscardCnt;
    V_INT64                 ClockForSync;
    V_UINT64                AudioRunningTime;
    V_UINT32                AudioFrameIsDelay;
    V_UINT32                AudioMute;
    V_UINT32                AudioVolume;
    A_TRACK_CHANNEL_MODE_E  AudioChannelMode;
    //for avsync
    ST_AVSYNC_SHOW_INFO     stAVSyncInfo;
}ST_ASINK_SHOW_INFO;

//ts demux
typedef struct _BenchmarkTsDemuxInfo
{
    V_UINT32 u32Apid;
    V_UINT32 u32Vpid;
    V_CHAR   acACodecName[MAX_CODEC_NAME];
    V_CHAR   acVCodecName[MAX_CODEC_NAME];
    V_INT32  i32AudioIsCa;//0 is not cramble,1 is cramble
    V_INT32  i32VideoIsCa;//0 is not cramble,1 is cramble
    V_INT32  i32PcrSync;//0 is not,1 is pcr
    V_UINT32 u32TunerID;//hwtsdemux tuner id;
}ST_TSDEMUX_SHOW_INFO;

typedef struct _SmeTsDemuxCmdInfo
{
    V_INT8 bDumpSrcAudioStream;    // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcVideoStream;
    V_UINT32 u32TunerID;//0 is not set,others to set.
    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
}ST_TSDEMUX_CMD_INFO;

typedef struct __SmeHWTSDemuxProcInfo {
    E_SME_PROC_CMD_TYPE_E eProcCmdType;
    V_VOID*               pBenchmarkHandle;

    ST_TSDEMUX_SHOW_INFO  stShowInfo;
    ST_TSDEMUX_CMD_INFO   stCmdInfo;
}ST_SMEHWTSDEMUX_PROC_INFO;

//sme demux
typedef struct _BenchmarkDemuxInfo
{
    struct timeval     sme_demux_created_time;
    struct timeval     first_output_i_frame_time;
}ST_SMEDEMUX_SHOW_INFO;

typedef struct __SmeDemuxCmdInfo
{
    V_INT8 bDumpSrcAudioStream;    // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcVideoStream;

    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
}ST_SMEDEMUX_CMD_INFO;

typedef struct __SmeDemuxProcInfo {
    E_SME_PROC_CMD_TYPE_E eProcCmdType;
    V_VOID*                pBenchmarkHandle;

    ST_SMEDEMUX_SHOW_INFO    stShowInfo;
    ST_SMEDEMUX_CMD_INFO stCmdInfo;
}ST_SMEDEMUX_PROC_INFO;

//audio decoder
typedef struct __AudioDecCmdInfo
{
    V_INT8 bDumpSinkStream;  // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcStream;
    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
}ST_ADEC_CMD_INFO;

typedef struct __AudioDecProcInfo {
    E_SME_PROC_CMD_TYPE_E eProcCmdType;
    V_VOID*                pBenchmarkHandle;
    ST_ADEC_SHOW_INFO    stShowInfo;
    ST_ADEC_CMD_INFO stCmdInfo;
}ST_AUDIODEC_PROC_INFO;

//video decoder
typedef struct __VideoDecCmdInfo
{
    V_INT8 bDumpSinkStream;  // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcStream;
    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
}ST_VDEC_CMD_INFO;

typedef struct __VideoDecProcInfo {
    E_SME_PROC_CMD_TYPE_E   eProcCmdType;
    V_VOID*                pBenchmarkHandle;

    ST_VDEC_SHOW_INFO    stShowInfo;
    ST_VDEC_CMD_INFO     stCmdInfo;
}ST_VIDEODEC_PROC_INFO;

//video sink
typedef struct __VideoSinkCmdInfo
{
    V_INT8 bDumpSinkStream;  // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcStream;
    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
    V_INT8 i8EnableAVSync;//0: disable avsync, 0XFF:not set, others enable
}ST_VSINK_CMD_INFO;

typedef struct __VideoSinkProcInfo {
    E_SME_PROC_CMD_TYPE_E   eProcCmdType;
    V_VOID*                pBenchmarkHandle;

    ST_VSINK_SHOW_INFO    stShowInfo;
    ST_VSINK_CMD_INFO     stCmdInfo;
}ST_VIDEOSINK_PROC_INFO;

//audio sink
typedef struct __AudioSinkCmdInfo
{
    V_INT8 bDumpSinkStream;  // 0:stop dump sink data, 1:start dump sink data;
    V_INT8 bDumpSrcStream;
    V_CHAR cDumpFileStorePath[DUMP_FILE_STORE_PATH_LENGTH];
}ST_ASINK_CMD_INFO;

typedef struct __AudioSinkProcInfo {
    E_SME_PROC_CMD_TYPE_E   eProcCmdType;
    V_VOID*                pBenchmarkHandle;

    ST_ASINK_SHOW_INFO    stShowInfo;
    ST_ASINK_CMD_INFO     stCmdInfo;
}ST_AUDIOSINK_PROC_INFO;
#endif /* __PROC_INFO_H__ */
