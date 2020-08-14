/*************************************************************************
 * Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keyled.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 * main\1    2007-10-16   w54542      NULL            Create this file.
 * main\2    2010-04-15   jianglei 40671    NULL         modify for HD.
 ***************************************************************************/
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
//#include <mach/hardware.h>
#include <asm/signal.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/poll.h>

#include "hi_type.h"
#include "hi_drv_struct.h"
#include "hi_drv_module.h"
#include "drv_gpio_ext.h"

#include "hi_unf_keyled.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"

#include "drv_keyled_fd650.h"

/*-------------------------------------------------------------------
 * macro define section
 *-----------------------------------------------------------------*/
#define DEFAULT_REP_KEY_TIME 300

#define CMD_MODE (0x1)      //0x1 for 5 digits 12 segs, 0 for 4 digits 13 segs
#define CMD_DIP_OFF 0x80
#define CMD_DIP_ON 0x8F

#define keyled_set_display_mode(index) display_mode_index = (index)
#define keyled_get_display_mode(index) display_mode_index;

#define keyled_udelay(x) udelay(x)
#define keyled_mdelay(x) udelay(x * 1000)

#define KEYLED_TRACE_LEVEL 9
#define keyled_trace(level, msg ...) do \
    { \
        if ((level) >= KEYLED_TRACE_LEVEL) \
        { \
            HI_INFO_KEYLED("keyled_trace:%s:%d: ", __FILE__, __LINE__); \
            HI_INFO_KEYLED(msg); \
            HI_INFO_KEYLED("\n"); \
        } \
    } while (0)

/*-------------------------------------------------------------------
 * date structure define section
 *-----------------------------------------------------------------*/

/*-------------------------------------------------------------------
 * variable define section
 *-----------------------------------------------------------------*/

static GPIO_EXT_FUNC_S *s_pGpioFunc = HI_NULL;

static keyled_dev_s keyled_dev;
static wait_queue_head_t wait_keyleds;

static HI_BOOL dot_flag = HI_FALSE;
static struct  timer_list dotflash_timer;
static HI_UNF_KEYLED_TIME_S s_keyled_time = {
    0
};
#define FD650_SEC_TIME (1000)

DEFINE_SPINLOCK(fd650lock);

static int s_display_switch = DISPLAY_ON;
static int s_key_curr = KEY_MACRO_NO;
static int s_key_last = KEY_MACRO_NO;
static int s_tingle_check = 0;
static int s_key_detected_flag = 0;
static int s_key_code_last = KEY_MACRO_NO;
static int s_key_code_curr = KEY_MACRO_NO;
static int s_bKeyPressFlag = 0;
static int s_cycle_cnt = 0;
static KeyLed_Status_E FD650Status = KeyOFF_LedOFF;

static HI_U32 display_mode_index = 0;

static HI_U32 LedDigDisCode_fd650[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

static int s_flash_level = FD650_BIT_INTENS7;

/*-------------------------------------------------------------------
 * array define section
 *-----------------------------------------------------------------*/
static HI_U8 disp_buf_arr[DISPLAY_REG_NUM] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00
};

static HI_U8 key_buf_arr[KEY_COL_NUM / 2] = {
    0x00, 0x00, 0x00, 0x00, 0x00
};

#if 0
static HI_U8 key_code_arr[KEY_REAL_NUM] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d
};

const HI_U8 fd650_display_mode_array[DISPLAY_MODE_NUM] = {
    0, 1, 2, 3
};
#endif

static HI_VOID hi_fd650_keyled_scan(unsigned long data);
static DEFINE_TIMER(keyled_scan_timer, hi_fd650_keyled_scan, 0, 0);

/*-------------------------------------------------------------------
 * function define section
 *-----------------------------------------------------------------*/
static HI_S32 hi_gpio_read_SDA_IN(void)
{
	HI_U32 sda_in;

	//keyled_trace(8,"\n  FD650 hi_gpio_read_SDA_IN \n");

	if (HI_FAILURE == s_pGpioFunc->pfnGpioReadBit(FD650_SDA, &sda_in))
	{
		keyled_trace(8, "\n  FD650 hi_gpio_read_SDA_IN hi_gpio_read_bit failed\n");
		return (-1);
	}

	return (sda_in);
}

