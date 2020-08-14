/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_win_setting.c
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
#include "com_osal.h"
#include "com_debug.h"
#include "com_config.h"
#include "ui_keyled.h"
#include "ui_language.h"
#include "ui_gfx.h"
#include "ui_window.h"
#include "ui_win_setting.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define UI_D_CONFIGWIN_X                (150)
#define UI_D_CONFIGWIN_Y                (88)
#define UI_D_CONFIGWIN_W                (420)
#define UI_D_CONFIGWIN_H                (300)

#define UI_D_CONTENT_POSX               (40)
#define UI_D_CONTENT_POSY               (55)
#define UI_D_CONTENT_HEIGHT             (32)
#define UI_D_CONTENT_INTERVAL           (45)
#define UI_D_CONTENT_BOX_WIDTH          (103)
#define UI_D_CONTENT_BOX_POSX           ((UI_D_CONFIGWIN_W - UI_D_CONTENT_POSX) - UI_D_CONTENT_BOX_WIDTH)

typedef enum tagUI_CFGWIN_ITEM_E
{
    UI_E_CFGWIN_ITEM_PID  = 0x01,
    UI_E_CFGWIN_ITEM_FREQUENCY,
    UI_E_CFGWIN_ITEM_SYMBOL,
    UI_E_CFGWIN_ITEM_MODULATION,
    UI_E_CFGWIN_ITEM_SUBMIT,

    UI_E_CFGWIN_ITEM_BUTT

} SSU_DESCRIPTOR_TYPE_E;

HI_S32 UI_ShowOtaSettingWin(LDDB_PARAM_OTA_S* pstOTAPara)
{
    HI_S32 s32PosX = 0;
    HI_S32 s32PosY = 0;
    HI_U32 u32KeyValue = 0;
    HI_HANDLE hWindow = HI_NULL_PTR;
    HI_HANDLE hSurface = HI_NULL_PTR;

    HI_S32 s32Ret = HI_FAILURE;

    COM_CHECK_PARAM(HI_NULL_PTR == pstOTAPara);

    hWindow = UI_WIN_Create(UI_D_CONFIGWIN_X,
                            UI_D_CONFIGWIN_Y,
                            UI_D_CONFIGWIN_W,
                            UI_D_CONFIGWIN_H,
                            UI_LAN_MAN_UPGRD);
    if (HI_NULL_PTR == hWindow)
    {
        COM_PrintFuncErr(UI_WIN_Create, COM_ERR_DEVICE);
        return COM_ERR_DEVICE;
    }

    s32Ret = UI_WIN_GetSurface(hWindow, &hSurface);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(UI_WIN_GetSurface, s32Ret);
        return s32Ret;
    }

    s32PosY = UI_D_CONTENT_POSY;
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_TITLE_PID, UI_D_CONTENT_POSX, s32PosY));
    COM_CHECK(UI_WIN_ElementCreate(hWindow, UI_E_CFGWIN_ITEM_PID, UI_E_ELEMENT_TYPE_INPUT, UI_D_CONTENT_BOX_POSX, s32PosY, UI_D_CONTENT_BOX_WIDTH, UI_D_CONTENT_HEIGHT));
    COM_CHECK(UI_WIN_ElementSetInputCount(hWindow, UI_E_CFGWIN_ITEM_PID, 4));
    COM_CHECK(UI_WIN_ElementSetContent(hWindow, UI_E_CFGWIN_ITEM_PID, 0, "7000"));

    s32PosY = s32PosY + UI_D_CONTENT_INTERVAL;
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_TITLE_FREQ, UI_D_CONTENT_POSX, s32PosY));
    COM_CHECK(UI_WIN_ElementCreate(hWindow, UI_E_CFGWIN_ITEM_FREQUENCY, UI_E_ELEMENT_TYPE_INPUT, UI_D_CONTENT_BOX_POSX, s32PosY, UI_D_CONTENT_BOX_WIDTH, UI_D_CONTENT_HEIGHT));
    COM_CHECK(UI_WIN_ElementSetInputCount(hWindow, UI_E_CFGWIN_ITEM_FREQUENCY, 3));
    COM_CHECK(UI_WIN_ElementSetContent(hWindow, UI_E_CFGWIN_ITEM_FREQUENCY, 0, "302"));

    s32PosY = s32PosY + UI_D_CONTENT_INTERVAL;
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_TITLE_SYM_RATE, UI_D_CONTENT_POSX, s32PosY));
    COM_CHECK(UI_WIN_ElementCreate(hWindow, UI_E_CFGWIN_ITEM_SYMBOL, UI_E_ELEMENT_TYPE_INPUT, UI_D_CONTENT_BOX_POSX, s32PosY, UI_D_CONTENT_BOX_WIDTH, UI_D_CONTENT_HEIGHT));
    COM_CHECK(UI_WIN_ElementSetInputCount(hWindow, UI_E_CFGWIN_ITEM_SYMBOL, 4));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_SYMBOL, 0, "6875"));

    s32PosY = s32PosY + UI_D_CONTENT_INTERVAL;
    COM_CHECK(UI_DrawText(hSurface, UI_LAN_TITLE_MODULATE, UI_D_CONTENT_POSX, s32PosY));
    COM_CHECK(UI_WIN_ElementCreate(hWindow, UI_E_CFGWIN_ITEM_MODULATION, UI_E_ELEMENT_TYPE_COMBOX, UI_D_CONTENT_BOX_POSX, s32PosY, UI_D_CONTENT_BOX_WIDTH, UI_D_CONTENT_HEIGHT));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, HI_UNF_MOD_TYPE_QAM_16,  "QAM16"));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, HI_UNF_MOD_TYPE_QAM_32,  "QAM32"));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, HI_UNF_MOD_TYPE_QAM_64,  "QAM64"));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, HI_UNF_MOD_TYPE_QAM_128, "QAM128"));
    COM_CHECK(UI_WIN_ElementAddContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, HI_UNF_MOD_TYPE_QAM_256, "QAM256"));

    s32PosX = (UI_D_CONFIGWIN_W - UI_D_CONTENT_BOX_WIDTH) / 2;
    s32PosY = s32PosY + UI_D_CONTENT_INTERVAL;
    COM_CHECK(UI_WIN_ElementCreate(hWindow, UI_E_CFGWIN_ITEM_SUBMIT, UI_E_ELEMENT_TYPE_BUTTOM, s32PosX, s32PosY, UI_D_CONTENT_BOX_WIDTH, UI_D_CONTENT_HEIGHT));
    COM_CHECK(UI_WIN_ElementSetContent(hWindow, UI_E_CFGWIN_ITEM_SUBMIT, 0, UI_LAN_BUTTON_START));

    COM_CHECK(UI_WIN_Show(hWindow));

    while (1)
    {
        s32Ret = UI_KEYLED_GetInput(&u32KeyValue);
        if (HI_SUCCESS != s32Ret)
        {
            COM_PrintFuncErr(UI_KEYLED_GetInput, s32Ret);
            UI_WIN_Hide(hWindow);
            return s32Ret;
        }

        if (UI_E_KEYVALUE_UNKNOWN != u32KeyValue)
        {
            s32Ret = UI_WIN_KeyResolution(hWindow, u32KeyValue);
            if (HI_SUCCESS != s32Ret)
            {
                COM_PrintFuncErr(UI_WIN_KeyResolution, s32Ret);
                UI_WIN_Hide(hWindow);
                return s32Ret;
            }

            switch (u32KeyValue)
            {
                case UI_E_KEYVALUE_OK:
                {
                    HI_U32 u32ElementID = 0;

                    s32Ret = UI_WIN_GetFocusElement(hWindow, &u32ElementID);
                    if (HI_SUCCESS != s32Ret)
                    {
                        COM_PrintFuncErr(UI_WIN_KeyResolution, s32Ret);
                        UI_WIN_Hide(hWindow);
                        return s32Ret;
                    }

                    if (UI_E_CFGWIN_ITEM_SUBMIT == u32ElementID)
                    {
                        HI_U32   u32Value = 0;
                        HI_CHAR* pcText = HI_NULL_PTR;

                        pstOTAPara->enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;

                        s32Ret = UI_WIN_ElementGetContent(hWindow, UI_E_CFGWIN_ITEM_PID, &u32Value, &pcText);
                        if (HI_SUCCESS != s32Ret)
                        {
                            COM_PrintFuncErr(UI_WIN_ElementGetContent, s32Ret);
                            UI_WIN_Hide(hWindow);
                            return s32Ret;
                        }

                        pstOTAPara->u32Pid = COM_StrToU32(pcText);

                        s32Ret = UI_WIN_ElementGetContent(hWindow, UI_E_CFGWIN_ITEM_FREQUENCY, &u32Value, &pcText);
                        if (HI_SUCCESS != s32Ret)
                        {
                            COM_PrintFuncErr(UI_WIN_ElementGetContent, s32Ret);
                            UI_WIN_Hide(hWindow);
                            UI_WIN_Destory(hWindow);
                            return s32Ret;
                        }

                        pstOTAPara->unParam.stCab.stConnectParam.u32Freq = COM_StrToU32(pcText) * 1000;

                        s32Ret = UI_WIN_ElementGetContent(hWindow, UI_E_CFGWIN_ITEM_SYMBOL, &u32Value, &pcText);
                        if (HI_SUCCESS != s32Ret)
                        {
                            COM_PrintFuncErr(UI_WIN_ElementGetContent, s32Ret);
                            UI_WIN_Hide(hWindow);
                            UI_WIN_Destory(hWindow);
                            return s32Ret;
                        }

                        pstOTAPara->unParam.stCab.stConnectParam.u32SymbolRate = COM_StrToU32(pcText) * 1000;

                        s32Ret = UI_WIN_ElementGetContent(hWindow, UI_E_CFGWIN_ITEM_MODULATION, &u32Value, &pcText);
                        if (HI_SUCCESS != s32Ret)
                        {
                            COM_PrintFuncErr(UI_WIN_ElementGetContent, s32Ret);
                            UI_WIN_Hide(hWindow);
                            UI_WIN_Destory(hWindow);
                            return s32Ret;
                        }

                        pstOTAPara->unParam.stCab.stConnectParam.enModType = u32Value;

                        UI_WIN_Hide(hWindow);
                        UI_WIN_Destory(hWindow);
                        return HI_SUCCESS;
                    }

                    break;
                }

                case UI_E_KEYVALUE_EXIT:
                {
                    UI_WIN_Hide(hWindow);
                    UI_WIN_Destory(hWindow);
                    return HI_FAILURE;
                }

                default:
                {
                    break;
                }
            }
        }

        COM_Delayms(10);
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

