/*******************************************************************************
 *              Copyright 2005 - 2006, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:    priv_keyled.h
 * Description: This is internal include file for keyled
 *
 * History:
 * Version      Date         Author       DefectNum    Description
 * main\1    2010-04-15   jianglei 40671    NULL       Create this file.
 ******************************************************************************/

#ifndef  __DRV_KEYLED_IOCTL_H__
#define  __DRV_KEYLED_IOCTL_H__

typedef struct tagGET_KEY_VALUE_S
{
    HI_U32 u32KeyCode;
    HI_U32 u32KeyStatus;
}GET_KEY_VALUE_S;

typedef enum hiHI_KEYLED_LEDTYPE_E
{
    HI_KEYLED_CATHODE = 0x0,
    HI_KEYLED_ANODE,
    HI_KEYLED_LEDTYPE_MAX
} HI_KEYLED_LEDTYPE_E;

typedef enum hiHI_KEYLED_CSTYPE_E
{
    HI_KEYLED_CS_LOW = 0x0,
    HI_KEYLED_CS_HIGH,
    HI_KEYLED_CSTYPE_MAX
} HI_KEYLED_CSTYPE_E;

typedef enum hiHI_KEYLED_LEDNUM_E
{
    HI_KEYLED_4LED = 0x0,
    HI_KEYLED_5LED,
    HI_KEYLED_LEDNUM_MAX
} HI_KEYLED_LEDNUM_E;

typedef enum hiHI_KEYLED_KEYSCAN_E
{
    HI_KEYLED_2MUL4 = 0x0,
    HI_KEYLED_1MUL8,
    HI_KEYLED_KEYSCAN_MAX
} HI_KEYLED_KEYSCAN_E;

typedef enum hiHI_KEYLED_KEYLEVEL_E
{
    HI_KEYLED_1MUL8_LOW = 0x0,
    HI_KEYLED_1MUL8_HIGH,
    HI_KEYLED_KEYLEVEL_MAX
} HI_KEYLED_KEYLEVEL_E;

typedef struct hiKEYLED_DISPLAY_S
{
    HI_U8 u8LedNum;
    HI_U8 *pu8LedData;
}HI_KEYLED_DISPLAY_S;


typedef struct hi_KEYLED_Mode_S
{
	HI_KEYLED_KEYSCAN_E KeyScanMode;
   	HI_KEYLED_LEDNUM_E LedNum;
    HI_KEYLED_KEYLEVEL_E KeyLevel;
}HI_KEYLED_MODE_S;

#define HI_KEYLED_KEY_OPEN_CMD              _IO(HI_ID_KEYLED, 1)
#define HI_KEYLED_KEY_CLOSE_CMD             _IO(HI_ID_KEYLED, 2)
#define HI_KEYLED_KEY_RESET_CMD             _IO(HI_ID_KEYLED, 3)
#define HI_KEYLED_KEY_GET_VALUE_CMD         _IOR(HI_ID_KEYLED, 4, GET_KEY_VALUE_S)
#define HI_KEYLED_SET_BLOCK_TIME_CMD        _IOW(HI_ID_KEYLED, 5, HI_U32)
#define HI_KEYLED_SET_IS_KEYUP_CMD          _IOW(HI_ID_KEYLED, 6, HI_U32)
#define HI_KEYLED_SET_IS_REPKEY_CMD         _IOW(HI_ID_KEYLED, 7, HI_U32)
#define HI_KEYLED_SET_REPKEY_TIME_CMD       _IOW(HI_ID_KEYLED, 8, HI_U32)

#define HI_KEYLED_LED_OPEN_CMD              _IO(HI_ID_KEYLED, 11)
#define HI_KEYLED_LED_CLOSE_CMD             _IO(HI_ID_KEYLED, 12)
#define HI_KEYLED_DISPLAY_CODE_CMD          _IOW(HI_ID_KEYLED, 13, HI_U32)
#define HI_KEYLED_DISPLAY_TIME_CMD          _IOW(HI_ID_KEYLED, 14, HI_UNF_KEYLED_TIME_S)
#define HI_KEYLED_SET_FLASH_PIN_CMD         _IOW(HI_ID_KEYLED, 15, HI_U32)
#define HI_KEYLED_CONFIG_FLASH_FREQ_CMD     _IOW(HI_ID_KEYLED, 16, HI_UNF_KEYLED_LEVEL_E)

#define HI_KEYLED_SELECT_CMD                _IOW(HI_ID_KEYLED, 17, HI_UNF_KEYLED_TYPE_E)
#define HI_KEYLED_SET_MODE_CMD              _IOW(HI_ID_KEYLED, 18, HI_KEYLED_MODE_S)
#define HI_KEYLED_DISPLAY_MULITLED_CMD      _IOW(HI_ID_KEYLED, 19, HI_KEYLED_DISPLAY_S)
#define HI_KEYLED_SETLOCK_CMD               _IOW(HI_ID_KEYLED, 20, HI_U32)

#endif  /*  __DRV_KEYLED_IOCTL_H__ */
