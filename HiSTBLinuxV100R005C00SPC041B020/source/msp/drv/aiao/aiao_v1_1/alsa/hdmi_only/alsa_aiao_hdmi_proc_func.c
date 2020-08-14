#include "alsa_aiao_hdmi_proc_func.h"

#ifdef CONFIG_AIAO_ALSA_PROC_SUPPORT
static void hiaudio_ao_hdmi_proc_read(struct snd_info_entry *entry,
                                    struct snd_info_buffer *buffer)
{

    struct hihdmiaudio_data *had = entry->private_data;

     snd_iprintf(buffer,
                        " card open cnt=%d, aiao_isr cnt=%d, pointor cnt=%d\n",
                         had->open_cnt,had->aiao_isr_num, had->local_isr_num
                         );    
     snd_iprintf(buffer,
                        "dma read bytes=0%d,dma read pos=%d\n",
                         had->runtime_appl_offset,had->hw_readpos
                         );


}



//Todo Jiaxi





int hiaudio_ao_hdmi_proc_init(void * card, const char * name, struct hihdmiaudio_data *had)
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
    snd_info_set_text_ops(had->entry, had, hiaudio_ao_hdmi_proc_read);    
    
    return 0;
}

        //TO DO    
    

void hiaudio_hdmi_proc_cleanup(void *p)
{
    struct hihdmiaudio_data *had = (struct hihdmiaudio_data *)p;

    had->aiao_isr_num = 0;
    had->local_isr_num = 0;
    had->runtime_appl_offset = 0;
    had->hw_readpos = 0;
}
#endif




