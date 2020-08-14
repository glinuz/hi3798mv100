//test alsa
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <semaphore.h>
#include "alsa/asoundlib.h"

#include "hi_resampler.h"
#include "hi_bluetooth.h"
#include "hi_unf_common.h"
#include "hi_unf_sound.h"
#include "hi_adp_mpi.h"
#include "hi_error_mpi.h"


#ifdef CONFIG_SUPPORT_CA_RELEASE
#define sample_printf
#else
#define sample_printf printf
#endif



#define CAST_SLEEP_TIME  5*1000     //5ms

//FILE *g_pAudCastFile = HI_NULL;
static pthread_t g_CastThd;
static HI_BOOL g_StopCastThread = HI_FALSE;
//static HI_BOOL g_PauseCastThread = HI_TRUE;



// 声卡配置参数
#define SAMPLE_RATE_48000 48000
#define SAMPLE_RATE_44100 44100
#define SAMPLE_RATE_16000 16000
#define SAMPLE_RATE_8000  8000
#define CHANNEL_STEREO 2
#define CHANNEL_MONO 1
#define FORMAT SND_PCM_FORMAT_S16_LE
#define PCM_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED
#define BUF_MAX_SIZE 8192
#define FRAMES  1024

HResampler hSRC = NULL;
short resampleBuf[BUF_MAX_SIZE*2];
snd_pcm_t *handle;

static     HI_HANDLE hCast;
static int s_bluZInit = 0;
static int change = 0;
static int init_flag = 0;

HI_VOID CastTthread(HI_VOID *args)
{
    HI_S32 Ret;
    HI_UNF_AO_FRAMEINFO_S  sAoFrame;
    HI_UNF_AO_FRAMEINFO_S *pstAOFrame = &sAoFrame;
    HI_HANDLE hCast = *(HI_HANDLE *)args;
    HI_U32 outsamps = 0;
    HI_U32 cnt = 0;

    while (!g_StopCastThread)
    {
        Ret = HI_UNF_SND_AcquireCastFrame(hCast, pstAOFrame, 0);
        if(HI_ERR_AO_CAST_TIMEOUT == Ret)
        {
            usleep(CAST_SLEEP_TIME);
            continue;
        }
        else if(HI_SUCCESS != Ret)
        {
            break;
        }

        if (hSRC != HI_NULL)
            outsamps = HI_Resampler_Process(hSRC, (short *)pstAOFrame->ps32PcmBuffer, pstAOFrame->u32PcmSamplesPerFrame, resampleBuf);


        if(handle != HI_NULL)
        {
            cnt = snd_pcm_writei(handle, resampleBuf, outsamps);
            if (cnt == -EPIPE)
            {
                /* EPIPE means underrun */
                int error;
                if((error = snd_pcm_prepare(handle)) < 0)
                {
                    sample_printf("snd_pcm_prepare error (%s)\n",snd_strerror(error));
                }
                else
                {
                    snd_pcm_status_t *status;
                    snd_pcm_status_alloca(&status);
                    error = snd_pcm_status(handle, status);
                    if (error < 0)
                    {
                        sample_printf("underrun occurred, snd_pcm_status error (%s)\n",snd_strerror(error));
                        break;
                    }
                    else
                    {
                        sample_printf("underrun occurred, state = %s\n", snd_pcm_state_name(snd_pcm_status_get_state(status)));
                        if (snd_pcm_status_get_state(status) == SND_PCM_STATE_XRUN)
                        {
                            if((error = snd_pcm_prepare(handle)) < 0)
                            {
                                sample_printf("snd_pcm_prepare error (%s)\n",snd_strerror(error));
                                break;
                            }
                        }
                    }
                }
            }
            else if(cnt == -EAGAIN)
            {
                //sample_printf("try again\n");
            }
            else if(cnt < 0)
            {
                sample_printf("error from writei: %d, %s\n",cnt,snd_strerror(cnt));
                break;
            }
            else if (cnt != (int)outsamps)
            {
               // sample_printf("short write, write %d frames, should write %d frames.\n", cnt,(int)outsamps);
            }
        }

        HI_UNF_SND_ReleaseCastFrame(hCast, pstAOFrame);

        usleep(CAST_SLEEP_TIME);
    }
    return;
}

