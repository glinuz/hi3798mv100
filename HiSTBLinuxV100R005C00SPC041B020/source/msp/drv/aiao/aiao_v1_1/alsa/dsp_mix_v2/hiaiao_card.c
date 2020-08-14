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

static struct snd_soc_card snd_soc_hiaiao;

#ifdef HI_ALSA_TLV320_SUPPORT
extern int hisi_alsa_codec_init(struct snd_soc_pcm_runtime* rtd);

void get_clock_params(int freq, int* mclk)
{
    int clk;
    clk = freq * 256;
    *mclk = clk;
}

static int hisi_alsa_codec_hw_params(struct snd_pcm_substream* substream,
                                     struct snd_pcm_hw_params* params)
{
    struct snd_soc_pcm_runtime* rtd = substream->private_data;
    struct snd_soc_dai* codec_dai = rtd->codec_dai;
    unsigned int freq = params_rate(params);
    unsigned int mclk;
    int ret;
    ATRC("machine hw_params get freq is %d\n", freq);
    ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S |
                              SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
    if (ret < 0)
    {
        return ret;
    }
    get_clock_params(freq, &mclk);
    ATRC("machine hw_params get mclk is %d\n", mclk);
    ret = snd_soc_dai_set_sysclk(codec_dai, 0, mclk, 0);
    if (ret < 0)
    {
        return ret;
    }
    return 0;
}

static struct snd_soc_ops hisi_codec_ops =
{
    .hw_params = hisi_alsa_codec_hw_params,
};

static struct snd_soc_dai_link hiaiao_dai[] =
{
    { /* Primary Playback i/f */
        .name = "AIAO Primary ",
        .stream_name = "Playback",
        .cpu_dai_name = "hisi-i2s",
        .codec_dai_name = "tlv320aic3x-hifi",
        .platform_name = "hisi-audio",
        .codec_name = "tlv320aic3x-codec.0-0018",
        .init = hisi_alsa_codec_init,   //#if 1//def HI_ALSA_TLV320_SUPPORT
        .ops = &hisi_codec_ops,
    },
};
#else
static int hisi_codec_init(struct snd_soc_pcm_runtime* rtd)
{
    /* init codec */
    return 0;
}

static int hisi_codec_hw_params(struct snd_pcm_substream* substream,
                                struct snd_pcm_hw_params* params)
{
    /* set codec clock system */
    return 0;
}

static struct snd_soc_ops hisi_codec_ops =
{
    .hw_params = hisi_codec_hw_params,
};

/* had-aiao digital audio interface glue - connects codec <--> CPU */

/* had-aiao digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link hiaiao_dai[] =
{
    { /* Primary Playback i/f */
        .name = "AIAO Primary TX",
        .stream_name = "Playback",
        .cpu_dai_name = "hisi-i2s",
        .codec_dai_name = "aiao-hifi",
        .platform_name = "hisi-audio",
        .codec_name = "hi-aiao",
        .init = hisi_codec_init,
        .ops = &hisi_codec_ops,
    },
};

#endif
#ifdef CONFIG_PM
static int hiaiao_suspend_post(struct snd_soc_card* card)
{
    return 0;
}

static int hiaiao_resume_pre(struct snd_soc_card* card)
{
    return 0;
}
#endif
static struct snd_soc_card snd_soc_hiaiao =
{
    .name = "HISI-AIAO",
    .owner = THIS_MODULE,
    .dai_link = hiaiao_dai,
    .num_links = ARRAY_SIZE(hiaiao_dai),
    .dapm_widgets = NULL,       //TODO ALSA Control
    .num_dapm_widgets = 0,
    .dapm_routes = NULL,
    .num_dapm_routes = 0,
    .controls = NULL,
    .num_controls = 0,
#ifdef CONFIG_PM
    .suspend_post = &hiaiao_suspend_post,
    .resume_pre = &hiaiao_resume_pre,
#endif
};

#if 0
static struct platform_device* hisi_snd_device;

