/*************************************************************************
 * Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: keyled_pt6964.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 * main\1    2012-04-19   chenqiang      NULL            Create this file.
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

#include "hi_common.h"
#include "hi_type.h"
#include "hi_drv_struct.h"

#include "drv_gpio_ext.h"
#include "hi_drv_module.h"

#include "hi_unf_keyled.h"
#include "hi_error_mpi.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"
#include "drv_keyled_pt6964.h"

/*-------------------------------------------------------------------
 * macro define section
 *-----------------------------------------------------------------*/
#define LED5_MODE
#define DEFAULT_REP_KEY_TIME 300

#define CMD_READ (0x42)
#define CMD_INC_WRITE (0x40)
#define CMD_FIX_WRITE (0x44)
#define CMD_DIP_ON (0x8f)
#define CMD_DIP_OFF (0x80)

#define keyled_set_display_mode(index) display_mode_index_pt6964 = (index)
#define keyled_get_display_mode(index) display_mode_index_pt6964;

#define keyled_udelay(x) udelay(x)
#define keyled_mdelay(x) udelay(x * 1000)

#define  GPIO_CLOCK_SET(val) s_pGpioFunc->pfnGpioWriteBit(CLK_PT6964, val)
#define  GPIO_STB_SET(val) s_pGpioFunc->pfnGpioWriteBit(STB_PT6964, val)

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

DEFINE_SPINLOCK(pt6964lock);


static HI_UNF_KEYLED_TIME_S s_keyled_time = {
    0
};

static HI_U8 s_display_switch = CMD_DIP_ON;
static int s_key_curr = KEY_MACRO_NO;
static int s_key_last = KEY_MACRO_NO;
static int s_tingle_check = 0;
static int s_key_detected_flag = 0;
static int s_key_code_last = KEY_MACRO_NO;
static int s_key_code_curr = KEY_MACRO_NO;
static int s_bKeyPressFlag = 0;
static int s_cycle_cnt = 0;
static KeyLed_Status_E Pt6964Status = KeyOFF_LedOFF;

static HI_U32 display_mode_index_pt6964 = 1;

