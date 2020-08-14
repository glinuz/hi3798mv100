/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_unf_hdmi.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/6/20
  Description   :
  History       :
  Date          : 2010/6/20
  Author        : q46153 l00168554
  Modification  :
*******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_unf_hdmi.h"
#include "hi_mpi_hdmi.h"

HI_S32 HI_UNF_HDMI_Init(void)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Init();

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_DeInit(void)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_DeInit();

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_OPEN_PARA_S *pstOpenPara)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Open(enHdmi,pstOpenPara);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Close(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetStatus(enHdmi, pHdmiStatus);

    return s32Ret;
}


HI_S32 HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pstSinkAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetSinkCapability(enHdmi, pstSinkAttr);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetAttr(enHdmi, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetAttr(enHdmi, pstAttr);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetCECCommand(enHdmi, pCECCmd);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout)
{
    HI_S32 s32Ret;

    if(timeout > 3000)
        return -1;

    s32Ret = HI_MPI_HDMI_GetCECCommand(enHdmi, pCECCmd, timeout);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_CECStatus(enHdmi, pStatus);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_CEC_Enable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_CEC_Enable(enHdmi);

    return s32Ret;
}
/*
HI_S32 HI_UNF_HDMI_AuthenticationInit(HI_UNF_HDMI_ID_E enHdmi)
HI_S32 HI_UNF_HDMI_AuthenticationDeInit(HI_UNF_HDMI_ID_E enHdmi)
HI_S32 HI_UNF_HDMI_AuthenticationEnable(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
*/
HI_S32 HI_UNF_HDMI_CEC_Disable(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_CEC_Disable(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetInfoFrame(enHdmi, pstInfoFrame);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetInfoFrame(enHdmi, enInfoFrameType, pstInfoFrame);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Start(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Stop(enHdmi);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetDeepColor(enHdmi, enDeepColor);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetDeepColor(enHdmi, penDeepColor);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetxvYCCMode(enHdmi, bEnable);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetAVMute(enHdmi, bAvMute);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_Force_GetEDID(HI_UNF_HDMI_ID_E enHdmi, HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_Force_GetEDID(enHdmi, u8Edid, u32EdidLength);

    return s32Ret;
}
HI_S32 HI_UNF_HDMI_ReadEDID(HI_U8 *u8Edid, HI_U32 *u32EdidLength)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_ReadEDID(u8Edid, u32EdidLength);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_RegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_RegCallbackFunc(enHdmi, pstCallbackFunc);

    return s32Ret;
}
HI_S32 HI_UNF_HDMI_UnRegCallbackFunc(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CALLBACK_FUNC_S *pstCallbackFunc)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_UnRegCallbackFunc(enHdmi, pstCallbackFunc);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_LoadHDCPKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_LoadHDCPKey(enHdmi, pstLoadKey);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_SetDelay(enHdmi, pstDelay);

    return s32Ret;
}

HI_S32 HI_UNF_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pstDelay)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_HDMI_GetDelay(enHdmi, pstDelay);

    return s32Ret;
}

/*------------------------------END--------------------------------*/
