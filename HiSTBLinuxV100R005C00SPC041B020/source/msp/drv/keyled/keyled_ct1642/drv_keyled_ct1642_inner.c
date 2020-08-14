/*************************************************************************
 * Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keyled.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 * main\1    2007-10-16   w54542            NULL         Create this file.
 * main\2    2010-04-15   jianglei 40671    NULL         modify for HD.
 ***************************************************************************/
#include <linux/vmalloc.h>
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
#include "hi_drv_dev.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"
#include "hi_kernel_adapt.h"
#include "hi_unf_keyled.h"
#include "hi_error_mpi.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"
#include "drv_keyled_ct1642_inner.h"
#include "hi_drv_reg.h"

#define DEFAULT_REP_KEY_TIME   200

static struct semaphore sem_keyled_std;

static struct  timer_list sleep_timer;
static struct  timer_list dotflash_timer;

static HI_U32  IsKeyOpen = 0;
static HI_U32  IsLedOpen = 0;
static HI_BOOL IsKeyPressed = HI_FALSE;
static HI_U8* keyled_base_addr = HI_NULL_PTR;
static HI_S32 keyled_timeout_cnt = 0;
//static HI_CHIP_TYPE_E enChipType;
//static HI_CHIP_VERSION_E enChipVersion;

//static int KeyScanMode = 0xff;
//static int LedNum = 0xff;

//module_param(KeyScanMode, int, S_IRUGO);
//module_param(LedNum, int, S_IRUGO);


//HI_U32  LedDigDisCode[] = {0x03,0x9F,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};
/* led display time when arm is stand by, add by David Huang 2008-05-02 start */

//HI_U32  LedDigDisDot[] = {0x02,0x9E,0x24,0x0c,0x98,0x48,0x40,0x1e,0x00,0x08};
/* led display time when arm is stand by, add by David Huang 2008-05-02 end */

static HI_U32 LedDigDisDot_ct1642[]  = {0xfd, 0x61, 0xdb, 0xf3, 0x67, 0xb7, 0xbf, 0xe1, 0xff, 0xf7};
static HI_U32 LedDigDisCode_ct1642[] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};


static keyled_dev_s keyled_dev;

static HI_BOOL dot_flag = HI_FALSE;

static HI_UNF_KEYLED_TIME_S s_keyled_time = {0};

static wait_queue_head_t wait_keyleds;

static KEYLED_SusSave SuspendSaveVal;

static HI_VOID keyled_displaytime(HI_UNF_KEYLED_TIME_S keyled_time, HI_BOOL dotflag);

