/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : ui_hdmi.c
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

#include "hi_unf_common.h"
#include "hi_unf_hdmi.h"
#include "com_debug.h"
#include "com_osal.h"

#ifdef HI_HDCP_SUPPORT
#include "hi_flash.h"
#endif

#ifdef HI_ADVCA_FUNCTION_RELEASE
#include "ca_ssd.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static HI_UNF_HDMI_ID_E             s_enHDMI;
static HI_UNF_HDMI_CALLBACK_FUNC_S  s_stCallBackFunc;

#ifdef HI_HDCP_SUPPORT
static HI_U32 s_u32HDCPFailCount = 0;

static HI_S32 hdmiHdcpLoadKey(HI_UNF_HDMI_ID_E enHDMIId)
{
    HI_U32 u32Len = 0;

    HI_UNF_HDMI_LOAD_KEY_S stLoadKey;
    HI_HANDLE hFlash = HI_INVALID_HANDLE;
    HI_Flash_InterInfo_S stFlashInfo;
    HI_U8* pu8TmpBuf = HI_NULL_PTR;
    FILE* pBinFile = HI_NULL_PTR;

    HI_S32 s32Ret = HI_SUCCESS;

    hFlash = HI_Flash_OpenByName("privatedata");
    if (HI_INVALID_HANDLE == hFlash)
    {
        COM_PrintFuncErr(HI_Flash_OpenByName, s32Ret);
        return COM_ERR_DEVICE;
    }

    s32Ret = HI_Flash_GetInfo(hFlash, &stFlashInfo);
    if (s32Ret != HI_SUCCESS)
    {
        COM_PrintFuncErr(HI_Flash_GetInfo, s32Ret);
        HI_Flash_Close(hFlash);
        return COM_ERR_DEVICE;
    }

    pu8TmpBuf = (HI_U8*)COM_MemMalloc(stFlashInfo.PageSize);
    if (HI_NULL_PTR == pu8TmpBuf)
    {
        COM_PrintErrCode(COM_ERR_MEM);
        HI_Flash_Close(hFlash);
        return COM_ERR_MEM;
    }

    COM_MemSet(pu8TmpBuf, 0x00, stFlashInfo.PageSize);
    s32Ret = HI_Flash_Read(hFlash, 0, pu8TmpBuf, stFlashInfo.PageSize, HI_FLASH_RW_FLAG_RAW);
    if (0 > s32Ret)
    {
        COM_PrintFuncErr(HI_Flash_Read, s32Ret);

        COM_MemFreeSetNull(pu8TmpBuf);
        HI_Flash_Close(hFlash);
        return COM_ERR_DEVICE;
    }

    COM_MemSet(&stLoadKey, 0, sizeof(HI_UNF_HDMI_LOAD_KEY_S));
    stLoadKey.u32KeyLength = 332;

#ifdef HI_ADVCA_FUNCTION_RELEASE
    CA_SSD_DecryptImage(pu8TmpBuf, 336, HI_CA_KEY_GROUP_1);
#else

    if (pu8TmpBuf[2] != 'H' || pu8TmpBuf[3] != 'I')
    {
        COM_INFO("HDCP key invalided!\n");
    }

#endif

    stLoadKey.pu8InputEncryptedKey = pu8TmpBuf;

    s32Ret = HI_UNF_HDMI_LoadHDCPKey(enHDMIId, &stLoadKey);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_LoadHDCPKey, s32Ret);
        COM_MemFreeSetNull(pu8TmpBuf);
        HI_Flash_Close(hFlash);
        return COM_ERR_DEVICE;
    }

    COM_MemFreeSetNull(pu8TmpBuf);
    HI_Flash_Close(hFlash);

    return HI_SUCCESS;
}

static HI_VOID hdmiHdcpFailProc(HI_UNF_HDMI_ID_E enHDMI)
{
    HI_UNF_HDMI_ATTR_S stHdmiAttr;

    s_u32HDCPFailCount ++;

    if (10 == s_u32HDCPFailCount)
    {
        HI_UNF_HDMI_GetAttr(enHDMI, &stHdmiAttr);
        stHdmiAttr.bEnableVideo  = HI_FALSE;
        stHdmiAttr.bEnableAudio  = HI_FALSE;
        HI_UNF_HDMI_SetAttr(enHDMI, &stHdmiAttr);
    }

    return;
}

#endif

