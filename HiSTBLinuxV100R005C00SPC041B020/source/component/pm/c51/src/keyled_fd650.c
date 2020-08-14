#include "base.h"
#include "gpio.h"
#include "keyled.h"

#ifdef KEYLED_FD650

/*************************************************************************
                        2.3 keyled_fd650 module
*************************************************************************/

/* 1 cfg param */
#define STB (5)
#define DIO (5)
#define CLK (3)

#define CMD_DIP_ON (0x8f)
#define CMD_DIP_OFF (0x80)

DATA_U32_S disp_num;
idata HI_U8 key_buf_arr[2] = {
    0x00, 0x00
};
code HI_U8 ledDisDot_fd650[10] = {
    0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef
};
code HI_U8 ledDisCode_fd650[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

/***************************/

//
//      FD650 cmd define
//

/***************************/
extern HI_U8 hi_gpio_read_SDA_IN(void);

#define FD650_SCL (CLK)
#define FD650_SDA (DIO)
#define HIGH (1)
#define LOW (0)

#define DELAY wait_minute_1(100)
#define FD650_SCL_SET gpio5_write_bit(FD650_SCL, HIGH)
#define FD650_SCL_CLR gpio5_write_bit(FD650_SCL, LOW)
#define FD650_SCL_D_OUT gpio5_dirset_bit(FD650_SCL, 0)
#define FD650_SDA_SET gpio5_write_bit(FD650_SDA, HIGH)
#define FD650_SDA_CLR gpio5_write_bit(FD650_SDA, LOW)
#define FD650_SDA_IN hi_gpio_read_SDA_IN()
#define FD650_SDA_D_OUT gpio5_dirset_bit(FD650_SDA, 0)
#define FD650_SDA_D_IN gpio5_dirset_bit(FD650_SDA, 1)

/* 设置系统参数命令 */
#define FD650_BIT_ENABLE 0x01            // 开启/关闭位
#define FD650_BIT_SLEEP 0x04             // 睡眠控制位
#define FD650_BIT_7SEG 0x08              // 7段控制位
#define FD650_BIT_INTENS1 0x10           // 1级亮度
#define FD650_BIT_INTENS2 0x20           // 2级亮度
#define FD650_BIT_INTENS3 0x30           // 3级亮度
#define FD650_BIT_INTENS4 0x40           // 4级亮度
#define FD650_BIT_INTENS5 0x50           // 5级亮度
#define FD650_BIT_INTENS6 0x60           // 6级亮度
#define FD650_BIT_INTENS7 0x70           // 7级亮度
#define FD650_BIT_INTENS8 0x00           // 8级亮度

#define FD650_SYSOFF 0x0400                // 关闭显示、关闭键盘
#define FD650_SYSON (FD650_SYSOFF | FD650_BIT_ENABLE)           // 开启显示、键盘
#define FD650_SLEEPOFF FD650_SYSOFF       // 关闭睡眠
#define FD650_SLEEPON (FD650_SYSOFF | FD650_BIT_SLEEP)         // 开启睡眠
#define FD650_7SEG_ON (FD650_SYSON | FD650_BIT_7SEG)         // 开启七段模式
#define FD650_8SEG_ON (FD650_SYSON | 0x00)         // 开启八段模式
#define FD650_SYSON_1 (FD650_SYSON | FD650_BIT_INTENS1)         // 开启显示、键盘、1级亮度
#define FD650_SYSON_2 (FD650_SYSON | FD650_BIT_INTENS2)         // 开启显示、键盘、2级亮度
#define FD650_SYSON_3 (FD650_SYSON | FD650_BIT_INTENS3)         // 开启显示、键盘、3级亮度
#define FD650_SYSON_4 (FD650_SYSON | FD650_BIT_INTENS4)         // 开启显示、键盘、4级亮度
#define FD650_SYSON_5 (FD650_SYSON | FD650_BIT_INTENS5)         // 开启显示、键盘、5级亮度
#define FD650_SYSON_6 (FD650_SYSON | FD650_BIT_INTENS6)         // 开启显示、键盘、6级亮度
#define FD650_SYSON_7 (FD650_SYSON | FD650_BIT_INTENS7)         // 开启显示、键盘、7级亮度
#define FD650_SYSON_8 (FD650_SYSON | FD650_BIT_INTENS8)         // 开启显示、键盘、8级亮度

/* 加载字数据命令 */
#define FD650_DIG0 0x1400
#define FD650_DIG1 0x1500
#define FD650_DIG2 0x1600
#define FD650_DIG3 0x1700
#define FD650_DOT 0x0080                     // 数码管小数点显示

// 读取按键代码命令
#define FD650_GET_KEY 0x0700                 // 获取按键,返回按键代码

HI_U8 hi_gpio_read_SDA_IN(void)
{
    HI_U8 sda_in = 0;

    gpio5_read_bit(FD650_SDA, &sda_in);

    return sda_in;
}

void FD650_Start( void ) reentrant
{
    //FD650_SDA_SET;
    FD650_SDA_D_OUT;
    FD650_SDA_SET;

    //FD650_SCL_SET;
    FD650_SCL_D_OUT;
    FD650_SCL_SET;
    DELAY;
    FD650_SDA_CLR;
    DELAY;
    FD650_SCL_CLR;
    return;
}

void FD650_Stop( void ) reentrant
{
    //FD650_SDA_CLR;
    FD650_SDA_D_OUT;
    FD650_SDA_CLR;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SDA_SET;
    DELAY;
    FD650_SDA_D_IN;
    return;
}

void FD650_WrByte( HI_U8 dat ) reentrant
{
    HI_U8 i;

    FD650_SDA_D_OUT;
    for (i = 0; i != 8; i++)
    {
        if (dat & 0x80)
        {
            FD650_SDA_SET;
        }
        else
        {
            FD650_SDA_CLR;
        }

        DELAY;
        FD650_SCL_SET;
        dat <<= 1;
        DELAY;  // 可选延时
        FD650_SCL_CLR;
    }

    FD650_SDA_SET;
	FD650_SDA_D_IN;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SCL_CLR;
    
    return;
}

HI_U8  FD650_RdByte( void ) reentrant
{
    HI_U8 dat, i;

    FD650_SDA_D_OUT; //增加设置为输出
    FD650_SDA_SET;
    FD650_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        DELAY;  // 可选延时
        FD650_SCL_SET;
        DELAY;  // 可选延时
        dat <<= 1;
        if (FD650_SDA_IN)
        {
            dat++;
        }

        FD650_SCL_CLR;
    }

    FD650_SDA_D_OUT; //增加设置为输出
    FD650_SDA_SET;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SCL_CLR;

    return dat;
}

