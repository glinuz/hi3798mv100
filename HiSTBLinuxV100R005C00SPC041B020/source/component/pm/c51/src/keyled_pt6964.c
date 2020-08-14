
#include "base.h"
#include "gpio.h"
#include "keyled.h"

#ifdef KEYLED_PT6964

/************************************************************************* 
						2.3 keyled_pt6964 module
*************************************************************************/
/* 1 cfg param */
#define PT6964_STB     (3)    /*STB signal pin connected with GPIO5_3*/ /*CNcomment:片选信号管脚与GPIO5_3相连*/
#define PT6964_CLK     (1)    /*CLK signal pin connected with GPIO5_2*/ /*CNcomment:时钟信号管脚与GPIO5_2相连*/
#define PT6964_DIN     (0)
#define PT6964_DOUT    (0)

#define PT6964_STANBY_KEY	   (46)   /* GPIO5_6 use to standby and wake up in pt6964 panel*/

/*pt6964 resource*/
#define PT6964_DISPLAY_REG_NUM  (14)
#define PT6964_KEY_REG_NUM      (5)

/*pt6964 command*/
#define CMD1_DISP_MODE_1   (0x1)  /*5 LEDs*/ /*CNcomment:5个数码管*/
#define CMD1_LED_ADDR_BASE (0xc0)
#define CMD1_LED_WRITE_INC (0x40)
#define CMD1_KEY_READ      (0x42)
#define CMD1_DIP_ON        (0x8a)
#define CMD1_DIP_OFF       (0x80)

#define KEY_MACRO_NO 	(0xff)

/*variable*/
idata HI_U8 disp_reg[PT6964_DISPLAY_REG_NUM] = {0};
//idata HI_U8 key_reg[PT6964_KEY_REG_NUM] = {0};
//code HI_U8 ledDisDot_pt6964 [10] = {0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};
code HI_U8 ledDisCode_pt6964[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void keyled_pt6964_tx_byte(HI_U8 val) reentrant
{
	HI_U8 i = 0;
	HI_U8 tmp = val;
	
	gpio5_dirset_bit(PT6964_DIN, 0);
	
	for (i = 0; i < 8; i++)
	{
		gpio5_write_bit(PT6964_CLK, 0);
		
		//LSB first, MSB later
		gpio5_write_bit(PT6964_DIN, tmp & 0x1);
		tmp = (tmp >> 1);
		
		gpio5_write_bit(PT6964_CLK, 1);		
	}
	
	return;
}

void keyled_pt6964_write(HI_U8 val) reentrant
{    
	/*send single command byte*/ /*CNcomment:发送单个命令字节*/
	gpio5_write_bit(PT6964_STB, 0);
	keyled_pt6964_tx_byte(val);
	gpio5_write_bit(PT6964_STB, 1);

	return;
}

void keyled_pt6964_led_update(void) reentrant
{
	HI_U8 i = 0;

	/*command 1:set display mode* / /*CNcomment: command 1:设置显示模式 */
	keyled_pt6964_write(CMD1_DISP_MODE_1);
	
	/*command 2:set write command  */ /*CNcomment: command 2:设置写命令*/
	keyled_pt6964_write(CMD1_LED_WRITE_INC);

	/*command 3:set the initialization address and send data */
	/*CNcomment: command 3:设置起始地址并发送数据*/
	gpio5_write_bit(PT6964_STB, 0);
	keyled_pt6964_tx_byte(CMD1_LED_ADDR_BASE);
	
	for (i = 0; i < PT6964_DISPLAY_REG_NUM; i++)
	{
		keyled_pt6964_tx_byte(disp_reg[i]);
	}
	gpio5_write_bit(PT6964_STB, 1);
	
	//command 4 :open LED display control
	keyled_pt6964_write(CMD1_DIP_ON);	

	return;
}

void keyled_pt6964_gpio_dirset(void)
{    
	/*Set gpio direction*/ /*CNcomment:gpio方向设定*/
	gpio5_dirset_bit(PT6964_STB, 0);
	gpio5_dirset_bit(PT6964_CLK, 0);

	gpio5_dirset_bit(PT6964_STANBY_KEY, 1);

    return ;
}

void no_display_usr()
{
	disp_reg[0] = 0x0;
	disp_reg[2] = 0x0;
	disp_reg[4] = 0x0;
	disp_reg[6] = 0x0;
	disp_reg[8] = 0x0;

	keyled_pt6964_led_update();
	
	return;
}

void KEYLED_Init_Usr(void)
{
#ifndef HI_ADVCA_RELEASE
        printf_str("PT6964 Board Init \r\n");
#endif
	
	keyled_pt6964_gpio_dirset();
	no_display_usr();

    //gpio_SetIntType(PT6964_STANBY_KEY);
    //gpio_IntEnable(PT6964_STANBY_KEY,HI_TRUE);
	
	return;
}

void keyled_disable_usr(void)
{		
	no_display_usr();
	
	return;
}

void keyled_isr_usr()
{
	/* GPIO wake up */
	//gpio_isr(PT6964_STANBY_KEY);

	return;
}

void chan_display_usr(DATA_U32_S channum)
{
	HI_U8 index;
  
	index = channum.val8[0];  
	disp_reg[0] = ledDisCode_pt6964[index];
	
	index = channum.val8[1];
	disp_reg[2] = ledDisCode_pt6964[index];

    if (time_dot)
	{
		disp_reg[4] = 0xff;
	}
	else
	{
		disp_reg[4] = 0x0;
	}
    
	index = channum.val8[2];
	disp_reg[6] = ledDisCode_pt6964[index];

	index = channum.val8[3];
	disp_reg[8] = ledDisCode_pt6964[index];

	keyled_pt6964_led_update();

	return;
}

void timer_display_usr() 
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

