/*************************************************************************
 * Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keyled.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 *
 ***************************************************************************/
#include <uboot.h>
#include "hi_keyled_ct1642.h"
#include "hi_reg.h"
#include "hi_unf_keyled.h"
#include "hi_drv_gpio.h"

/*-------------------------------------------------------------------------------
 * macro define
 *------------------------------------------------------------------------------*/
#define DEFAULT_DELAYTIME 300

#define keyled_mdelay(x) udelay(x * 1000)

#define  GPIO_CLOCK_SET(val) HI_DRV_GPIO_WriteBit(CLK_CT1642, val)
#define  GPIO_DATA_SET(val) HI_DRV_GPIO_WriteBit(DAT_CT1642, val)
#define  GPIO_KEY_GET(val) HI_DRV_GPIO_ReadBit(KEY_CT1642, &val)

#define  KEYLED_TIMER_INTERVAL_MS 2

/*-------------------------------------------------------------------------------
 * variable define
 *------------------------------------------------------------------------------*/

//static int s_key_detected_flag = 0;
static int s_key_code_curr = KEY_MACRO_NO;

//static unsigned int g_u32GpioDir  = (0x101e4000 + 0x400);
//static unsigned int	g_u32Random =0 ;
//HI_U32 display_mode_index  = 0;

typedef struct hiKEY_INFO_S
{
    HI_U32 KeyVal;
    HI_U32 KeyPressState;
}HI_KEY_INFO_S;

static HI_KEY_INFO_S g_OldKey;

static HI_U8 v_LedCode[LED_BIT_COUNT];

#if 0
HI_VOID hi_gpio_dirset_bit(HI_U32 gpio5, HI_U32 idx, HI_U32 val)
{
    HI_U32 regData;

    HI_REG_READ(GPIO5_REG_DIR, regData);
    if (val)
    {
        regData &= ~(1 << idx);     // input
    }
    else
    {
        regData |= (1 << idx);      // output
    }

    HI_REG_WRITE(GPIO5_REG_DIR, regData);

    return;
}

HI_VOID hi_gpio_write_bit(HI_U32 gpio5, HI_U32 idx, HI_U32 val)
{
    HI_U32 regData;

    HI_REG_READ(GPIO5_REG_DIR, regData);
    if ((regData & (1 << idx)) == 0)
    {
        return;
    }

    if (val)
    {
        regData = (1 << idx);
    }
    else
    {
        regData = 0;
    }

    HI_REG_WRITE(GPIO5_BASE_ADDR + (0x4 << idx), regData);

    return;
}

HI_VOID hi_gpio_read_bit(HI_U32 gpio5, HI_U32 idx, HI_U32 * val)
{
    HI_U32 regData;

    HI_REG_READ(GPIO5_REG_DIR, regData);

    if (regData & (1 << idx))
    {
        return;
    }

    HI_REG_READ(GPIO5_BASE_ADDR + (0x4 << idx), regData);

    if (regData & (1 << idx))
    {
        *val = 1;
    }
    else
    {
        *val = 0;
    }

    return;
}
#endif

HI_VOID keyled_gpio_dirset(HI_VOID)
{
    HI_DRV_GPIO_SetDirBit(CLK_CT1642, 0);  //output
    HI_DRV_GPIO_SetDirBit(DAT_CT1642, 0);  //output
    HI_DRV_GPIO_SetDirBit(KEY_CT1642, 1);  //input
}

#if 0
HI_S32 keyled_gpio_switch(HI_U32 mode)
{
    if (mode)
    {
        //switch to gpio
        HI_REG_READ((0x10203000 + 0x74), regDeInit[0]);
        HI_REG_READ((0x10203000 + 0x88), regDeInit[3]);
        HI_REG_WRITE((0x10203000 + 0x74), 0x00);
        HI_REG_WRITE((0x10203000 + 0x88), 0x00);
    }
    else
    {
        //switch to led
        HI_REG_WRITE((0x10203000 + 0x74), regDeInit[0]);
        HI_REG_WRITE((0x10203000 + 0x88), regDeInit[3]);
    }

    return 0;
}
#endif