void FD650_Write( HI_U16 cmd )   reentrant
{
    FD650_Start();
    FD650_WrByte((HI_U8)(((HI_U8)(cmd >> 7) & 0x3E) | 0x40));
    FD650_WrByte((HI_U8)cmd);
    FD650_Stop();

    return;
}

HI_U8 keyled_fd650_get_keycode(HI_U8 u8rawkey)
{
    if ((u8rawkey & 0x7f) == 0)
    {
        return 0xFF;
    }

    return u8rawkey;
}

void keyled_fd650_readkey(void)
{
    HI_U8 keycode = 0;

    FD650_Start();
    FD650_WrByte((HI_U8)(FD650_GET_KEY >> 7) & 0x3E | 0x01 | 0x40);
    keycode = FD650_RdByte();
    FD650_Stop();
    if ((keycode & 0x00000040) == 0)
    {
        keycode = 0;
    }

    key_buf_arr[0] = keyled_fd650_get_keycode(keycode);

    return;
}

void keyled_fd650_display_addr_inc(void)
{
    FD650_Write( FD650_SYSON_2);
    FD650_Write( FD650_DIG0 | (HI_U8)disp_num.val8[0] ); //发显示数据
    FD650_Write( FD650_DIG1 | (HI_U8)disp_num.val8[1] );
    FD650_Write( FD650_DIG2 | (HI_U8)disp_num.val8[2] );
    FD650_Write( FD650_DIG3 | (HI_U8)disp_num.val8[3] );
    return;
}

void no_display_usr(void)
{
    disp_num.val32 = 0x0;
    
    keyled_fd650_display_addr_inc();
    return;
}

void KEYLED_Init_Usr(void)
{
#ifndef HI_ADVCA_RELEASE
        printf_str("FD650 Board Init \r\n");
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
    HI_U8 i, j;
    HI_U8 key;

    keyled_fd650_readkey();

    if (key_buf_arr[0] == klPmocVal)
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
    disp_num.val8[0] = ledDisCode_fd650[index];
    index = channum.val8[1];
    disp_num.val8[1] = (ledDisCode_fd650[index] | (time_dot << 7));
    index = channum.val8[2];
    disp_num.val8[2] = ledDisCode_fd650[index];
    index = channum.val8[3];
    disp_num.val8[3] = ledDisCode_fd650[index];

    keyled_fd650_display_addr_inc();
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
    DATA_U32_S channum;
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
