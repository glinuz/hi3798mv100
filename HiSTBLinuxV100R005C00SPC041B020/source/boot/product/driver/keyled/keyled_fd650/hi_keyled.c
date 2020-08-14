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
#include "hi_keyled_fd650.h"
#include "hi_reg.h"
#include "hi_unf_keyled.h"

/*-------------------------------------------------------------------------------
 * macro define
 *------------------------------------------------------------------------------*/
#define DEFAULT_DELAYTIME 300

#define CMD_READ (0x42)
#define CMD_INC_WRITE (0x40)
#define CMD_FIX_WRITE (0x44)
#define CMD_DIP_ON (0x8f)
#define CMD_DIP_OFF (0x80)

#if 0
#define  GPIO5_BASE_ADDR 0x101e4000
#define  GPIO5_REG_DIR 0x101e4400
#define  GPIO5_REG_IS 0x101e4404
#define  GPIO5_REG_IBE 0x101e4408
#define  GPIO5_REG_IEV 0x101e440c
#define  GPIO5_REG_IE 0x101e4410
#define  GPIO5_REG_RIS 0x101e4414
#define  GPIO5_REG_MIS 0x101e4418
#define  GPIO5_REG_IC 0x101e441c
#define  GPIO5_REG_RSV 0x101e4420

#define BUF_HEAD keyled_dev.buf[keyled_dev.head]
#define BUF_TAIL keyled_dev.buf[keyled_dev.tail]
#define BUF_LAST keyled_dev.buf[(keyled_dev.head == 0) ? (keyled_dev.buf_len - 1) : (keyled_dev.head - 1)]
#define INC_BUF(x, len) ((++(x)) % (len))
#endif

//#define IO_READ(addr) *((volatile unsigned int *)(addr))
//static unsigned int g_u32GpioDir = (0x101e4000 + 0x400);
//static unsigned int g_u32Random	 = 0;

typedef struct hiKEY_INFO_S
{
    HI_U32 KeyVal;
    HI_U32 KeyPressState;
}HI_KEY_INFO_S;

static HI_KEY_INFO_S g_OldKey;


#if 0
static void  time_delay_us(unsigned int usec)
{
    int i, j, regvalue;

#if 0
    mdelay(1);
#else
    for (i = 0; i < 100 * usec; i++)
    {
        regvalue = IO_READ(g_u32GpioDir);
        g_u32Random += regvalue;
        for (j = 0; j < 30; j++)
        {
            regvalue = IO_READ(g_u32GpioDir);
        }
    }
#endif
    return;
}

#define keyled_udelay(x) time_delay_us(x)
#endif

//#define keyled_mdelay(x) udelay(x * 1000)

/*-------------------------------------------------------------------------------
 * variable define
 *------------------------------------------------------------------------------*/
//static int s_display_switch = DISPLAY_ON;

//static int s_key_curr = KEY_MACRO_NO;
//static int s_key_last = KEY_MACRO_NO;
//static int s_tingle_check = 0;
//static int s_key_detected_flag = 0;
static int s_key_code_curr = KEY_MACRO_NO;

//static int s_bKeyPressFlag = 0;
//static int s_cycle_cnt = 0;

//HI_U32 display_mode_index = 0;

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

#if 0
HI_U8 key_code_arr[KEY_REAL_NUM] = {
    0x00, 0x01, 0x02, 0x03,
    0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b,
    0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b,
    0x1c, 0x1d
};
#endif

const HI_U8 display_mode_array[DISPLAY_MODE_NUM] = {
    0, 1, 2, 3
};

#if 0
static HI_U32 regDeInit[4] = {
    0
};
#endif

#if 0
HI_VOID hi_gpio_dirset_bit(HI_U32 gpio5, HI_U32 idx, HI_U32 val)
{
    HI_U32 regData;

    HI_REG_READ(GPIO5_REG_DIR, regData);
    if (!val)
    {
        regData |= (1 << idx);      // output
    }
    else
    {
        regData &= ~(1 << idx);     // input
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
    *val = regData;

    return;
}
#endif

HI_S32 hi_gpio_read_SDA_IN(void)
{
    HI_U32 sda_in;

    HI_DRV_GPIO_ReadBit(FD650_SDA, &sda_in);

    return (sda_in);
}

HI_S32 keyled_gpio_switch(HI_U32 mode)
{
#if 0
    HI_U32 u32sysctrl = 0;

    if (mode)
    {
        HI_REG_READ((0x10203000 + 0x74), regDeInit[0]);
        HI_REG_WRITE((0x10203000 + 0x74), 0x00);
    }
    else
    {
        HI_REG_WRITE((0x10203000 + 0x74), regDeInit[0]);
    }
#endif
    return 0;
}

void FD650_Start( void )
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

void FD650_Stop( void )
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

void FD650_WrByte( HI_U8 dat )
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
        DELAY;  // optional delay
        FD650_SCL_CLR;
    }

    FD650_SDA_SET;
	FD650_SDA_D_IN;
    DELAY;
    FD650_SCL_SET;
    DELAY;
    FD650_SCL_CLR;
    FD650_SDA_D_IN;
    return;
}

