/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:   hi_keyled.c
* Description:
*
* History:
* Version   Date                Author          DefectNum     Description
***********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include <uboot.h>
#include "hi_type.h"
#include "hi_reg.h"

#include "hi_keyled_inner.h"
#include "hi_common.h"

static HI_U32 keyled_base_addr = KEYLED_PHY_ADDR;
//static HI_U32 MultConfigVir = 0x10203000;

static HI_VOID keyled_enable_ledc (HI_VOID);
static HI_VOID keyled_disable_ledc(HI_VOID);
static HI_VOID keyled_enable_key(HI_VOID);
static HI_VOID keyled_disable_key(HI_VOID);
static HI_U32  keyled_get_key(HI_VOID);
static HI_VOID keyled_display(HI_U32 u32lowdata);
static HI_VOID keyled_config_clktim(HI_U32 clk);
static HI_VOID keyled_config_fretim(HI_U32 fre);
static HI_VOID keyled_config_keytim(HI_U32 freq);
static HI_VOID keyled_config_type(HI_U32 ledtype, HI_U32 ledCStype);

HI_S32 HI_KEYLED_Open(HI_VOID)
{
    //HI_U32 Ret;
    //HI_KEYLED_MODE_S KeyLedMode;
    //HI_CHIP_TYPE_E enChipType;
    //HI_CHIP_VERSION_E enChipVersion;

    CheckKeyledDeviceRepeatFd();

    keyled_base_addr = KEYLED_PHY_ADDR;

	HI_REG_WRITE(keyled_base_addr + LEDC_SYSTIM, 0x7);

    keyled_config_clktim(CLKTIM_DEFAULT);
    keyled_config_fretim(FRETIM_DEFAULT);
    keyled_config_keytim(KEY_DEFAULT);
    //keyled_config_type(LEDC_ANODE, !LEDC_HIGHT_LEVEL);
    keyled_config_type(!LEDC_ANODE, KEY_TYPE_CT1642);

    //KeyLedMode.KeyScanMode = HI_KEYLED_1MUL8;
    //KeyLedMode.LedNum = HI_KEYLED_4LED;

	/* clear led first to avoid splash when open - chenxu add 20071108 */

    keyled_display(0x00000000);

    HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, (KEYPAD_PRESS_INTR | KEYPAD_RELEASE_INTR));
    keyled_enable_ledc();
    keyled_enable_key();

    g_s32KEYLEDFd = 1;

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Close(HI_VOID)
{
    if (0 >= g_s32KEYLEDFd)
    {
        return HI_SUCCESS;
    }

    keyled_disable_ledc();
    keyled_disable_key();

    g_s32KEYLEDFd = 0;

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Enable(HI_VOID)
{
    CheckKeyledDeviceFd();

    keyled_enable_ledc();
    keyled_enable_key();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Disable(HI_VOID)
{
    CheckKeyledDeviceFd();

    keyled_disable_ledc();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_GetValue(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId)
{
    HI_U32 regvalue = 0;
    HI_U32 key = 0;

    static HI_U32 s_u32PreKey = KEYPAD_ERROR;

    CheckKeyledDeviceFd();
    CheckKeyledNULLPointer(pu32PressStatus);
    CheckKeyledNULLPointer(pu32KeyId);

    udelay(1000);
    HI_REG_READ(keyled_base_addr + LEDC_KEYINT, regvalue);

    if (regvalue & KEYPAD_PRESS_INTR)
    {
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & KEYPAD_PRESS_INTR);

        udelay(1000);
        key = keyled_get_key();
        if (KEYPAD_ERROR != key)
        {
            *pu32KeyId	= key;
            s_u32PreKey = key;
            *pu32PressStatus = KEY_PRESS;
        }
        else
        {
            return HI_FAILURE;
        }
    }
    else if (regvalue & KEYPAD_RELEASE_INTR)
    {
        /* clear the initr flag*/
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & KEYPAD_RELEASE_INTR);

        udelay(1000);
        *pu32PressStatus = KEY_RELEASE;
        *pu32KeyId = s_u32PreKey;
    }
    else
    {
        HI_REG_WRITE(keyled_base_addr + LEDC_KEYINT, regvalue & (KEYPAD_PRESS_INTR | KEYPAD_RELEASE_INTR));
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Display(HI_U32 data)
{
    CheckKeyledDeviceFd();

    keyled_display(data);

    return HI_SUCCESS;
}

static HI_VOID keyled_enable_ledc (HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue |= (LEDC_ENABLE | LED_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);
}

static HI_VOID keyled_disable_ledc(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue &= (~LEDC_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    udelay(1000);
	HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, (regvalue & (~LED_ENABLE)));

    return;
}

static HI_VOID keyled_enable_key(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue |= (KEYPAD_ENABLE | KEYPAD_PRESS_INTR | KEYPAD_RELEASE_INTR | LED_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);
}

static HI_VOID keyled_disable_key(HI_VOID)
{
    HI_U32 regvalue = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    regvalue &= (~KEYPAD_ENABLE);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    udelay(1000);
	HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, (regvalue & (~LED_ENABLE)));
}

static HI_U32 keyled_get_key(HI_VOID)
{
    HI_U32 key = 0;

    HI_REG_READ(keyled_base_addr + LEDC_KEYDATA, key);

    switch (key)
    {
    case KEY_7_PRESS:
        return (8);
    case KEY_6_PRESS:
        return (6);
    case KEY_5_PRESS:
        return (5);
    case KEY_4_PRESS:
        return (4);
    case KEY_3_PRESS:
        return (3);
    case KEY_2_PRESS:
        return (2);
    case KEY_1_PRESS:
        return (1);
    case KEY_0_PRESS:
        return (0);
    case KEY_65comb_PRESS:        /* used for upgrade*/
        return (11);
    default:
        return (KEYPAD_ERROR);
    }
}

static HI_VOID keyled_display(HI_U32 u32lowdata)
{
    HI_U32 datavalue = 0;
    HI_U32 regvalue	 = 0;

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    datavalue = u32lowdata & 0x00ff;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA1, datavalue);

    datavalue = 0;
    datavalue = (u32lowdata & 0x00ff00) >> 8;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA2, datavalue);

    datavalue = 0;
    datavalue = (u32lowdata & 0x00ff0000) >> 16;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA3, datavalue);

    datavalue = 0;
    datavalue = (u32lowdata & 0xff000000) >> 24;
    HI_REG_WRITE(keyled_base_addr + LEDC_DATA4, datavalue);

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

