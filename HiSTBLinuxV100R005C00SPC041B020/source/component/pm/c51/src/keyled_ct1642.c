
#include "base.h"
#include "gpio.h"
#include "keyled.h"

#ifdef KEYLED_CT1642


/************************************************************************* 
						2.4 keyled_ct1642 module
*************************************************************************/
/* 1 register addr */


/* 1 cfg param */
#define CT1642_CLK     (2) //GPIO5_2
#define CT1642_DAT     (4) //GPIO5_4
#define CT1642_KEY     (7) //GPIO5_7

#define  FALSE 0
#define  TRUE  1

/*variable*/
DATA_U32_S disp_num;

idata HI_U8 key_buf_ct1642;
                               
code HI_U8 ledDisCode_ct1642[16] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6, 0xee, 0x3e, 0x9c, 0x7a, 0x9e, 0x8e};
//code HI_U8 ledDisDot_ct1642[10] = {0xfd, 0x61, 0xdb, 0xf3, 0x67, 0xb7, 0xbf, 0xe1, 0xff, 0xf7};

#define  GPIO_CLOCK_SET(val)    gpio5_write_bit(CT1642_CLK, val)
#define  GPIO_DATA_SET(val)     gpio5_write_bit(CT1642_DAT, val)
#define  GPIO_KEY_GET(val)      gpio5_read_bit(CT1642_KEY, &val)

void keyled_ct1642_gpio_dirset(void)
{    
     /*config  gpio  direction */
     /*CNcomment:gpio方向设定*/
	gpio5_dirset_bit(CT1642_DAT, 0);
	gpio5_dirset_bit(CT1642_CLK, 0);
	gpio5_dirset_bit(CT1642_KEY, 1);

    return;
}

void ct1642_send_bit_data(HI_U8 v_character, HI_U8 v_position)reentrant
{
	HI_U8 BitPosition ; 	
	HI_U8 BitCharacter = v_character;	 
	HI_U8 i;
	
	switch (v_position)
	{
		case 0: 
		{
			BitPosition = 0xe; 
			BitCharacter |= 0x01; //to light on D1
			break;
		}
		case 1: 
		{
			BitPosition = 0xd;
			break;
		}
		case 2: 
		{
			BitPosition = 0xb; 
			break;
		}
		case 3: 
		{
			BitPosition = 0x7; 
			break;
		}
		case 4:
		{
			BitPosition = 0xf; 
			break;
		}
		default:
		{
			BitPosition = 0xf; 
			BitCharacter = 0x00;
		}									
	}

	/* To choose which LED to show, 4 bits, 1:off; 0:on */
	for (i = 0; i < 4; i++)
	{
		GPIO_CLOCK_SET(FALSE);
		if ((BitPosition << i) & 0x08)
		{
			GPIO_DATA_SET(TRUE);
		}
		else
		{
			GPIO_DATA_SET(FALSE);
		}
		GPIO_CLOCK_SET(TRUE);
	}

	/* To jump 6 meaningless bits  */
	for (i = 0; i < 6; i++)
	{
		GPIO_CLOCK_SET(FALSE);
		GPIO_CLOCK_SET(TRUE);	
	}

	for(i = 0; i < 8; i++)
	{			
		GPIO_CLOCK_SET(FALSE);
		if ((BitCharacter << i) & 0x80)
		{
			GPIO_DATA_SET(TRUE);
		}
		else
		{
			GPIO_DATA_SET(FALSE);
		}
		
		GPIO_CLOCK_SET(TRUE);
	}

	/* when clock is high, put the 18 bits data to flip-latch*/
	GPIO_DATA_SET(FALSE);			
	GPIO_DATA_SET(TRUE);

	/* output the 18 bits data */
	GPIO_CLOCK_SET(FALSE);
	GPIO_DATA_SET(FALSE);
	GPIO_DATA_SET(TRUE);
	
	return;
}

void keyled_ct1642_readkey(void)
{
	HI_U8 nKeyPress = 0;				

	/* Before read key , need to close LED */
	ct1642_send_bit_data(0xff,0x04); 
	wait_minute_1(1);
	
	GPIO_KEY_GET(nKeyPress);	
	if (1 == nKeyPress)
	{
	    /* suppose the last right key is designed for power */
	    /* CNcomment:假定最右的那个键是待机键               */
		ct1642_send_bit_data(1 << 7, 0x04); 
		wait_minute_1(1);
		nKeyPress = 0;
		
		GPIO_KEY_GET(nKeyPress); 
		if (1 == nKeyPress)
		{
			key_buf_ct1642 = 0x8;       
			return;
		}
	}	
	
	key_buf_ct1642 = 0;
	
	return;

}

void keyled_ct1642_display_addr_inc(void)reentrant
{
	HI_U8 i = 0;
	
 	for(i = 0; i < 4; i++)
	{ 
		ct1642_send_bit_data(disp_num.val8[i], i);
		wait_minute_1(5);
 	}
	return ;
}

void no_display_usr(void)
{	
	disp_num.val32 = 0x0;
	
	keyled_ct1642_display_addr_inc();
	
	return;
}


void KEYLED_Init_Usr(void)
{
#ifndef HI_ADVCA_RELEASE
        printf_str("CT1642 Board Init \r\n");
#endif

	keyled_ct1642_gpio_dirset();
	
	no_display_usr();
	
	return;
}

void keyled_disable_usr(void)
{
	no_display_usr();
	
	return;
}

void keyled_isr_usr(void)
{
	HI_U8 key = 0xff;
	
	keyled_ct1642_readkey();
	key = key_buf_ct1642;

	if(key == klPmocVal)
	{
		pmocType = HI_UNF_PMOC_WKUP_KEYLED;
		pmocflag = HI_TRUE;
	}
	
	return;
}

void chan_display_usr(DATA_U32_S channum)
{
	HI_U8 index;
	
	index = channum.val8[0];  
	disp_num.val8[0] = ledDisCode_ct1642[index];
	
	index = channum.val8[1];
	disp_num.val8[1] = (ledDisCode_ct1642[index] | (time_dot << 0));

	index = channum.val8[2];
	disp_num.val8[2] = ledDisCode_ct1642[index];

	index = channum.val8[3];
	disp_num.val8[3] = ledDisCode_ct1642[index];
	
	keyled_ct1642_display_addr_inc();

	return;
}

void timer_display_usr(void)
{
    DATA_U32_S channum;

    channum.val8[0] = time_hour / 10;
    channum.val8[1] = time_hour % 10;
    channum.val8[2] = time_minute / 10;
    channum.val8[3] = time_minute % 10;

    chan_display_usr(channum);
 
    return;
}


void dbg_display_usr(HI_U16 val)
{
	DATA_U32_S  channum;
    HI_U8 i;

    if (val > 9999)
    {
        return;
    }

    for (i = 0; i < 4; i++)
    {
        channum.val8[3 - i] = (val % 10);
        val = val / 10;
    }
   
    chan_display_usr(channum);
    
	return;
}

#endif