static HI_U32 LedDigDisCode_pt6964[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

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

const HI_U8 display_mode_array_pt6964[DISPLAY_MODE_NUM] = {
    0, 1, 2, 3
};

static HI_VOID hi_pt6964_keyled_scan(unsigned long data);
static DEFINE_TIMER(keyled_scan_timer, hi_pt6964_keyled_scan, 0, 0);

HI_VOID		   keyled_pt6964_displaytime(HI_UNF_KEYLED_TIME_S keyled_time, HI_BOOL dotflag);

/*-------------------------------------------------------------------
 * function define section
 *-----------------------------------------------------------------*/

static HI_VOID keyled_dotflash_handler_pt6964(unsigned long data)
{
	mod_timer(&dotflash_timer,jiffies + msecs_to_jiffies(SEC_TIMING));

    if (dot_flag)
    {
        dot_flag = HI_FALSE;
    }
    else
    {
        dot_flag = HI_TRUE;
    }

    keyled_pt6964_displaytime(s_keyled_time, dot_flag);

    return;
}

HI_S32 keyled_pt6964_gpio_dirset(HI_VOID)
{
    s_pGpioFunc->pfnGpioDirSetBit(STB_PT6964, 0);  //output
    s_pGpioFunc->pfnGpioDirSetBit(CLK_PT6964, 0);  //output

    return HI_SUCCESS;
}

HI_S32 keyled_pt6964_tx_byte(HI_U8 u8data)
{
    HI_U8 bitval = 0;
    HI_U8 j = 0;
    HI_SIZE_T flag;
    
    spin_lock_irqsave(&pt6964lock, flag);        

    s_pGpioFunc->pfnGpioDirSetBit(DIN_PT6964, 0);
    keyled_udelay(1);

    for (j = 0; j < 8; j++)
    {
        GPIO_CLOCK_SET(0);
        keyled_udelay(1);

        //LSB send first, MSB send last
        bitval = (u8data >> j) & 0x1;
        s_pGpioFunc->pfnGpioWriteBit(DIN_PT6964, bitval);

        keyled_udelay(2);

        GPIO_CLOCK_SET(1);

        keyled_udelay(1);
    }

    spin_unlock_irqrestore(&pt6964lock, flag);    

    return HI_SUCCESS;
}

HI_S32 keyled_pt6964_write(HI_U8 u8data)
{
    GPIO_STB_SET(1);
    GPIO_CLOCK_SET(1);

    GPIO_STB_SET(0);

    keyled_udelay(1);

    keyled_pt6964_tx_byte(u8data);

    GPIO_STB_SET(1);

    return HI_SUCCESS;
}

HI_S32 keyled_pt6964_rx_byte(HI_U8 *u8data)
{
    HI_U8 u8val = 0;
    HI_U8 u8realval = 0;
    HI_U32 bitval = 0;
    HI_U8 j = 0;
    HI_SIZE_T flag;

    if (NULL == u8data)
    {
        HI_ERR_KEYLED("null pointer\n");
        return HI_FAILURE;
    } 
           
    spin_lock_irqsave(&pt6964lock, flag);    

    s_pGpioFunc->pfnGpioDirSetBit(DOUT_PT6964, 1);

    for (j = 0; j < 8; j++)
    {
        GPIO_CLOCK_SET(0);

        keyled_udelay(1);

        GPIO_CLOCK_SET(1);

        keyled_udelay(1);

        s_pGpioFunc->pfnGpioReadBit(DOUT_PT6964, &bitval);
        u8val <<= 1;
        u8val  += (bitval & 0x1);

        keyled_udelay(1);
    }
    
    spin_unlock_irqrestore(&pt6964lock, flag);
    
    HI_INFO_KEYLED("u8val:0x%x\n", u8val);

    u8realval = 0;
    for (j = 0; j < 8; j++)
    {
        u8realval <<= 1;
        u8realval  += (u8val >> j) & 0x1;
    }

    HI_INFO_KEYLED("u8realval:0x%x\n", u8realval);

    *u8data = u8realval;

    return HI_SUCCESS;
}

HI_S32 keyled_pt6964_read(HI_U8 *u8buf, HI_U32 u32len)
{
    HI_U8 i = 0;

    if (NULL == u8buf)
    {
        HI_ERR_KEYLED("null pointer\n");
        return HI_FAILURE;
    }

    GPIO_STB_SET(1);

    GPIO_CLOCK_SET(1);

    GPIO_STB_SET(0);

    keyled_pt6964_tx_byte(CMD_READ);

    keyled_udelay(2);    //twait

    for (i = 0; i < u32len; i++)
    {
        keyled_pt6964_rx_byte(&u8buf[i]);
    }

    GPIO_STB_SET(1);

    return HI_SUCCESS;
}

#if 0
HI_S32 keyled_pt6964_display_addr_inc_init(HI_U8 u8addr, HI_U8 *u8buf, HI_U32 u32len, HI_U8 u8ctrl)
{
    HI_U32 i = 0;

    //command 2:set write command
    keyled_pt6964_write(CMD_INC_WRITE);

    //command 3:set start address
    s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 1);

    s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + CLK_PT6964, 1);

    s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 0);

    keyled_pt6964_tx_byte(u8addr + 0xc0);

    //:set display data
    for (i = 0; i < u32len; i++)
    {
        keyled_pt6964_tx_byte(u8buf[i]);
        HI_INFO_KEYLED("disp_buf_arr[%d]:0x%x\n", i, u8buf[i]);
    }

    s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 1);

    //command 1:set display mode
    keyled_pt6964_write(display_mode_array_pt6964[display_mode_index_pt6964]);

    //command 4
    keyled_pt6964_write(CMD_DIP_OFF);

    //command 1:set display mode
    keyled_pt6964_write(display_mode_array_pt6964[display_mode_index_pt6964]);
    HI_INFO_KEYLED("display_mode_array[%d] = %d \n",
                   display_mode_index_pt6964, display_mode_array_pt6964[display_mode_index_pt6964]);

    //command 4
    keyled_pt6964_write(CMD_DIP_ON);

    return HI_SUCCESS;
}

#endif

