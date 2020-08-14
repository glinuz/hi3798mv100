#include "base.h"
#include "keyled.h"

#ifdef KEYLED_74HC164

/************************************************************************* 
						2.2 keyled_std module
*************************************************************************/
/* 1 register addr */ /*CNcomment:寄存器地址*/
#define KEYLED_PHY_ADDR 	(0xF8003000)
#define LEDC_CONTROL    	(KEYLED_PHY_ADDR + 0x0)   /*LED control register*/   /*CNcomment:控制寄存器 */
#define LEDC_CONFIG     	(KEYLED_PHY_ADDR + 0x4)   /*LED config register*/   /* CNcomment:LED显示配置寄存器  */
#define LEDC_KEYINT     	(KEYLED_PHY_ADDR + 0x8)   /*Key interrupt register */  /*CNcomment: 键盘采样中断寄存器 */
#define LEDC_KEYDATA    	(KEYLED_PHY_ADDR + 0xc)   /*Status of keypad sampling in LED module*/  /*CNcomment: LED模块采样键盘状态 */
#define LEDC_CLKTIM     	(KEYLED_PHY_ADDR + 0x10)   /*Parameter of LEDClk signal about the electic high or low*/  /*CNcomment: LedClk信号高低电平时间参数 */
#define LEDC_FRETIM     	(KEYLED_PHY_ADDR + 0x14)   /*Frequency of LED fresh*/  /*CNcomment: LED刷新频率 */
#define LEDC_FLASHTIM   	(KEYLED_PHY_ADDR + 0x18)   /*Frequency of LED flash*/  /* CNcomment:LED闪烁频率 */
#define LEDC_KEYTIM     	(KEYLED_PHY_ADDR + 0x1c)   /*Frequency of key scan*/  /*CNcomment: 键盘扫描频率 */
#define LEDC_DATA1      	(KEYLED_PHY_ADDR + 0x20)   /*The first data register for LED display*/  /*CNcomment: 第一个LED显示数据寄存器 */
#define LEDC_DATA2      	(KEYLED_PHY_ADDR + 0x24)   /*The second data register for LED display*/ /* CNcomment:第二个LED显示数据寄存器 */
#define LEDC_DATA3      	(KEYLED_PHY_ADDR + 0x28)   /*The third data register for LED display*/  /*CNcomment: 第三个LED显示数据寄存器 */
#define LEDC_DATA4      	(KEYLED_PHY_ADDR + 0x2c)   /*The fourth data register for LED display*/  /*CNcomment: 第四个LED显示数据寄存器 */
//#define LEDC_SYSTIM         (KEYLED_PHY_ADDR + 0x34)   

/* 2 Control param */
/* 2.1 Key control */ /*CNcomment:按键控制*/
#define KEYPAD_ENABLE        0x00000008    /* Enable key scan */  /* CNcomment: 键盘扫描使能 */ 
#define KEYPAD_PRESS_INTR    0x00000002    /*Keypad press down interrupt mask*/ /*CNcomment:键盘按下中断屏蔽 */
#define KEYPAD_RELEASE_INTR  0x00000001    /*keypad release interrupt mask*/ /*CNcomment: 键盘释放中断屏蔽 */

/* 2.2 key data    */
#define KEY_7_PRESS 		 0x80  /*(1 << 7)*/        
#define KEY_6_PRESS 		 0x40  /*(1 << 6)*/       
#define KEY_5_PRESS 		 0x20  /*(1 << 5)*/        
#define KEY_4_PRESS 		 0x10  /*(1 << 4)*/       
#define KEY_3_PRESS 		 0x08  /*(1 << 3) */        
#define KEY_2_PRESS 		 0x04  /*(1 << 2) */       
#define KEY_1_PRESS 		 0x02  /*(1 << 1)*/       
#define KEY_0_PRESS 		 0x01  /*(1 << 0)*/         
#define KEYPAD_ERROR    	(1000)   