/****************************************************************
 *
 *	Function Name:FD650_Start
 *
 *	Description:
 *
 *  Parameter:
 *
 *  return：
****************************************************************/
void FD650_Start( void )
{
    //keyled_trace(8,"\n=== FD650_Start ===\n");

	/** Move position, first set it to output, then set the value */
    /**CNcomment:移动位置，先设输出再拉电平 */
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

/****************************************************************
 *
 *	Function Name:FD650_Stop
 *
 *	Description:
 *
 *  Parameter:
 *
 *  return：
****************************************************************/
void FD650_Stop( void )
{
    //keyled_trace(8,"\n=== FD650_Stop ===\n");
    
    /** Move position, first set it to output, then set the value */
    /**CNcomment:移动位置，先设输出再拉电平 */
    //FD650_SDA_CLR;
    FD650_SDA_D_OUT;
    FD650_SDA_CLR;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SDA_SET;
    DELAY;
    //FD650_SDA_D_IN;
    return;
}

/****************************************************************
 *
 *	Function Name:FD650_WrByte
 *
 *	Description: Write One Byte data
 *
 *  Parameter: data
 *
 *  return：
****************************************************************/
void FD650_WrByte( HI_U8 dat )
{
    HI_U8 i;

    //keyled_trace(2,"\n=== FD650_WrByte  %x ===\n",dat);

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
        DELAY;  /** This delay is optional */ /**CNcomment:可选延时*/
        FD650_SCL_CLR;
    }

	/** Change position, first set the value, then set it to output */
    /**CNcomment:交换位置，电平设置后再设为输出 */
    
    FD650_SDA_SET;
	FD650_SDA_D_IN;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SCL_CLR;
    //FD650_SDA_D_IN;
    return;
}

HI_U8  FD650_RdByte( void )
{
    HI_U8 dat, i;

    //keyled_trace(3, "\n=== FD650_RdByte ===\n");

    FD650_SDA_D_OUT;
    FD650_SDA_SET;
    FD650_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        DELAY;  /** This delay is optional */
        FD650_SCL_SET;
        DELAY;  /** This delay is optional */
        dat <<= 1;
        if (FD650_SDA_IN)
        {
            dat++;
        }

        FD650_SCL_CLR;
    }

    FD650_SDA_D_OUT;
    FD650_SDA_SET;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SCL_CLR;

    //keyled_trace(8, "\n=== FD650_RdByte  %d ===\n", dat);
    return dat;
}

void FD650_Write( HI_U16 cmd )
{
	HI_SIZE_T flag;
    
    spin_lock_irqsave(&fd650lock, flag); 
    //keyled_trace(8,"\n  FD650 FD650_Write\n");

    FD650_Start();
    FD650_WrByte(((HI_U8)(cmd >> 7) & 0x3E) | 0x40);
    FD650_WrByte((HI_U8)cmd);
    FD650_Stop();

	spin_unlock_irqrestore(&fd650lock, flag);

    return;
}

HI_U8 FD650_Read( void )
{
    HI_U8 keycode = 0;

	HI_SIZE_T flag;
    
    spin_lock_irqsave(&fd650lock, flag);      

	//keyled_trace(8,"\n  FD650 FD650_Read \n");
	
	FD650_Start(); 
	FD650_WrByte((HI_U8)((FD650_GET_KEY>>7)&0x3E)|0x01|0x40);
	keycode=FD650_RdByte();

    //keyled_trace(8,"\n FD650_Read keycode = %x \n", keycode);

    FD650_Stop();
    if ((keycode & 0x00000040) == 0)
    {
        keycode = 0;
    }

	//keyled_trace(8,"\n FD650_Read keycode = %x \n", keycode);
    //if (keycode)
    //{
    //    keyled_trace(8,"FD650 get:%x!\n", keycode);
    //}

	spin_unlock_irqrestore(&fd650lock, flag);  

    return keycode;
}

HI_S32 keyled_fd650_gpio_dirset_write(HI_VOID)
{
    s_pGpioFunc->pfnGpioDirSetBit(FD650_SCL, FD50_OUTPUT);  //output
    s_pGpioFunc->pfnGpioDirSetBit(FD650_SDA, FD50_OUTPUT);  //output

    return 0;
}

