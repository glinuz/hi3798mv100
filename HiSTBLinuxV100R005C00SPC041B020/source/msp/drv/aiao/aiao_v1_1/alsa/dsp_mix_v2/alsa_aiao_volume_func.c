/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: alsa_aiao_volume_func.c
 * Description: aiao alsa set volume interface func
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    
 ********************************************************************************/

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/control.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/slab.h>

#include "hi_drv_mem.h"

#include "hi_drv_ao.h"
#include "alsa_aiao_comm.h"
#include "drv_ao_func.h"

#ifdef CONFIG_ALSA_VOLUMN_SUPPORT

//#define AIAO_ALSA_DEBUG
#ifdef AIAO_ALSA_DEBUG
#define ATRP()    print(KERN_ALERT"\nfunc:%s line:%d ", __func__, __LINE__)
#define ATRC    print
#else
#define ATRP()
#define ATRC(fmt, ...)
#endif

#define HI_VOLUME_ALL     0
#define HI_VOLUME_HDMI  1
#define HI_VOLUME_SPDIF 2
#define HI_VOLUME_ADAC  3
#define HI_VOLUME_I2S  4

#define HI_VOLUME_CAPTURE  5
#define HI_VOLUME_SWITCH  6
#define HI_OUTPUT_DELAY   7


struct hiaudio_sw_volume *hswvol = HI_NULL;    //global volune 

/* hisi mixer control */
struct hisoc_mixer_control {
    int index;
    int volume_all, volume_hdmi, volume_spdif, volume_adac;
    int max, min;
};

int snd_soc_info_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    struct hisoc_mixer_control *mc = (struct hisoc_mixer_control *)kcontrol->private_value;

    ATRP();
    
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;       //default stereo
    uinfo->value.integer.min = mc->min;
    uinfo->value.integer.max = mc->max;
    return 0;
}

int snd_soc_get_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    struct hisoc_mixer_control *mc = (struct hisoc_mixer_control *)kcontrol->private_value;

    ATRP();

    switch(mc->index)
    {
        case HI_VOLUME_ALL:
            ucontrol->value.integer.value[0]= hswvol->v_all;
            ucontrol->value.integer.value[1]= hswvol->v_all;
            //ATRC("\nget  HI_VOLUME_ALL %d", hswvol->v_all);
            break;
        case HI_VOLUME_HDMI:
            ucontrol->value.integer.value[0]= hswvol->v_hdmi;
            ucontrol->value.integer.value[1]= hswvol->v_hdmi;
            //ATRC("\nget  HI_VOLUME_HDMI %d", hswvol->v_hdmi);
            break;
        case HI_VOLUME_SPDIF:
            ucontrol->value.integer.value[0]= hswvol->v_spdif;
            ucontrol->value.integer.value[1]= hswvol->v_spdif;
            //ATRC("\nget  HI_VOLUME_SPDIF %d", hswvol->v_spdif);
            break;
        case HI_VOLUME_ADAC:
            ucontrol->value.integer.value[0]= hswvol->v_adac;
            ucontrol->value.integer.value[1]= hswvol->v_adac;
            //ATRC("\nget  HI_VOLUME_ADAC %d", hswvol->v_adac);
            break;
        case HI_VOLUME_CAPTURE:
            ucontrol->value.integer.value[0]= hswvol->v_capture;
            ucontrol->value.integer.value[1]= hswvol->v_capture;
            //ATRC("\nget  HI_VOLUME_CAPTURE %d", hswvol->v_capture);
            break;
        default : 
            break;
    }

    return 0;
}

int snd_soc_put_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    struct hisoc_mixer_control *mc =    (struct hisoc_mixer_control *)kcontrol->private_value;
    int err;
    unsigned int val;
    HI_UNF_SND_GAIN_ATTR_S stGain;

    ATRP();

    val = ucontrol->value.integer.value[0];
    //ATRC("\n  put val  %d\n", val);
    stGain.bLinearMode = HI_TRUE;
    stGain.s32Gain = val;

    switch(mc->index)
    {
        case HI_VOLUME_ALL:
            //ATRC("\nput  HI_VOLUME_ALL %d", stGain.s32Gain);
            err = aoe_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL, &stGain);
            if (!err)
            {
                hswvol->v_all = stGain.s32Gain;
                hswvol->v_hdmi = stGain.s32Gain;
                hswvol->v_spdif = stGain.s32Gain;
                hswvol->v_adac = stGain.s32Gain;
            }
            break;
        case HI_VOLUME_HDMI:
            //ATRC("\nput  HI_VOLUME_HDMI %d", stGain.s32Gain);
            err = aoe_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_HDMI0, &stGain);
            if(!err)
                hswvol->v_hdmi = stGain.s32Gain;
            break;

        case HI_VOLUME_SPDIF:
            //ATRC("\nput  HI_VOLUME_SPDIF %d", stGain.s32Gain);
            err = aoe_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, &stGain);
            if(!err)
                hswvol->v_spdif = stGain.s32Gain;
            break;
            
        case HI_VOLUME_ADAC:
            //ATRC("\nput  HI_VOLUME_ADAC %d", stGain.s32Gain);
            err = aoe_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_DAC0, &stGain);
            if(!err)
                hswvol->v_adac= stGain.s32Gain;
            break;
        case HI_VOLUME_CAPTURE:
            //ATRC("\nput  HI_VOLUME_CAPTURE %d", stGain.s32Gain);
            hswvol->v_capture= stGain.s32Gain;
            err = 0;
            break;
        default : 
            err = -1;
            break;
    }
    return err;
}

