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
#ifndef __ALSA_I2S_AIAO_COMM_H__
#define __ALSA_I2S_AIAO_COMM_H__

#include <sound/soc.h>
#include <sound/pcm.h>
#if 1//def HI_ALSA_AI_SUPPORT
#include "hal_aiao_common.h"
#endif
//TODO 
#define CONFIG_AIAO_ALSA_PROC_SUPPORT
#define USE_DSP_ISR
//#define CONFIG_I2S_ALSA_VOLUMN_SUPPORT

#define AIAO_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |\
	SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | SNDRV_PCM_RATE_44100 | \
	SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)

#define AIAO_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE)






struct hisi_aiao_hw_hwparams {
	unsigned int channels;		 /* channels */
	unsigned int rate;		        /* rate in Hz */
	snd_pcm_format_t format;	 /* SNDRV_PCM_FORMAT_* */
       unsigned int frame_size;
       unsigned int buffer_bytes;
	snd_pcm_uframes_t period_size;  /* period size */
	unsigned int periods;		          /* periods */
	snd_pcm_uframes_t buffer_size;  /* buffer size */
};

//private hisi audio data
struct hii2saudio_data {
    struct hisi_aiao_hw_hwparams sthwparam;    //aiao hw params
    struct hisi_aiao_hw_hwparams sthwparam_ai;    //aiao hw params
    struct mutex mutex;
    
    int open_cnt;    //snd open counter 

    
    int hw_readpos; //port read pointer
    //todo
    
    struct file file;
    //isr
    unsigned int local_isr_num;
    unsigned int aiao_isr_num;
    

    //ack
    
    unsigned int runtime_appl_offset;  
    AIAO_IsrFunc *IsrAoProc;     //ISR func for alsa
	#if 1//def HI_ALSA_AI_SUPPORT
	int ai_handle;
	struct file cfile;
	unsigned int ai_writepos;
    unsigned int ai_readpos;
    AIAO_IsrFunc *IsrAiProc;     //ISR func for alsa
    unsigned int isr_total_cnt_c;
	#endif   

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    //pointer
    struct snd_info_entry *entry;
#endif

};

#ifdef CONFIG_I2S_ALSA_VOLUMN_SUPPORT
struct hiaudio_sw_volume {
    signed int v_all;
    signed int v_hdmi;
    signed int v_spdif;
    signed int v_adac;
    signed int v_i2s;
};
#endif

#endif
