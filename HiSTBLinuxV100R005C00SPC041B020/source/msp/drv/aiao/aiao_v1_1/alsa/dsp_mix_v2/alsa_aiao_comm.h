/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv comm
 * Description: alsa drv comm
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1
 ********************************************************************************/
#ifndef __ALSA_AIAO_COMM_H__
#define __ALSA_AIAO_COMM_H__

#include <sound/soc.h>
#include <sound/pcm.h>
#ifdef HI_ALSA_AI_SUPPORT
#include "hal_aiao_common.h"
#endif
#include "drv_ao_func.h"

//TODO
#define CONFIG_AIAO_ALSA_PROC_SUPPORT
#define CONFIG_ALSA_VOLUMN_SUPPORT
#define MUTE_FRAME_OUTPUT //with mute frame trigger

//#define MUTE_FRAME_INPUT    //For Virtual AI

#define AIAO_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 | SNDRV_PCM_RATE_16000 |\
                    SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | \
                    SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)
#define AIAO_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
                      SNDRV_PCM_FMTBIT_S24_LE)
#define CMD_START  0xa
#define CMD_STOP   0xb
#define AI_RATES SNDRV_PCM_RATE_48000

struct aiao_hwparams
{
    unsigned int channels;       /* channels */
    unsigned int rate;              /* rate in Hz */
    snd_pcm_format_t format;     /* SNDRV_PCM_FORMAT_* */
    unsigned int frame_size;
    unsigned int buffer_bytes;
    snd_pcm_uframes_t period_size;  /* period size */
    unsigned int period_bytes;
    unsigned int periods;           /* periods */
    snd_pcm_uframes_t buffer_size;  /* buffer size */
};

//private audio data
struct hiaudio_data
{
    struct file file;
    struct aiao_hwparams stAoHwParam;
    HI_VOID* pVirBaseAddr;   //alsa dma buf
    HI_VOID* pPhyBaseAddr;   //for debug

    //mutex;
    HI_HANDLE hTimerHandle;    //interrupt handle
    HI_U32 u32SndOpen;

    HI_HANDLE u32TrackId;
    atomic_t atmTrackState;

    HI_U32 u32SendTryCnt;
    HI_U32 u32HadSent;
    HI_U32 u32DiscardSent;
    HI_U32 u32SendFailCnt;
    HI_U32 u32HwPointer; //port read pointer

    AO_FRAMEINFO_S stAOFrame;
    HI_U32 u32PollTimeNs;
    HI_BOOL bTriggerStartOK;
#ifdef HI_ALSA_HRTIMER_SUPPORT
    ktime_t hrtperoid;
    struct hrtimer hrt;
    struct snd_pcm_substream* substream;
#else
    HI_U32 u32IsrConfig;
    HI_BOOL bTimerEnable;
#endif

    //struct tasklet_struct tasklet;
    struct workqueue_struct* workq;
    struct work_struct work;
    HI_U32 u32Cmd;

    HI_U32 u32MuteFrameTime;
    HI_U32 u32PortDelayMs;

#ifdef HI_ALSA_AI_SUPPORT
    struct aiao_hwparams sthwparam_ai; //ai hw params
    int ai_handle;
    struct file cfile;
    unsigned int ack_c_cnt;
    unsigned int ai_writepos;
    unsigned int ai_readpos;
    unsigned int last_c_pos;
    unsigned int current_c_pos;
    AIAO_IsrFunc* IsrProc;     //ISR func for alsa
    unsigned int isr_total_cnt_c;
#ifdef MUTE_FRAME_INPUT
    struct snd_pcm_substream* aisubstream;
    atomic_t atmRecordState;
    HI_U32 u32AiPollTimeNs;
    ktime_t aihrtperoid;
    struct hrtimer aihrt;
    struct tasklet_struct aitasklet;
#endif    //MUTE_FRAME_INPUT
#endif    //HI_ALSA_AI_SUPPORT

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    struct snd_info_entry* entry;
#endif

#ifdef CONFIG_PM
    HI_U32 u32SuspendState;
#endif
};

#ifdef CONFIG_ALSA_VOLUMN_SUPPORT
struct hiaudio_sw_volume
{
    signed int v_all;
    signed int v_hdmi;
    signed int v_spdif;
    signed int v_adac;
    signed int v_i2s;
    signed int v_capture;    //for google cts
    signed int v_mute;
};
#endif

#endif
