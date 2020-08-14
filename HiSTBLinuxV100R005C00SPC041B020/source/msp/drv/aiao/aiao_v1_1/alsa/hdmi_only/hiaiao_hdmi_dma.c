/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: had-dma.c
 * Description: aiao alsa dma interface
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    HISI Audio Team
 ********************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/kthread.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <asm/io.h>
#include <linux/jiffies.h>
#include <linux/time.h>
#include <linux/wait.h>
#ifdef CONFIG_PM
 #include <linux/pm.h>
#endif
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/control.h>
#include <sound/initval.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>

#include "drv_ao_hdmi_func.h"
#include "alsa_aiao_hdmi_proc_func.h"
#include "alsa_aiao_hdmi_comm.h"
#include "hi_drv_ao.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mem.h"
#include "hi_drv_proc.h"
#include "hi_drv_dev.h"
#include "drv_ao_ioctl.h"
#include "drv_ao_private.h"

//#define AIAO_ALSA_DEBUG
//#define ALSA_TIME_DEBUG
#ifdef AIAO_ALSA_DEBUG
#define ATRP()    print(KERN_ALERT"\nfunc:%s line:%d \n", __func__, __LINE__)
#define ATRC      print
#else
#define ATRP() 
#define ATRC(fmt, ...) 
#endif

#ifdef  ALSA_TIME_DEBUG
static struct timespec curtime;
static struct timeval end_time,start_time;
static unsigned long time_coast;
static struct timeval end_time_free,start_time_free;
static unsigned long time_coast_free;
#endif

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
#define ALSA_PROC_AO_NAME "hi_hw_hdmi_ao_data"
#endif

#define AO_HDMI_BUFFER_SIZE    1024*32
#define INITIAL_VALUE  0xffffffff
#define AIAO_DF_PeriodBufSize 2048

static const struct snd_pcm_hardware aiao_hw_hardware = {
    .info           = SNDRV_PCM_INFO_MMAP |
                  SNDRV_PCM_INFO_MMAP_VALID |
                  SNDRV_PCM_INFO_INTERLEAVED |
                  SNDRV_PCM_INFO_BLOCK_TRANSFER |
                  SNDRV_PCM_INFO_PAUSE |
                  SNDRV_PCM_INFO_RESUME,
    .formats        = SNDRV_PCM_FMTBIT_S16_LE |
                  SNDRV_PCM_FMTBIT_S24_LE,
    .rates = AIAO_RATES,
    .channels_min       = 2,
    .channels_max       = 2,
    .period_bytes_min   = 1920,     
    .period_bytes_max   = 1920,    
    .periods_min        = 4,
    .periods_max        = 8,
    .buffer_bytes_max   = AO_HDMI_BUFFER_SIZE,
};

#define I2S_ONLY_SND HI_UNF_SND_1

static AO_ALSA_I2S_Param_S stAOI2sAlsaAttr;

void hi_hdmi_setup_dma(struct hihdmiaudio_data *had)
{
    unsigned int real_size = had->sthwparam.period_size*had->sthwparam.frame_size;

    hi_ao_hdmi_alsa_update_writeptr(I2S_ONLY_SND,&(real_size));
}

void hi_hdmi_start_dma(struct hihdmiaudio_data *had )
{
    unsigned int real_size = (had->sthwparam.period_size * had->sthwparam.frame_size) * 3;
    hi_ao_hdmi_alsa_update_writeptr(I2S_ONLY_SND, &(real_size));  
}

static irqreturn_t IsrAOFunc(AIAO_PORT_ID_E enPortID,HI_U32 u32IntRawStatus,void * dev_id)
{
    struct snd_pcm_substream *substream = dev_id;
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
  //  ATRP();
     had->aiao_isr_num++;
     hi_ao_hdmi_alsa_query_readpos(I2S_ONLY_SND, &(had->hw_readpos));
     had->runtime_appl_offset = (had->sthwparam.period_size * had->sthwparam.frame_size) * (had->aiao_isr_num);

     hi_hdmi_setup_dma(had);
     snd_pcm_period_elapsed(substream); 

     had->IsrAoProc(enPortID,u32IntRawStatus,NULL);    
     return IRQ_HANDLED;
}