static HI_VOID keyled_send_bit_data(HI_U8 v_character, HI_U8 v_position)
{
    HI_U8 BitPosition;                    /*LED Selected*/
    HI_U8 BitCharacter = v_character;     /*LED display Code*/
    HI_U8 i;

    switch (v_position)
    {
    case 0:
    {
        BitPosition	  = 0xef;
        BitCharacter |= 0x01;
        break;
    }                                                                    /*display thousand LED,BitCharacter|=0x01 show light on */
    /*Power indicator light D1 */
    case 1:
    {
        BitPosition = 0xdf;
        break;
    }                                                                    /*display hundred LED */
    /*1--D2 light on,0--D2 light off　*/
    case 2:
    {
        BitPosition = 0xbf;
        break;
    }                                                                    /*display decade LED  */
    case 3:
    {
        BitPosition = 0x7f;
        break;
    }                                                                    /*display last LED  */
    case 4:
    {
        BitPosition = 0xff;
        break;
    }                                                                    /*display off for key scan */
    default:
    {
        BitPosition	 = 0xff;
        BitCharacter = 0x00;
    }                                                                    /*default no display  */
    }

    for (i = 0; i < 8; i++)                /* send 8bit addr*/
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

    for (i = 0; i < 8; i++)                /* send 8bit addr*/
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

HI_S32 keyled_get_keycode(HI_VOID)
{
    HI_U8 i;

    HI_S32 s32flag = 0;
    HI_U32 nKeyPress = 0;
    HI_S32 s32key_code_sum = 0;

    HI_U32 const v_KeyCode[9] =
    {
        /*Key connect with pin Q9*/
        STB_KEY_MENU,

        /*Key connect with pin Q8*/ STB_KEY_OK,

        /*Key connect with pin Q7*/ STB_KEY_UP,

        /*Key connect with pin Q6*/ STB_KEY_DOWN,

        /*Key connect with pin Q5*/ STB_KEY_LEFT,

        /*Key connect with pin Q4*/ STB_KEY_RIGHT,

        /*Key connect with pin Q3*/ STB_KEY_EXIT,

        /*Key connect with pin Q2*/ STB_KEY_BUTT,

        /*Key that not exist     */ STB_KEY_NULL
    };

    for (i = 0; i < 8; i++)                             /*query  key*/
    {
        keyled_send_bit_data(1 << i, 0x04);
        udelay(5);
        GPIO_KEY_GET(nKeyPress);
        if (1 == nKeyPress)
        {
            s32flag++;
            s32key_code_sum += v_KeyCode[i];
        }
    }

    if (s32flag > 0)
    {
        switch (s32key_code_sum)
        {
        case STB_KEY_MENU:
            return (3);
        case STB_KEY_OK:
            return (5);
        case STB_KEY_UP:
            return (1);
        case STB_KEY_DOWN:
            return (2);
        case STB_KEY_LEFT:
            return (4);
        case STB_KEY_RIGHT:
            return (6);
        case STB_KEY_EXIT:
            return (7);
        case STB_KEY_BUTT:
            return (8);
        case STB_KEY_UPGRADE:
            return (11);
        default:
            return (STB_KEY_NULL);
        }
    }
    else                                             /*when none*/
    {
        return STB_KEY_NULL;
    }
}

static int  keyled_scan_key(HI_VOID)
{
    HI_S32 s32keycode = 0;

    s32keycode = keyled_get_keycode();

    if (STB_KEY_NULL == s32keycode)
    {
        //s_key_detected_flag = 0;
        s_key_code_curr = KEY_MACRO_NO;
    }
    else
    {
        //s_key_detected_flag = 1;
        s_key_code_curr = s32keycode;
    }

    return HI_SUCCESS;
}

static int  keyled_led_scan(HI_U8 v_position)
{
    keyled_send_bit_data(v_LedCode[v_position], v_position);
    return HI_SUCCESS;
}

HI_VOID keyled_clear_buf_func(HI_VOID)
{
    s_key_code_curr = KEY_MACRO_NO;

    return;
}

HI_S32 HI_KEYLED_Open(HI_VOID)
{
    CheckKeyledDeviceRepeatFd();

    g_s32KEYLEDFd = 1;

    s_key_code_curr = KEY_MACRO_NO;
    g_OldKey.KeyVal = KEY_MACRO_NO;
    g_OldKey.KeyPressState = KEY_RELEASE;

    //	keyled_gpio_switch(1);   /* mask pin function config,it need congfig in boot reg*/

    keyled_gpio_dirset();

    keyled_clear_buf_func();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Close(HI_VOID)
{
    CheckKeyledDeviceFd();

    g_s32KEYLEDFd = 0;

    //switch to gpio
    //	keyled_gpio_switch(0);  /* mask pin function config,it need congfig in boot reg*/

    keyled_clear_buf_func();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Enable(HI_VOID)
{
    CheckKeyledDeviceFd();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Disable(HI_VOID)
{
    CheckKeyledDeviceFd();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    CheckKeyledDeviceFd();
    CheckKeyledNULLPointer(pu32PressStatus);
    CheckKeyledNULLPointer(pu32KeyId);

    keyled_scan_key();

    if (KEY_MACRO_NO == s_key_code_curr)
    {
        if (KEY_MACRO_NO != g_OldKey.KeyVal)
        {
            if (KEY_PRESS == g_OldKey.KeyPressState)
            {
                g_OldKey.KeyPressState = KEY_RELEASE;

                *pu32KeyId = g_OldKey.KeyVal;
                *pu32PressStatus = g_OldKey.KeyPressState;
                return HI_SUCCESS;
            }
            else
            {
                g_OldKey.KeyVal = KEY_MACRO_NO;
                g_OldKey.KeyPressState = KEY_RELEASE;
                return HI_FAILURE;
            }
        }

        return HI_FAILURE;
    }
    else
    {
        if ((g_OldKey.KeyVal == s_key_code_curr) && (KEY_PRESS == g_OldKey.KeyPressState))
        {
            return HI_FAILURE;
        }

        g_OldKey.KeyVal = s_key_code_curr;
        g_OldKey.KeyPressState = KEY_PRESS;

        *pu32KeyId = s_key_code_curr;
        *pu32PressStatus = KEY_PRESS;

        return HI_SUCCESS;
    }
}

HI_S32 HI_KEYLED_Display(HI_U32 u32Value)
{
    HI_S32 v_position;
    HI_S32 i = 0;

    CheckKeyledDeviceFd();

    for (v_position = 0; v_position < LED_BIT_COUNT; v_position++)
    {
        v_LedCode[v_position] = ((HI_U32)u32Value >> (8 * v_position)) & 0xff;
    }

    while (i++ < 5)
    {
        for (v_position = 0; v_position < LED_BIT_COUNT; v_position++)
        {
            keyled_led_scan(v_position);
            keyled_mdelay(1);
        }
    }

    keyled_led_scan(5);  /* >5, display off  */

    return HI_SUCCESS;
}

#if 0

/*     --7--
 *    |     |
 *   2|     |6   。
 *    |--1--    0
 *    |     |      。
 *   3|     |5
 *     --4--
 */

HI_U8 sNumTbl[] = {
    0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6
};                                                                           //ok

/*only 9 of the capital letters are effective ('C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U')*/
/*CNcomment:大写字母只显示9个有效('C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U')*/
HI_U8 sChrTblUp[] = {
    0xee, 0xfe, 0x9c, 0xfc, 0x9e, 0x8e, 0x00,
    0x6e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00,
    0xfc, 0xce, 0x00, 0x00, 0xb6, 0x00, 0x7c,
    0x00, 0x00, 0x00, 0x00, 0x00
};

/*only 15 of the smalll letters are effective (b c d E, g h i, l n o, P q S(5) t, u)*/
/*CNcomment:小写字母只显示15个有效(b c d E, g h i, l n o, P q S(5) t, u) */
HI_U8 sChrTblLow[] = {
    0x00, 0x3e, 0x1a, 0x7a, 0xde, 0x00, 0xf6,
    0x2e, 0x20, 0x00, 0x00, 0x60, 0x00, 0x2a,
    0x3a, 0xce, 0xe6, 0x00, 0xb6, 0x1e, 0x38,
    0x00, 0x00, 0x00, 0x00, 0x00
};

HI_S32 HI_KEYLED_DisplayString(HI_CHAR *fpString)
{
    unsigned int tmpData = 0;
    unsigned char DispCode[5] = {
        0
    };
    unsigned char j;

    if (NULL == fpString)
    {
        return -1;
    }

    memcpy(DispCode, fpString, 4);

    for (j = 0; j < 4; j++)
    {
        if ((DispCode[j] >= 'A') && (DispCode[j] <= 'Z'))
        {
            DispCode[j] = sChrTblUp[DispCode[j] - 'A'];
        }
        else if ((DispCode[j] >= 'a') && (DispCode[j] <= 'z'))
        {
            DispCode[j] = sChrTblLow[DispCode[j] - 'a'];
        }
        else if ((DispCode[j] >= '0') && (DispCode[j] <= '9'))
        {
            DispCode[j] = sNumTbl[DispCode[j] - '0'];
        }
        else if (DispCode[j] == '-')
        {
            DispCode[j] = 0x02;
        }
        else if (DispCode[j] == '_')
        {
            DispCode[j] = 0x10;
        }
        else if ((DispCode[j] == ':') || (DispCode[j] == '.'))
        {
            DispCode[j] = 0x01;
        }
        else  /* FIXME: may be should add symbol ':' and '.' */
        {
            DispCode[j] = 0x0;  /* default: not display */
        }
    }

    tmpData = (DispCode[3] << 24) | (DispCode[2] << 16) | (DispCode[1] << 8) | (DispCode[0]);

    HI_KEYLED_Display(tmpData);
    return 0;
}
#endif

/* add  the unf interface for new boot */
HI_S32 HI_UNF_KEYLED_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEYLED_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEY_Open(HI_VOID)
{
    return HI_KEYLED_Open();
}

HI_S32 HI_UNF_KEY_Close(HI_VOID)
{
    return HI_KEYLED_Close();
}

HI_S32 HI_UNF_LED_Open(HI_VOID)
{
    return HI_KEYLED_Open();
}

HI_S32 HI_UNF_LED_Close(HI_VOID)
{
    return HI_KEYLED_Close();
}

HI_S32 HI_UNF_KEY_IsKeyUp(HI_U32 u32IsKeyUp)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEY_IsRepKey(HI_U32 u32IsRepKey)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_KEY_SetBlockTime(HI_U32 u32BlockTimeMs)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_LED_Display(HI_U32 u32CodeValue)
{
    return HI_KEYLED_Display(u32CodeValue);
}

HI_S32 HI_UNF_KEY_GetValue(HI_U32 * pu32PressStatus, HI_U32 * pu32KeyId)
{
    return HI_KEYLED_GetValue(pu32PressStatus, pu32KeyId);
}

HI_S32 HI_UNF_KEYLED_SelectType(HI_UNF_KEYLED_TYPE_E enKeyLedType)
{
    return HI_SUCCESS;
}