int  hisndcard_init(void)
{
    int ret;

    hisi_snd_device = platform_device_alloc("soc-audio", -1);
    if (!hisi_snd_device)
    {
        return -ENOMEM;
    }

    platform_set_drvdata(hisi_snd_device, &snd_soc_hiaiao);

    ret = platform_device_add(hisi_snd_device);
    if (ret)
    {
        platform_device_put(hisi_snd_device);
        return ret;
    }

    return ret;
}

void hisndcard_exit(void)
{
    platform_device_unregister(hisi_snd_device);
}
#else

static __init int htd_snd_dev_probe(struct platform_device* pdev)
{
    struct snd_soc_card* card = &snd_soc_hiaiao;
    int ret;

    card->dev = &pdev->dev;
    ret = snd_soc_register_card(card);
    if (ret)
    {
        dev_err(&pdev->dev, "snd_soc_register_card() failed: %d\n",
                ret);
    }

    return ret;
}

static __exit int htd_snd_dev_remove(struct platform_device* pdev)
{
    struct snd_soc_card* card = platform_get_drvdata(pdev);

    snd_soc_unregister_card(card);
    return 0;
}

static struct platform_driver htd_sndcard_driver =
{
    .driver = {
        .name = "hisi-sndcard",
        .owner = THIS_MODULE,
#ifdef CONFIG_PM
        .pm = &snd_soc_pm_ops,
#endif
    },
    .probe  = htd_snd_dev_probe,
    .remove = __exit_p(htd_snd_dev_remove),
};

struct platform_device* hisi_snd_device;
EXPORT_SYMBOL(hisi_snd_device);

int hisndcard_init(void)
{
    int ret = 0;
    //register device
    hisi_snd_device = platform_device_alloc("hisi-sndcard", -1);
    if (!hisi_snd_device)
    {
        return -ENOMEM;
    }

    ret = platform_device_add(hisi_snd_device);
    if (ret)
    {
        platform_device_put(hisi_snd_device);
        return ret;
    }

    //register drvier
    return platform_driver_register(&htd_sndcard_driver);
}

void hisndcard_exit(void)
{
    platform_device_unregister(hisi_snd_device);
    platform_driver_unregister(&htd_sndcard_driver);
}
#endif

extern int hiaudio_dma_init(void);
extern int hiaudio_i2s_init(void);
extern int aiao_codec_init(void);
extern void aiao_codec_deinit(void);
extern void hiaudio_i2s_deinit(void);
extern void hiaudio_dma_deinit(void);

//static int __init hisnd_module_init(void)
int AO_ALSA_ModInit(void)
{
    int ret;

    //step 1  register platform dma component
    ret = hiaudio_dma_init();
    if (ret)
    {
        TRP();
        return ret;
    }
    //step 2  register cpu dai component
    ret = hiaudio_i2s_init();
    if (ret)
    {
        TRP();
        goto  Err_I2s_Init;
    }
    //step 3 register codec component
    ret = aiao_codec_init();
    if (ret)
    {
        TRP();
        goto  Err_Codec_Init;
    }
    //step 4 register card link
    ret = hisndcard_init();
    if (ret)
    {
        TRP();
        goto  Err_SndCard_Init;
    }
    return 0;

Err_SndCard_Init:
    aiao_codec_deinit();
Err_Codec_Init:
    hiaudio_i2s_deinit();
Err_I2s_Init:
    hiaudio_dma_deinit();

    return ret;
}

//static void __exit hisnd_module_deinit(void)
void AO_ALSA_ModExit(void)
{
    aiao_codec_deinit();
    hiaudio_i2s_deinit();
    hiaudio_dma_deinit();
    hisndcard_exit();
}

//module_init(hisnd_module_init);
//module_exit(hisnd_module_deinit);

MODULE_AUTHOR("<HISI Audio Team>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("hisi aiao ALSA driver");
MODULE_SUPPORTED_DEVICE("{{cv200}}");