HI_S32 keyled_fd650_gpio_dirset_read(HI_VOID)
{
    s_pGpioFunc->pfnGpioDirSetBit(FD650_SCL, FD50_OUTPUT);  //output
    s_pGpioFunc->pfnGpioDirSetBit(FD650_SDA, FD50_INPUT); //input

    return 0;
}

HI_S32 keyled_fd650_display_config(HI_U8 *u8buf)
{
    HI_S32 i = 0;

    if (NULL == u8buf)
    {
        keyled_trace(8, "null pointer(u8buf)\n");
        return -1;
    }

    for (i = 0; i < DISPLAY_REG_NUM; i++)
    {
        disp_buf_arr[i] = u8buf[i];
        keyled_trace(8, "disp_buf_arr[%d]:0x%x\n", i, disp_buf_arr[i]);
    }

    return 0;
}

HI_S32 keyled_fd650_tx_byte(HI_U8 u8data)
{
	HI_SIZE_T flag;
    
    spin_lock_irqsave(&fd650lock, flag);  
    FD650_WrByte(u8data);
	spin_unlock_irqrestore(&fd650lock, flag);  
    return 0;
}

HI_S32 keyled_fd650_write_burst(HI_U8 *u8buf, HI_U32 u32len)
{
    HI_S32 i = 0;

    //keyled_trace(8,"\n  FD650 keyled_write_burst \n");

    keyled_fd650_gpio_dirset_write();
    keyled_udelay(1);

    //hi_gpio_write_bit(FD650_GROUP, STB, 1);

    s_pGpioFunc->pfnGpioWriteBit(FD650_SCL, 1);

    // hi_gpio_write_bit(FD650_GROUP, STB, 0);

    keyled_udelay(1);

    for (i = 0; i < u32len; i++)
    {
        keyled_fd650_tx_byte(u8buf[i]);
    }

    //hi_gpio_write_bit(GROUP, STB, 1);

    return 0;
}

HI_S32 keyled_fd650_write(HI_U8 u8data)
{
    keyled_fd650_write_burst(&u8data, 1);

    return 0;
}


HI_S32 keyled_fd650_rx_byte(HI_U8 *u8data)
{
    *u8data = FD650_RdByte();

    return 0;
}

HI_S32 keyled_fd650_read(HI_U8 *u8buf, HI_U32 u32len)
{
    HI_U8 keycode = 0;

    //keyled_trace(8,"\n  FD650 keyled_read\n");

    if (NULL == u8buf)
    {
        keyled_trace(8, "null pointer\n");
        return HI_FAILURE;
    }

    keycode = FD650_Read();

    *u8buf = keycode;
    return 0;
}

HI_S32 keyled_fd650_display_addr_inc(HI_U8 u8addr, HI_U8 *u8buf, HI_U32 u32len, HI_U8 u8ctrl)
{
    //HI_S32 i = 0;
    HI_U8 disp_tmp[4] = {
        0
    };

    //keyled_trace(8,"\n  FD650 keyled_display_addr_inc\n");
    if (u8ctrl != CMD_DIP_OFF)
    {
        disp_tmp[0] = u8buf[0];
        disp_tmp[1] = u8buf[2];
        disp_tmp[2] = u8buf[4];
        disp_tmp[3] = u8buf[6];

        FD650_Write(FD650_SYSON | s_flash_level);
        FD650_Write( FD650_DIG0 | (HI_U8)disp_tmp[0] ); //send display data
        FD650_Write( FD650_DIG1 | (HI_U8)disp_tmp[1] );
        FD650_Write( FD650_DIG2 | (HI_U8)disp_tmp[2] );
        FD650_Write( FD650_DIG3 | (HI_U8)disp_tmp[3] );
    }
    else
    {
        disp_tmp[0] = u8buf[0];
        disp_tmp[1] = u8buf[2];
        disp_tmp[2] = u8buf[4];
        disp_tmp[3] = u8buf[6];
        FD650_Write(FD650_SYSOFF | u8ctrl);
        FD650_Write( FD650_DIG0 | (HI_U8)disp_tmp[3] );
        FD650_Write( FD650_DIG1 | (HI_U8)disp_tmp[2] );
        FD650_Write( FD650_DIG2 | (HI_U8)disp_tmp[1] );
        FD650_Write( FD650_DIG3 | (HI_U8)disp_tmp[0] );
    }

    return 0;
}

