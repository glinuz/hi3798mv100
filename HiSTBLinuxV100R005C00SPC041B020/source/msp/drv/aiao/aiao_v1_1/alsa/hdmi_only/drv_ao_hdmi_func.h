/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_aiao_alsa_func.h
 * Description: drv aiao alsa func h
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    
 ********************************************************************************/

#ifndef __DRV_AIAO_HDMI_ALSA_FUNC_H__
#define __DRV_AIAO_HDMI_ALSA_FUNC_H__

#include "hi_type.h"
#include <linux/fs.h>
#include "hi_unf_sound.h"
#include "hal_aiao_common.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */
int ao_dma_hdmi_stop(HI_UNF_SND_E snd_idx, void * p);
int ao_dma_hdmi_start(HI_UNF_SND_E snd_idx, void * p);
int ao_dma_hdmi_prepare(int dma_Index, void * p);
int ao_dma_hdmi_gethwparam(void *param);
int hi_ao_hdmi_alsa_get_proc_func(AIAO_IsrFunc **pFunc);
int hi_ao_hdmi_alsa_set_proc_func(AIAO_IsrFunc *pFunc);

//int hi_ao_alsa_update_readptr(HI_UNF_SND_E snd_idx,HI_U32 *pu32WritePos);
//void hi_ao_alsa_query_writepos(HI_UNF_SND_E snd_idx,HI_U32 *pos);
int hi_ao_hdmi_alsa_update_writeptr(HI_UNF_SND_E snd_idx,HI_U32 *pu32WritePos);

void hi_ao_hdmi_alsa_query_readpos(HI_UNF_SND_E snd_idx,HI_U32 *pos);

//Playback

int ao_dma_hdmi_getopendefparam(void *p);
int  ao_dma_hdmi_open(void *p, void *pAlsaAttr, struct file *file);
int  ao_dma_hdmi_close(struct file *file,HI_UNF_SND_E snd_idx);
int ao_dma_hdmi_flushbuf(HI_UNF_SND_E snd_idx);
int ao_hdmi_set_volume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S stGain);
int ao_hdmi_get_volume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S *stGain);


#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif 
