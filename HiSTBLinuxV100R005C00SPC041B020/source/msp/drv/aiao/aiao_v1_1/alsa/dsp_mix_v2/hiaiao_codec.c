#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>

#include "alsa_aiao_comm.h"

#ifdef CONFIG_ALSA_VOLUMN_SUPPORT
#include "alsa_aiao_volume_func.h"
#endif
#ifdef HI_ALSA_TLV320_SUPPORT 
#include "alsa_i2c.h"

int aiao_codec_init(void)
{
    int ret = 0;

	godbox_aic31_reset();
	init_i2c();
    return ret;
}
void aiao_codec_deinit(void)
{
	remove_i2c();
}
#else
static int aiao_hw_params(struct snd_pcm_substream *substream,
				struct snd_pcm_hw_params *params,
				struct snd_soc_dai *dai)
{
	return 0;
}
static int aiao_mute(struct snd_soc_dai *dai, int mute)
{
	return 0;
}
static int aiao_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	return 0;
}
static int aiao_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
	return 0;
}

static const struct snd_soc_dai_ops aiao_dai_ops = {
	.hw_params	= aiao_hw_params,
	.digital_mute	= aiao_mute,
	.set_fmt	= aiao_set_dai_fmt,
	.set_sysclk	= aiao_set_dai_sysclk,
};

static struct snd_soc_dai_driver aiao_dai = {
	.name = "aiao-hifi",
	.playback = {
		.stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = AIAO_RATES,
		.formats = AIAO_FORMATS,},
	#ifdef HI_ALSA_AI_SUPPORT      //only support alsa AI to creae capture
	.capture = {
		.stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = AIAO_RATES,
		.formats = AIAO_FORMATS,},
	#endif
	.ops = &aiao_dai_ops,
};

static int hicodec_probe(struct snd_soc_codec *codec)
{
#ifdef CONFIG_ALSA_VOLUMN_SUPPORT
    return hiaudio_volume_register(codec);
#else     
    return 0;
#endif
}
static int hicodec_remove(struct snd_soc_codec *codec)
{
#ifdef CONFIG_ALSA_VOLUMN_SUPPORT
    hiaudio_volume_unregister();
#endif

    return 0;
}
static int hicodec_suspend(struct snd_soc_codec *codec)
{
	return 0;
}
static int hicodec_resume(struct snd_soc_codec *codec)
{
	return 0;
}
static int hicodec_set_bias_level(struct snd_soc_codec *codec,
				 enum snd_soc_bias_level level)
{
	return 0;
}

static struct snd_soc_codec_driver soc_codec_dev_aiao = {

	.probe =	hicodec_probe,
	.remove =	hicodec_remove,
	.suspend =	hicodec_suspend,
	.resume =	hicodec_resume,
	.set_bias_level = hicodec_set_bias_level,
};

static int __init aiao_codec_probe(struct platform_device *pdev)
{
	return snd_soc_register_codec(&pdev->dev,
			&soc_codec_dev_aiao, &aiao_dai, 1);
}

static int __exit aiao_codec_remove(struct platform_device *pdev)
{
	snd_soc_unregister_codec(&pdev->dev);
	return 0;
}


static struct platform_driver aiao_codec_driver = {
	.driver = {
		.name = "hi-aiao",
		.owner = THIS_MODULE,
	},
	.probe = aiao_codec_probe,
	.remove = __exit_p(aiao_codec_remove),
};

static struct platform_device *aiao_codec_device;

int aiao_codec_init(void)
{
    int ret = 0;

    //register device
    aiao_codec_device = platform_device_alloc("hi-aiao", -1);
    if (!aiao_codec_device) {
        return -ENOMEM;
    }
    ret = platform_device_add(aiao_codec_device);
    if (ret) {
        platform_device_put(aiao_codec_device);
        return ret;
    }

    //register drvier
    return platform_driver_register(&aiao_codec_driver);
}

void aiao_codec_deinit(void)
{
    platform_device_unregister(aiao_codec_device);
    platform_driver_unregister(&aiao_codec_driver);
}
#endif

//module_platform_driver(aiao_codec_driver);
//MODULE_DESCRIPTION("aiao ALSA soc codec driver");
//MODULE_AUTHOR("");
//MODULE_LICENSE("GPL");