static int hi_hdmi_dma_prepare(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);

    ATRP(); 
    had->aiao_isr_num = 0;
    had->local_isr_num = 0;           
    return ao_dma_hdmi_flushbuf(I2S_ONLY_SND);    
}

static int hi_hdmi_dma_trigger(struct snd_pcm_substream *substream, int cmd)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret = 0;

    ATRC("trigger cmd id %d\n",cmd);
    switch(cmd) 
    {
    case SNDRV_PCM_TRIGGER_START:
    case SNDRV_PCM_TRIGGER_RESUME:
    case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:

           ret = ao_dma_hdmi_start(I2S_ONLY_SND, NULL);
            if(ret)
            {   
                ATRC("AIAO ALSA start dma Fail \n");
            }
            hi_hdmi_start_dma(had);
        break;
    case SNDRV_PCM_TRIGGER_STOP:
    case SNDRV_PCM_TRIGGER_SUSPEND:
    case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
            ret = ao_dma_hdmi_stop(I2S_ONLY_SND, NULL);
          if(ret)
            {
              ATRC("AIAO ALSA stop dma Fail \n");

        }
        break;        
    default:
        ret = -EINVAL;
        break;
    }
    return 0;
}

static int hi_hdmi_dma_mmap(struct snd_pcm_substream *substream,
    struct vm_area_struct *vma)
{
    //TODO 
    struct snd_pcm_runtime *runtime = substream->runtime;
    int ret;
    unsigned int size;
    ATRP();

    vma->vm_flags |= VM_IO;
    vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
    size = vma->vm_end - vma->vm_start;

    ret = io_remap_pfn_range(vma,
                vma->vm_start,
                runtime->dma_addr >> PAGE_SHIFT,
                size, 
                vma->vm_page_prot);

    if (ret)
        return -EAGAIN;

    return 0;
}

static snd_pcm_uframes_t hi_hdmi_dma_pointer(struct snd_pcm_substream *substream)
{
    struct snd_pcm_runtime *runtime = substream->runtime;
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    snd_pcm_uframes_t frame_offset = 0;
    unsigned int bytes_offset = 0;

    had->local_isr_num++;
        bytes_offset = had->hw_readpos;
    
    if(bytes_offset >= snd_pcm_lib_buffer_bytes(substream))
       bytes_offset = 0;
     
    frame_offset = bytes_to_frames(runtime, bytes_offset);
    return frame_offset;
}

static void hi_aiao_set_params(struct snd_pcm_substream *substream,
    struct snd_pcm_hw_params *params,struct snd_pcm_runtime *runtime)     
{
         stAOI2sAlsaAttr.IsrFunc                 =(AIAO_IsrFunc*)IsrAOFunc;
         stAOI2sAlsaAttr.substream               =(void*)substream;
         stAOI2sAlsaAttr.stBuf.u32BufPhyAddr     = runtime->dma_addr;    // for dma buffer
         stAOI2sAlsaAttr.stBuf.tBufVirAddr     = (int)runtime->dma_area;
         stAOI2sAlsaAttr.stBuf.u32BufSize        = runtime->dma_bytes; 
         stAOI2sAlsaAttr.stBuf.u32PeriodByteSize = params_buffer_bytes(params)/params_periods(params);
         stAOI2sAlsaAttr.stBuf.u32Periods        = params_periods(params);
         stAOI2sAlsaAttr.enRate                  = params_rate(params);
         stAOI2sAlsaAttr.bAlsaI2sUse             = HI_TRUE;
}

static int hi_hdmi_dma_hwparams(struct snd_pcm_substream *substream,
    struct snd_pcm_hw_params *params)
{
    unsigned int buffer_bytes;
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_pcm_runtime *runtime = substream->runtime;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    HI_UNF_SND_ATTR_S stAttr;
    int ret = 0;
    AO_SND_Open_Param_S stAoParam;
    
    ATRP();
    buffer_bytes = params_buffer_bytes(params);
    
    if(snd_pcm_lib_malloc_pages(substream, buffer_bytes) < 0)
        return -ENOMEM;

