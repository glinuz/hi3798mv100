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

#include "hi_drv_ao.h"
#include "alsa_aiao_hdmi_comm.h"
#include "drv_ao_hdmi_func.h"
#include "hi_drv_mem.h"

#ifdef CONFIG_HDMI_ALSA_VOLUMN_SUPPORT

//#define AIAO_ALSA_DEBUG
#ifdef AIAO_ALSA_DEBUG
#define ATRP()    print(KERN_ALERT"\nfunc:%s line:%d ", __func__, __LINE__)
#define ATRC      print
#else
#define ATRP() 
#define ATRC(fmt, ...) 
#endif

#define HI_VOLUME_ALL     0
#define HI_VOLUME_HDMI  1
#define HI_VOLUME_SPDIF 2
#define HI_VOLUME_ADAC  3
#define HI_VOLUME_I2S  4

struct hiaudio_hdmi_sw_volume *hdmiswvol = HI_NULL;  //global volune 

/* hisi mixer control */
struct hisoc_hdmi_mixer_control {
    int index;
    int volume_all, volume_hdmi, volume_spdif, volume_adac;
    int max, min;
};

static int snd_soc_hdmi_info_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_info *uinfo)
{
    struct hisoc_hdmi_mixer_control *mc = (struct hisoc_hdmi_mixer_control *)kcontrol->private_value;

    ATRP();
    
    uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 2;       //default stereo
    uinfo->value.integer.min = mc->min;
    uinfo->value.integer.max = mc->max;
    return 0;
}

static int snd_soc_hdmi_get_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    struct hisoc_hdmi_mixer_control *mc = (struct hisoc_hdmi_mixer_control *)kcontrol->private_value;
    HI_UNF_SND_GAIN_ATTR_S stGain;
    int err;
    ATRP();

    switch(mc->index)
    {
            //ATRC("\nget  HI_VOLUME_ALL %d", hswvol->v_all);

        case HI_VOLUME_HDMI:
            err = ao_hdmi_get_volume(HI_UNF_SND_1,HI_UNF_SND_OUTPUTPORT_HDMI0,&stGain);
            if(err == HI_SUCCESS)
            {   hdmiswvol->v_hdmi = stGain.s32Gain;
            ucontrol->value.integer.value[0]= hdmiswvol->v_hdmi;
            ucontrol->value.integer.value[1]= hdmiswvol->v_hdmi;
            ATRC("\nget  HI_VOLUME_HDMI %d\n", hdmiswvol->v_hdmi);
            }
            else
            {
                ATRC("\n You should Open the Card first !!!\n");
            }
            break;
            /*
         case HI_VOLUME_ALL:
            ucontrol->value.integer.value[0]= hdmiswvol->v_all;
            ucontrol->value.integer.value[1]= hdmiswvol->v_all;
            //ATRC("\nget  HI_VOLUME_ALL %d", hdmiswvol->v_all);
            break;
        case HI_VOLUME_SPDIF:
            ucontrol->value.integer.value[0]= hdmiswvol->v_spdif;
            ucontrol->value.integer.value[1]= hdmiswvol->v_spdif;
            //ATRC("\nget  HI_VOLUME_SPDIF %d", hswvol->v_spdif);
            break;
        case HI_VOLUME_ADAC:
            ucontrol->value.integer.value[0]= hdmiswvol->v_adac;
            ucontrol->value.integer.value[1]= hdmiswvol->v_adac;
            //ATRC("\nget  HI_VOLUME_ADAC %d", hswvol->v_adac);
            break;*/
        default : 
            break;
    }

    return 0;
}

static int snd_soc_hdmi_put_hisivolsw(struct snd_kcontrol *kcontrol,
    struct snd_ctl_elem_value *ucontrol)
{
    struct hisoc_hdmi_mixer_control *mc =   (struct hisoc_hdmi_mixer_control *)kcontrol->private_value;
    int err;
    unsigned int val;
    HI_UNF_SND_GAIN_ATTR_S stGain;

    ATRP();