static HI_VOID keyled_config_clktim(HI_U32 clk)
{
    HI_S32 regvalue = 0;
    HI_S32 clktim = 0;
	HI_U32 systim = 0;

    if (0 == clk)
    {
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
        freq = KEY_DEFAULT;
    }

    HI_REG_READ(keyled_base_addr + LEDC_CONTROL, regvalue);
    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue & ~LEDC_ENABLE);

    HI_REG_READ(keyled_base_addr + LEDC_SYSTIM, systim);	
    HI_REG_READ(keyled_base_addr + LEDC_CLKTIM, clktim);
    HI_REG_READ(keyled_base_addr + LEDC_FRETIM, fretim);

    keytim = (SYSTEM_CLK * 1000 * 1000) 
		/ ((fretim + 1) * (systim + 1) * 16 * 40 * 5 * 8 * (clktim + 1) * freq) - 1;
    if (keytim < 0)
    {
        keytim = 0;
    }

    HI_REG_WRITE(keyled_base_addr + LEDC_KEYTIM, keytim);

    HI_REG_WRITE(keyled_base_addr + LEDC_CONTROL, regvalue);

    return;
}

/* config led type and voltage type */
static HI_VOID keyled_config_type(HI_U32 ledtype, HI_U32 keytype)
{
    HI_U32 regvalue = 0;

    if (LEDC_ANODE == ledtype)
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

#if 0
/*     --7--
 *    |     |
 *   2|     |6
 *    |--1--
 *    |     |
 *   3|     |5
 *     --4-- .0
 */
static unsigned char sNumTbl[10] = {
    0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09
};

static unsigned char sChrTblUp[26] = {
    0x11, 0xC1, 0x63, 0x85, 0x61, 0x71, 0x41, 0x91, 0xDF, 0x8F,  /* A -> J */
    0xFD, 0xE3, 0xFD, 0x13, 0xC5, 0x31, 0x19, 0xFD, 0x49, 0xE1,  /* K -> T */
    0x83, 0xC7, 0xFD, 0xFD, 0xFD, 0xFD                                   /* U -> Z */
};

static unsigned char sChrTblLow[26] = {
    0x05, 0xC1, 0xE5, 0x85, 0x21, 0x71, 0x09, 0xD1, 0xDF, 0x8F,  /* a -> j */
    0xFD, 0x9F, 0xFD, 0x13, 0xC5, 0x31, 0x19, 0xF5, 0x49, 0xE1,  /* k -> t */
    0xC7, 0xC7, 0xFD, 0xFD, 0xFD, 0xFD                                   /* u -> z */
};

HI_S32 HI_KEYLED_DisplayString(HI_CHAR *fpString)
{
    unsigned int tmpData = 0;
    unsigned char DispCode[4] = {
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
            DispCode[j] = ~sChrTblUp[DispCode[j] - 'A'];
        }
        else if ((DispCode[j] >= 'a') && (DispCode[j] <= 'z'))
        {
            DispCode[j] = ~sChrTblLow[DispCode[j] - 'a'];
        }
        else if ((DispCode[j] >= '0') && (DispCode[j] <= '9'))
        {
            DispCode[j] = ~sNumTbl[DispCode[j] - '0'];
        }
        else if (DispCode[j] == '-')
        {
            DispCode[j] = 0x02;
        }
        else if (DispCode[j] == '_')
        {
            DispCode[j] = 0x08;
        }
        else if (DispCode[j] == '.')
        {
            DispCode[j] = 0x01;
        }
        else  /* FIXME: may be should add symbol ':' and '.' */
        {
            DispCode[j] = 0x0;  /* default: not display */
        }
    }

    tmpData = (DispCode[0] << 24) | (DispCode[1] << 16) | (DispCode[2] << 8) | (DispCode[3]);

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


#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */
