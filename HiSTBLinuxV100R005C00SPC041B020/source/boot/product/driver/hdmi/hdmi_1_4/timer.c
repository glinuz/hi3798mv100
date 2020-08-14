#include "timer.h"
#include "hi_unf_hdmi.h"
#include "hi_common.h"

HI_U32 OldTime = 0;

/*
hdmi Init 
time costed : 0 
delay 1000ms 
time costed : 93857 
delay 1000ms 
time costed : 187770 
delay 100ms 
time costed : 197317 
delay 100ms 
time costed : 206854 
delay 10ms 
time costed : 207954 
delay 10ms 
time costed : 209046 
delay 1ms 
time costed : 209297 
delay 1ms 
time costed : 209581 

every gettimer is 10 us 
so time costed : 93857 similar to 938570us similar to 940ms
*/
void GetTimer()
{
#ifndef HI_MINIBOOT_SUPPORT
    int newtime = 0;
    newtime = get_timer(0);
    
    if(OldTime == 0)
    {
        OldTime = newtime;
    }
    
    HI_INFO_PRINT(HI_ID_HDMI,"time costed : %d0 us \n",newtime - OldTime);
    //OldTime = newtime;
    return ;
#endif
}

void ResetTimer()
{
    OldTime = 0;
}


