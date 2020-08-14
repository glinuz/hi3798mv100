/*************************************************************************
 * Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_keyled.c
 * Description:
 *
 * History:
 * Version   Date             Author       DefectNum    Description
 ***************************************************************************/
#include <uboot.h>
#include "hi_keyled_pt6961.h"
#include "hi_reg.h"
#include "hi_unf_keyled.h"
#include "hi_drv_gpio.h"

/*-------------------------------------------------------------------------------
 * macro define
 *------------------------------------------------------------------------------*/

#define CMD_READ (0x42)
#define CMD_INC_WRITE (0x40)
#define CMD_FIX_WRITE (0x44)
#define CMD_DIP_ON (0x8f)
#define CMD_DIP_OFF (0x80)

typedef struct hiKEY_INFO_S
{
    HI_U32 KeyVal;
    HI_U32 KeyPressState;
}HI_KEY_INFO_S;

static HI_KEY_INFO_S g_OldKey;

#define keyled_udelay(x) udelay(x)

/*-------------------------------------------------------------------------------
 * variable define
 *------------------------------------------------------------------------------*/
static int s_key_code_curr = KEY_MACRO_NO;

/*
    array define
*/
HI_U8 disp_buf_arr[DISPLAY_REG_NUM] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00
};

HI_U8 key_buf_arr[KEY_COL_NUM / 2] = {
    0x00, 0x00, 0x00, 0x00, 0x00
};

const HI_U8 display_mode_array[DISPLAY_MODE_NUM] = {
    0, 1, 2, 3
};

#define  GPIO_CLOCK_SET(val) HI_DRV_GPIO_WriteBit(CLK_PT6961, val)
#define  GPIO_STB_SET(val) HI_DRV_GPIO_WriteBit(STB_PT6961, val)

HI_VOID keyled_gpio_dirset(HI_VOID)
{
    HI_DRV_GPIO_SetDirBit(STB_PT6961, 0);

    HI_DRV_GPIO_SetDirBit(CLK_PT6961, 0);
    HI_DRV_GPIO_SetDirBit(DIN_PT6961, 0);
    HI_DRV_GPIO_SetDirBit(DOUT_PT6961, 1);
}

HI_S32 keyled_tx_byte(HI_U8 u8data)
{
    HI_U8 bitval = 0;
    int j = 0;

    for (j = 0; j < 8; j++)
    {
        GPIO_CLOCK_SET(0);

        //keyled_udelay(1);

        //LSB send first, MSB send last
        bitval = (u8data >> j) & 0x1;
        HI_DRV_GPIO_WriteBit(DIN_PT6961, bitval);

        //keyled_udelay(2);

        GPIO_CLOCK_SET(1);

        //keyled_udelay(1);
    }

    return 0;
}

HI_S32 keyled_write(HI_U8 u8data)
{
    GPIO_STB_SET(0);
	
    keyled_udelay(1);
	
    keyled_tx_byte(u8data);
	
    GPIO_STB_SET(1);

    return 0;
}

HI_S32 keyled_rx_byte(HI_U8 *u8data)
{
    HI_U8 u8val = 0;
    HI_U32 bitval = 0;
    int j = 0;

    if (NULL == u8data)
    {
        keyled_trace(8, "null pointer\n");
        return HI_FAILURE;
    }

    for (j = 0; j < 8; j++)
    {
        GPIO_CLOCK_SET(0);
		
        keyled_udelay(1);
		
        GPIO_CLOCK_SET(1);
		
        keyled_udelay(1);
		
        HI_DRV_GPIO_ReadBit(DOUT_PT6961, &bitval);
		
        if (bitval)
        {
            u8val |= (1 << j);
        }

        keyled_udelay(1);
    }

    *u8data = u8val;

    return 0;
}

HI_S32 keyled_read(HI_U8 *u8buf, HI_U32 u32len)
{
    int i = 0;

    if (NULL == u8buf)
    {
        keyled_trace(8, "null pointer\n");
        return HI_FAILURE;
    }

    GPIO_STB_SET(1);
	
    GPIO_CLOCK_SET(1);
	
    GPIO_STB_SET(0);
	
    keyled_tx_byte(CMD_READ);
	
    keyled_udelay(4);    //twait

    for (i = 0; i < u32len; i++)
    {
        keyled_rx_byte(&u8buf[i]);
    }

    GPIO_STB_SET(1);

    return 0;
}

