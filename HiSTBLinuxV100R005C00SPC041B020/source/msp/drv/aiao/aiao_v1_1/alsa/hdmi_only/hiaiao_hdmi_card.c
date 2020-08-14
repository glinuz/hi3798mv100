#include <linux/clk.h>
#include <linux/module.h>
#include <sound/soc.h>
#if 0//def HI_ALSA_TLV320_SUPPORT 
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
static int hiaiao_hdmi_suspend_post(struct snd_soc_card *card)
{
    return 0;
}

static int hiaiao_hdmi_resume_pre(struct snd_soc_card *card)
{
    int ret = 0;

    return ret;
}

static int hisi_codec_init(struct snd_soc_pcm_runtime *rtd)
{
    /* init codec */
    return 0;
}
static int hisi_codec_hdmi_params(struct snd_pcm_substream *substream,
    struct snd_pcm_hw_params *params)
{
    /* set codec clock system */
    return 0;
}

static struct snd_soc_ops hisi_codec_ops = {
    .hw_params = hisi_codec_hdmi_params,
};

/* had-aiao digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link hiaiao_hdmi_dai[] = {
    { /* Primary Playback i/f */
        .name = "AIAO Primary",
        .stream_name = "Playback capture",
        .cpu_dai_name = "hisi-hdmi-only",           
        .codec_dai_name = "aiao-hifi-hdmi-only",        
        .platform_name = "hisi-audio-hdmi",     
        .codec_name = "hi-aiao-hdmi",           
        .init = hisi_codec_init,    
        .ops = &hisi_codec_ops,
    },
};

static struct snd_soc_card hdmi_snd_soc_hiaiao = {
    .name = "HISI-AIAO-HDMI",
    .owner = THIS_MODULE,
    .dai_link = hiaiao_hdmi_dai,
    .num_links =ARRAY_SIZE(hiaiao_hdmi_dai),
    .dapm_widgets = NULL,       //TODO ALSA Control
    .num_dapm_widgets = 0,
    .dapm_routes = NULL,
    .num_dapm_routes = 0,
    .controls = NULL,
    .num_controls = 0,
    .suspend_post = &hiaiao_hdmi_suspend_post,
    .resume_pre = &hiaiao_hdmi_resume_pre,
};

static __init int htd_hdmi_snd_dev_probe(struct platform_device *pdev)
{
    struct snd_soc_card *card = &hdmi_snd_soc_hiaiao;
    int ret;

    card->dev = &pdev->dev;
    ret = snd_soc_register_card(card);
    if (ret) {
        dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
            ret);
    }

    return ret;
}

static __exit int htd_hdmi_snd_dev_remove(struct platform_device *pdev)
{
    struct snd_soc_card *card = platform_get_drvdata(pdev);

    snd_soc_unregister_card(card);
    return 0;
}

static struct platform_driver htd_hdmi_sndcard_driver = {
    .driver = {
        .name = "hisi-hdmi_only-sndcard",
        .owner = THIS_MODULE,
        .pm = &snd_soc_pm_ops,
    },
    .probe  = htd_hdmi_snd_dev_probe,
    .remove = __exit_p(htd_hdmi_snd_dev_remove),
};

static struct platform_device *hisi_hdmi_snd_device;

int hisndcard_hdmi_init(void)
{
    int ret = 0;
    //register device
    hisi_hdmi_snd_device = platform_device_alloc("hisi-hdmi_only-sndcard", -1);
    if (!hisi_hdmi_snd_device) {
        return -ENOMEM;
    }
    
    ret = platform_device_add(hisi_hdmi_snd_device);
    if (ret) {
        platform_device_put(hisi_hdmi_snd_device);
       return ret;
    }

    //register drvier
    return platform_driver_register(&htd_hdmi_sndcard_driver);
}

void hisndcard_hdmi_exit(void)
{
    platform_device_unregister(hisi_hdmi_snd_device);
    platform_driver_unregister(&htd_hdmi_sndcard_driver);
}

extern int  hiaudio_hdmi_dma_init(void);
extern int  hiaudio_hdmi_i2s_init(void);
extern int  aiao_hdmi_codec_init(void);
extern void aiao_hdmi_codec_deinit(void);
extern void hiaudio_hdmi_i2s_deinit(void);
extern void hiaudio_hdmi_dma_deinit(void);

int HDMI_ALSA_ModInit(void)
{
    int ret;

    //step 1  register platform dma component
    ret = hiaudio_hdmi_dma_init();
    if(ret)
    {
        TRP();
        return ret;
    }
    //step 2  register cpu dai component
    ret = hiaudio_hdmi_i2s_init();
    if(ret)
    {
        TRP();
        goto  Err_I2s_Init;
    }
    //step 3 register codec component
    ret = aiao_hdmi_codec_init();
    if(ret)
    {
        TRP();
        goto  Err_Codec_Init;
    }
    //step 4 register card link 
    ret = hisndcard_hdmi_init();
    if(ret)
    {
        TRP();
        goto  Err_SndCard_Init;
    }
    return 0;

Err_SndCard_Init:
    aiao_hdmi_codec_deinit();
Err_Codec_Init:
    hiaudio_hdmi_i2s_deinit();
Err_I2s_Init:
    hiaudio_hdmi_dma_deinit();

    return ret;
}

//static void __exit hisnd_module_deinit(void)
void HDMI_ALSA_ModExit(void)
{
    aiao_hdmi_codec_deinit();
    hiaudio_hdmi_i2s_deinit();
    hiaudio_hdmi_dma_deinit();
    hisndcard_hdmi_exit();
}

MODULE_AUTHOR("<HISI Audio Team>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hisi aiao ALSA HDMI only driver");
MODULE_SUPPORTED_DEVICE("{{cv200}}");
