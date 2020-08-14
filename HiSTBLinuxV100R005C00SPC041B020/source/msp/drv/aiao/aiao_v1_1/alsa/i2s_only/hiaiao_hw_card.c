#include <linux/clk.h>
#include <linux/module.h>
#include <sound/soc.h>
#ifdef HI_ALSA_TLV320_SUPPORT 
#include "alsa_i2c.h"
#endif

//#define AIAO_ALSA_CARD_DEBUG
#ifdef AIAO_ALSA_CARD_DEBUG
#define TRP()    print(KERN_ALERT"\nfunc:%s line:%d \n", __func__, __LINE__)
#define ATRC(x...)      print(KERN_ALERT x)
#else
#define TRP() 
#define ATRC(fmt, ...) 
#endif

#ifdef HI_ALSA_TLV320_SUPPORT 
#define REG_NUM 110
int tlv320_reg[REG_NUM];
extern int hisi_alsa_codec_init(struct snd_soc_pcm_runtime *rtd);
void hisi_tlv_store_reg(struct snd_soc_card *card)
{
    int tmp;
    struct snd_soc_pcm_runtime *rtd = card->rtd;
    struct snd_soc_codec *codec = rtd->codec;
    for(tmp=2;tmp<REG_NUM;tmp++)
       tlv320_reg[tmp] = snd_soc_read(codec,tmp);
}
void hisi_tlv_write_reg(struct snd_soc_card *card)
{
    int tmp;
    struct snd_soc_pcm_runtime *rtd = card->rtd;
    struct snd_soc_codec *codec = rtd->codec;
    for(tmp=2;tmp<REG_NUM;tmp++)
    {
      snd_soc_write(codec,tmp,tlv320_reg[tmp]);
    }
}
static int hiaiao_hw_suspend_post(struct snd_soc_card *card)
{
    hisi_tlv_store_reg(card);

	return 0;
}

static int hiaiao_hw_resume_pre(struct snd_soc_card *card)
{
	int ret = 0;

	i2c_pins_init();
	godbox_aic31_reset();
   // hisi_alsa_hw_codec_init(rtd);
    hisi_tlv_write_reg(card);
	return ret;
}

static void get_clock_params(int freq,int * mclk)
{
	int clk;
    
	clk = freq*256;
	*mclk = clk;

}

static int hisi_alsa_codec_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
    struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *codec_dai = rtd->codec_dai;
	
	unsigned int freq = params_rate(params);
	unsigned int mclk;
	int ret;

    ATRC("machine hw_params get freq is %d\n",freq);
    
    ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
			SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
	if (ret < 0) return ret;

	get_clock_params(freq,&mclk);

     ATRC("machine hw_params get mclk is %d\n",mclk);

	ret = snd_soc_dai_set_sysclk(codec_dai, 0, mclk, 0);
	if (ret < 0) 
        return ret;
    
    return 0;
}
/*
static int hisi_alsa_codec_prepare(struct snd_pcm_substream *substream)
{
    int ret = 1;
    if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
        godbox_aic31_mute(0);
    
    return ret;

}
static void hisi_alsa_codec_shutdown(struct snd_pcm_substream *substream)
{
    if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
        godbox_aic31_mute(1);

}
*/
static struct snd_soc_ops hisi_hw_codec_ops = {
	.hw_params = hisi_alsa_codec_hw_params,
   // .prepare = hisi_alsa_codec_prepare,
   // .shutdown = hisi_alsa_codec_shutdown,
};

/* had-aiao digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link hiaiao_i2s_dai[] = {
    { /* Primary Playback i/f */
    	.name = "AIAO Primary ",
    	.stream_name = "Playback capture",
    	.cpu_dai_name = "hisi-i2s-only",			
    	.codec_dai_name = "tlv320aic3x-hifi",		
    	.platform_name = "hisi-audio-i2s",		
    	.codec_name = "tlv320aic3x-codec.0-0018",			
    	.init = hisi_alsa_codec_init,	
    	.ops = &hisi_hw_codec_ops,
    },
};
#else
static int hiaiao_hw_suspend_post(struct snd_soc_card *card)
{

	return 0;
}