HI_S32 keyled_display_addr_inc(HI_U8 *u8buf)
{
    HI_S32 i = 0;

    //command 2:set write command
    keyled_write(CMD_INC_WRITE);

    //command 3:set start address
    GPIO_STB_SET(0);
    keyled_tx_byte(0xc0);

    //set display data
    for (i = 0; i < 8; i++)
    {
        keyled_tx_byte(u8buf[i]);
    }

    GPIO_STB_SET(1);

    //command 1:set display mode
    keyled_write(0x3);

    //command 4
    keyled_write(CMD_DIP_ON);
    return 0;
}

HI_S32 keyled_display_addr_inc_init(HI_U8 *u8buf)
{
    HI_S32 i = 0;

    //command 2:set write command
    keyled_write(CMD_INC_WRITE);

    //command 3:set start address
    GPIO_STB_SET(1);
	
    GPIO_CLOCK_SET(1);
	
    GPIO_STB_SET(0);
	
    keyled_tx_byte(0xc0);

    //:set display data
    for (i = 0; i < 4; i++)
    {
        keyled_tx_byte(u8buf[i]);
    }

    GPIO_STB_SET(1);

    //command 1:set display mode
    keyled_write(0x3);

    //command 4
    keyled_write(CMD_DIP_OFF);

    return 0;
}

HI_S32 keyled_get_keycode(HI_VOID)
{
    HI_S32 i;
    HI_S32 s32key_code_sum = 0;
    HI_S32 s32key_code = 0;
    HI_S32 s32flag = 0;

    keyled_read(key_buf_arr, KEY_COL_NUM / 2);

    for (i = 0; i < 5; i++)
    {
        if (0 != (key_buf_arr[i] & 0x3f))
        {
            s32key_code = (i << 8) + key_buf_arr[i]; //in order to distinguish key value, add key_buff_arr flag
            s32key_code_sum += s32key_code;
            s32flag++;
        }
    }

    if (s32flag > 0)
    {
        switch (s32key_code_sum)
        {
        case 260:             //standby
            return (0);
        case 2:               //menu
            return (3);
        case 1:               //ok
            return (5);
        case 16:              //left
            return (4);
        case 32:              //right
            return (2);
        case 8:               //up
            return (6);
        case 4:               //down
            return (1);
        case 3:              //upgrade
            return (11);
        default:
            return (HI_FAILURE);
        }
    }
    else
    {
        return HI_FAILURE;
    }
}

static int hi_keyled_scan_key(void)
{
    HI_S32 s32keycode = 0;

    s32keycode = keyled_get_keycode();

    if (HI_FAILURE == s32keycode)
    {
        s_key_code_curr = KEY_MACRO_NO;
    }
    else
    {
        s_key_code_curr = s32keycode;
    }

    return HI_SUCCESS;
}

#if 0
static int hi_keyled_scan(void)
{
    HI_S32 s32keycode = 0;

    //set display
    keyled_display_addr_inc(disp_buf_arr);

    //get key val
    keyled_read(key_buf_arr, KEY_COL_NUM / 2);

    s32keycode = keyled_get_keycode();

    if (HI_FAILURE == s32keycode)
    {
        s_key_detected_flag = 0;
        s_key_code_curr = KEY_MACRO_NO;
    }
    else
    {
        s_key_detected_flag = 1;
        s_key_code_curr = s32keycode;
    }

    return HI_SUCCESS;
}

#endif

HI_VOID hi_keyled_clear_buf_func(HI_VOID)
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

    //	keyled_gpio_switch(1);  /* mask pin function config,it need congfig in boot reg*/

    keyled_gpio_dirset();
    keyled_display_addr_inc_init(disp_buf_arr);

    hi_keyled_clear_buf_func();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Close(HI_VOID)
{
    CheckKeyledDeviceFd();

    g_s32KEYLEDFd = 0;

    //switch to gpio
    //	keyled_gpio_switch(0);  /* mask pin function config,it need congfig in boot reg*/

    hi_keyled_clear_buf_func();

    return HI_SUCCESS;
}