HI_S32 keyled_pt6964_display_addr_inc(HI_U8 u8addr, HI_U8 *u8buf, HI_U32 u32len, HI_U8 u8ctrl)
{
    HI_U32 i = 0;

    //modified according to mingwei
    //keyled_pt6964_write(DISPLAY_OFF);

    //command 1:set display mode
    keyled_pt6964_write(display_mode_array_pt6964[display_mode_index_pt6964]);
    HI_INFO_KEYLED("display_mode_array[%d]:0x%x\n",
                   display_mode_index_pt6964, display_mode_array_pt6964[display_mode_index_pt6964]);

    //command 2:set write command
    keyled_pt6964_write(CMD_INC_WRITE);

    //command 3:set start address
    GPIO_STB_SET(1);

    GPIO_CLOCK_SET(1);

    GPIO_STB_SET(0);

    keyled_pt6964_tx_byte(u8addr + 0xc0);

    //:set display data
    for (i = 0; i < u32len; i++)
    {
        keyled_pt6964_tx_byte(u8buf[i]);
        HI_INFO_KEYLED("disp_buf_arr[%d]:0x%x\n", i, u8buf[i]);
    }

    GPIO_STB_SET(1);

    //command 4
    keyled_pt6964_write(u8ctrl);

    return HI_SUCCESS;
}

#if 0
HI_S32 keyled_pt6964_display_addr_fix(HI_U8 u8addr, HI_U8 *u8buf, HI_U32 u32len, HI_U8 u8ctrl)
{
    HI_U32 i = 0;

    keyled_pt6964_write(display_mode_array_pt6964[display_mode_index_pt6964]);

    //command 1
    keyled_pt6964_write(CMD_FIX_WRITE);

    for (i = 0; i < u32len; i++)
    {
        //command 2
        s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 1);

        s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + CLK_PT6964, 1);

        s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 0);

        keyled_pt6964_tx_byte(u8addr);

        keyled_pt6964_tx_byte(u8buf[i]);

        s_pGpioFunc->pfnGpioWriteBit(GROUP * HI_GPIO_BIT_NUM + STB_PT6964, 1);
    }

    //command 4
    keyled_pt6964_write(u8ctrl);

    return HI_SUCCESS;
}

#endif

HI_S32 keyled_pt6964_get_keycode(HI_VOID)
{
    HI_U8 i, j;
    HI_S32 s32flag = 0;
    HI_S32 s32key_code_arr_sum = 0;

    for (i = 0; i < 5; i++)
    {
        if (0 != (key_buf_arr[i] & 0x3f))
        {
            for (j = 0; j < 6; j++)
            {
                if (key_buf_arr[i] & (0x1 << j))
                {
                    s32flag++;
                    s32key_code_arr_sum += key_code_arr[i * 6 + j];
                }
            }
        }
    }

    if (s32flag > 0)
    {
        return s32key_code_arr_sum;
    }
    else
    {
        return HI_FAILURE;
    }
}

HI_S32 keyled_pt6964_handle_keycode(HI_VOID)
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
                if (s_bKeyPressFlag)  /*last key is valid*/
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

            if ((s_cycle_cnt >= keyled_dev.repkey_delaytime / (SCAN_INTV_PT6964))
                && (s_bKeyPressFlag))
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

    return HI_SUCCESS;
}

#if 0
HI_VOID keyled_pt6964_enable_ledc (HI_VOID)
{
    s_display_switch = DISPLAY_ON;

    return;
}

HI_VOID keyled_pt6964_disable_ledc (HI_VOID)
{
    s_display_switch = DISPLAY_OFF;

    return;
}

HI_VOID keyled_pt6964_display_lockled(HI_BOOL bLockFlag)
{
    HI_U8 i;
    HI_U8 u8ctrl = 0;

    for (i = 0; i < DISPLAY_REG_NUM / 2; i++)
    {
        if (HI_TRUE == bLockFlag)
        {
            disp_buf_arr[2 * i + 1] = 0x1;
        }
        else
        {
            disp_buf_arr[2 * i + 1] = 0x2;
        }

        HI_INFO_KEYLED("disp_buf_arr[%d]:0x%x\n", 2 * i + 1, disp_buf_arr[2 * i + 1]);
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u8ctrl = CMD_DIP_OFF;
    }
    else
    {
        u8ctrl = CMD_DIP_ON;
    }

    //set display
    keyled_pt6964_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u8ctrl);

    return;
}

HI_VOID keyled_pt6964_display(HI_U32 u32lowdata)
{
    HI_U8 i;
    HI_U8 u8ctrl = 0;

    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        disp_buf_arr[i * 2] = ((HI_U32)u32lowdata >> (8 * i)) & 0xff;
    }

    if (DISPLAY_OFF == s_display_switch)
    {
        u8ctrl = CMD_DIP_OFF;
    }
    else
    {
        u8ctrl = CMD_DIP_ON;
    }

    //set display
    keyled_pt6964_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, u8ctrl);

    return;
}

#endif

HI_VOID keyled_pt6964_displaytime(HI_UNF_KEYLED_TIME_S keyled_time, HI_BOOL dotflag)
{
    HI_U8 index = 0;

    index = keyled_time.u32Hour / 10;
    disp_buf_arr[0] = LedDigDisCode_pt6964[index];
    index = keyled_time.u32Hour % 10;
    disp_buf_arr[2] = LedDigDisCode_pt6964[index];

    if (dotflag)
    {
        disp_buf_arr[4] = 0xff;
    }
    else
    {
        disp_buf_arr[4] = 0x0;
    }

    index = keyled_time.u32Minute / 10;
    disp_buf_arr[6] = LedDigDisCode_pt6964[index];
    index = keyled_time.u32Minute % 10;
    disp_buf_arr[8] = LedDigDisCode_pt6964[index];

    //s_keyled_time = keyled_time;

    //set display
    keyled_pt6964_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, s_display_switch);

    return;
}

static HI_VOID hi_pt6964_keyled_scan(unsigned long data)
{
    HI_S32 s32keycode = 0;

	mod_timer(&keyled_scan_timer,jiffies + msecs_to_jiffies(SCAN_INTV_PT6964));

    //get key val
    if (Pt6964Status & KeyON)
    {
        keyled_pt6964_read(key_buf_arr, KEY_COL_NUM / 2);

        s32keycode = keyled_pt6964_get_keycode();

        if (HI_FAILURE == s32keycode)
        {
            s_key_detected_flag = 0;
            s_key_code_curr = KEY_MACRO_NO;
        }
        else
        {
            s_key_detected_flag = 1;
            s_key_code_curr = s32keycode;
            keyled_dev.key_come++;
            HI_INFO_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d, getkey:%d\n",
                           keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len, s_key_code_curr);
        }

        keyled_pt6964_handle_keycode();
    }

    return;
}

HI_VOID keyled_pt6964_clear_keybuf_func(HI_VOID)
{
    keyled_dev.tail = keyled_dev.head;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;

    return;
}

static HI_S32 keyled_pt6964_soft_init(HI_VOID)
{
    memset(disp_buf_arr, 0, DISPLAY_REG_NUM);

    //keyled_pt6964_display_addr_inc_init(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_OFF);

    return HI_SUCCESS;
}

HI_S32 KEYLED_Init_PT6964(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (KeyOFF_LedOFF != Pt6964Status)
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

    s_display_switch = CMD_DIP_ON;
    s_key_curr = KEY_MACRO_NO;
    s_key_last = KEY_MACRO_NO;
    s_tingle_check = 0;

    dotflash_timer.function = keyled_dotflash_handler_pt6964;
    init_timer(&dotflash_timer);

    keyled_pt6964_gpio_dirset();

    keyled_pt6964_clear_keybuf_func();

    keyled_pt6964_soft_init();

    del_timer_sync(&keyled_scan_timer);
    keyled_scan_timer.expires = jiffies + msecs_to_jiffies(SCAN_INTV_PT6964);
    add_timer(&keyled_scan_timer);

    return HI_SUCCESS;
}

