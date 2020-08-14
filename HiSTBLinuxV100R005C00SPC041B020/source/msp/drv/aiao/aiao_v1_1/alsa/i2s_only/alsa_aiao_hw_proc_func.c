#include "alsa_aiao_hw_proc_func.h"

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
static void hiaudio_ao_hw_proc_read(struct snd_info_entry *entry,
                                    struct snd_info_buffer *buffer)
{

    struct hii2saudio_data *had = entry->private_data;

     snd_iprintf(buffer,
                        " i2s card open cnt is %d,aiao_isr cnt is %d,pointor cnt is %d\n",
                         had->open_cnt,had->aiao_isr_num, had->local_isr_num
                         );

     snd_iprintf(buffer,
                        "dma read pos is %d,dam read bytes is %d\n",
                          had->hw_readpos,had->runtime_appl_offset
                         );


}



//Todo Jiaxi





int hiaudio_ao_hw_proc_init(void * card, const char * name, struct hii2saudio_data *had)
{
    int ret;
    if((NULL == card) || (NULL == name) || (NULL == had))
    {
        return -EINVAL;
    }
    ret = snd_card_proc_new((struct snd_card *)card, name, &had->entry);
    if(ret)
    {
        //TO DO    
    }
    snd_info_set_text_ops(had->entry, had, hiaudio_ao_hw_proc_read);    
    
    return 0;
}

#if 1//def HI_ALSA_AI_SUPPORT
static void hiaudio_ai_hw_proc_read(struct snd_info_entry *entry,
                                    struct snd_info_buffer *buffer)
{
    struct hii2saudio_data *had = entry->private_data;
     snd_iprintf(buffer,
                        "ai_handle=%d,dma isr cnt is %d\n",
                         had->ai_handle, had->isr_total_cnt_c
                         );
    snd_iprintf(buffer,
                       "  dma writepos=%d,dma read pos=%d\n",
                          had->ai_writepos,had->ai_readpos
                         );
}
int hiaudio_ai_hw_proc_init(void * card, const char * name, struct hii2saudio_data *had)
{
    int ret;
    if((NULL == card) || (NULL == name) || (NULL == had))
    {
        return -EINVAL;
    }
    ret = snd_card_proc_new((struct snd_card *)card, name, &had->entry);
    if(ret)
    {
        //TO DO    
    }
    snd_info_set_text_ops(had->entry, had, hiaudio_ai_hw_proc_read);    
    
    return 0;
}
#endif

void hiaudio_hw_proc_cleanup(void *p)
{
    struct hii2saudio_data *had = (struct hii2saudio_data *)p;
    had->aiao_isr_num = 0;
    had->local_isr_num = 0;
    had->runtime_appl_offset = 0;
    had->hw_readpos = 0;
    had->isr_total_cnt_c = 0;
    had->ai_writepos = 0;
    had->ai_readpos = 0;
}
#endif




