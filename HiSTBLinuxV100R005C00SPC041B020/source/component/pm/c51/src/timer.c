#include "base.h"
#include "keyled.h"

/************************************************************************* 
						timer  module
*************************************************************************/
//HI_U32 time_suspend_start = 0;
//HI_U32 time_suspend_end = 0;
//HI_U16 time_day = 0;
HI_U8  time_hour = 0;
HI_U8  time_minute = 0;
HI_U8  time_second = 0;
HI_U8  time_cnt = 0;
HI_U8  time_mod = 0;
HI_U8  time_dot = 0;
HI_U8  time_type = 0;
//DATA_U32_S  channum = {0};
DATA_U32_S  waittime = {0xffffffff};

void TIMER_Init(void)
{
	/* 1. disable timer1 and set the 32-bit count*/
	regData.val32 = 0x02;
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL;
	write_regVal();

	/* 2. set the Timer Load */
    /*For the unstandard panel , the led refresh /key scan must be set by yourself*/
    /*CNcomment: 客户前面板必须人为调用led刷新/key 扫描*/  
#if (defined KEYLED_CT1642)
    time_mod = 50;
	regData.val32 = 0x00075300;
#elif (defined KEYLED_CT1642_INNER) || (defined KEYLED_74HC164)
	time_mod = 1;
    regData.val32 = 0x016e3600;
#else
    time_mod = 8;
	regData.val32 = 0x002dc6c0;
#endif
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_LOAD;
	write_regVal();

	/* 4. set Timer control */  
	regData.val32 = 0x62;  /* one  frequency-division*/ /*CNcomment:1分频*/ 
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL;
	write_regVal();
	
	/* 5. clear int */
	regData.val32 = 0x01;
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_INTCLR;
	write_regVal();
}

void TIMER_Isr(void)
{
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_RIS;
	read_regVal();
	
	if (regData.val8[3])
	{
		// disable interrupt	
		regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL;
		read_regVal();
		regData.val8[3] &= 0xcf;
		write_regVal();
		
		// clear interrupt
		regData.val32 = 0x01;
		regAddr.val32 = TIMER_BASE_ADDR + TIMER_INTCLR;
		write_regVal();
		
		// enable interrupt
		regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL;
		read_regVal();
		regData.val8[3] |= 0x30;
		write_regVal();

	    time_cnt++;
		
		if (time_cnt != time_mod)
		{
#ifdef KEYLED_CT1642 //Only ct1642 need to refresh LED Data 20ms once.
			if(time_type == TIME_DISPLAY)
			{
				timer_display();
			}
			else if (time_type == DIGITAL_DISPLAY)
			{
				chan_display();
			}
#endif
			return;
		}
		time_cnt = 0;

		time_second++;
		if (time_second >= 60)
		{
			time_second -= 60;
			time_minute++; 
		}
		
		if (time_minute >= 60)
		{
			time_minute -= 60;
			time_hour++;
		}
		
		if (time_hour >= 24)
		{
			time_hour = 0;
		}

		if (time_type == TIME_DISPLAY)
		{	
			time_dot = (1 - time_dot);	
			timer_display();	
		}
		else if (time_type == DIGITAL_DISPLAY)
		{
#ifdef KEYLED_CT1642
			chan_display();
#endif
		}
		else
		{
			// do not need to do anything
		}

		// 3 time for waking up 
		if (waittime.val32 != 0)
		{
			waittime.val32--;
		}

        if (0 == waittime.val32)
        {
		    pmocType = HI_UNF_PMOC_WKUP_TIMEOUT;
		    pmocflag = HI_TRUE;
        }
	}
	return;
}

void TIMER_Enable(void) 
{ 
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL;
	read_regVal(); 
	regData.val8[3] |= 0x80; 
	write_regVal(); 

	return;
}

void TIMER_Disable(void)
{ 
	regAddr.val32 = TIMER_BASE_ADDR + TIMER_CONTROL; 
	read_regVal();
	regData.val8[3] &= 0x7f; 
	write_regVal(); 

	return;	
}