HI_S32 keyled_fd650_display_addr_inc_init(HI_U8 u8addr, HI_U8 *u8buf, HI_U32 u32len, HI_U8 u8ctrl)
{
    //keyled_trace(8,"\n  FD650 keyled_display_addr_inc_init\n");

    keyled_fd650_display_addr_inc(u8addr, u8buf, u32len, u8ctrl);
    return 0;
}

static HI_U32 hi_key_map(HI_U32 key)
{
    keyled_trace(8, "\n hi_key_map key = %d \n", key);

    switch (key)
    {
    case 4:      //menu
        return 3;
    case 16:      //ok
        return 5;
    case 8:      //left
        return 4;
    case 32:      //right
        return 6;
    case 64:      //down
        return 7;
    case 2:      //up
        return 2;
    case 1:     //standby
        return 1;
    // case 25:   //6969 indicator, no use for app
    //    return 25;
    case 128:    // quit
        return 8;
    default:
        break;
    }

    return 0xff;
}

HI_S32 keyled_fd650_get_keycode(HI_VOID)
{
    //keyled_trace(8,"\n  FD650 keyled_get_keycode \n");
    if (key_buf_arr[0])
    {
        return key_buf_arr[0];
    }

    return 0xff;
}

HI_S32 keyled_fd650_get_keycode_sleep(void)
{
    HI_S32 s32keyCode = 0;

    keyled_fd650_read(key_buf_arr, KEY_COL_NUM / 2);

    //keyled_trace(8,"\n  FD650 Keyled_get_keycode_sleep\n");

    s32keyCode = keyled_fd650_get_keycode();
    s32keyCode = hi_key_map(s32keyCode);

    //keyled_trace(8,"\n  FD650 Keyled_get_keycode_sleep %d  \n", s32keyCode);

    return s32keyCode;
}

HI_S32 keyled_fd650_handle_keycode(HI_VOID)
{
    if (0 == s_key_detected_flag)  //no key pressed in this period
    {
        if (s_key_code_last == KEY_MACRO_NO)  //no key in last period
        {
            ;
        }
        else  //has key in last period
        {
            if (s_bKeyPressFlag)
            {
                /* report last key 'RELEASE', and recode no key pressed in this time */
                if (keyled_dev.enable_keyup)
                {
                    KEYLED_BUF_HEAD.keyled_code	 = s_key_code_last;
                    KEYLED_BUF_HEAD.keyled_state = KEY_RELEASE;
                    keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                    wake_up_interruptible(&wait_keyleds);
                }

                s_bKeyPressFlag = HI_FALSE;
            }
            else    /*warning: key twitter*/
            {
                ;
            }

            s_key_code_last = KEY_MACRO_NO;
        }
    }
    else  //has key pressed in this period
    {
        if (s_key_code_curr != s_key_code_last)  //new key
        {
            if (KEY_MACRO_NO == s_key_code_last)  /*no key in last scan*/
            {
                ;
            }
            else    /*has key in last scan*/
            {
                if(s_bKeyPressFlag)  /*last key is valid*/
                {
                    if (keyled_dev.enable_keyup)
                    {
                        KEYLED_BUF_HEAD.keyled_state = KEY_RELEASE;
                        KEYLED_BUF_HEAD.keyled_code	 = s_key_code_last;
                        keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                        wake_up_interruptible(&wait_keyleds);
                    }

                    s_bKeyPressFlag = HI_FALSE;
                }
                else   /*warning: key twitter */
                {
                    ;
                }
            }

            s_cycle_cnt = 0;
            s_key_code_last = s_key_code_curr;
        }
        else  //old key
        {
            s_cycle_cnt++;
            if ((1 == s_cycle_cnt) && (HI_TRUE != s_bKeyPressFlag))
            {
                KEYLED_BUF_HEAD.keyled_state = KEY_PRESS;
                KEYLED_BUF_HEAD.keyled_code	 = s_key_code_last;
                keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                wake_up_interruptible(&wait_keyleds);

                s_cycle_cnt = 0;
                s_bKeyPressFlag = HI_TRUE;
            }
            if ((s_cycle_cnt >= keyled_dev.repkey_delaytime / (SCAN_INTV)) && (s_bKeyPressFlag))
            {
                if (keyled_dev.enable_repkey)
                {
                    KEYLED_BUF_HEAD.keyled_state = KEY_HOLD;
                    KEYLED_BUF_HEAD.keyled_code	 = s_key_code_last;
                    keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                    wake_up_interruptible(&wait_keyleds);
                }

                s_cycle_cnt = 0;
                s_bKeyPressFlag = HI_TRUE;
            }
        }
    }

    s_key_code_curr = KEY_MACRO_NO;
    s_key_detected_flag = 0;

    return HI_FAILURE;
}