/* config led clock's frequency */
static HI_VOID keyled_config_clktim(HI_U32 clk)
{
    HI_S32 regvalue = 0;
    HI_S32 clktim = 0;
    HI_U32 systim = 0;

    if (0 == clk)
    {
        HI_ERR_KEYLED("prompt: clk can't be 0, set to default %d\n", CLKTIM_DEFAULT);
        clk = CLKTIM_DEFAULT;
    }

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    HI_REG_READ(keyled_base_addr + LEDC_SYSTIM, systim);

    clktim = (SYSTEM_CLK * 1000) / (2 * (systim + 1) * clk) - 1;
    if (clktim < 0)
    {
        clktim = 0;
    }

    HI_REG_WRITE(keyled_base_addr + LEDC_CLKTIM, clktim);

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* config led refresh frequncy: example fre = 200Hz */
static HI_VOID keyled_config_fretim(HI_U32 fre)
{
    HI_S32 regvalue = 0;
    HI_S32 fretim = 0;
    HI_S32 clktim = 0;
    HI_U32 systim = 0;

    if (0 == fre)
    {
        HI_ERR_KEYLED("prompt: fre can't be 0, set to default %d\n", FRETIM_DEFAULT);
        fre = FRETIM_DEFAULT;
    }

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    HI_REG_READ(keyled_base_addr + LEDC_CLKTIM, clktim);
    HI_REG_READ(keyled_base_addr + LEDC_SYSTIM, systim);

    fretim = (SYSTEM_CLK * 1000 * 1000) / (fre * 16 * 40 * (clktim + 1) * (systim + 1) * 5) - 1;
    if (fretim < 0)
    {
        fretim = 0;
    }

    HI_REG_WRITE(keyled_base_addr + LEDC_FRETIM, fretim);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* config keypad scanning frequency: example freq= 40Hz */
static HI_VOID keyled_config_keytim(HI_U32 freq)
{
    HI_S32 regvalue = 0;
    HI_S32 fretim = 0;
    HI_S32 clktim = 0;
    HI_S32 keytim = 0;
    HI_U32 systim = 0;

    if (0 == freq)
    {
        HI_ERR_KEYLED("prompt: freq can't be 0, set to default %d\n", KEY_DEFAULT);
        freq = KEY_DEFAULT;
    }

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    HI_REG_READ(keyled_base_addr + LEDC_SYSTIM, systim);
    HI_REG_READ(keyled_base_addr + LEDC_CLKTIM, clktim);
    HI_REG_READ(keyled_base_addr + LEDC_FRETIM, fretim);

    keytim = (SYSTEM_CLK * 1000 * 1000) / ((fretim + 1) * (systim + 1) * 16 * 40 * 5 * 8 * (clktim + 1) * freq) - 1;
    if (keytim < 0)
    {
        keytim = 0;
    }
    HI_INFO_KEYLED("keytim=%d, fretim=%d, clktim=%d, freq= %d, SYSTEM_CLK=%d/n",
                                    keytim, fretim, clktim, freq, SYSTEM_CLK);

    HI_REG_WRITE(keyled_base_addr + LEDC_KEYTIM, keytim);

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* config led flash frequency: example flash = 40Hz */
static HI_VOID keyled_config_flashtim(HI_U32 flash)
{
    HI_U32 regvalue = 0;
    HI_U32 flashtim = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    flashtim = 62 - (flash - 1) * 15;

    HI_REG_WRITE(keyled_base_addr + LEDC_FLASHTIM, flashtim);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* disable keypad  only, not affect other hardware para*/
static HI_VOID keyled_disable_key(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue &= ((~KEYPAD_ENABLE) & (~LEDC_ENABLE));
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    /* add by z00149549 for DTS2012102304374 */
    msleep(1);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, (regvalue & (~LED_ENABLE)));

    return;
}

/* enable keypad , at the same time enabled ledc*/
static HI_VOID keyled_enable_key(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue |= (KEYPAD_ENABLE
        | KEYPAD_PRESS_INTR | KEYPAD_RELEASE_INTR | LEDC_ENABLE | LED_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* config led type and key type */
static HI_VOID keyled_config_type(HI_U32 ledtype, HI_U32 keytype)
{
    HI_U32 regvalue = 0;

    if (HI_KEYLED_ANODE == ledtype)
    {
        regvalue |= LEDC_ANODE;
    }
    else
    {
        regvalue &= ~LEDC_ANODE;
    }

    if (KEY_TYPE_CT1642 == keytype)
    {
        regvalue |= LEDC_KEY_TYPE;
    }
    else
    {
        regvalue &= ~LEDC_KEY_TYPE;
    }

    regvalue |= KEY_SCAN_MODE;
    regvalue |= KEY_SCAN_LEVEL;

    HI_REG_WRITE(keyled_base_addr + LEDC_CONFIG, regvalue);

    return;
}

/* enable ledc display */
static HI_VOID keyled_enable_ledc (HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue |= (LEDC_ENABLE | LED_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* disable ledc display */
static HI_VOID keyled_disable_ledc(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue &= ~LEDC_ENABLE;
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    /* add by z00149549 for DTS2012102304374 */
    msleep(1);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, (regvalue & (~LED_ENABLE)));

    return;
}

/* get key value from ledc hardware */
static HI_U32 keyled_get_key(HI_VOID)
{
    HI_U32 key = 0;

    HI_REG_READ(keyled_base_addr + LEDC_KEYDATA, key);

    switch (key)
    {
        case KEY_7_PRESS:
        case KEY_6_PRESS:
        case KEY_5_PRESS:
        case KEY_4_PRESS:
        case KEY_3_PRESS:
        case KEY_2_PRESS:
        case KEY_1_PRESS:
        case KEY_0_PRESS:
        case KEY_76comb_PRESS:
        case KEY_74comb_PRESS:
        case KEY_72comb_PRESS:
            return (key);
        default:
            return (KEYPAD_ERROR);
    }
}

/* if one key pressed and there is no intr reported in a fixed time, timeout_handler creat a holded key*/
static HI_VOID keyled_timeout_handler(unsigned long data)
{
    HI_U32 key = 0;

    //keyled_timeout_cnt++;

    if (keyled_timeout_cnt > 10)
    {
        keyled_timeout_cnt = 0;
        key = keyled_get_key();
        if (KEYPAD_ERROR == (key & 0xffffffff))
        {
			if(IsKeyPressed == HI_FALSE)
			{
				wake_up_interruptible(&wait_keyleds);
				return;
			}
            if (keyled_dev.enable_keyup)
            {
                KEYLED_BUF_HEAD = KEYLED_BUF_LAST;
                KEYLED_BUF_HEAD.keyled_state = KEY_RELEASE;
                keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);
                keyled_dev.key_come++;

                wake_up_interruptible(&wait_keyleds);
            }
        }
        else
        {
			mod_timer(&sleep_timer,jiffies + msecs_to_jiffies(keyled_dev.repkey_delaytime));

            KEYLED_BUF_HEAD = KEYLED_BUF_LAST;
            KEYLED_BUF_HEAD.keyled_state = KEY_HOLD;
            keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);
            keyled_dev.key_come++;

            HI_INFO_KEYLED("key hold:head=%d, tail=%d\n", keyled_dev.head, keyled_dev.tail);

            wake_up_interruptible(&wait_keyleds);
        }
    }
    else
    {
    	if(IsKeyPressed == HI_FALSE)
		{
			wake_up_interruptible(&wait_keyleds);
			return;
		}

	    mod_timer(&sleep_timer,jiffies + msecs_to_jiffies(keyled_dev.repkey_delaytime));

        KEYLED_BUF_HEAD = KEYLED_BUF_LAST;
        KEYLED_BUF_HEAD.keyled_state = KEY_HOLD;
        keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);
        keyled_dev.key_come++;

        HI_INFO_KEYLED("key hold:head=%d, tail=%d\n", keyled_dev.head, keyled_dev.tail);

        wake_up_interruptible(&wait_keyleds);
    }

    return;
}

static HI_VOID keyled_dotflash_handler(unsigned long data)
{
	mod_timer(&dotflash_timer,jiffies + msecs_to_jiffies(DOT_FLASH_TIME));

    if (dot_flag)
    {
        dot_flag = HI_FALSE;
    }
    else
    {
        dot_flag = HI_TRUE;
    }

    keyled_displaytime(s_keyled_time, dot_flag);

    return;
}

static HI_VOID keyled_interrupt_mask(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue &= ~0x3;
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

static HI_VOID keyled_interrupt_unmask(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue |= 0x3;
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}


/* process release intr and press intr */
static irqreturn_t keyled_interrupt_handler(int irq, HI_VOID *dev_id)
{
    HI_U32 regvalue = 0;
    HI_U32 key = 0;

    keyled_interrupt_mask();

    HI_REG_READ(keyled_base_addr + LEDC_KEYINT, regvalue);

    if (regvalue & KEYPAD_PRESS_INTR)
    {
        //HI_INFO_KEYLED("keyled press interrupt.\n");
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & KEYPAD_PRESS_INTR);

        key = keyled_get_key();

        if (KEYPAD_ERROR != key)
        {
            if (keyled_dev.enable_repkey)
            {
				mod_timer(&sleep_timer,jiffies + msecs_to_jiffies(keyled_dev.repkey_delaytime));
                keyled_timeout_cnt = 0;
            }

            KEYLED_BUF_HEAD.keyled_code  = key;
            KEYLED_BUF_HEAD.keyled_state = KEY_PRESS;
			IsKeyPressed = HI_TRUE;
            keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);
            keyled_dev.key_come++;

            HI_INFO_KEYLED("key press:head=%d, tail=%d, key=0x%x\n", keyled_dev.head, keyled_dev.tail, key);

            wake_up_interruptible(&wait_keyleds);
        }
    }
    else if (regvalue & KEYPAD_RELEASE_INTR)
    {
        //HI_INFO_KEYLED("keyled release interrupt.\n");
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & KEYPAD_RELEASE_INTR);

        HI_REG_READ(keyled_base_addr + LEDC_KEYDATA, key);
        if ((((~key)) & KEYLED_BUF_LAST.keyled_code) == KEYLED_BUF_LAST.keyled_code)
        {
			IsKeyPressed = HI_FALSE;
            if (keyled_dev.enable_keyup && (KEYLED_BUF_LAST.keyled_state != KEY_RELEASE))
            {
                KEYLED_BUF_HEAD = KEYLED_BUF_LAST;
                KEYLED_BUF_HEAD.keyled_state = KEY_RELEASE;
                keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);
                keyled_dev.key_come++;

                HI_INFO_KEYLED("key up:head=%d, tail=%d, key=0x%x\n", keyled_dev.head, keyled_dev.tail, key);

                wake_up_interruptible(&wait_keyleds);
            }
        }
    }
    else
    {
        //HI_ERR_KEYLED("keyled interrupt error\n");
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & (KEYPAD_RELEASE_INTR | KEYPAD_PRESS_INTR));

        IsKeyPressed = HI_FALSE;
    }

    keyled_interrupt_unmask();

    return IRQ_HANDLED;
}

static HI_VOID keyled_clear_keybuf_func(HI_VOID)
{
    /*AI7D02372:modified by w54542 start in 2008-03-18*/
    /*set tail to head is can also clear keyled buf and it will keep the value in BUF_LAST which will be usefull after wake up by power*/
    keyled_dev.tail = keyled_dev.head;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;

    /*when wake up from low power mode, ir clear buf must do more work,as follow*/
    del_timer_sync(&sleep_timer);
    keyled_timeout_cnt = 0;
    /*AI7D02372:modified by w54542 end in 2008-03-18*/

    return;
}

/* led display data */
static HI_VOID keyled_display(HI_U32 u32lowdata)
{
    HI_U32 datavalue = 0;
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    datavalue = u32lowdata & 0x00ff;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA1, datavalue);
    SuspendSaveVal.LedData[0] = datavalue;

    datavalue = (u32lowdata & 0x00ff00) >> 8;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA2, datavalue);
    SuspendSaveVal.LedData[1] = datavalue;

    datavalue = (u32lowdata & 0x00ff0000) >> 16;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA3, datavalue);
    SuspendSaveVal.LedData[2] = datavalue;

    datavalue = (u32lowdata & 0xff000000) >> 24;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA4, datavalue);
    SuspendSaveVal.LedData[3] = datavalue;

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    SuspendSaveVal.bDisNormalData = HI_TRUE;
    SuspendSaveVal.bDisTimeData = HI_FALSE;

    return;
}

/* led display time */
static HI_VOID keyled_displaytime(HI_UNF_KEYLED_TIME_S keyled_time, HI_BOOL dotflag)
{
    HI_U32 datavalue = 0;
    HI_U32 index = 0;
    HI_U32 regvalue = 0;

    HI_INFO_KEYLED("disp time:%d:%d\n", keyled_time.u32Hour, keyled_time.u32Minute);

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    index = keyled_time.u32Hour / 10;
    if (HI_FALSE == dotflag)
    {
        datavalue = LedDigDisCode_ct1642[index];
    }
    else
    {
        datavalue = LedDigDisDot_ct1642[index];
    }
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA1, datavalue);

    index = keyled_time.u32Hour % 10;
    if (HI_FALSE == dotflag)
    {
        datavalue = LedDigDisCode_ct1642[index];
    }
    else
    {
        datavalue = LedDigDisDot_ct1642[index];
    }
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA2, datavalue);

    index = keyled_time.u32Minute / 10;
    datavalue = LedDigDisCode_ct1642[index];
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA3, datavalue);

    index = keyled_time.u32Minute % 10;
    datavalue = LedDigDisCode_ct1642[index];
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA4, datavalue);

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    SuspendSaveVal.bDisNormalData = HI_FALSE;
    SuspendSaveVal.bDisTimeData = HI_TRUE;
    SuspendSaveVal.TimeData= keyled_time;
    SuspendSaveVal.dotflag = dotflag;

    return;
}


/* set led light pin  */
static HI_S32 keyled_set_flashpin(HI_UNF_KEYLED_LIGHT_E pin)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);

    /* add by z00149549 for DTS2012102401972 */
    regvalue &= ~FLASH_ENABLE_4;

    switch (pin)
    {
        case HI_UNF_KEYLED_LIGHT_1:
            regvalue |= LEDC_FLASH_1;
            break;
        case HI_UNF_KEYLED_LIGHT_2:
            regvalue |= LEDC_FLASH_2;
            break;
        case HI_UNF_KEYLED_LIGHT_3:
            regvalue |= LEDC_FLASH_3;
            break;
        case HI_UNF_KEYLED_LIGHT_4:
            regvalue |= LEDC_FLASH_4;
            break;
        case HI_UNF_KEYLED_LIGHT_ALL:
            regvalue |= FLASH_ENABLE_4;
            break;
        case HI_UNF_KEYLED_LIGHT_NONE:
            regvalue &= ~FLASH_ENABLE_4;
            break;
        default:
            return HI_FAILURE;
    }

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    keyled_dev.FlashPin = pin;

    return HI_SUCCESS;
}

/* set led flash level */
static HI_S32 keyled_set_flash(HI_UNF_KEYLED_LEVEL_E level)
{
    HI_U32 flashfreq = 0;

    switch (level)
    {
        case HI_UNF_KEYLED_LEVEL_1:
            flashfreq = FLASH_LEVEL1;
            break;
        case HI_UNF_KEYLED_LEVEL_2:
            flashfreq = FLASH_LEVEL2;
            break;
        case HI_UNF_KEYLED_LEVEL_3:
            flashfreq = FLASH_LEVEL3;
            break;
        case HI_UNF_KEYLED_LEVEL_4:
            flashfreq = FLASH_LEVEL4;
            break;
        case HI_UNF_KEYLED_LEVEL_5:
            flashfreq = FLASH_LEVEL5;
            break;
        default:
            return HI_FAILURE;
    }

    keyled_config_flashtim(flashfreq);
    keyled_dev.FlashLevel = level;

    return HI_SUCCESS;
}

/*********************************************************************************************/
/*Device Manage Layer Interface*/

HI_S32 KEYLED_Init_CT1642(HI_VOID)
{
    HI_S32 err = 0;

    if (HI_NULL_PTR != keyled_base_addr)
    {
        return HI_SUCCESS;
    }

    HI_INIT_MUTEX(&sem_keyled_std);

    keyled_dev.buf_len = KEYBUF_LEN;

    /* remap keyled register to memory */
	keyled_base_addr = (HI_U8 *)ioremap_nocache(KEYLED_PHY_ADDR, KEYLED_ADDR_SIZE);
	if (keyled_base_addr == HI_NULL)
	{
		HI_ERR_KEYLED("keyled ioremap_nocache err! \n");
		return -EFAULT ;
	}
    
    init_waitqueue_head(&wait_keyleds);

     /* set timer for keyled */
    sleep_timer.function = keyled_timeout_handler;
    init_timer(&sleep_timer);

    /* set timer for keyled */
    dotflash_timer.function = keyled_dotflash_handler;
    init_timer(&dotflash_timer);

    /* init keyled data structure and members */
    keyled_clear_keybuf_func();

     /* registe keyled irq handle */
    err = request_irq(KEYLED_IRQ, keyled_interrupt_handler, IRQF_DISABLED, "hi_keyled_ct1642_irq", NULL);
    if (0 != err)
    {
        HI_ERR_KEYLED("keyled request irq failed\n");
        return -EFAULT;
    }
	else
	{
		if(HI_SUCCESS != HI_DRV_SYS_SetIrqAffinity(HI_ID_KEYLED, KEYLED_IRQ))
		{
        	HI_ERR_KEYLED("HI_DRV_SYS_SetIrqAffinity failed.\n");
    	}	
	}

    /* disable keyled before open device */
    keyled_disable_key();

    /* clear LED DATA register */
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA1, 0x0);
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA2, 0x0);
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA3, 0x0);
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA4, 0x0);

    /*config system frequency divide to 0x6 */
    HI_REG_WRITE(keyled_base_addr + LEDC_SYSTIM, SYSTIM_DEFAULT);

    keyled_config_clktim(CLKTIM_DEFAULT);
    keyled_config_fretim(FRETIM_DEFAULT); // todo:This value may need to change
    keyled_config_keytim(KEY_DEFAULT);
    keyled_config_type(HI_KEYLED_CATHODE, KEY_TYPE_CT1642);


    keyled_dev.KeyLedMode.KeyScanMode = HI_KEYLED_1MUL8;
    keyled_dev.KeyLedMode.LedNum = HI_KEYLED_4LED;

#if 0
    if (HI_CHIP_TYPE_HI3716M == enChipType && HI_CHIP_VERSION_V300 == enChipVersion)
    {
        HI_INFO_KEYLED("KeyScanMode =%d LedNum = %d \n ",keyled_dev.KeyLedMode.KeyScanMode ,\
            keyled_dev.KeyLedMode.LedNum);

        ret = KEYLED_SetMode( keyled_dev.KeyLedMode);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_KEYLED("KEYLED_SetMode pram err\n");
        }
    }
#endif

    HI_INFO_KEYLED(" Inint ok\n");

    return HI_SUCCESS;
}

HI_S32 KEYLED_DeInit_CT1642(HI_VOID)
{
    if (HI_NULL_PTR == keyled_base_addr)
    {
        return HI_SUCCESS;
    }

    if ((IsKeyOpen > 0) || (IsLedOpen > 0))
    {
        return HI_SUCCESS;
    }

    /* release irq */
    free_irq(KEYLED_IRQ, NULL);
    keyled_base_addr = HI_NULL_PTR;

    HI_INFO_KEYLED(" Deint ok\n");

    return HI_SUCCESS;
}


HI_S32 KEYLED_KEY_Open_CT1642(HI_VOID)
{
    HI_U32 RegVal;

    HI_INFO_KEYLED(" start IsKeyOpen=%d\n", IsKeyOpen);
    KEYLED_Init_CT1642();

    /* avoid device reopen */
    if (IsKeyOpen > 0)
    {
        return HI_SUCCESS;
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    IsKeyOpen++;

    keyled_dev.head = 0;
    keyled_dev.tail = 0;
    keyled_dev.buf_len = KEYBUF_LEN;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;
    keyled_dev.enable_repkey = 1;
    keyled_dev.blocktime = DEFAULT_BLOCKTIME;
    keyled_dev.repkey_delaytime = DEFAULT_REP_KEY_TIME;
    keyled_dev.enable_keyup = 1;
    dot_flag = HI_FALSE;
    keyled_timeout_cnt = 0;

    /* clear keyled interrupt status */
    HI_REG_READ(keyled_base_addr + LEDC_KEYINT, RegVal);
    RegVal |= 0x3;
    HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, RegVal);
    keyled_enable_key();

    HI_INFO_KEYLED(" ok\n");
    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Close_CT1642(HI_VOID)
{
    HI_INFO_KEYLED(" start IsKeyOpen=%d\n", IsKeyOpen);

    if (IsKeyOpen == 0)
    {
        return HI_SUCCESS;
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    del_timer_sync(&sleep_timer);

    /* clear inttrupt status */
    HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, 0x3);
    keyled_disable_key();

    IsKeyOpen = 0;

    KEYLED_DeInit_CT1642();
    HI_INFO_KEYLED(" ok\n");
    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Reset_CT1642(HI_VOID)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_clear_keybuf_func();

    up(&sem_keyled_std);
    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_GetValue_CT1642(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    keyled_key_info_s keyled_to_user = {0};
    HI_S32 err = 0;

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    up(&sem_keyled_std);

   //HI_ERR_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n", keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);

    if ((keyled_dev.head) == (keyled_dev.tail))
    {
        if (0 == keyled_dev.blocktime)
        {
            return HI_ERR_KEYLED_NO_NEW_KEY;
        }

        err = wait_event_interruptible_timeout(wait_keyleds, (keyled_dev.head != keyled_dev.tail),
                                               (long)(keyled_dev.blocktime*HZ/1000));
        if (err <= 0)  /*not wake up by received key*/
        {
            //HI_ERR_KEYLED("wake up by timeout\n");
            return HI_ERR_KEYLED_TIMEOUT;
        }
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (keyled_dev.head != keyled_dev.tail)
    {
        keyled_to_user  = KEYLED_BUF_TAIL;
        keyled_dev.tail = KEYLED_INC_BUF(keyled_dev.tail, keyled_dev.buf_len);
        keyled_dev.key_read++;

        *pu32PressStatus = keyled_to_user.keyled_state;
        switch (keyled_to_user.keyled_code)
        {
            case KEY_7_PRESS:
                *pu32KeyId = 8;
                break;
            case KEY_6_PRESS:
                *pu32KeyId = 6;
                break;
            case KEY_5_PRESS:
                *pu32KeyId = 5;
                break;
            case KEY_4_PRESS:
                *pu32KeyId = 4;
                break;
            case KEY_3_PRESS:
                *pu32KeyId = 3;
                break;
            case KEY_2_PRESS:
                *pu32KeyId = 2;
                break;
            case KEY_1_PRESS:
                *pu32KeyId = 1;
                break;
            case KEY_0_PRESS:
                *pu32KeyId = 0;
                break;
            case KEY_76comb_PRESS:
                *pu32KeyId = 13;
                break;
            case KEY_74comb_PRESS:
                *pu32KeyId = 11;
                break;
            case KEY_72comb_PRESS:
                *pu32KeyId = 9;
                break;
            default:
                up(&sem_keyled_std);
                return (KEYPAD_ERROR);
        }

        HI_INFO_KEYLED("get a key:head=%d, tail=%d, key=0x%x, status=%d \n",
                    keyled_dev.head, keyled_dev.tail, *pu32KeyId, *pu32PressStatus);
        up(&sem_keyled_std);
        return HI_SUCCESS;
    }
    else
    {
       up(&sem_keyled_std);
       return HI_ERR_KEYLED_NO_NEW_KEY;
    }
}

HI_S32 KEYLED_KEY_SetBlockTime_CT1642(HI_U32 u32BlockTimeMs)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_dev.blocktime = u32BlockTimeMs;

    up(&sem_keyled_std);

    return HI_SUCCESS;
}


HI_S32 KEYLED_KEY_SetRepTime_CT1642(HI_U32 u32RepTimeMs)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_dev.repkey_delaytime = u32RepTimeMs;

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsRepKey_CT1642(HI_U32 u32IsRepKey)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_dev.enable_repkey = u32IsRepKey;

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsKeyUp_CT1642(HI_U32 u32IsKeyUp)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    if (IsKeyOpen == 0)
    {
        HI_ERR_KEYLED("Key not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_dev.enable_keyup = u32IsKeyUp;

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Open_CT1642(HI_VOID)
{
    HI_INFO_KEYLED(" start IsLedOpen=%d\n", IsLedOpen);

    KEYLED_Init_CT1642();

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    /* avoid device reopen */
    if (IsLedOpen > 0)
    {
        HI_ERR_KEYLED("reopen led, IsLedOpen=%d\n", IsLedOpen);
        up(&sem_keyled_std);
        return HI_SUCCESS;
    }
    IsLedOpen++;

    keyled_enable_ledc();
    HI_INFO_KEYLED(" ok\n");

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Close_CT1642(HI_VOID)
{
    HI_INFO_KEYLED(" start IsLedOpen=%d\n", IsLedOpen);

    if (IsLedOpen == 0)
    {
        return HI_SUCCESS;
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    del_timer_sync(&dotflash_timer);

    keyled_disable_ledc();
    keyled_set_flashpin(HI_UNF_KEYLED_LIGHT_NONE);

    IsLedOpen = 0;

    KEYLED_DeInit_CT1642();
    HI_INFO_KEYLED(" ok\n");

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Display_CT1642(HI_U32 u32CodeValue)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (IsLedOpen == 0)
    {
        HI_ERR_KEYLED("Led not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    del_timer_sync(&dotflash_timer);
    keyled_display(u32CodeValue);
    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_DisplayTime_CT1642(HI_UNF_KEYLED_TIME_S stKeyLedTime)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    if (IsLedOpen == 0)
    {
        HI_ERR_KEYLED("Led not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    del_timer_sync(&dotflash_timer);
    dotflash_timer.expires = jiffies + msecs_to_jiffies(DOT_FLASH_TIME);
    add_timer(&dotflash_timer);

    dot_flag = HI_TRUE;
    keyled_displaytime(stKeyLedTime, dot_flag);
    s_keyled_time = stKeyLedTime;

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashPin_CT1642(HI_UNF_KEYLED_LIGHT_E enPin)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    if (IsLedOpen == 0)
    {
        HI_ERR_KEYLED("Led not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }
    keyled_set_flashpin(enPin);

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashFreq_CT1642(HI_UNF_KEYLED_LEVEL_E enLevel)
{
    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    if (IsLedOpen == 0)
    {
        HI_ERR_KEYLED("Led not open\n");
        up(&sem_keyled_std);
        return HI_FAILURE;
    }

    keyled_set_flash(enLevel);
    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_GetProcInfo_CT1642(KEYLED_PROC_INFO_S *stInfo)
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

HI_S32 KEYLED_Suspend_CT1642(HI_VOID)
{
    if ( 0 == IsLedOpen && 0 == IsKeyOpen )
    {
        return HI_SUCCESS;
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }

    keyled_disable_key();
    keyled_disable_ledc();

    HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, 0x3);
//  wake_up_interruptible(&wait_keyleds);
    del_timer_sync(&sleep_timer);

    up(&sem_keyled_std);

    return HI_SUCCESS;
}

HI_S32 KEYLED_Resume_CT1642(HI_VOID)
{
    HI_U32 RegVal;

    if ( 0 == IsLedOpen && 0 == IsKeyOpen )
    {
        return HI_SUCCESS;
    }

    if (down_interruptible(&sem_keyled_std))
    {
        HI_FATAL_KEYLED("Semaphore lock is  error. \n");
        return HI_FAILURE;
    }
    /* disable keyled before open device */
    keyled_disable_key();
    keyled_config_clktim(CLKTIM_DEFAULT);
    keyled_config_fretim(FRETIM_DEFAULT);
    keyled_config_keytim(KEY_DEFAULT);
    keyled_config_type(HI_KEYLED_CATHODE, KEY_TYPE_CT1642);

    /* clear keyled interrupt status */
    HI_REG_READ(keyled_base_addr + LEDC_KEYINT, RegVal);
    RegVal |= 0x3;
    HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, RegVal);

    keyled_clear_keybuf_func();

    HI_INFO_KEYLED("keyled_dev.tail = %d  keyled_dev.head = %d \n",keyled_dev.tail, keyled_dev.head);

    keyled_set_flashpin(keyled_dev.FlashPin);
    keyled_set_flash(keyled_dev.FlashLevel);

    if (HI_TRUE == SuspendSaveVal.bDisTimeData)
    {
        keyled_displaytime(SuspendSaveVal.TimeData,SuspendSaveVal.dotflag);
    }
    else if (HI_TRUE == SuspendSaveVal.bDisNormalData)
    {
        keyled_display(SuspendSaveVal.LedData[0] | SuspendSaveVal.LedData[1] << 8
                | SuspendSaveVal.LedData[2] << 16 | SuspendSaveVal.LedData[3] << 24);
    }
    else
    {
        HI_ERR_KEYLED("Resume Display error. \n");
    }

    keyled_enable_key();
    keyled_enable_ledc();

    up(&sem_keyled_std);

    return HI_SUCCESS;
}