static int snd_alsa_get_support_param(unsigned int *prate,unsigned int *pchannel)
{
    HI_S32 Ret;
    snd_pcm_hw_params_t *params;
    unsigned int val = 0;
    int dir;
    snd_pcm_uframes_t frames = FRAMES;
    snd_pcm_t *handle;
    unsigned int rate;
    unsigned int channel = 0;

    Ret = snd_pcm_open(&handle, "bluetooth",SND_PCM_STREAM_PLAYBACK, 0); // 播放模式打开
    if (Ret < 0)
    {
        sample_printf("unable to open pcm device: %s\n",snd_strerror(Ret));
        return -1;
    }

    snd_pcm_hw_params_alloca(&params);
    Ret = snd_pcm_hw_params_any(handle, params);
    if (Ret < 0)
    {
        sample_printf(("Broken configuration for this PCM: no configurations available"));
        return -1;
    }

    Ret =snd_pcm_hw_params_set_access(handle, params,PCM_ACCESS);
    if (Ret < 0)
    {
        sample_printf("unable to set access: %s\n", snd_strerror(Ret));
        return -1;
    }

    Ret = snd_pcm_hw_params_set_format(handle, params,FORMAT);
    if (Ret < 0)
    {
        sample_printf("unable to set Format: %s\n", snd_strerror(Ret));
        return -1;
    }

    Ret = snd_pcm_hw_params_set_channels(handle, params, CHANNEL_STEREO);
    if (Ret < 0)
    {
        sample_printf("unable to set channels: %d\n", CHANNEL_STEREO);
        Ret = snd_pcm_hw_params_set_channels(handle, params, CHANNEL_MONO);
        if (Ret < 0)
        {
            sample_printf("unable to set channels: %s\n", snd_strerror(Ret));
            return -1;
        }
    }

    rate = SAMPLE_RATE_48000;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
    if (rate != SAMPLE_RATE_48000)
    {
        sample_printf("The rate %d Hz is not supported,support %d\n", SAMPLE_RATE_48000, rate);
        rate = SAMPLE_RATE_44100;
        snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
        if (rate != SAMPLE_RATE_44100)
        {
            sample_printf("The rate %d Hz is not supported,support %d\n", SAMPLE_RATE_44100, rate);
            rate = SAMPLE_RATE_16000;
            snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
            if (rate != SAMPLE_RATE_16000)
            {
                sample_printf("The rate %d Hz is not supported,support %d\n", SAMPLE_RATE_16000, rate);
                rate = SAMPLE_RATE_8000;
                snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
                 if (rate != SAMPLE_RATE_8000)
                {
                    sample_printf("The rate %d Hz is not supported,support %d\n", SAMPLE_RATE_8000, rate);
                    return -1;
                }
                else
                {
                    Ret = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
                    if (Ret < 0)
                    {
                        sample_printf("unable to set periods: %s\n", snd_strerror(Ret));
                        return -1;
                    }
                }
            }
        }
    }
    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    snd_pcm_hw_params_get_period_time(params, &val, 0);
    snd_pcm_hw_params_get_channels(params, &channel);

    *prate= rate;
    *pchannel = channel;
    snd_pcm_close(handle);

    return 0;
}

HI_S32 snd_bluz_start(HI_VOID)
{
    HI_S32 Ret;

    snd_pcm_hw_params_t *params;
    unsigned int val = 0;
    unsigned int channel = 0;
    int dir;
    snd_pcm_uframes_t frames = FRAMES;
    unsigned int rate;
    unsigned int u32DevRate;
    unsigned int u32DevChanel;

    if(s_bluZInit == 1)
    {
        sample_printf("bluetooth started\n");
        return HI_SUCCESS;
    }

    Ret = snd_alsa_get_support_param(&u32DevRate, &u32DevChanel);
    if (Ret < 0)
    {
        sample_printf("unable Get default param\n");
        return HI_FAILURE;
    }
    sample_printf("Get  param finish: %d,rate %d\n",u32DevChanel,u32DevRate);

    Ret = snd_pcm_open(&handle, "plug:bluetooth",SND_PCM_STREAM_PLAYBACK , SND_PCM_NONBLOCK); // 播放模式打开
    if (Ret < 0)
    {
        sample_printf("unable to open pcm device: %s\n",snd_strerror(Ret));
        return HI_FAILURE;
    }

    snd_pcm_hw_params_alloca(&params);
    Ret= snd_pcm_hw_params_any(handle, params);
    if (Ret < 0)
    {
        sample_printf("Broken configuration for this PCM: no configurations available");
        return HI_FAILURE;
    }

    Ret = snd_pcm_hw_params_set_access(handle, params,PCM_ACCESS);
    if (Ret < 0)
    {
        sample_printf("unable to set access: %s\n", snd_strerror(Ret));
        return HI_FAILURE;
    }

    Ret = snd_pcm_hw_params_set_format(handle, params,FORMAT);
    if (Ret < 0)
    {
        sample_printf("unable to set Format: %s\n", snd_strerror(Ret));
        return HI_FAILURE;
    }

    Ret = snd_pcm_hw_params_set_channels(handle, params, CHANNEL_STEREO);
    if (Ret < 0)
    {
        sample_printf("unable to set channels: %d\n", CHANNEL_STEREO);
        return HI_FAILURE;
    }

    rate = u32DevRate;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
    if (rate != u32DevRate)
    {
        sample_printf("The rate %d Hz is not supported,support %d\n", u32DevRate, rate);

    }

    if(rate == SAMPLE_RATE_8000)
    {
        Ret = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
        if (Ret < 0)
        {
            sample_printf("unable to set periods: %s\n", snd_strerror(Ret));
            return HI_FAILURE;
        }
    }

    Ret = snd_pcm_hw_params(handle, params);
    if (Ret < 0)
    {
        sample_printf("unable to set hw parameters: %s\n", snd_strerror(Ret));
        return HI_FAILURE;
    }

    snd_pcm_hw_params_get_period_size(params, &frames, 0);
    snd_pcm_hw_params_get_period_time(params, &val, 0);
    snd_pcm_hw_params_get_channels(params, &channel);

    snd_pcm_prepare(handle);


    hSRC = HI_Resampler_Create(SAMPLE_RATE_48000, rate, CHANNEL_STEREO);
    if(hSRC == NULL)
    {
        sample_printf(" *** ResamplerCreate Fail ***\n");
        snd_pcm_close(handle);
        return HI_FAILURE;
    }

    g_StopCastThread = HI_FALSE;
    s_bluZInit = 1;

    pthread_create(&g_CastThd, HI_NULL, (HI_VOID *)CastTthread, &hCast);
    return 0;

}