HI_VOID keyled_fd650_enable_ledc (HI_VOID)
{
    s_display_switch = DISPLAY_ON;
    FD650_Write(FD650_SYSON);
    return;
}

HI_VOID keyled_fd650_disable_ledc (HI_VOID)
{
    s_display_switch = DISPLAY_OFF;
    FD650_Write(FD650_SYSOFF);
    return;
}

HI_VOID keyled_display_setLevel(int level)
{
    keyled_trace(8, "\n  keyled_display_setLevel %d \n", level);
    switch (level)
    {
    case 1:
        s_flash_level = FD650_BIT_INTENS2;
        break;
    case 2:
        s_flash_level = FD650_BIT_INTENS3;
        break;
    case 3:
        s_flash_level = FD650_BIT_INTENS4;
        break;
    case 4:
        s_flash_level = FD650_BIT_INTENS7;
        break;
    default:
        s_flash_level = FD650_BIT_INTENS8;
        break;
    }
}

HI_VOID keyled_fd650_display_lockled(HI_BOOL bLockFlag)
{
    HI_U32 u32ctrl = 0;

    if (HI_TRUE == bLockFlag)
    {
        disp_buf_arr[6] |= 0x80;
    }
    else
    {
        disp_buf_arr[6] &= 0x7f;
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u32ctrl = CMD_DIP_OFF;
    }
    else
    {
        u32ctrl = s_flash_level;
    }

    //set display
    keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u32ctrl);
}

HI_VOID keyled_fd650_display_dot(HI_BOOL bLockFlag)
{
    HI_U32 u32ctrl = 0;

    if (HI_TRUE == bLockFlag)
    {
        disp_buf_arr[4] |= 0x80;
    }
    else
    {
        disp_buf_arr[4] &= 0x7f;
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u32ctrl = CMD_DIP_OFF;
    }
    else
    {
        u32ctrl = s_flash_level;
    }

    //set display
    keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u32ctrl);
}

HI_VOID keyled_fd650_display(HI_U32 u32lowdata)
{
    HI_S32 i;
    HI_U32 u32ctrl = 0;

    keyled_trace(8, "\n  FD650 keyled_display \n");

    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        if (i == 0)
        {
            disp_buf_arr[(DISPLAY_SEG_CURR - i - 1) * 2] &= 0x80;
            disp_buf_arr[(DISPLAY_SEG_CURR - i - 1) * 2] += ((HI_U32)u32lowdata >> (8 * i)) & 0xff;
        }
        else
        {
            disp_buf_arr[(DISPLAY_SEG_CURR - i - 1) * 2] = ((HI_U32)u32lowdata >> (8 * i)) & 0xff;
        }
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u32ctrl = CMD_DIP_OFF;
    }
    else
    {
        u32ctrl = s_flash_level;
    }

    //set display
    keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u32ctrl);
}

