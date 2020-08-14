/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_keyled.c
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_unf_ir.h"
#include "hi_unf_keyled.h"
#include "com_osal.h"
#include "com_debug.h"
#include "ui_keyled.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#ifdef HI_BUILD_WITH_KEYLED

#if defined(HI_KEYLED_74HC164)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_STD
#elif defined(HI_KEYLED_PT6961)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_PT6961
#elif defined(HI_KEYLED_CT1642)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_CT1642
#elif defined(HI_KEYLED_PT6964)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_PT6964
#elif defined(HI_KEYLED_FD650)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_FD650
#elif defined(HI_KEYLED_GPIOKEY)
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_GPIOKEY
#else
#define UI_D_KEYLED_TYPE    HI_UNF_KEYLED_TYPE_BUTT
#endif

#define UI_D_MAXNUM_DISSTRING   (5)
#define UI_D_MAXNUM_ASCII       (128)
#define UI_D_KEYBLOCKTIME       (200)

typedef enum tagUI_PANELKEY_E
{
#if defined(HI_KEYLED_GPIOKEY) && defined(CHIP_TYPE_hi3716mv420)
    UI_E_PANELKEY_MENU     = 0x40,
    UI_E_PANELKEY_OK       = 0x2,
    UI_E_PANELKEY_LEFT     = 0x10,
    UI_E_PANELKEY_RIGHT    = 0x20,
    UI_E_PANELKEY_UP       = 0x4,
    UI_E_PANELKEY_DOWN     = 0x8,
    UI_E_PANELKEY_EXIT     = 0x1,
#else
    UI_E_PANELKEY_MENU     = 0,
    UI_E_PANELKEY_OK       = 1,
    UI_E_PANELKEY_LEFT     = 2,
    UI_E_PANELKEY_RIGHT    = 3,
    UI_E_PANELKEY_UP       = 4,
    UI_E_PANELKEY_DOWN     = 5,
    UI_E_PANELKEY_EXIT     = 6,
#endif
    UI_E_PANELKEY_UNKNOWN

} UI_PANELKEY_E;

#if defined(HI_KEYLED_74HC164)

static HI_U8 s_au8DigitalCode[UI_D_MAXNUM_ASCII] =
{
    /**< 00 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /**< 01 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /**< 02 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /**< 03 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /**< 04 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0xff, 0xff, 0x03, 0x9F,
    /**< 05 */ 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09, 0xff, 0xff,
    /**< 06 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0xC1, 0x63, 0x85, 0x61,
    /**< 07 */ 0x71, 0x41, 0x91, 0xDF, 0x8F, 0xFD, 0xE3, 0xFD, 0x13, 0xC5,
    /**< 08 */ 0x31, 0x19, 0xFD, 0x49, 0xE1, 0x83, 0xC7, 0xFD, 0xFD, 0xFD,
    /**< 09 */ 0xFD, 0xff, 0xff, 0xff, 0xff, 0x10, 0xff, 0x05, 0xC1, 0xE5,
    /**< 10 */ 0x85, 0x21, 0x71, 0x09, 0xD1, 0xDF, 0x8F, 0xFD, 0x9F, 0xFD,
    /**< 11 */ 0x13, 0xC5, 0x31, 0x19, 0xF5, 0x49, 0xE1, 0xC7, 0xC7, 0xFD,
    /**< 12 */ 0xFD, 0xFD, 0xFD, 0xff, 0xff, 0xff, 0xff, 0xff
};

#elif defined(HI_KEYLED_PT6961) || defined(HI_KEYLED_PT6964) || defined(HI_KEYLED_FD650)

