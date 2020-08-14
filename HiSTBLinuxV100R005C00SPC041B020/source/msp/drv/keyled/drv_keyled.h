/*******************************************************************************
*             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: keyled.h
* Description:
*
* History:
* Version   Date              Author       DefectNum       Description
*******************************************************************************/
#ifndef __KEYLED_H__
#define __KEYLED_H__

#define KEYBUF_LEN (8)

/* state of the key */
#define KEY_PRESS (0x0)           /* key pressed */     /*CNcomment: 按键按下状态 */
#define KEY_HOLD (0x01)           /* key holden */        /* CNcomment:按键长按状态 */
#define KEY_RELEASE (0x02)        /* key release */     /*CNcomment: 按键释放状态 */

#define DEFAULT_BLOCKTIME 500   /*500ms*/

typedef enum enum_KeyLed_Status
{
	KeyOFF_LedOFF = 0,
	KeyON, 
	LedON,
	KeyON_LedON
}KeyLed_Status_E;

typedef struct hiKEYLED_PROC_INFO_S
{
    HI_U32 KeyBufSize;	       /*  size of  keybuf  */                                   /*CNcomment: keybuf大小*/
    HI_U32 KeyBufHead;	       /*  head of keybuf  */                                    /*CNcomment: keybuf头*/
    HI_U32 KeyBufTail;	       /*  tail of keybuf  */                                    /*CNcomment: keybuf结尾*/
	HI_U32 KeyComeNum;	         /*  the top num of keybuf  */                             /*CNcomment: keybuf到达个数*/
    HI_U32 KeyReadNum;	       /*  number that the keybuf can read  */                   /*CNcomment: keybuf读取个数*/
    HI_U32 u32RepKeyTimeMs;    /*whether it is the time we think it is repeat key*/      /*CNcomment: 按键重复判断时间*/
    HI_U32 u32IsRepKeyEnable;  /*whether the  function of repeat key  is enable*/        /*CNcomment: 是否使能重复按键*/
    HI_U32 u32IsUpKeyEnable;   /*whether the function of key up is enable*/              /*CNcomment: 是否使能按键弹起*/
    HI_U32 u32BlockTime;
    HI_UNF_KEYLED_LIGHT_E enFlashPin;
    HI_UNF_KEYLED_LEVEL_E enFlashLevel;  /*level of the LED twinkle*/                              /*CNcomment: 数码管闪烁级别*/
    HI_UNF_KEYLED_TYPE_E enKeyLedType;
    HI_UNF_KEYLED_TIME_S stLedTime;
    HI_U32 u32DispCode;        /*the number that the LED is displaying */                /*CNcomment: 当前数码管显示的码型值*/

}KEYLED_PROC_INFO_S;


typedef struct
{
    HI_U32 keyled_code;
    HI_U32 keyled_state;
}keyled_key_info_s;

typedef struct
{
    HI_U32            head, tail;
    keyled_key_info_s buf[KEYBUF_LEN ];
    HI_U32            buf_len;
    HI_U32            key_come;
    HI_U32            key_read;

    HI_U32 enable_repkey;
    HI_U32 repkey_delaytime;

    HI_U32 enable_keyup;
    HI_U32 blocktime;
    HI_KEYLED_MODE_S KeyLedMode;
	HI_UNF_KEYLED_LIGHT_E FlashPin;
	HI_UNF_KEYLED_LEVEL_E FlashLevel;
} keyled_dev_s;

typedef struct
{
	HI_BOOL bDisNormalData;
	HI_BOOL bDisTimeData;
	HI_BOOL dotflag;
    HI_U8   LedDisNum;
	HI_U8   LedData[5];
	HI_UNF_KEYLED_TIME_S TimeData;
}KEYLED_SusSave;


#define KEYLED_BUF_HEAD keyled_dev.buf[keyled_dev.head]
#define KEYLED_BUF_TAIL keyled_dev.buf[keyled_dev.tail]
#define KEYLED_BUF_LAST keyled_dev.buf[(keyled_dev.head == 0) ? (keyled_dev.buf_len - 1) : (keyled_dev.head - 1)]
#define KEYLED_INC_BUF(x, len) (((x) + 1) % (len))

/* keyled operation */
typedef struct tagKEYLED_OPT_S
{
    HI_S32 (*KEYLED_KEY_Open)(HI_VOID);
    HI_S32 (*KEYLED_KEY_Close)(HI_VOID);
    HI_S32 (*KEYLED_KEY_Reset)(HI_VOID);
    HI_S32 (*KEYLED_KEY_GetValue)(HI_U32 *pu32PressStatus, HI_U32 *pu32KeyId);
    HI_S32 (*KEYLED_KEY_SetBlockTime)(HI_U32 u32BlockTimeMs);
    HI_S32 (*KEYLED_KEY_SetRepTime)(HI_U32 u32RepTimeMs);
    HI_S32 (*KEYLED_KEY_IsRepKey)(HI_U32 u32IsRepKey);
    HI_S32 (*KEYLED_KEY_IsKeyUp)(HI_U32 u32IsKeyUp);

    HI_S32 (*KEYLED_LED_Open)(HI_VOID);
    HI_S32 (*KEYLED_LED_Close)(HI_VOID);
    HI_S32 (*KEYLED_LED_Display)(HI_U32 u32CodeValue);
    HI_S32 (*KEYLED_LED_DisplayTime)(HI_UNF_KEYLED_TIME_S stKeyLedTime);
    HI_S32 (*KEYLED_LED_SetFlashPin)(HI_UNF_KEYLED_LIGHT_E enPin);
    HI_S32 (*KEYLED_LED_SetFlashFreq)(HI_UNF_KEYLED_LEVEL_E enLevel);
    HI_S32 (*KEYLED_LED_DisplayLED)(HI_KEYLED_DISPLAY_S LedData);

    HI_S32 (*KEYLED_SetMode)(HI_KEYLED_MODE_S KeyledMode);

	HI_S32 (*KEYLED_LED_SetLockIndicator)(HI_BOOL bLock);
    
    /*get the proc information of  the module */ /*CNcomment:获取模块的PROC信息*/
    HI_S32 (*KEYLED_GetProcInfo)(KEYLED_PROC_INFO_S *stInfo);

    HI_S32 (*KEYLED_Suspend)(HI_VOID);
    HI_S32 (*KEYLED_Resume)(HI_VOID);
} KEYLED_OPT_S;

extern	HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);    


#endif