HI_S32 snd_bluz_stop()
{
    if (1 != s_bluZInit)
    {
        sample_printf("[%s] no bluz inited\n", __FUNCTION__);
        return HI_SUCCESS;
    }

    s_bluZInit = 0;

    g_StopCastThread = HI_TRUE;

    pthread_join(g_CastThd, HI_NULL);

    HI_Resampler_Free(hSRC);
    snd_pcm_close(handle);

    handle = HI_NULL;
    hSRC = HI_NULL;

    return HI_SUCCESS;
}

HI_VOID change_config(HI_VOID)
{
    if( change == 0 )
    {
        system("cp /etc/bluetooth/FirstConf/asound.conf /etc/asound.conf");
        system("cp /usr/share/bluetooth/share/alsa/alsa_bak.conf /usr/share/bluetooth/share/alsa/alsa.conf -rf");
        system("sync");
        system("cat /etc/asound.conf");
        change = 1;
    }
    else
    {
        system("cp /etc/bluetooth/SecondConf/asound.conf /etc/asound.conf");
        system("cp /usr/share/bluetooth/share/alsa/alsa_bak.conf /usr/share/bluetooth/share/alsa/alsa.conf -rf");

        system("sync");
        system("cat /etc/asound.conf");
        change = 0;
    }
}

HI_VOID snd_cast_destroy()
{
    HI_S32 Ret;

    if(g_StopCastThread == HI_FALSE)
        snd_bluz_stop();

    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_FALSE);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
    }

    Ret = HI_UNF_SND_DestroyCast(hCast);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("HI_UNF_SND_DestroyCast err, ret[0x%x] \n", Ret);
    }
}

HI_S32 snd_cast_create()
{
    HI_UNF_SND_CAST_ATTR_S stCastAttr;
    HI_S32 Ret;

    Ret = HI_UNF_SND_GetDefaultCastAttr(HI_UNF_SND_0, &stCastAttr);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Get Default Cast Attr Failed \n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_SND_CreateCast(HI_UNF_SND_0,  &stCastAttr, &hCast);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Cast Create Failed \n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_SND_SetCastEnable(hCast, HI_TRUE);
    if(HI_SUCCESS != Ret)
    {
        sample_printf("Cast Enable Failed \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}


HI_S32 snd_bluz_Deinit()
{
    if (1 != init_flag)
    {
        sample_printf("[%s] no need to deinit\n", __FUNCTION__);
        return HI_SUCCESS;
    }

    HIADP_Snd_DeInit();
    HI_SYS_DeInit();

    init_flag = 0;

    return HI_SUCCESS;
}


HI_S32 snd_bluz_init()
{
    HI_S32 Ret;

    HI_SYS_Init();

    Ret = HIADP_Snd_Init();
    if (HI_SUCCESS != Ret)
    {
        sample_printf("call SndInit failed.\n");
        return HI_FAILURE;
    }
    init_flag = 1;

    return HI_SUCCESS;
}

int main(int argc, char *argv[])
{
    HI_CHAR InputCmd[32];
    HI_S32 Ret;

    Ret = snd_bluz_init();
    if(HI_SUCCESS != Ret)
    {
      sample_printf("snd_bluz_init failed\n");
      return 0;
    }

    Ret = snd_cast_create();
    if(HI_SUCCESS != Ret)
    {
      sample_printf("snd_cast_create failed\n");
      goto BLUEZ_DEINIT;
    }

    while (InputCmd[0] != 'q')
    {
        sample_printf("please input c to change config\n");
        sample_printf("please input the k to start cast!, s to stop and deinit\n");

        SAMPLE_GET_INPUTCMD(InputCmd);

        switch( InputCmd[0] )
        {
             case 'c':
                    {
                        change_config();
                        break;
                    }
             case 'k':
                   {
                        Ret = snd_bluz_start();
                        if(HI_SUCCESS != Ret)
                        {
                          sample_printf("snd_bluz_start failed\n");
                          goto CAST_DEINIT;
                        }
                        break;
                   }
            case 's':
                   {
                        Ret = snd_bluz_stop();
                        if(HI_SUCCESS != Ret)
                        {
                          sample_printf("snd_bluz_start failed\n");
                          goto CAST_DEINIT;
                        }
                        break;
                   }
            default:
                   sample_printf("input error!\n");
                   break;
        }
    }

CAST_DEINIT:
    snd_cast_destroy();
BLUEZ_DEINIT:
    snd_bluz_Deinit();

    return 0;
}