    val = ucontrol->value.integer.value[0];
    ATRC("\n  put val  %d\n", val);
    stGain.bLinearMode = HI_TRUE;
    stGain.s32Gain = val;

    switch(mc->index)
    {
            //ATRC("\nput  HI_VOLUME_ALL %d", stGain.s32Gain);

        case HI_VOLUME_HDMI:
            ATRC("\nput  HI_VOLUME_HDMI %d", stGain.s32Gain);
            err = ao_hdmi_set_volume(HI_UNF_SND_1, HI_UNF_SND_OUTPUTPORT_HDMI0, stGain);
            if(!err)
            {
                hdmiswvol->v_hdmi = stGain.s32Gain;
            }
            else
            {
                ATRC("\n You should Open the Card first !!!\n");
            }
            break;

      /*  case HI_VOLUME_SPDIF:
            //ATRC("\nput  HI_VOLUME_SPDIF %d", stGain.s32Gain);
            err = ao_hdmi_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_SPDIF0, stGain);
            if(!err)
                hswvol->v_spdif = stGain.s32Gain;
            break;
            
        case HI_VOLUME_ADAC:
            //ATRC("\nput  HI_VOLUME_ADAC %d", stGain.s32Gain);
            err = ao_hdmi_set_volume(HI_UNF_SND_0, HI_UNF_SND_OUTPUTPORT_DAC0, stGain);
            if(!err)
                hswvol->v_adac= stGain.s32Gain;
            break;*/
        default : 
            err = -1;
            break;
    }
    return err;
}


#define HISOC_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
    ((unsigned long)&(struct hisoc_hdmi_mixer_control) \
    {.index = xindex, .volume_all = xall, .volume_hdmi = xhdmi, \
    .volume_spdif = xspdif, .volume_adac = xadac, .max = xmax, \
    .min = xmin})

#define HISOC_SINGLE_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
    HISOC_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin)

#define HISOC_SINGLE(xname, xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) \
{    .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = xname, \
     .info = snd_soc_hdmi_info_hisivolsw, .get = snd_soc_hdmi_get_hisivolsw,\
     .access = SNDRV_CTL_ELEM_ACCESS_READWRITE,\
     .put = snd_soc_hdmi_put_hisivolsw, \
     .private_value =  HISOC_SINGLE_VALUE(xindex, xall, xhdmi, xspdif, xadac,xmax, xmin) }

static const struct snd_kcontrol_new hisi_snd_hdmi_controls[] = {
//    HISOC_SINGLE("Master Playback Volume", HI_VOLUME_ALL , 99, 99, 99, 99, 99, 0),
    HISOC_SINGLE("PCM Playback Volume", HI_VOLUME_HDMI , 99, 99, 99, 99, 99, 0),
 //   HISOC_SINGLE("Spdif Playback Volume", HI_VOLUME_SPDIF , 99, 99, 99, 99, 99, 0),
 //   HISOC_SINGLE("Adac Playback Volume", HI_VOLUME_ADAC , 99, 99, 99, 99, 99, 0),
};

int hiaudio_hdmi_volume_register(struct snd_soc_codec *codec)
{
    hdmiswvol = HI_KZALLOC(HI_ID_AO, sizeof(struct hiaudio_hdmi_sw_volume), GFP_KERNEL);
    if (HI_NULL == hdmiswvol)
    {
        HI_ERR_AO("HI_KZALLOC hiaudio_hdmi_sw_volume failed!\n");
        return -ENOMEM; 
    }
    hdmiswvol->v_all = 99;    //default value 
    hdmiswvol->v_hdmi = 99;
    hdmiswvol->v_spdif = 99;
    hdmiswvol->v_adac = 99;
    //hswvol->v_i2s = 99;
    return snd_soc_add_codec_controls(codec, hisi_snd_hdmi_controls, ARRAY_SIZE(hisi_snd_hdmi_controls));
}

void hiaudio_hdmi_volume_unregister(void)
{
    if(hdmiswvol)
    {
        HI_KFREE(HI_ID_AO, hdmiswvol);
        hdmiswvol = HI_NULL;
    }
}
#endif