static HI_VOID hdmiPlugProc(HI_UNF_HDMI_ID_E enHDMI)
{
    HI_UNF_HDMI_ATTR_S stHdmiAttr;
    HI_UNF_HDMI_STATUS_S stHdmiStatus;
    HI_UNF_EDID_BASE_INFO_S stSinkCap;

    HI_S32 s32Ret = HI_SUCCESS;

    COM_DBG_FuncEnter();

    COM_MemSet(&stHdmiStatus, 0x00, sizeof(&stHdmiStatus));

    s32Ret = HI_UNF_HDMI_GetStatus(enHDMI, &stHdmiStatus);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_GetStatus, s32Ret);
        return;
    }

    if (HI_TRUE != stHdmiStatus.bConnected)
    {
        COM_PrintErrCode(COM_ERR_UNAVAILABLE);
        return;
    }

    COM_MemSet(&stHdmiAttr, 0x00, sizeof(&stHdmiAttr));
    s32Ret = HI_UNF_HDMI_GetAttr(enHDMI, &stHdmiAttr);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_GetAttr, s32Ret);

        COM_MemSet(&stHdmiAttr, 0x00, sizeof(&stHdmiAttr));
        stHdmiAttr.bEnableHdmi  = HI_TRUE;
        stHdmiAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }

    COM_MemSet(&stSinkCap, 0x00, sizeof(&stSinkCap));
    s32Ret = HI_UNF_HDMI_GetSinkCapability(enHDMI, &stSinkCap);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_GetSinkCapability, s32Ret);
        stHdmiAttr.bEnableHdmi  = HI_TRUE;
        stHdmiAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
    else
    {
        if (HI_TRUE == stSinkCap.bSupportHdmi)
        {
            stHdmiAttr.bEnableHdmi = HI_TRUE;

            if (HI_TRUE != stSinkCap.stColorSpace.bYCbCr444)
            {
                stHdmiAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
            }
            else
            {
                stHdmiAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
            }
        }
        else
        {
            stHdmiAttr.bEnableHdmi  = HI_FALSE;
            stHdmiAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
        }
    }

    if (HI_TRUE == stHdmiAttr.bEnableHdmi)
    {
        stHdmiAttr.bEnableAudio         = HI_FALSE;
        stHdmiAttr.bEnableVideo         = HI_TRUE;
        stHdmiAttr.bEnableAudInfoFrame  = HI_TRUE;
        stHdmiAttr.bEnableAviInfoFrame  = HI_TRUE;
    }
    else
    {
        stHdmiAttr.bEnableAudio         = HI_FALSE;
        stHdmiAttr.bEnableVideo         = HI_TRUE;
        stHdmiAttr.bEnableAudInfoFrame  = HI_FALSE;
        stHdmiAttr.bEnableAviInfoFrame  = HI_FALSE;
    }

#ifdef HI_HDCP_SUPPORT
    stHdmiAttr.bHDCPEnable = HI_TRUE;
#endif

    COM_CHECK(HI_UNF_HDMI_SetAttr(enHDMI, &stHdmiAttr));
    COM_CHECK(HI_UNF_HDMI_Start(enHDMI));

    COM_DBG_FuncExit();
    return;
}

static HI_VOID hdmiUnPlugProc(HI_UNF_HDMI_ID_E enHDMI)
{
    COM_DBG_FuncEnter();

    HI_UNF_HDMI_Stop(s_enHDMI);

    COM_DBG_FuncExit();
    return;
}

static HI_VOID hdmiEventProc(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID* pPrivateData)
{
    switch (event)
    {
        case HI_UNF_HDMI_EVENT_HOTPLUG:
        {
            hdmiPlugProc(s_enHDMI);
            break;
        }

        case HI_UNF_HDMI_EVENT_NO_PLUG:
        {
            hdmiUnPlugProc(s_enHDMI);
            break;
        }

#ifdef HI_HDCP_SUPPORT

        case HI_UNF_HDMI_EVENT_HDCP_FAIL:
        {
            hdmiHdcpFailProc(s_enHDMI);
            break;
        }

#endif

        default:
        {
            break;
        }
    }

    return;
}


HI_S32 UI_HDMI_Init(HI_UNF_HDMI_ID_E enHDMIId, HI_UNF_ENC_FMT_E enWantFmt)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_UNF_HDMI_OPEN_PARA_S stOpenParam;

    COM_DBG_FuncEnter();

    s_enHDMI = enHDMIId;

    s32Ret = HI_UNF_HDMI_Init();
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_Init, s32Ret);
        return COM_ERR_DEVICE;
    }

#ifdef HI_HDCP_SUPPORT
    s32Ret = hdmiHdcpLoadKey(enHDMIId);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(hdmiHdcpLoadKey, s32Ret);
        HI_UNF_HDMI_DeInit();
        return COM_ERR_DEVICE;
    }

#endif

    s_stCallBackFunc.pfnHdmiEventCallback = hdmiEventProc;
    s_stCallBackFunc.pPrivateData = HI_NULL_PTR;

    s32Ret = HI_UNF_HDMI_RegCallbackFunc(enHDMIId, &s_stCallBackFunc);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_RegCallbackFunc, s32Ret);
        HI_UNF_HDMI_DeInit();
        return COM_ERR_DEVICE;
    }

    stOpenParam.enDefaultMode = HI_UNF_HDMI_DEFAULT_ACTION_HDMI;
    s32Ret = HI_UNF_HDMI_Open(enHDMIId, &stOpenParam);
    if (HI_SUCCESS != s32Ret)
    {
        COM_PrintFuncErr(HI_UNF_HDMI_Open, s32Ret);
        HI_UNF_HDMI_DeInit();
        return COM_ERR_DEVICE;
    }


    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

HI_S32 UI_HDMI_DeInit(HI_UNF_HDMI_ID_E enHDMIId)
{
    COM_DBG_FuncEnter();

    HI_UNF_HDMI_Stop(enHDMIId);

    HI_UNF_HDMI_Close(enHDMIId);

    HI_UNF_HDMI_UnRegCallbackFunc(enHDMIId, &s_stCallBackFunc);

    HI_UNF_HDMI_DeInit();

    COM_DBG_FuncExit();
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