/* 2.3 led control */
#define LED_ENABLE          0x00000200    /*Enable LED */  /* CNcomment:LED模块使能 */
#define LEDC_ENABLE    		0x00000100    /*Enable LED display*/  /* CNcomment:LED显示使能 */
#define LEDC_FLASH_4   		0x00000080    /*Enable the fourth LED flash*/  /*CNcomment: 第四LED管闪烁使能 */
#define LEDC_FLASH_3   		0x00000040    /*Enable the third LED flash*/  /*CNcomment: 第三LED管闪烁使能 */
#define LEDC_FLASH_2   		0x00000020    /*Enable the second LED flash*/  /* CNcomment:第二LED管闪烁使能 */
#define LEDC_FLASH_1   		0x00000010    /*Enable the first LED flash*/  /*CNcomment: 第一LED管闪烁使能 */
#define LEDC_FLASH_ALL 		0x000000f0    /*Enable all LED flash*/  /* CNcomment:全部LED管闪烁 */

/* 2.4  led config */
#define LEDC_ANODE       	0x00000002   /*(1 << 1)*/
#define LEDC_HIGHT_LEVEL 	0x00000001   /*(1 << 0)*/

/* 2.5  def clk*/
#define SYSTEM_CLK       	(24)   /*unit::MHz def:24MHz*/   /* CNcomment:单位:MHz def:24MHz*/
#define CLKTIM_DEFAULT   	(250)  /*unit:KHz min:210KHz*/   /* CNcomment:单位:KHz min:210KHz*/
#define FRETIM_DEFAULT   	(650)  /*unit:Hz  min:108Hz*/   /* CNcomment:单位:Hz  min:108Hz*/
#define FLASHTIM_DEFAULT 	(60)   /*unit:Hz 10Hz*/   /* CNcomment:单位:Hz 10Hz*/
#define FLASHLEV_DEFAULT 	(1)    /*1means  the slowest while 5 means fastest*/   /*CNcomment: 1 最慢  5 最快 */
#define KEY_DEFAULT      	(60)   /*unit:Hz  min:0.5Hz*/   /* CNcomment:单位:Hz  min:0.5Hz */

/*variable*/
code HI_U8 ledDisCode[11] = {0x03,0x9F,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0xff};

static HI_U8 KeyPressFlag = 0;

void KEYLED_Init_Usr(void)
{
#ifndef HI_ADVCA_RELEASE
        printf_str("74HC164 Board Init \r\n");
#endif

	KeyPressFlag = 0;

    /* Disable keyled  */
	regData.val32 = 0x0;
	regAddr.val32 = LEDC_CONTROL;
	write_regVal();
	
	regData.val32 = 0x6;
	regAddr.val32 = LEDC_CONFIG;
	write_regVal();
	
	regData.val32 = 0x2;
	regAddr.val32 = LEDC_CLKTIM;
	write_regVal();
	
	regData.val32 = 0x2;
	regAddr.val32 = LEDC_FRETIM;
	write_regVal();
	
#if 0 //use default value
	flashtim = 62 - (FLASHLEV_DEFAULT - 1) * 15;
	regData.val32 = fretim;
	regAddr.val32 = LEDC_FLASHTIM;
	write_regVal();
#endif

	regData.val32 = 0x4;
	regAddr.val32 = LEDC_KEYTIM;
	write_regVal();

    /*Clean all interrupts*/  /*CNcomment: 2.1 清所有中断*/
	regData.val32 = 0;
	regAddr.val32 = LEDC_KEYINT;
	write_regVal();

	/* Mask bits*/  /*CNcomment:2.2 屏蔽位*/ 
	regData.val32 = 0;
	regData.val8[3] |= 0x3;  //  KEYPAD_PRESS_INTR | KEYPAD_RELEASE_INTR 
	regAddr.val32 = LEDC_CONTROL;
	write_regVal();

    /* Enable Module/LED/KEY */
    regAddr.val32 = LEDC_CONTROL;
    read_regVal();
    regData.val32 |= (LED_ENABLE | LEDC_ENABLE | KEYPAD_ENABLE);
    write_regVal();

	return;	
}