    if(had->open_cnt == 0)
    {
        
         ao_dma_hdmi_gethwparam(&stAttr);
     
         stAttr.enSampleRate =  params_rate(params);
         stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enChannel = params_channels(params);
         switch (params_format(params)) 
         {
              case SNDRV_PCM_FORMAT_S16_LE:
                stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
                break;
              case SNDRV_PCM_FMTBIT_S24_LE:
                stAttr.stOutport[0].unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_24;
                break;
              default:
                  break;
         }

         ret = hi_ao_hdmi_alsa_get_proc_func(&had->IsrAoProc);//get proc func
         hi_aiao_set_params(substream,params,runtime);
    
        stAoParam.enSound = I2S_ONLY_SND;    
        memcpy(&stAoParam.stAttr, &stAttr, sizeof(HI_UNF_SND_ATTR_S));     
        ret = ao_dma_hdmi_open(&stAoParam, (void *)&stAOI2sAlsaAttr, &had->file);
        if(HI_SUCCESS != ret)
        {
            ret =  -EINVAL;             //TODO  different sample rete  
            goto err_AllocateDma;
        } 
        had->open_cnt++;
    }
    
    had->sthwparam.channels = params_channels(params);
    had->sthwparam.rate = params_rate(params);
    had->sthwparam.format = params_format(params);
    had->sthwparam.periods = params_periods(params);
    had->sthwparam.period_size = params_period_size(params);
    had->sthwparam.buffer_size = params_buffer_size(params);
    had->sthwparam.buffer_bytes = params_buffer_bytes(params);

    switch (had->sthwparam.format) 
    {
        case SNDRV_PCM_FORMAT_S16_LE:
            had->sthwparam.frame_size = 2 * had->sthwparam.channels;
            break;
        case SNDRV_PCM_FMTBIT_S24_LE:
            had->sthwparam.frame_size = 3 * had->sthwparam.channels;
            break;
        default:
            break;
    }
    
    return ret;
err_AllocateDma:
   ret = snd_pcm_lib_free_pages(substream);

    return HI_FAILURE;
}

static int hi_hdmi_dma_hwfree(struct snd_pcm_substream *substream)
{
    int ret = HI_SUCCESS;
    return ret;
}

static int hi_hdmi_dma_open(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret = 0;
 
    ATRP();
    ret = snd_soc_set_runtime_hwparams(substream, &aiao_hw_hardware);
    if(ret)
        return ret;

    ret = snd_pcm_hw_constraint_integer(substream->runtime, SNDRV_PCM_HW_PARAM_PERIODS); 
    had->open_cnt = 0;
    had->hw_readpos = 0;
    //isr
    had->aiao_isr_num = 0;
    had->local_isr_num = 0;    
    had->runtime_appl_offset = 0;
    return ret;
}

static int hi_hdmi_dma_close(struct snd_pcm_substream *substream)
{
    struct snd_soc_pcm_runtime *soc_rtd = substream->private_data;
    struct snd_soc_platform *platform = soc_rtd->platform;
    struct hihdmiaudio_data *had = snd_soc_platform_get_drvdata(platform);
    int ret;

    ATRP();
    #ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    hiaudio_hdmi_proc_cleanup((void*)had);
    #endif

    if(had->open_cnt ==1)
    {  
       ret = ao_dma_hdmi_close(&had->file,I2S_ONLY_SND);
       if(ret < 0)
       {
           ATRC("\n  ao_dma_close fail !\n");
       }
        else
       {
           had->open_cnt = 0;
       }      
       hi_ao_hdmi_alsa_set_proc_func(had->IsrAoProc);
    }   
    snd_pcm_lib_free_pages(substream);
    return HI_SUCCESS;
}
   
static struct snd_pcm_ops hi_hdmi_dma_ops = {
    .open = hi_hdmi_dma_open,
    .close = hi_hdmi_dma_close,
    .ioctl  = snd_pcm_lib_ioctl,
    .hw_params = hi_hdmi_dma_hwparams,
    .hw_free = hi_hdmi_dma_hwfree,
    .prepare = hi_hdmi_dma_prepare,
    .trigger = hi_hdmi_dma_trigger,
    .pointer = hi_hdmi_dma_pointer,
    .mmap = hi_hdmi_dma_mmap,
//    .ack = hi_hw_dma_ack,
};