HI_VOID keyled_fd650_displaytime(HI_UNF_KEYLED_TIME_S keyled_time, HI_BOOL dotflag)
{
    HI_S32 i;
    int index = 0;

    HI_U32 u32ctrl = 0;

#if 0
    disp_buf_arr[6] &= 0x80;

    index = keyled_time.u32Hour / 10;
    disp_buf_arr[6] += LedDigDisCode_fd650[index];

    index = keyled_time.u32Hour % 10;
    disp_buf_arr[4] = LedDigDisCode_fd650[index];

    if (dotflag)
    {
        disp_buf_arr[4] += 0x80;
    }

    index = keyled_time.u32Minute / 10;
    disp_buf_arr[2] = LedDigDisCode_fd650[index];

    index = keyled_time.u32Minute % 10;
    disp_buf_arr[0] = LedDigDisCode_fd650[index];
#else
    disp_buf_arr[0] &= 0x80;

    index = keyled_time.u32Hour / 10;
    disp_buf_arr[0] += LedDigDisCode_fd650[index];

    index = keyled_time.u32Hour % 10;
    disp_buf_arr[2] = LedDigDisCode_fd650[index];

    if (dotflag)
    {
        disp_buf_arr[2] += 0x80;
    }

    index = keyled_time.u32Minute / 10;
    disp_buf_arr[4] = LedDigDisCode_fd650[index];

    index = keyled_time.u32Minute % 10;
    disp_buf_arr[6] = LedDigDisCode_fd650[index];
#endif
    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        disp_buf_arr[2 * i + 1] = 0x1;
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u32ctrl = CMD_DIP_OFF;
    }
    else
    {
        u32ctrl = s_flash_level;
    }

    //set display
    keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u32ctrl);
}

static HI_VOID hi_fd650_keyled_scan(unsigned long data)
{
    HI_S32 s32keycode = 0;
    HI_U32 u32ctrl = 0;
	
	mod_timer(&keyled_scan_timer,jiffies + msecs_to_jiffies(SCAN_INTV));

    if (DISPLAY_OFF == s_display_switch)
    {
        u32ctrl = CMD_DIP_OFF;
    }
    else
    {
        u32ctrl = s_flash_level;
    }

    //set display - for fd650
    //keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u32ctrl);

    if (FD650Status & KeyON)
    {
        //get key val
        key_buf_arr[0] = 0;

        keyled_fd650_read(key_buf_arr, KEY_COL_NUM / 2);
		
        s32keycode = keyled_fd650_get_keycode();

        //keyled_trace(8,"\n  s32keycode = 0x%x \n", s32keycode);

        if ((HI_FAILURE == s32keycode) || (s32keycode == 0xFF))
        {
            s_key_detected_flag = 0;
            s_key_code_curr = KEY_MACRO_NO;
        }
        else
        {
            s_key_detected_flag = 1;
            s_key_code_curr = s32keycode;
            keyled_dev.key_come++;
            //keyled_trace(8,"enter hi_keyled_read : head=%d, tail=%d, buf_len=%d, getkey:%d\n", keyled_dev.head,
            //       keyled_dev.tail, keyled_dev.buf_len,
            //       s_key_code_curr);
        }

        keyled_fd650_handle_keycode();
    }

    return;
}

HI_VOID keyled_fd650_clear_keybuf_func(HI_VOID)
{
    keyled_dev.tail = keyled_dev.head;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;

    return;
}

static HI_S32 keyled_fd650_soft_init(HI_VOID)
{
    memset(disp_buf_arr, 0, DISPLAY_REG_NUM);

    keyled_fd650_display_addr_inc_init(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_OFF);

    return HI_SUCCESS;
}

static HI_VOID keyled_dotflash_handler_fd650(unsigned long data)
{
   	mod_timer(&dotflash_timer,jiffies + msecs_to_jiffies(FD650_SEC_TIME));

    if (dot_flag)
    {
        dot_flag = HI_FALSE;
    }
    else
    {
        dot_flag = HI_TRUE;
    }

    keyled_fd650_displaytime(s_keyled_time, dot_flag);

    return;
}