static int hiaiao_hw_resume_pre(struct snd_soc_card *card)
{
	int ret = 0;

	return ret;
}

static int hisi_codec_init(struct snd_soc_pcm_runtime *rtd)
{
    /* init codec */
    return 0;
}
static int hisi_codec_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
    /* set codec clock system */
    return 0;
}

static struct snd_soc_ops hisi_codec_ops = {
	.hw_params = hisi_codec_hw_params,
};

/* had-aiao digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link hiaiao_i2s_dai[] = {
    { /* Primary Playback i/f */
    	.name = "AIAO Primary",
    	.stream_name = "Playback capture",
    	.cpu_dai_name = "hisi-i2s-only",			
    	.codec_dai_name = "aiao-hifi-i2s-only",		
    	.platform_name = "hisi-audio-i2s",		
    	.codec_name = "hi-aiao-i2s",			
    	.init = hisi_codec_init,	
    	.ops = &hisi_codec_ops,
    },
};
#endif

static struct snd_soc_card hw_snd_soc_hiaiao = {
    .name = "HISI-AIAO-I2S",
    .owner = THIS_MODULE,
    .dai_link = hiaiao_i2s_dai,
    .num_links =ARRAY_SIZE(hiaiao_i2s_dai),
    .dapm_widgets = NULL,       //TODO ALSA Control
    .num_dapm_widgets = 0,
    .dapm_routes = NULL,
    .num_dapm_routes = 0,
    .controls = NULL,
    .num_controls = 0,
    .suspend_post = &hiaiao_hw_suspend_post,
    .resume_pre = &hiaiao_hw_resume_pre,
};

static struct platform_device *hisi_hw_snd_device;
int hisndcard_hw_init(void)
{
    int ret = 0;
    //register device
    hisi_hw_snd_device = platform_device_alloc("soc-audio", -1);
    if (!hisi_hw_snd_device) {
    	return -ENOMEM;
    }
    platform_set_drvdata(hisi_hw_snd_device,&hw_snd_soc_hiaiao);
    ret = platform_device_add(hisi_hw_snd_device);
    if (ret) {
    	platform_device_put(hisi_hw_snd_device);
       
    }
   return ret;
}

void hisndcard_hw_exit(void)
{
    platform_device_unregister(hisi_hw_snd_device);
  //  platform_driver_unregister(&htd_hw_sndcard_driver);
}
extern int hiaudio_hw_dma_init(void);
extern int hiaudio_hw_i2s_init(void);
extern int aiao_hw_codec_init(void);
extern void aiao_hw_codec_deinit(void);
extern void hiaudio_hw_i2s_deinit(void);
extern void hiaudio_hw_dma_deinit(void);


//static int __init hisnd_module_init(void)
int I2S_ALSA_ModInit(void)
{
    int ret;

    //step 1  register platform dma component
    ret = hiaudio_hw_dma_init();
    if(ret)
    {
        TRP();
        return ret;
    }
    //step 2  register cpu dai component
    ret = hiaudio_hw_i2s_init();
    if(ret)
    {
        TRP();
        goto  Err_I2s_Init;
    }
    //step 3 register codec component
    ret = aiao_hw_codec_init();
    if(ret)
    {
        TRP();
        goto  Err_Codec_Init;
    }
    //step 4 register card link 
    ret = hisndcard_hw_init();
    if(ret)
    {
        TRP();
        goto  Err_SndCard_Init;
    }
    return 0;

Err_SndCard_Init:
    aiao_hw_codec_deinit();
Err_Codec_Init:
    hiaudio_hw_i2s_deinit();
Err_I2s_Init:
    hiaudio_hw_dma_deinit();

    return ret;
}

//static void __exit hisnd_module_deinit(void)
void I2S_ALSA_ModExit(void)
{
    aiao_hw_codec_deinit();
    hiaudio_hw_i2s_deinit();
    hiaudio_hw_dma_deinit();
    hisndcard_hw_exit();
    
}

MODULE_AUTHOR("<HISI Audio Team>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hisi aiao ALSA I2S only driver");
MODULE_SUPPORTED_DEVICE("{{cv200}}");
