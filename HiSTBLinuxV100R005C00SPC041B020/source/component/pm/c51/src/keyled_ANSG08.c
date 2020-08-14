#include "base.h"
#include "gpio.h"
#include "keyled.h"

#ifdef KEYLED_ANSG08

#define ANSG08_DEVICE_ADDRESS     0x48
#define ANSG08_PORT_MODE          0x4F
#define ANSG08_PWM_CTRL1          0x43
#define ANSG08_OUTPUT             0x2A    

extern HI_U8 hi_gpio_read_SDA_IN(void);

#define ANSG08_SCL (1)
#define ANSG08_SDA (0)
#define SPEAKER (3)

#define HIGH (1)
#define LOW (0)

#define DELAY wait_minute_1(100)
#define SPEAKER_D_OUT gpio5_dirset_bit(SPEAKER, 0)
#define SPEAKER_SET gpio5_write_bit(SPEAKER, HIGH)
#define SPEAKER_CLR gpio5_write_bit(SPEAKER, LOW)

#define ANSG08_SCL_SET gpio5_write_bit(ANSG08_SCL, HIGH)
#define ANSG08_SCL_CLR gpio5_write_bit(ANSG08_SCL, LOW)
#define ANSG08_SCL_D_OUT gpio5_dirset_bit(ANSG08_SCL, 0)
#define ANSG08_SDA_SET gpio5_write_bit(ANSG08_SDA, HIGH)
#define ANSG08_SDA_CLR gpio5_write_bit(ANSG08_SDA, LOW)
#define ANSG08_SDA_IN hi_gpio_read_SDA_IN()
#define ANSG08_SDA_D_OUT gpio5_dirset_bit(ANSG08_SDA, 0)
#define ANSG08_SDA_D_IN gpio5_dirset_bit(ANSG08_SDA, 1)

HI_U8 hi_gpio_read_SDA_IN(void)
{
    HI_U8 sda_in = 0;

    gpio5_read_bit(ANSG08_SDA, &sda_in);

    return sda_in;
}

void ANSG08_Start( void ) reentrant
{
    ANSG08_SDA_D_OUT; // SDA输出
    ANSG08_SDA_SET;   // 在SCL为高时SDA由1变为0开始总线

    ANSG08_SCL_D_OUT;
    ANSG08_SCL_SET;
    DELAY;
    ANSG08_SDA_CLR;
    DELAY;
    ANSG08_SCL_CLR;
    
    return;
}

void ANSG08_Stop( void ) reentrant
{
    ANSG08_SDA_D_OUT; // SDA输出
    ANSG08_SDA_CLR;   // 在SCL为高时SDA由0变为1结束总线
    DELAY;
    ANSG08_SCL_SET;
    DELAY;
    ANSG08_SDA_SET;
    
    return;
}

HI_U8 ANSG08_Get_Ack(void)
{
    HI_U8 get_ack;

    ANSG08_SDA_D_IN; // SDA输入
    ANSG08_SDA_SET;  // 8位发送完后释放数据线，准备接收应答位
    ANSG08_SCL_SET;
    
    if(ANSG08_SDA_IN) // 从设备无应答
    { 
        get_ack = 1; 
    }    
    else // 从设备正确应答
    {   
        get_ack = 0; 
    }
    
    ANSG08_SCL_CLR;

    return get_ack;
}

void ANSG08_WrByte( HI_U8 dat ) reentrant
{
    HI_U8 i;

    ANSG08_SDA_D_OUT;
    for (i = 0; i != 8; i++)
    {
        if (dat & 0x80)     // 先发送最高位MSB
        {
            ANSG08_SDA_SET;
        }
        else
        {
            ANSG08_SDA_CLR;
        }

        DELAY;
        ANSG08_SCL_SET;    // SCL为高时，保持数据稳定
        dat <<= 1;
        DELAY;
        ANSG08_SCL_CLR;    // SCL为低时，改变数据
    }

    ANSG08_Get_Ack();
    
    return;
}

void ANSG08_Set_Ack(HI_U8 set_ack)
{
    ANSG08_SDA_D_OUT;    // SDA输出
    if (set_ack)
    { 
        ANSG08_SDA_SET; // 不确认，主设备告诉从设备，接收数据结束。
    }    
    else        
    { 
        ANSG08_SDA_CLR;    // 确认，主设备每收到一个字节后都要发送确认
    }    
    DELAY;
    ANSG08_SCL_SET;
    DELAY;
    ANSG08_SCL_CLR;
}


HI_U8 ANSG08_RdByte( void ) reentrant
{
    HI_U8 dat, i;

    ANSG08_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        DELAY;
        ANSG08_SCL_SET;
        DELAY;
        dat <<= 1;
        if (ANSG08_SDA_IN)
        {
            dat++;
        }

        ANSG08_SCL_CLR;
    }

    return dat;
}

HI_U8 ANSG08_Read(HI_U8 u8Reg) reentrant
{
    HI_U8 u8Data = 0;
    
    ANSG08_Start();
    ANSG08_WrByte(ANSG08_DEVICE_ADDRESS);
    ANSG08_WrByte(u8Reg);
    ANSG08_Stop();

    ANSG08_Start();
    ANSG08_WrByte(ANSG08_DEVICE_ADDRESS | 0x01);
    u8Data = ANSG08_RdByte();
    ANSG08_Set_Ack(1);
    ANSG08_Stop();

    return u8Data;
}


void ANSG08_Write(HI_U8 u8Reg, HI_U8 u8Data)   reentrant
{
    ANSG08_Start();
    ANSG08_WrByte(ANSG08_DEVICE_ADDRESS);
    ANSG08_WrByte(u8Reg);
    ANSG08_WrByte(u8Data);
    ANSG08_Stop();

    return;
}

void Speaker(void)
{
    SPEAKER_D_OUT;
	SPEAKER_SET;
    wait_minute_3(1, 210, 210);
    SPEAKER_CLR;
}


HI_U8 keyled_ansg08_readkey(void)
{
    HI_U8 keycode = 0;

    keycode = ANSG08_Read(ANSG08_OUTPUT);

	if (keycode != 0)
	{
        ANSG08_Write(ANSG08_PORT_MODE, 0x01);
	
        ANSG08_Write(ANSG08_PWM_CTRL1, 0x0f);

		Speaker();

		wait_minute_3(2, 210, 210);

        ANSG08_Write(ANSG08_PWM_CTRL1, 0x0);
	}

    return keycode;
}

void no_display_usr(void)
{
    return;
}

void KEYLED_Init_Usr(void)
{
#ifndef HI_ADVCA_RELEASE
        printf_str("ANSG08 Board Init \r\n");
#endif
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
    HI_U8 key;

    key = keyled_ansg08_readkey();

	if (key == klPmocVal)
    {
        pmocType = HI_UNF_PMOC_WKUP_KEYLED;
        pmocflag = HI_TRUE;
    }
	

    return;
}

void chan_display_usr(DATA_U32_S channum)
{
    return;
}

void timer_display_usr() 
{
	return;
}

void dbg_display_usr(HI_U16 val)
{
    return;
}

#endif