HI_U8  FD650_RdByte( void )
{
    HI_U8 dat, i;

    FD650_SDA_D_OUT;
    FD650_SDA_SET;
    FD650_SDA_D_IN;
    dat = 0;
    for (i = 0; i != 8; i++)
    {
        DELAY;
        FD650_SCL_SET;
        DELAY;
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

    return dat;
}

void FD650_Write( HI_U16 cmd )
{
    FD650_Start();
    FD650_WrByte(((HI_U8)(cmd >> 7) & 0x3E) | 0x40);
    FD650_WrByte((HI_U8)cmd);
    FD650_Stop();

    return;
}

HI_U8 FD650_Read( void )
{
    HI_U8 keycode = 0;

    FD650_Start();
    FD650_WrByte((((HI_U8)(FD650_GET_KEY >> 7) & 0x3E) | 0x01) | 0x40);
    keycode = FD650_RdByte();

    FD650_Stop();
    if ((keycode & 0x00000040) == 0)
    {
        keycode = 0;
    }

    return keycode;
}

HI_S32 keyled_read(HI_U8 *u8buf, HI_U32 u32len)
{
    HI_U8 keycode = 0;

    if (NULL == u8buf)
    {
        keyled_trace(8, "null pointer\n");
        return HI_FAILURE;
    }

    keycode = FD650_Read();

    *u8buf = keycode;
    return 0;
}

HI_S32 keyled_display_addr_inc(HI_U8 *u8buf)
{
    HI_U8 disp_tmp[4] = {
        0
    };

    disp_tmp[0] = u8buf[0];
    disp_tmp[1] = u8buf[2];
    disp_tmp[2] = u8buf[4];
    disp_tmp[3] = u8buf[6];

    FD650_Write(FD650_SYSON_7);
    FD650_Write( FD650_DIG0 | (HI_U8)disp_tmp[0] );
    FD650_Write( FD650_DIG1 | (HI_U8)disp_tmp[1] );
    FD650_Write( FD650_DIG2 | (HI_U8)disp_tmp[2] );
    FD650_Write( FD650_DIG3 | (HI_U8)disp_tmp[3] );

    return 0;
}

HI_S32 keyled_display_addr_inc_init(HI_U8 *u8buf)
{
    return keyled_display_addr_inc(u8buf);
}

HI_S32 keyled_get_keycode(HI_VOID)
{
    keyled_read(key_buf_arr, KEY_COL_NUM / 2);

    if ((key_buf_arr[0] & 0x7f) == 0)
    {
        return HI_FAILURE;
    }

    return key_buf_arr[0];
}

static int hi_keyled_scan_key(void)
{
    HI_S32 s32keycode = 0;

    s32keycode = keyled_get_keycode();

    if (HI_FAILURE == s32keycode)
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

HI_VOID keyled_display_lockled(HI_BOOL bLockFlag)
{
    //HI_S32 i;
    //HI_U32 u32ctrl = 0;

    if (HI_TRUE == bLockFlag)
    {
        disp_buf_arr[0] |= 0x80;
    }
    else
    {
        disp_buf_arr[0] &= 0x7f;
    }

    //set display
    keyled_display_addr_inc(disp_buf_arr);
    return;
}

HI_S32 HI_KEYLED_Open(HI_VOID)
{
    CheckKeyledDeviceRepeatFd();

    g_s32KEYLEDFd = 1;
    //s_display_switch = DISPLAY_ON;
    s_key_code_curr	 = KEY_MACRO_NO;
    g_OldKey.KeyVal	 = KEY_MACRO_NO;
    g_OldKey.KeyPressState = KEY_RELEASE;

    keyled_gpio_switch(1);

    keyled_display_addr_inc_init(disp_buf_arr);

    hi_keyled_clear_buf_func();

    return HI_SUCCESS;
}

HI_S32 HI_KEYLED_Close(HI_VOID)
{
    CheckKeyledDeviceFd();

    g_s32KEYLEDFd = 0;

    //switch to gpio
    //keyled_gpio_switch(0);

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
    //HI_U32 regvalue = 0;
    //HI_U32 key = 0;
    //HI_S32 ret = 0;

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

HI_S32 HI_KEYLED_SetLockIndicator(HI_BOOL bLock)
{
    keyled_display_lockled(bLock);
    return HI_SUCCESS;
}

#if 0
/*   --0--
 *  |     |
 * 5|     |1
 *  |--6--
 *  |     |
 * 4|     |2
 *   --3-- .7
 */
static unsigned char sNumTbl[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};
static unsigned char sChrTblUp[26] = {
    0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x0E,  /* A -> J */
    0x00, 0x38, 0x00, 0x37, 0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78,  /* K -> T */
    0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00                                       /* U -> Z */
};
static unsigned char sChrTblLow[26] = {
    0x5F, 0x7c, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x06, 0x0E,  /* a -> j */
    0x00, 0x38, 0x00, 0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78,  /* k -> t */
    0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00                                       /* u -> z */
};

HI_S32 HI_KEYLED_DisplayString(HI_U8 *fpString)
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

        #if 0
        else if (DispCode[j] == '-')
        {
            DispCode[j] = 0x02;
        }
        else if (DispCode[j] == '_')
        {
            DispCode[j] = ~0x08;
        }
        else if (DispCode[j] == '.')
        {
            DispCode[j] = 0x01;
        }
        #endif
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


