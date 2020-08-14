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
//#include <linux/kcom.h>
#include <mach/hardware.h>
#include <asm/signal.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/poll.h>
#include "hi_type.h"
#include "hi_drv_struct.h"
#include "hi_drv_mem.h"
#include "drv_gpio_ext.h"
#include "hi_drv_module.h"

#include "hi_unf_keyled.h"
#include "hi_error_mpi.h"
#include "hi_drv_keyled.h"
#include "drv_keyled_ioctl.h"
#include "drv_keyled.h"
#include "drv_keyled_ct1642.h"
#ifdef CONFIG_HIGH_RES_TIMERS
#include <linux/hrtimer.h>
#endif

static  GPIO_EXT_FUNC_S *s_pGpioFunc = HI_NULL;

static atomic_t g_Ct1642Open = ATOMIC_INIT(0);

#define DEFAULT_REP_KEY_TIME   300
static  HI_U8  v_LedCode[LED_BIT_COUNT];

static keyled_dev_s keyled_dev;
static HI_UNF_KEYLED_TIME_S keyled_time = {0};
static wait_queue_head_t wait_keyleds;

static HI_U32 s_key_detected_flag = 0;
HI_BOOL g_bTimerRunFlag =  HI_FALSE;

static HI_U32 s_key_code_last = KEY_MACRO_NO;
static HI_U32 s_key_code_curr = KEY_MACRO_NO;
static HI_U32 s_bKeyPressFlag = 0;

static HI_U32 s_cycle_cnt = 0;
//static HI_U32 regDeInit[4] = {0};
//static HI_U32 LedDigDisCode_ct1642[] = {0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6};
static HI_U32 LedDigDisDot_ct1642[]  = {0xfd, 0x61, 0xdb, 0xf3, 0x67, 0xb7, 0xbf, 0xe1, 0xff, 0xf7};

static HI_VOID keyled_led_scan(HI_U8 v_position);                           /*LED scan manage func*/
static HI_VOID keyled_send_bit_data(HI_U8 v_character, HI_U8 v_position);   /*light 1 LED data func*/
static HI_VOID keyled_key_scan(HI_VOID);                                    /*KEY scan manage func*/

#define  GPIO_CLOCK_SET(val)    s_pGpioFunc->pfnGpioWriteBit(CLK_CT1642, val)
#define  GPIO_DATA_SET(val)     s_pGpioFunc->pfnGpioWriteBit(DAT_CT1642, val)
#define  GPIO_KEY_GET(val)      s_pGpioFunc->pfnGpioReadBit(KEY_CT1642, &val)
#define  KEYLED_TIMER_INTERVAL_MS 2

HI_VOID * pLed_timer = NULL;

/* BEGIN: Added by fzh, 2010/11/9*/
HI_VOID * HI_HIGH_RES_TimerInit(HI_VOID *pTimerProc)
{

#ifdef CONFIG_HIGH_RES_TIMERS       //defined in linux kernel
    struct hrtimer *pstApi_timer = NULL;
    pstApi_timer = (struct hrtimer *)HI_KMALLOC(HI_ID_KEYLED, sizeof(struct hrtimer),GFP_ATOMIC);
    if (NULL != pstApi_timer)
    {
        hrtimer_init(pstApi_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        pstApi_timer->function = pTimerProc;
        HI_INFO_KEYLED("hrtimer_init OK! \n");
        return (void *)pstApi_timer;
    }
#else
    struct timer_list *pstApi_timer = NULL;
    pstApi_timer = (struct timer_list *)HI_KMALLOC(HI_ID_KEYLED, sizeof(struct timer_list),GFP_ATOMIC);
    if (NULL != pstApi_timer)
    {
        init_timer(pstApi_timer);
        pstApi_timer->function = pTimerProc;
        pstApi_timer->data = 0;
        HI_INFO_KEYLED("init_timer OK! \n");
        return (void *)pstApi_timer;
    }
#endif

    return NULL;
}

HI_S32 HI_HIGH_RES_TimerStart(HI_VOID *handle,HI_BOOL bInit)
{
    if(handle == NULL)
    {
        HI_ERR_KEYLED("HI_HIGH_RES_TimerStart handle invalid \n");
        return HI_FAILURE;
    }

#ifdef CONFIG_HIGH_RES_TIMERS    //defined in linux kernel
    hrtimer_start((struct hrtimer *)handle,ktime_set(0,KEYLED_TIMER_INTERVAL_MS*1000000),HRTIMER_MODE_REL);
#else
    {
        struct timer_list *pstApi_timer = NULL;
        pstApi_timer = (struct timer_list *)handle;
        pstApi_timer->expires = (jiffies + msecs_to_jiffies(KEYLED_TIMER_INTERVAL_MS));
		if(bInit == HI_TRUE)
		{
        	add_timer(pstApi_timer);
		}
		else
		{
			mod_timer(pstApi_timer,pstApi_timer->expires);
		}
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_HIGH_RES_TimerUpdate(HI_VOID *handle)
{
    if(handle == NULL)
    {
        HI_ERR_KEYLED("HI_HIGH_RES_TimerUpdate handle invalid \n");
        return HI_FAILURE;
    }

#ifdef CONFIG_HIGH_RES_TIMERS    //defined in linux kernel
    hrtimer_forward((struct hrtimer *)handle, ktime_get(), ktime_set(0,KEYLED_TIMER_INTERVAL_MS*1000000));
#else
    {
        struct timer_list *pstApi_timer = NULL;
        pstApi_timer = (struct timer_list *)handle;
        pstApi_timer->expires = (jiffies + msecs_to_jiffies(KEYLED_TIMER_INTERVAL_MS));
        mod_timer(pstApi_timer,pstApi_timer->expires );
    }
#endif

    return HI_SUCCESS;
}

HI_S32 HI_HIGH_RES_TimerDestroy(HI_VOID *handle)
{
    if(handle == NULL)
    {
        HI_ERR_KEYLED("HI_HIGH_RES_TimerDestroy handle invalid \n");
        return HI_FAILURE;
    }

#ifdef CONFIG_HIGH_RES_TIMERS    //defined in linux kernel
    hrtimer_cancel((struct hrtimer *)handle);
#else
    del_timer_sync((struct timer_list *)handle);
#endif

    HI_KFREE(HI_ID_KEYLED, handle);

    return HI_SUCCESS;
}

/* END:   Added by fzh, 2010/11/9 */

HI_VOID LED_TimerProc(HI_VOID)
{
    static HI_U8 v_led_position = 0;

    if (HI_SUCCESS != HI_HIGH_RES_TimerUpdate(pLed_timer))
    {
        HI_ERR_KEYLED("Err in HI_HIGH_RES_TimerUpdate!\n");
        return;
    }

    if(v_led_position == 4)
    {
        keyled_key_scan();
        v_led_position = 0;
    }
    else
    {
        keyled_led_scan(v_led_position);
        v_led_position++;
    }
    if (HI_SUCCESS != HI_HIGH_RES_TimerStart(pLed_timer,HI_FALSE))
    {
        HI_ERR_KEYLED("Err in HI_HIGH_RES_TimerStart!\n");
        return;
    }

    return;
}

HI_S32 LED_InitTimer(HI_VOID)
{
    if (NULL != pLed_timer)
    {
        HI_ERR_KEYLED("LED_InitTimer pLed_timer is not NULL\n");
        return HI_FAILURE;
    }

    pLed_timer = HI_HIGH_RES_TimerInit(LED_TimerProc);

    if (HI_SUCCESS != HI_HIGH_RES_TimerStart(pLed_timer,HI_TRUE))
    {
        HI_ERR_KEYLED("HI_HIGH_RES_TimerStart start failure \n");
        return HI_FAILURE;
    }
    g_bTimerRunFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_VOID LED_DeleteTimer(HI_VOID)
{
    HI_HIGH_RES_TimerDestroy(pLed_timer);

    pLed_timer = NULL;
    g_bTimerRunFlag = HI_FALSE;

    HI_INFO_KEYLED("DeleteTimer g_bTimerRunFlag =  %d\n", g_bTimerRunFlag);

    return;
}

static HI_VOID keyled_send_bit_data(HI_U8 v_character, HI_U8 v_position)
{
    HI_U8 BitPosition ;                   /*LED Selected*/
    HI_U8 BitCharacter = v_character;     /*LED display Code*/
    HI_U8 i;

    switch(v_position)
    {
        /* display thousand LED,BitCharacter|=0x01 show light on */
        /* Power indicator light D1                              */
        case 0:
        {
            BitPosition = 0xef;
            BitCharacter |= 0x01;
            break;
        }

        /* display hundred LED          */
        /* 1--D2 light on,0 D2 lightoff */
        case 1:
        {
            BitPosition = 0xdf;
            break;
        }

        /* display decade LED */
        case 2:
        {
            BitPosition = 0xbf;
            break;
        }

        /* display last LED  */
        case 3:
        {
            BitPosition = 0x7f;
            break;
        }

        /* display off for key scan */
        case 4:
        {
            BitPosition = 0xff;
            break;
        }

        /* default no display  */
        default:
        {
            BitPosition = 0xff;
            BitCharacter = 0x00;
        }
    }

    for (i = 0; i < 8; i++)          /* send 8bit addr*/
    {
        GPIO_CLOCK_SET(HI_FALSE);
        if ((BitPosition << i) & 0x80)
        {
            GPIO_DATA_SET(HI_TRUE);
        }
        else
        {
            GPIO_DATA_SET(HI_FALSE);
        }
        GPIO_CLOCK_SET(HI_TRUE);
    }

    GPIO_DATA_SET(HI_FALSE);        /* send two bit nop*/
    GPIO_CLOCK_SET(HI_FALSE);
    GPIO_CLOCK_SET(HI_TRUE);

    GPIO_DATA_SET(HI_FALSE);
    GPIO_CLOCK_SET(HI_FALSE);
    GPIO_CLOCK_SET(HI_TRUE);

    for (i = 0; i < 8; i++)          /* send 8bit addr*/
    {
        GPIO_CLOCK_SET(HI_FALSE);
        if ((BitCharacter << i) & 0x80)
        {
            GPIO_DATA_SET(HI_TRUE);
        }
        else
        {
            GPIO_DATA_SET(HI_FALSE);
        }
        GPIO_CLOCK_SET(HI_TRUE);
    }

    GPIO_DATA_SET(HI_FALSE);         /* Output flip-latch Data*/
    GPIO_DATA_SET(HI_TRUE);

    GPIO_CLOCK_SET(HI_FALSE);
    GPIO_DATA_SET(HI_FALSE);
    GPIO_DATA_SET(HI_TRUE);
    GPIO_CLOCK_SET(HI_TRUE);

    return;
}

HI_S32 keyled_ct1642_get_keycode(HI_VOID)
{
    HI_U8 i;
    HI_S32 s32flag = 0;
    HI_S32 nKeyPress = 0;
    HI_S32 s32key_code_sum = 0;
    HI_S32 const v_KeyCode[9] =
    {
        11, /*Key connect with pin Q9*/
        12, /*Key connect with pin Q8*/
        13, /*Key connect with pin Q7*/
        14, /*Key connect with pin Q6*/
        15, /*Key connect with pin Q5*/
        16, /*Key connect with pin Q4*/
        17, /*Key connect with pin Q3*/
        18, /*Key connect with pin Q2*/
        STB_KEY_NULL /*Key that not exist*/
    };


    for (i = 0; i < 8; i++) /*query which key*/
    {
        keyled_send_bit_data(1 << i, 0x04);
        udelay(5);
        GPIO_KEY_GET(nKeyPress);

        if(1 == nKeyPress)
        {
           s32flag++;
           s32key_code_sum += v_KeyCode[i];
        }
    }

    if (s32flag > 0)
    {
        return s32key_code_sum;
    }
    else /*when none*/
    {
        return HI_FAILURE;
    }
}

HI_S32 keyled_ct1642_handle_keycode(HI_VOID)
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
                if(keyled_dev.enable_keyup)
                {
                    KEYLED_BUF_HEAD.keyled_code = s_key_code_last;
                    KEYLED_BUF_HEAD.keyled_state = KEY_RELEASE;
                    keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                    wake_up_interruptible(&wait_keyleds);
                }

                s_bKeyPressFlag =HI_FALSE;
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
                        KEYLED_BUF_HEAD.keyled_state= KEY_RELEASE;
                        KEYLED_BUF_HEAD.keyled_code = s_key_code_last;
                        keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                        wake_up_interruptible(&wait_keyleds);
                    }
                    s_bKeyPressFlag =HI_FALSE;
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
            s_cycle_cnt ++;
            if ((1 == s_cycle_cnt) && (HI_TRUE != s_bKeyPressFlag))
            {
                KEYLED_BUF_HEAD.keyled_state = KEY_PRESS;
                KEYLED_BUF_HEAD.keyled_code = s_key_code_last;
                keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                wake_up_interruptible(&wait_keyleds);
                s_cycle_cnt = 0;
                s_bKeyPressFlag = HI_TRUE;
            }

            if ((s_cycle_cnt >= keyled_dev.repkey_delaytime * 8 / (SCAN_INTV_CT1642))
                        && (s_bKeyPressFlag))
            {
                if (keyled_dev.enable_repkey)
                {
                    KEYLED_BUF_HEAD.keyled_state = KEY_HOLD;
                    KEYLED_BUF_HEAD.keyled_code = s_key_code_last;
                    keyled_dev.head = KEYLED_INC_BUF(keyled_dev.head, keyled_dev.buf_len);

                    wake_up_interruptible(&wait_keyleds);
                }

            s_cycle_cnt = 0;
            s_bKeyPressFlag =HI_TRUE;
           }
        }
    }

    s_key_code_curr = KEY_MACRO_NO;
    s_key_detected_flag = 0;

    return HI_FAILURE;
}

static HI_VOID keyled_key_scan(HI_VOID)
{
    HI_S32 s32keycode = 0;

    s32keycode = keyled_ct1642_get_keycode();

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

    keyled_ct1642_handle_keycode();

}

static HI_VOID keyled_led_scan(HI_U8 v_position)
{
    keyled_send_bit_data(v_LedCode[v_position],v_position);

    return;
}

static HI_VOID keyled_led_display_off(HI_VOID)
{
    keyled_send_bit_data(0x00, 0x04);

    return;
}

HI_S32 keyled_ct1642_gpio_dirset(HI_VOID)
{
    s_pGpioFunc->pfnGpioDirSetBit(CLK_CT1642, 0);  //output
    s_pGpioFunc->pfnGpioDirSetBit(DAT_CT1642, 0);  //output
    s_pGpioFunc->pfnGpioDirSetBit(KEY_CT1642, 1);  //input

    return 0;
}

HI_VOID keyled_ct1642_clear_keybuf_func(HI_VOID)
{
    keyled_dev.tail = keyled_dev.head;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;

    return;
}

#if 0
HI_VOID pin_ct1642_config(HI_VOID)
{
    HI_REG_READ(IO_ADDRESS(0x10203000 + 0x74), regDeInit[0]);
    HI_REG_READ(IO_ADDRESS(0x10203000 + 0x88), regDeInit[1]);

    HI_REG_WRITE(IO_ADDRESS(0x10203000 + 0x74), 0x00);
    HI_REG_WRITE(IO_ADDRESS(0x10203000 + 0x88), 0x0);
    return;
}