HI_S32 KEYLED_Init_FD650(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (KeyOFF_LedOFF != FD650Status)
    {
        return HI_SUCCESS;
    }

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&s_pGpioFunc);
    if ((HI_SUCCESS != s32Ret) || (s_pGpioFunc == HI_NULL))
    {
        HI_ERR_KEYLED("Get GPIO function failed! \n");
        return HI_FAILURE;
    }

    init_waitqueue_head(&wait_keyleds);

    keyled_dev.head = 0;
    keyled_dev.tail = 0;
    keyled_dev.buf_len = KEYBUF_LEN;
    keyled_dev.enable_repkey = 1;
    keyled_dev.key_come	 = 0;
    keyled_dev.key_read	 = 0;
    keyled_dev.blocktime = DEFAULT_BLOCKTIME;
    keyled_dev.repkey_delaytime = DEFAULT_REP_KEY_TIME;
    keyled_dev.enable_keyup = 1;

    //s_display_switch = DISPLAY_ON;
    s_key_curr = KEY_MACRO_NO;
    s_key_last = KEY_MACRO_NO;
    s_tingle_check = 0;

    keyled_fd650_clear_keybuf_func();
    keyled_fd650_enable_ledc();
    keyled_fd650_soft_init();

    del_timer_sync(&keyled_scan_timer);
    keyled_scan_timer.expires = jiffies + msecs_to_jiffies(SCAN_INTV);
    add_timer(&keyled_scan_timer);

    dotflash_timer.function = keyled_dotflash_handler_fd650;
    init_timer(&dotflash_timer);

    return HI_SUCCESS;
}

