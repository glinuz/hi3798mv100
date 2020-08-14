#include "alsa_aiao_proc_func.h"

static void hiaudio_ao_proc_read(struct snd_info_entry* entry,
                                 struct snd_info_buffer* buffer)
{
    struct hiaudio_data* had = entry->private_data;

    snd_iprintf(buffer,
                "TimerHandle=0x%x, SndOpen=0x%x  TrackId=0x%x\n",
                had->hTimerHandle, had->u32SndOpen, had->u32TrackId
               );
    snd_iprintf(buffer,
                "TrackState=0x%x, SendTrack(Try/Ok)=0x%x/0x%x,Discard(0x%x),Fail(0x%x)\n",
                atomic_read(&had->atmTrackState), had->u32SendTryCnt, had->u32HadSent, had->u32DiscardSent, had->u32SendFailCnt
               );
    snd_iprintf(buffer,
                "periods=0x%x, PeroidTime=%dns, MuteFrameTime/PortDelay=%d/%dms, HwPointer=0x%x\n",
                had->stAoHwParam.periods, had->u32PollTimeNs, had->u32MuteFrameTime, had->u32PortDelayMs, had->u32HwPointer
               );
    snd_iprintf(buffer,
                "pVirBaseAddr=0x%p, pPhyBaseAddr=0x%p, SamplesPerFrame(Peroid)=0x%x\n",
                had->pVirBaseAddr, had->pPhyBaseAddr, had->stAOFrame.u32PcmSamplesPerFrame
               );
#ifdef CONFIG_PM
    snd_iprintf(buffer, " -----pm-----\n");
    snd_iprintf(buffer,
                " suspend_state=0x%x\n",
                had->u32SuspendState
               );
#endif
}

int hiaudio_ao_proc_init(void* card, const char* name, struct hiaudio_data* had)
{
    int ret;
    if ((NULL == card) || (NULL == name) || (NULL == had))
    {
        return -EINVAL;
    }
    ret = snd_card_proc_new((struct snd_card*)card, name, &had->entry);
    if (ret)
    {
        //TO DO
    }
    snd_info_set_text_ops(had->entry, had, hiaudio_ao_proc_read);

    return 0;
}

#ifdef HI_ALSA_AI_SUPPORT
static void hiaudio_ai_proc_read(struct snd_info_entry* entry,
                                 struct snd_info_buffer* buffer)
{
    struct hiaudio_data* had = entry->private_data;
    snd_iprintf(buffer,
                "ai_handle=%d,dma isr cnt =%d\n",
                had->ai_handle, had->isr_total_cnt_c
               );
    snd_iprintf(buffer,
                "  dma writepos=%d,dma read pos=%d\n",
                had->ai_writepos, had->ai_readpos
               );
#ifdef MUTE_FRAME_INPUT
    snd_iprintf(buffer,
                "  periods:%d, peroidtimeNs=%d,current_c_pos=%d\n",
                had->sthwparam_ai.periods, had->u32AiPollTimeNs, had->current_c_pos
               );
#endif
}

int hiaudio_ai_proc_init(void* card, const char* name, struct hiaudio_data* had)
{
    int ret;
    if ((NULL == card) || (NULL == name) || (NULL == had))
    {
        return -EINVAL;
    }
    ret = snd_card_proc_new((struct snd_card*)card, name, &had->entry);
    if (ret)
    {
    }
    snd_info_set_text_ops(had->entry, had, hiaudio_ai_proc_read);
    return 0;
}
#endif

void hiaudio_proc_cleanup(void)
{
    //TODO
}