static HI_U8 s_au8DigitalCode[UI_D_MAXNUM_ASCII] =
{
    /**< 00 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 01 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 02 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 03 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 04 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x3f, 0x06,
    /**< 05 */ 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x00,
    /**< 06 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x7f, 0x39, 0x3f, 0x79,
    /**< 07 */ 0x71, 0x00, 0x76, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x3f,
    /**< 08 */ 0x73, 0x00, 0x00, 0x6d, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00,
    /**< 09 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x7c, 0x58,
    /**< 10 */ 0x5e, 0x7b, 0x00, 0x6f, 0x74, 0x04, 0x00, 0x00, 0x06, 0x00,
    /**< 11 */ 0x54, 0x5c, 0x73, 0x67, 0x50, 0x6d, 0x78, 0x1c, 0x00, 0x00,
    /**< 12 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#elif defined(HI_KEYLED_CT1642)
static HI_U8 s_au8DigitalCode[UI_D_MAXNUM_ASCII] =
{
    /**< 00 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 01 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 02 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 03 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /**< 04 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0xfc, 0x60,
    /**< 05 */ 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6, 0x00, 0x00,
    /**< 06 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0xfe, 0x9c, 0xfc, 0x9e,
    /**< 07 */ 0x8e, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xfc,
    /**< 08 */ 0xce, 0x00, 0x00, 0xb6, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00,
    /**< 09 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x3e, 0x1a,
    /**< 10 */ 0x7a, 0xde, 0x00, 0xf6, 0x2e, 0x20, 0x00, 0x00, 0x60, 0x00,
    /**< 11 */ 0x2a, 0x3a, 0xce, 0xe6, 0x0a, 0xb6, 0x1e, 0x38, 0x00, 0x00,
    /**< 12 */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#else
static HI_U8 s_au8DigitalCode[UI_D_MAXNUM_ASCII] = {0x00};
#endif

#endif



#ifdef HI_BUILD_WITH_IR
static HI_S32 uiIRInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEYLED_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_IR_Enable, s32Ret);
        return COM_ERR_DEVICE;
    }

    return HI_SUCCESS;
}
static HI_S32 uiIRDeInit(HI_VOID)
{
    COM_CHECK(HI_UNF_IR_Enable(HI_FALSE));

    COM_CHECK(HI_UNF_IR_DeInit());

    return HI_SUCCESS;
}

static HI_S32 uiIRGetValue(HI_U32* pu32KeyValue)
{
    HI_U32 u32KeyStatus = 0;
    HI_U64 u64KeyValue = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_IR_GetValue(&u32KeyStatus, &u64KeyValue, 0);
    if (HI_SUCCESS != s32Ret)
    {
        *pu32KeyValue = UI_E_KEYVALUE_UNKNOWN;
        return HI_SUCCESS;
    }

    if (HI_UNF_KEY_STATUS_UP != u32KeyStatus)
    {
        *pu32KeyValue = UI_E_KEYVALUE_UNKNOWN;
        return HI_SUCCESS;
    }

    *pu32KeyValue = (HI_U32)u64KeyValue;

    return HI_SUCCESS;
}

#endif

#ifdef HI_BUILD_WITH_KEYLED
static HI_S32 uiKEYLEDInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_KEYLED_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEYLED_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_KEYLED_SelectType(UI_D_KEYLED_TYPE);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEYLED_SelectType, s32Ret);
        HI_UNF_KEYLED_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_LED_Open();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_LED_Open, s32Ret);
        HI_UNF_KEYLED_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_KEY_Open();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEY_Open, s32Ret);
        HI_UNF_LED_Close();
        HI_UNF_KEYLED_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_KEY_IsKeyUp(1);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEY_IsKeyUp, s32Ret);
        HI_UNF_KEY_Close();
        HI_UNF_LED_Close();
        HI_UNF_KEYLED_DeInit();
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_KEY_IsRepKey(0);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEY_IsRepKey, s32Ret);

        HI_UNF_KEY_Close();
        HI_UNF_LED_Close();
        HI_UNF_KEYLED_DeInit();

        return COM_ERR_DEVICE;
    }

    s32Ret = HI_UNF_KEY_SetBlockTime(UI_D_KEYBLOCKTIME);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEY_SetBlockTime, s32Ret);

        HI_UNF_KEY_Close();
        HI_UNF_LED_Close();
        HI_UNF_KEYLED_DeInit();

        return COM_ERR_DEVICE;
    }

    return HI_SUCCESS;
}


static HI_S32 uiKEYLEDDeInit(HI_VOID)
{
    COM_CHECK(HI_UNF_KEY_Close());
    COM_CHECK(HI_UNF_LED_Close());
    COM_CHECK(HI_UNF_KEYLED_DeInit());

    return HI_SUCCESS;
}