#if 0
HI_S32 HI_KEYLED_Enable(HI_VOID)
{
    CheckKeyledDeviceFd();

    s_display_switch = DISPLAY_ON;
    hi_keyled_scan();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Disable(HI_VOID)
{
    CheckKeyledDeviceFd();

    s_display_switch = DISPLAY_OFF;
    hi_keyled_scan();

    return HI_SUCCESS;
}

#endif

HI_S32 HI_KEYLED_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    CheckKeyledDeviceFd();
    CheckKeyledNULLPointer(pu32PressStatus);
    CheckKeyledNULLPointer(pu32KeyId);

    hi_keyled_scan_key();

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

HI_S32 HI_KEYLED_Display(HI_U32 data)
{
    HI_S32 i = 0;

    CheckKeyledDeviceFd();

    for (i = 0; i < 4; i++)
    {
        disp_buf_arr[i * 2] = (data >> (8 * i)) & 0xff;
    }

    keyled_display_addr_inc(disp_buf_arr);

    return HI_SUCCESS;
}

#if 0

/*     --7--
 *    |     |
 *   2|     |6
 *    |--1--
 *    |     |
 *   3|     |5
 *     --4-- .0
 */
#if 0
static unsigned char sNumTbl[10] = {
    0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09
};

static unsigned char sChrTblUp[26] = {
    0x11, 0xC1, 0x63, 0x85, 0x61, 0x71, 0x41, 0x91, 0xDF, 0x8F,  /* A -> J */
    0xFD, 0xE3, 0xFD, 0x13, 0xC5, 0x31, 0x19, 0xFD, 0x49, 0xE1,  /* K -> T */
    0x83, 0xC7, 0xFD, 0xFD, 0xFD, 0xFD                           /* U -> Z */
};

static unsigned char sChrTblLow[26] = {
    0x05, 0xC1, 0xE5, 0x85, 0x21, 0x71, 0x09, 0xD1, 0xDF, 0x8F,  /* a -> j */
    0xFD, 0x9F, 0xFD, 0x13, 0xC5, 0x31, 0x19, 0xF5, 0x49, 0xE1,  /* k -> t */
    0xC7, 0xC7, 0xFD, 0xFD, 0xFD, 0xFD                           /* u -> z */
};
#endif

/*   --0--
 *  |     |
 * 5|     |1
 *  |--6--
 *  |     |
 * 4|     |2
 *   --3-- .7
 */
#if 1

HI_U8 sNumTbl[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f
};                                                                    //pt6961_ok

/*only 9 of the capital letters are effective ('C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U')*/
/*CNcomment:大写字母只显示9个有效('C'、'E'、'F'、'H'、'L'、'O'、'P'、'S'、'U')*/
HI_U8 sChrTblUp[] = {
    0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71, 0x00,
    0x76, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00,
    0x3f, 0x73, 0x00, 0x00, 0x6d, 0x00,
    0x3e, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*only 15 of the smalll letters are effective (b c d E, g h i, l n o, P q S(5) t, u)*/
/*CNcomment:小写字母只显示15个有效(b c d E, g h i, l n o, P q S(5) t, u) */
HI_U8 sChrTblLow[] = {
    0x00, 0x7c, 0x58, 0x5e, 0x7b, 0x00, 0x6f,
    0x74, 0x04, 0x00, 0x00, 0x06, 0x00, 0x54,
    0x5c, 0x73, 0x67, 0x00, 0x6d, 0x78,
    0x1c, 0x00, 0x00, 0x00, 0x00, 0x00
};

#else
static unsigned char sNumTbl[] = {
    0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f
};                                                                                 //ok
static unsigned char sChrTblUp[] = {
    0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x00, 0x76, 0x30, 0x00, /* A -> J */
    0x00, 0x38, 0x00, 0x3b, 0x3f, 0x73, 0x00, 0x08, 0xEd, 0x00, /* K -> T */
    0x3e, 0x00, 0x00, 0x00, 0x00, 0x00                         /* U -> Z */
};
static unsigned char sChrTblLow[] = {
    0xDC, 0x7c, 0x58, 0x5e, 0x7b, 0x4E, 0x6f, 0x74, 0x04, 0x00, /* a -> j */
    0x00, 0x06, 0x00, 0x54, 0x5c, 0x73, 0x67, 0x50, 0x6d, 0x78, /* k -> t */
    0x1c, 0x00, 0x00, 0x00,	 0x0, 0x00                          /* u -> z */
};
#endif
HI_S32 HI_KEYLED_DisplayString(HI_CHAR *fpString)
{
    HI_U32 tmpData = 0;
    HI_U8 DispCode[4] = {
        0
    };
    HI_U8 j;

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
            DispCode[j] = 0x40;
        }
        else if (DispCode[j] == '_')
        {
            DispCode[j] = 0x08;
        }
        else if (DispCode[j] == '.')
        {
            DispCode[j] = 0x80;
        }
        else  /* FIXME: may be should add symbol ':' and '.' */
        {
            DispCode[j] = 0x0;  /* default: not display */
        }
    }

    tmpData = (DispCode[3] << 24) | (DispCode[2] << 16) | (DispCode[1] << 8) | (DispCode[0]);

    //memcpy((void *)&tmpData, DispCode, 4);

    HI_KEYLED_Display(tmpData);
    udelay(3000);

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