void keyled_disable_usr(void)
{
	regAddr.val32 = LEDC_CONTROL;
	read_regVal();
	regData.val32 &= ~(LEDC_ENABLE | KEYPAD_ENABLE);
	write_regVal();
    
	return;
}

void keyled_isr_usr(void)
{
	HI_U8 key,IntVal;

	regAddr.val32 = LEDC_KEYINT;
	read_regVal();
	IntVal = regData.val8[3] & 0x3;
	if(IntVal)
	{
        /* Mask interrupt */
		regAddr.val32 = LEDC_CONTROL;
		read_regVal();
		regData.val8[3] &= 0xfc ;  
		write_regVal();
        
		if (IntVal & 0x2)// keypress int
		{
			/* Get key value  */
			regAddr.val32 = LEDC_KEYDATA;
			read_regVal();
			switch (regData.val8[3])
			{
			case KEY_7_PRESS:
				key = 7;
				break;
			case KEY_6_PRESS:
				key = 6;
				break;
			case KEY_5_PRESS:
				key = 5;
				break;
			case KEY_4_PRESS:
				key = 4;
				break;
			case KEY_3_PRESS:
				key = 3;
				break;
			case KEY_2_PRESS:
				key = 2;
				break;
			case KEY_1_PRESS:
				key = 1;
				break;
			case KEY_0_PRESS:
				key = 0;
				break;
			default:
				key = 0xff;
				break;
			}

			if (key == klPmocVal)
			{
				KeyPressFlag = 1;
			}			
		}
		else if ((1 == KeyPressFlag) && (IntVal & 0x1))  //key release int 
		{
			pmocType = HI_UNF_PMOC_WKUP_KEYLED;
			pmocflag = HI_TRUE;
		}

        /*clear press and release key int*/
		regData.val32 |= 0x3;
		regAddr.val32  = LEDC_KEYINT;
		write_regVal();

        if (HI_TRUE != pmocflag)
        {
            /* Unmask key interrupt */
			regAddr.val32 = LEDC_CONTROL;
			read_regVal();
			regData.val8[3] |= 0x3 ; 
			write_regVal();
        }
	}
    
	return;
}

void chan_display_usr(DATA_U32_S channum)
{
	HI_U8 index;  
	
	/* Disable LEDC */
	regAddr.val32 = LEDC_CONTROL;
	read_regVal();
	regData.val8[2] &= 0xfe; 	// &= ~LEDC_ENABLE; 
	write_regVal();
	
	/* Thousand digit */ 
	index = channum.val8[0];  
	regData.val32 = 0;
	regData.val8[3] = ledDisCode[index];
	regAddr.val32 = LEDC_DATA1;
	write_regVal();

	/* Hundred digit */ 
	index = channum.val8[1];
	regData.val32 = 0;
	regData.val8[3] = (ledDisCode[index] & (~time_dot));
	regAddr.val32 = LEDC_DATA2;
	write_regVal();

	/* Ten digit */
	index = channum.val8[2]; 
	regData.val32 = 0;
	regData.val8[3] = (ledDisCode[index] & (~time_dot));
	regAddr.val32 = LEDC_DATA3;
	write_regVal();
 
	index = channum.val8[3];
	regData.val32 = 0;
	regData.val8[3] = ledDisCode[index];
	regAddr.val32 = LEDC_DATA4;
	write_regVal();

	regAddr.val32 = LEDC_CONTROL;
	read_regVal();
	regData.val8[2] |= 0x03; //LEDC_ENABLE; 
	write_regVal();
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

void no_display_usr(void)
{
    DATA_U32_S  channum;

    channum.val32 = 0x0a0a0a0a;
    
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