UI_KEYVALUE_E uiKEYLEDKeyConvert(UI_PANELKEY_E enPanelKey)
{
    switch (enPanelKey)
    {
        case UI_E_PANELKEY_MENU:  {return UI_E_KEYVALUE_MENU;}
        case UI_E_PANELKEY_OK:    {return UI_E_KEYVALUE_OK;}
        case UI_E_PANELKEY_LEFT:  {return UI_E_KEYVALUE_LEFT;}
        case UI_E_PANELKEY_RIGHT: {return UI_E_KEYVALUE_RIGHT;}
        case UI_E_PANELKEY_UP:    {return UI_E_KEYVALUE_UP;}
        case UI_E_PANELKEY_DOWN:  {return UI_E_KEYVALUE_DOWN;}
        case UI_E_PANELKEY_EXIT:  {return UI_E_KEYVALUE_EXIT;}
        default: {return UI_E_KEYVALUE_UNKNOWN;}
    }
}

static HI_S32 uiKEYLEDGetValue(HI_U32* pu32KeyValue)
{
    HI_U32 u32KeyValue = 0;
    HI_U32 u32KeyStatus = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = HI_UNF_KEY_GetValue(&u32KeyStatus, &u32KeyValue);
    if (HI_SUCCESS != s32Ret)
    {
        *pu32KeyValue = UI_E_KEYVALUE_UNKNOWN;
    }
    else
    {
        *pu32KeyValue = (0x02 == u32KeyStatus) ? uiKEYLEDKeyConvert(u32KeyValue) : UI_E_KEYVALUE_UNKNOWN;
    }

    return HI_SUCCESS;
}

static HI_S32 uiKEYLEDDisplay(HI_CHAR* pcString)
{
    HI_U8 i = 0;
    HI_U8 au8DisCode[UI_D_MAXNUM_DISSTRING] = {0};
    HI_S32 s32Ret = HI_SUCCESS;

    COM_StrNCpy(au8DisCode, pcString, UI_D_MAXNUM_DISSTRING - 1);

    for (i = 0; i < UI_D_MAXNUM_DISSTRING; i++)
    {
        if (au8DisCode[i] >= UI_D_MAXNUM_ASCII)
        {
            COM_PrintErrCode(COM_ERR_UNAVAILABLE);
            return COM_ERR_UNAVAILABLE;
        }

        au8DisCode[i] = s_au8DigitalCode[au8DisCode[i]];
    }

    s32Ret = HI_UNF_LED_Display((HI_U32)au8DisCode);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_KEYLED_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

    return HI_SUCCESS;
}
#endif

HI_S32 UI_KEYLED_Init(HI_VOID)
{
    COM_DBG_FuncEnter();

#ifdef HI_BUILD_WITH_IR
    COM_CHECK(uiIRInit());
#endif

#ifdef HI_BUILD_WITH_KEYLED
    COM_CHECK(uiKEYLEDInit());
#endif

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_KEYLED_DeInit(HI_VOID)
{
    COM_DBG_FuncEnter();

#ifdef HI_BUILD_WITH_KEYLED
    COM_CHECK(uiKEYLEDDeInit());
#endif

#ifdef HI_BUILD_WITH_IR
    COM_CHECK(uiIRDeInit());
#endif

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}


HI_S32 UI_KEYLED_GetInput(HI_U32* pu32KeyValue)
{
    HI_U32 u32KeyValue = UI_E_KEYVALUE_UNKNOWN;
    HI_S32 s32Ret = HI_SUCCESS;

    COM_CHECK_PARAM(HI_NULL_PTR == pu32KeyValue);

#ifdef HI_BUILD_WITH_IR
    s32Ret = uiIRGetValue(&u32KeyValue);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(uiIRGetValue, s32Ret);
        return COM_ERR_DEVICE;
    }

#endif

#ifdef HI_BUILD_WITH_KEYLED
    if (UI_E_KEYVALUE_UNKNOWN == u32KeyValue)
    {
        s32Ret = uiKEYLEDGetValue(&u32KeyValue);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(uiKEYLEDGetValue, s32Ret);
            return COM_ERR_DEVICE;
        }
    }

#endif

    *pu32KeyValue = u32KeyValue;

    return HI_SUCCESS;
}

HI_S32 UI_KEYLED_Display(HI_CHAR* pcString)
{
    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_CHECK_PARAM(HI_NULL_PTR == pcString);

#ifdef HI_BUILD_WITH_KEYLED
    s32Ret = uiKEYLEDDisplay(pcString);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(uiKEYLEDDisplay, s32Ret);
        return COM_ERR_DEVICE;
    }

#endif

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


