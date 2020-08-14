
#include "base.h"
#include "keyled.h"

/************************************************************************* 
						2.0  keyled module
*************************************************************************/
HI_U8  klPmocVal = 0;
extern void keyled_isr_usr(void);
extern void KEYLED_Init_Usr(void);
extern void keyled_disable_usr(void);
extern void timer_display_usr(void);
extern void chan_display_usr(DATA_U32_S channum);
extern void no_display_usr(void);
extern void dbg_display_usr(HI_U16 val); 

HI_VOID timer_display(HI_VOID)
{
    
    timer_display_usr(); 

	return;
}

HI_VOID chan_display(HI_VOID)
{ 	
    DATA_U32_S  channum;

    regAddr.val32 = g_u32DateBaseAddr + DATA_DISPVAL;

    read_regVal();
    channum.val32 = regData.val32;

	chan_display_usr(channum);

	return;
}

HI_VOID no_display(HI_VOID) 
{
	no_display_usr(); 

	return;
}     

HI_VOID KEYLED_Early_Display(HI_VOID)
{
	if (time_type == TIME_DISPLAY )
	{
		timer_display();
	}
	else if (time_type == DIGITAL_DISPLAY)
	{
		chan_display();
	}
	else
	{
		no_display();
	}
	
	return;
}

HI_VOID dbg_display(HI_U16 val)
{
    dbg_display_usr(val); 
   
	return;
}

HI_VOID KEYLED_Init(HI_VOID) 
{ 	
    KEYLED_Init_Usr(); 

    return;
}

HI_VOID KEYLED_Disable(HI_VOID) 
{
    keyled_disable_usr(); 

	return;
}

HI_VOID KEYLED_Isr(HI_VOID) 
{
    keyled_isr_usr();
    
	return;
}