HI_VOID CT1642_MV300PinConfig(HI_VOID)
{

    return;
}
#endif

HI_S32 KEYLED_Init_CT1642(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_GPIO, (HI_VOID**)&s_pGpioFunc);
    if (HI_SUCCESS != s32Ret || s_pGpioFunc == HI_NULL)
    {
        HI_ERR_KEYLED("Get GPIO function failed! \n");
        return HI_FAILURE;
    }

    init_waitqueue_head(&wait_keyleds);

    keyled_dev.head = 0;
    keyled_dev.tail = 0;
    keyled_dev.buf_len = KEYBUF_LEN;
    keyled_dev.enable_repkey = 1;
    keyled_dev.enable_keyup = 1;
    keyled_dev.key_come = 0;
    keyled_dev.key_read = 0;
    keyled_dev.blocktime = DEFAULT_BLOCKTIME;
    keyled_dev.repkey_delaytime = DEFAULT_REP_KEY_TIME;

    keyled_ct1642_gpio_dirset();
    keyled_ct1642_clear_keybuf_func();

    if (HI_FALSE == g_bTimerRunFlag)
    {
        LED_InitTimer();
        HI_INFO_KEYLED("KEYLED_Init_CT1642  do  LED_InitTimer\n");
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_DeInit_CT1642(HI_VOID)
{

    LED_DeleteTimer();
    keyled_led_display_off();

    //HI_REG_WRITE(IO_ADDRESS(0x10203000 + 0x74), regDeInit[0]);
    //HI_REG_WRITE(IO_ADDRESS(0x10203000 + 0x88), regDeInit[1]);

    return HI_SUCCESS;
}

/* led display data */
HI_S32 KEYLED_LED_Display_CT1642(HI_U32 u32Value)
{
    HI_U8 i;

    for (i = 0; i < LED_BIT_COUNT; i++)
    {
#if !defined(CHIP_TYPE_hi3716c) && !defined(CHIP_TYPE_hi3716h) && !defined(CHIP_TYPE_hi3716m)
        v_LedCode[LED_BIT_COUNT-1-i] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
#else
        v_LedCode[i] = ((HI_U32)u32Value >> (8 * i)) & 0xff;
#endif
    
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Open_CT1642(HI_VOID)
{
    HI_INFO_KEYLED("KEYLED_KEY_Open_CT1642 --- g_Ct1642Open = %d \n",g_Ct1642Open);

    if (1 == atomic_inc_return(&g_Ct1642Open))
    {
        KEYLED_Init_CT1642();
        HI_INFO_KEYLED("KEYLED ct1642 init \n");
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Close_CT1642(HI_VOID)
{
    HI_INFO_KEYLED("KEYLED_KEY_Close_CT1642 --- g_Ct1642Open = %d \n",g_Ct1642Open);

    if (atomic_dec_and_test(&g_Ct1642Open))
    {
        HI_INFO_KEYLED("KEYLED ct1642 deinit \n");
        KEYLED_DeInit_CT1642();
    }

    if (atomic_read(&g_Ct1642Open) < 0)
    {
        atomic_set(&g_Ct1642Open,0);
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_Reset_CT1642(HI_VOID)
{
    keyled_ct1642_clear_keybuf_func();

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_GetValue_CT1642(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    keyled_key_info_s keyled_to_user = {0};
    HI_S32 err = 0;

    HI_INFO_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n",
                        keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);

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

    HI_INFO_KEYLED("enter hi_keyled_read : head=%d, tail=%d, buf_len=%d\n",
                        keyled_dev.head, keyled_dev.tail, keyled_dev.buf_len);

    if (keyled_dev.head != keyled_dev.tail)
    {
        keyled_to_user  = KEYLED_BUF_TAIL;
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

HI_S32 KEYLED_KEY_IsRepKey_CT1642(HI_U32 u32IsRepKey)
{
    keyled_dev.enable_repkey = u32IsRepKey;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_IsKeyUp_CT1642(HI_U32 u32IsKeyUp)
{
    keyled_dev.enable_keyup = u32IsKeyUp;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_SetBlockTime_CT1642(HI_U32 u32BlockTimeMs)
{
    keyled_dev.blocktime = u32BlockTimeMs;

    return HI_SUCCESS;
}

HI_S32 KEYLED_KEY_SetRepTime_CT1642(HI_U32 u32RepTimeMs)
{
    keyled_dev.repkey_delaytime = u32RepTimeMs;

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Open_CT1642(HI_VOID)
{
    HI_INFO_KEYLED("KEYLED_LED_Open_CT1642 --- g_Ct1642Open = %d \n",g_Ct1642Open);

    if (1 == atomic_inc_return(&g_Ct1642Open))
    {
        KEYLED_Init_CT1642();
        HI_INFO_KEYLED("KEYLED ct1642 init \n");
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_Close_CT1642(HI_VOID)
{
    HI_INFO_KEYLED("KEYLED_LED_Close_CT1642 --- g_Ct1642Open = %d \n",g_Ct1642Open);

    if (atomic_dec_and_test(&g_Ct1642Open))
    {
        HI_INFO_KEYLED("KEYLED ct1642 deinit \n");
        KEYLED_DeInit_CT1642();
    }

    if (atomic_read(&g_Ct1642Open) < 0)
    {
        atomic_set(&g_Ct1642Open,0);
    }

    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashPin_CT1642(HI_UNF_KEYLED_LIGHT_E enPin)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_SetFlashFreq_CT1642(HI_UNF_KEYLED_LEVEL_E enLevel)
{
    return HI_SUCCESS;
}

HI_S32 KEYLED_LED_DisplayTime_CT1642(HI_UNF_KEYLED_TIME_S stKeyLedTime)
{
    int index = 0;
    keyled_time = stKeyLedTime;

#if !defined(CHIP_TYPE_hi3716c) && !defined(CHIP_TYPE_hi3716h) && !defined(CHIP_TYPE_hi3716m)
    index = keyled_time.u32Hour / 10;
    v_LedCode[3] = LedDigDisDot_ct1642[index];
    index = keyled_time.u32Hour % 10;
    v_LedCode[2] = LedDigDisDot_ct1642[index];

    index = keyled_time.u32Minute / 10;
    v_LedCode[1] = LedDigDisDot_ct1642[index];
    index = keyled_time.u32Minute % 10;
    v_LedCode[0] = LedDigDisDot_ct1642[index];
#else
    index = keyled_time.u32Hour / 10;
    v_LedCode[0] = LedDigDisDot_ct1642[index];
    index = keyled_time.u32Hour % 10;
    v_LedCode[1] = LedDigDisDot_ct1642[index];

    index = keyled_time.u32Minute / 10;
    v_LedCode[2] = LedDigDisDot_ct1642[index];
    index = keyled_time.u32Minute % 10;
    v_LedCode[3] = LedDigDisDot_ct1642[index];
#endif

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
    keyled_ct1642_clear_keybuf_func();

    return HI_SUCCESS;
}

 HI_S32 KEYLED_Resume_CT1642(HI_VOID)
{
    /* del by z00149549 for the settings of GPIO5 do not lose when power up */
    //keyled_ct1642_gpio_dirset();

    return HI_SUCCESS;
}

#ifndef MODULE

EXPORT_SYMBOL(KEYLED_KEY_Open_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_Close_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_Reset_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_GetValue_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_SetBlockTime_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_SetRepTime_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_IsRepKey_CT1642);
EXPORT_SYMBOL(KEYLED_KEY_IsKeyUp_CT1642);

EXPORT_SYMBOL(KEYLED_LED_Open_CT1642);
EXPORT_SYMBOL(KEYLED_LED_Close_CT1642);
EXPORT_SYMBOL(KEYLED_LED_Display_CT1642);
EXPORT_SYMBOL(KEYLED_LED_DisplayTime_CT1642);
EXPORT_SYMBOL(KEYLED_LED_SetFlashPin_CT1642);
EXPORT_SYMBOL(KEYLED_LED_SetFlashFreq_CT1642);
EXPORT_SYMBOL(KEYLED_Suspend_CT1642);
EXPORT_SYMBOL(KEYLED_Resume_CT1642);
#endif