HI_S32 KEYLED_DeInit_PT6964(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Open_PT6964(HI_VOID)
{
    KEYLED_Init_PT6964();
    Pt6964Status |= KeyON;

    HI_INFO_KEYLED("start IsKeyOpen=%d\n", (Pt6964Status & KeyON) ? 1 : 0);

#ifdef LED5_MODE
    keyled_set_display_mode(1);        /* 5 led 13 segment mode*/
#else
    keyled_set_display_mode(0);        /* 4 led 14 segment mode*/
#endif

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Close_PT6964(HI_VOID)
{
    keyled_pt6964_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, CMD_DIP_OFF);
    Pt6964Status &= ~KeyON;

    HI_INFO_KEYLED("stop IsKeyOpen=%d\n", (Pt6964Status & KeyON) ? 1 : 0);

    del_timer_sync(&keyled_scan_timer);

    KEYLED_DeInit_PT6964();

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Reset_PT6964(HI_VOID)
{
    keyled_pt6964_clear_keybuf_func();

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_GetValue_PT6964(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    keyled_key_info_s keyled_to_user = {
        0
    };
    HI_S32 err = 0;

    //HI_INFO_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n", keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);
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
            HI_INFO_KEYLED("wake up by timeout\n");
            return HI_ERR_KEYLED_TIMEOUT;
        }
    }

    //HI_INFO_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n", keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);

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
}

HI_S32 KEYLED_KEY_SetBlockTime_PT6964(HI_U32 u32BlockTimeMs)
{
    keyled_dev.blocktime = u32BlockTimeMs;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_SetRepTime_PT6964(HI_U32 u32RepTimeMs)
{
    keyled_dev.repkey_delaytime = u32RepTimeMs;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsRepKey_PT6964(HI_U32 u32IsRepKey)
{
    keyled_dev.enable_repkey = u32IsRepKey;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsKeyUp_PT6964(HI_U32 u32IsKeyUp)
{
    keyled_dev.enable_keyup = u32IsKeyUp;

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Open_PT6964(HI_VOID)
{
    KEYLED_Init_PT6964();

    Pt6964Status |= LedON;
    HI_INFO_KEYLED("start IsLedOpen=%d\n", (Pt6964Status & LedON) ? 1 : 0);

#ifdef LED5_MODE
    keyled_set_display_mode(1);    /* 5 led 13 segment mode*/
#else
    keyled_set_display_mode(0);    /* 4 led 14 segment mode*/
#endif
    s_display_switch = CMD_DIP_ON;

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Close_PT6964(HI_VOID)
{
    Pt6964Status &= ~LedON;
    HI_INFO_KEYLED("stop IsLedOpen=%d\n", (Pt6964Status & LedON) ? 1 : 0);
    s_display_switch = CMD_DIP_OFF;

    del_timer_sync(&dotflash_timer);
    KEYLED_DeInit_PT6964();

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Display_PT6964(HI_U32 u32Value)
{
    HI_U8 i = 0;

    del_timer_sync(&dotflash_timer);

#ifdef LED5_MODE
    disp_buf_arr[0] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
    i++;
    disp_buf_arr[2] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
    i++;
    disp_buf_arr[6] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
    i++;
    disp_buf_arr[8] = ((HI_U32)u32Value >> (8 * i)) & 0xff;

    disp_buf_arr[4] = 0;
#else
    for (i = 0; i < DISPLAY_SEG_CURR; i++)
    {
        disp_buf_arr[i * 2] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
    }
#endif

    //set display
    keyled_pt6964_display_addr_inc(DISPLAY_REG_START_ADDR, disp_buf_arr, DISPLAY_REG_NUM, s_display_switch);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_DisplayTime_PT6964(HI_UNF_KEYLED_TIME_S stKeyLedTime)
{
    del_timer_sync(&dotflash_timer);
    dotflash_timer.expires = jiffies + msecs_to_jiffies(SEC_TIMING);
    add_timer(&dotflash_timer);

    dot_flag = HI_TRUE;

    s_keyled_time = stKeyLedTime;

    keyled_pt6964_displaytime(stKeyLedTime, dot_flag);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashPin_PT6964(HI_UNF_KEYLED_LIGHT_E enPin)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashFreq_PT6964(HI_UNF_KEYLED_LEVEL_E enLevel)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_GetProcInfo_PT6964(KEYLED_PROC_INFO_S *stInfo)
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

HI_S32 KEYLED_Suspend_PT6964(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_Resume_PT6964(HI_VOID)
{
    return HI_SUCCESS;
}