static int hi_hdmi_dma_pcm_new(struct snd_soc_pcm_runtime *rtd)
{
    struct snd_pcm *pcm = rtd->pcm;
    int ret = 0;
    if(pcm->streams[SNDRV_PCM_STREAM_PLAYBACK].substream || 
       pcm->streams[SNDRV_PCM_STREAM_CAPTURE].substream)
    {
        ret = snd_pcm_lib_preallocate_pages_for_all(pcm, SNDRV_DMA_TYPE_DEV,NULL,
                AO_HDMI_BUFFER_SIZE, AO_HDMI_BUFFER_SIZE / 2);
        if(ret)
        {
            ATRC("dma buffer allocation fail \n");
            return ret;
        }
    }
    return ret;
}

static void hi_hdmi_dma_pcm_free(struct snd_pcm *pcm)
{
    snd_pcm_lib_preallocate_free_for_all(pcm);
}

static int hi_hdmi_dma_probe(struct snd_soc_platform *soc_platform)
{
    int ret = 0;
    
#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    struct hihdmiaudio_data *had = dev_get_drvdata(soc_platform->dev);
    ret = hiaudio_ao_hdmi_proc_init(soc_platform->card->snd_card, ALSA_PROC_AO_NAME, had);
    if(ret < 0)
    {
        ATRC("had_hdmi_probe fail %d", ret);
    }

    //ATRC("had_init_debugfs fail %d", ret);
#endif    
    return ret;
}

static struct snd_soc_platform_driver aiao_soc_hdmi_platform_drv =
{
    .ops         = &hi_hdmi_dma_ops,
    .pcm_new   = hi_hdmi_dma_pcm_new,
    .pcm_free   = hi_hdmi_dma_pcm_free,
    .probe      = hi_hdmi_dma_probe,
};

static int __init soc_hdmi_snd_platform_probe(struct platform_device *pdev)
{
    struct hihdmiaudio_data *had;
    int ret = -EINVAL;

    had = HI_KZALLOC(HI_ID_AO, sizeof(struct hihdmiaudio_data), GFP_KERNEL);
    if(had == NULL)
        return -ENOMEM;

    dev_set_drvdata(&pdev->dev, had);

    //init had data struct
    mutex_init(&had->mutex);
    had->open_cnt = 0;
    had->hw_readpos = 0;
    had->local_isr_num = 0;
    had->aiao_isr_num = 0;
    memset(&had->sthwparam, 0, sizeof(struct hisi_aiao_hdmi_hwparams));
#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
    had->entry = NULL;
#endif

    ret = snd_soc_register_platform(&pdev->dev, &aiao_soc_hdmi_platform_drv);
    if (ret < 0)
        goto err;

    return ret;
err:
    HI_KFREE(HI_ID_AO, had);
    return ret;
}

static int __exit soc_hdmi_snd_platform_remove(struct platform_device *pdev)
{
    struct hihdmiaudio_data *had = dev_get_drvdata(&pdev->dev);

    if(had)
    {
        HI_KFREE(HI_ID_AO, had);
    }

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
  ///  hiaudio_hw_proc_cleanup();
#endif
    snd_soc_unregister_platform(&pdev->dev);
    return 0;
}

static struct platform_driver hiaudio_hdmi_dma_driver = {
    .driver = {
        .name = "hisi-audio-hdmi",
        .owner = THIS_MODULE,
    },
    .probe = soc_hdmi_snd_platform_probe,
    .remove = __exit_p(soc_hdmi_snd_platform_remove),
};

static struct platform_device *hiaudio_hdmi_dma_device;

int hiaudio_hdmi_dma_init(void)
{
    int ret =0;
    
    hiaudio_hdmi_dma_device = platform_device_alloc("hisi-audio-hdmi", -1);
    if (!hiaudio_hdmi_dma_device) {
        HI_FATAL_AO(KERN_ERR "hiaudio-dma SoC platform device: Unable to register\n");
        return -ENOMEM;
    }
    
    ret = platform_device_add(hiaudio_hdmi_dma_device);
    if (ret) {
        HI_FATAL_AO(KERN_ERR "hiaudio-dma SoC platform device: Unable to add\n");
        platform_device_put(hiaudio_hdmi_dma_device);
       return ret;
    }

    return platform_driver_register(&hiaudio_hdmi_dma_driver);
}

void hiaudio_hdmi_dma_deinit(void)
{
    platform_device_unregister(hiaudio_hdmi_dma_device);
    platform_driver_unregister(&hiaudio_hdmi_dma_driver);
}