int snd_soc_info_hisi_switch(struct snd_kcontrol* kcontrol,
                             struct snd_ctl_elem_info* uinfo)
{
    ATRP();
    uinfo->type = SNDRV_CTL_ELEM_TYPE_BOOLEAN;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 1;
    return 0;
}

int snd_soc_get_hisi_switch(struct snd_kcontrol* kcontrol,
                            struct snd_ctl_elem_value* ucontrol)
{
    ucontrol->value.integer.value[0] = hswvol->v_mute;
    return 0;
}

int snd_soc_put_hisi_switch(struct snd_kcontrol* kcontrol,
                            struct snd_ctl_elem_value* ucontrol)
{
    unsigned int val;
    int ret;

    val = ucontrol->value.integer.value[0];
    ret = aoe_set_mute(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_ALL, (HI_BOOL)val);
    if (HI_SUCCESS == ret)
    {
        hswvol->v_mute = val;
    }
    //ATRC("snd_soc_put_hisi_switch val = %d\n", val);
    return ret;
}

int snd_soc_info_hisi_output_delay(struct snd_kcontrol* kcontrol,
                                   struct snd_ctl_elem_info* uinfo)
{
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
    uinfo->value.integer.min = 0;
    uinfo->value.integer.max = 10000;

    return 0;
}

int snd_soc_get_hisi_output_delay(struct snd_kcontrol* kcontrol,
                                  struct snd_ctl_elem_value* ucontrol)
{
    int err;
    HI_U32 delayMS = 0;

    err = aoe_get_delayms(HI_UNF_SND_0, &delayMS);
    if (!err)
    {
        ucontrol->value.integer.value[0] = delayMS;
    }
    else
    {
        ucontrol->value.integer.value[0] = 0;
    }

    return 0;
}

int snd_soc_put_hisi_output_delay(struct snd_kcontrol* kcontrol,
                                  struct snd_ctl_elem_value* ucontrol)
{
    return 0;
}

#define HISOC_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
    ((unsigned long)&(struct hisoc_mixer_control) \
    {.index = xindex, .volume_all = xall, .volume_hdmi = xhdmi, \
    .volume_spdif = xspdif, .volume_adac = xadac, .max = xmax, \
    .min = xmin})

#define HISOC_SINGLE_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
    HISOC_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin)

#define HISOC_SINGLE(xname, xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
{   .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
    .info = snd_soc_info_hisivolsw, .get = snd_soc_get_hisivolsw,\
    .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,\
    .put = snd_soc_put_hisivolsw, \
    .private_value =  HISOC_SINGLE_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin)\
}

#define HISOC_SINGLE_SWITCH(xname, xindex) \
{ \
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
    .name = xname, \
    .info = snd_soc_info_hisi_switch, \
    .get = snd_soc_get_hisi_switch, \
    .access = SNDRV_CTL_ELEM_ACCESS_READWRITE, \
    .put = snd_soc_put_hisi_switch, \
}

#define HISOC_SINGLE_OUTPUTDELAY(xname, xindex) \
{ \
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER, \
    .name = xname, \
    .info = snd_soc_info_hisi_output_delay, \
    .get = snd_soc_get_hisi_output_delay, \
    .access = SNDRV_CTL_ELEM_ACCESS_READWRITE, \
    .put = snd_soc_put_hisi_output_delay, \
}

static const struct snd_kcontrol_new hisi_snd_controls[] =
{
    HISOC_SINGLE("PGA Capture Volume", HI_VOLUME_CAPTURE, 99, 99, 99, 99, 99, 0),  //"PGA" is matched for Android code
    HISOC_SINGLE("Master Playback Volume", HI_VOLUME_ALL , 99, 99, 99, 99, 99, 0),
    HISOC_SINGLE("Hdmi Playback Volume", HI_VOLUME_HDMI , 99, 99, 99, 99, 99, 0),
    HISOC_SINGLE("Spdif Playback Volume", HI_VOLUME_SPDIF , 99, 99, 99, 99, 99, 0),
    HISOC_SINGLE("Adac Playback Volume", HI_VOLUME_ADAC , 99, 99, 99, 99, 99, 0),
    HISOC_SINGLE_SWITCH("ALL Playback Switch", HI_VOLUME_SWITCH),
    HISOC_SINGLE_OUTPUTDELAY("Output Delay", HI_OUTPUT_DELAY)
};

int hiaudio_volume_register(struct snd_soc_codec *codec)
{
    hswvol = HI_KZALLOC(HI_ID_AO, sizeof(struct hiaudio_sw_volume), GFP_KERNEL);
    if (HI_NULL == hswvol)
    {
        HI_ERR_AO("HI_KZALLOC hiaudio_sw_volume failed!\n");
        return HI_FAILURE;
    }
    hswvol->v_all = 30;    //default value 
    hswvol->v_hdmi = 30;
    hswvol->v_spdif = 30;
    hswvol->v_adac = 30;
    hswvol->v_capture = 30;
    hswvol->v_mute = 0;
    //hswvol->v_i2s = 99;
    return snd_soc_add_codec_controls(codec, hisi_snd_controls, ARRAY_SIZE(hisi_snd_controls));
}

void hiaudio_volume_unregister(void)
{
    if(hswvol)
    {
        HI_KFREE(HI_ID_AO, hswvol);
        hswvol = HI_NULL;
    }
}

#endif