HI_S32 KEYLED_DeInit_FD650(HI_VOID)
{
    if (KeyOFF_LedOFF == FD650Status)
    {
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Open_FD650(HI_VOID)
{
    KEYLED_Init_FD650();
    FD650Status |= KeyON;
    HI_INFO_KEYLED("start IsKeyOpen=%d\n", (FD650Status & KeyON) ? 1 : 0);
    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Close_FD650(HI_VOID)
{
    FD650Status &= ~KeyON;
    HI_INFO_KEYLED("stop IsKeyOpen=%d\n", (FD650Status & KeyON) ? 1 : 0);
    del_timer_sync(&keyled_scan_timer);
    del_timer_sync(&dotflash_timer);

    KEYLED_DeInit_FD650();

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Reset_FD650(HI_VOID)
{
    keyled_fd650_clear_keybuf_func();

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_GetValue_FD650(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    keyled_key_info_s keyled_to_user = {
        0
    };
    HI_S32 err = 0;

    //    HI_S32 res = 0;

    //HI_ERR_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n", keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);
    if ((keyled_dev.head) == (keyled_dev.tail))
    {
        if (0 == keyled_dev.blocktime)
        {
            return HI_ERR_KEYLED_NO_NEW_KEY;
        }

        err = wait_event_interruptible_timeout(wait_keyleds, (keyled_dev.head != keyled_dev.tail),
                                               (long)(keyled_dev.blocktime * HZ / 1000));
        if (err <= 0)  /*not wake up by received key*/
        {
            //HI_ERR_KEYLED("wake up by timeout\n");
            return HI_ERR_KEYLED_TIMEOUT;
        }
    }

    //HI_ERR_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n", keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);

    if (keyled_dev.head != keyled_dev.tail)
    {
        keyled_to_user	= KEYLED_BUF_TAIL;
        keyled_dev.tail = KEYLED_INC_BUF(keyled_dev.tail, keyled_dev.buf_len);
        keyled_dev.key_read++;

        *pu32PressStatus = keyled_to_user.keyled_state;
        *pu32KeyId = keyled_to_user.keyled_code;
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_KEYLED_NO_NEW_KEY;
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_SetBlockTime_FD650(HI_U32 u32BlockTimeMs)
{
    keyled_dev.blocktime = u32BlockTimeMs;
    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_SetRepTime_FD650(HI_U32 u32RepTimeMs)
{
    keyled_dev.repkey_delaytime = u32RepTimeMs;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsRepKey_FD650(HI_U32 u32IsRepKey)
{
    keyled_dev.enable_repkey = u32IsRepKey;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsKeyUp_FD650(HI_U32 u32IsKeyUp)
{
    keyled_dev.enable_keyup = u32IsKeyUp;

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Open_FD650(HI_VOID)
{
    KEYLED_Init_FD650();

    FD650Status |= LedON;
    HI_INFO_KEYLED("start IsLedOpen=%d\n", (FD650Status & LedON) ? 1 : 0);

    keyled_set_display_mode(CMD_MODE);
    s_display_switch = DISPLAY_ON;

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Close_FD650(HI_VOID)
{
    FD650Status &= ~LedON;
    HI_INFO_KEYLED("stop IsLedOpen=%d\n", (FD650Status & LedON) ? 1 : 0);
    s_display_switch = DISPLAY_OFF;

    KEYLED_DeInit_FD650();

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Display_FD650(HI_U32 u32Value)
{
    HI_U32 i;

    del_timer_sync(&dotflash_timer);

    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        if (i == 3)
        {
            disp_buf_arr[i * 2] &= 0x80;
            disp_buf_arr[i * 2] += ((HI_U32)u32Value >> (8 * i)) & 0xff;
        }
        else
        {
            disp_buf_arr[i * 2] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
        }
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_OFF);
    }
    else
    {
        keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_ON);
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_DisplayTime_FD650(HI_UNF_KEYLED_TIME_S stKeyLedTime)
{
#if 0
    HI_U8 disp_buf_tmp[DISPLAY_REG_NUM];
    int index = 0;
    int i;

    memset(disp_buf_tmp, 0, DISPLAY_REG_NUM);
    keyled_time = stKeyLedTime;

    index = keyled_time.u32Hour / 10;
    disp_buf_tmp[0] = LedDigDisCode_fd650[index];
    index = keyled_time.u32Hour % 10;
    disp_buf_tmp[2] = LedDigDisCode_fd650[index];

    index = keyled_time.u32Minute / 10;
    disp_buf_tmp[4] = LedDigDisCode_fd650[index];

    disp_buf_tmp[6] = (0x80 & disp_buf_arr[6]);
    index = keyled_time.u32Minute % 10;
    disp_buf_tmp[6] += LedDigDisCode_fd650[index];

    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        disp_buf_tmp[2 * i + 1] = 0x1;
    }

    s_keyled_time = keyled_time;
    dot_flag = HI_TRUE;

    //	keyled_fd650_displaytime(stKeyLedTime, 0);
    keyled_fd650_display_config(disp_buf_tmp);
    keyled_fd650_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_ON);
    add_timer(&dotflash_timer);
#else
    del_timer_sync(&dotflash_timer);
    dotflash_timer.expires = jiffies + msecs_to_jiffies(570);
    add_timer(&dotflash_timer);

    dot_flag = HI_TRUE;

    //memset(disp_buf_tmp, 0, DISPLAY_REG_NUM);
    s_keyled_time = stKeyLedTime;

    keyled_fd650_displaytime(s_keyled_time, dot_flag);
#endif
    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashPin_FD650(HI_UNF_KEYLED_LIGHT_E enPin)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashFreq_FD650(HI_UNF_KEYLED_LEVEL_E enLevel)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_GetProcInfo_FD650(KEYLED_PROC_INFO_S *stInfo)
{
    stInfo->KeyBufSize = keyled_dev.buf_len;
    stInfo->KeyBufHead = keyled_dev.head;
    stInfo->KeyBufTail = keyled_dev.tail;
    stInfo->KeyComeNum = keyled_dev.key_come;
    stInfo->KeyReadNum = keyled_dev.key_read;
    stInfo->u32RepKeyTimeMs = keyled_dev.repkey_delaytime;
    stInfo->u32IsRepKeyEnable = keyled_dev.enable_repkey;
    stInfo->u32IsUpKeyEnable = keyled_dev.enable_keyup;
    stInfo->u32BlockTime = keyled_dev.blocktime;
    stInfo->enFlashPin = keyled_dev.FlashPin;
    stInfo->enFlashLevel = keyled_dev.FlashLevel;
    stInfo->KeyBufTail= keyled_dev.tail;

    return HI_SUCCESS;
}


HI_S32 KEYLED_Suspend_FD650(HI_VOID)
{
    return 0;
}

HI_S32 KEYLED_Resume_FD650(HI_VOID)
{
    return 0;
}

HI_S32 KEYLED_LED_SetLockIndicator_FD650(HI_BOOL bLock)
{
    keyled_fd650_display_lockled(bLock);
    return HI_SUCCESS;
}
