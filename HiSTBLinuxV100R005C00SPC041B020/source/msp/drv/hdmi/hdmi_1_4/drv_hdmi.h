/******************************************************************************

  Copyright (C), 2010-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : otpm_hdmi.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/6/20
  Description   :
  History       :
  Date          : 2010/11/22
  Author        : l00168554
  Modification  :
*******************************************************************************/

#ifndef __OPTM_HDMI_H__
#define __OPTM_HDMI_H__

#include "hi_error_mpi.h"
//#include "mpi_priv_hdmi.h"
#include "hi_drv_hdmi.h"
#include "hi_drv_disp.h"
#include "drv_hdmi_ioctl.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

HI_U32 DRV_HDMI_Init(HI_U32 FromUserSpace);
HI_U32 DRV_HDMI_DeInit(HI_U32 FromUserSpace);
HI_U32 DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi, HDMI_OPEN_S *pOpen, HI_U32 FromUserSpace,HI_U32 u32ProcID);
HI_U32 DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi);
HI_U32 DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_EDID_BASE_INFO_S *pstSinkAttr);
HI_U32 DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr);
HI_U32 DRV_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_ATTR_S *pstAttr);
HI_U32 DRV_HDMI_SetCECCommand(HI_UNF_HDMI_ID_E enHdmi, const HI_UNF_HDMI_CEC_CMD_S  *pCECCmd);
HI_U32 DRV_HDMI_GetCECCommand(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_CMD_S  *pCECCmd, HI_U32 timeout);
HI_U32 DRV_HDMI_CECStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_CEC_STATUS_S  *pStatus);
HI_U32 DRV_HDMI_SetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
HI_U32 DRV_HDMI_GetInfoFrame(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);
HI_U32 DRV_HDMI_ReadEvent(HI_UNF_HDMI_ID_E enHdmi,HI_U32 procID);
void DRV_HDMI_NotifyEvent(HI_UNF_HDMI_EVENT_TYPE_E event);
HI_U32 DRV_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi);
HI_U32 DRV_HDMI_Stop(HI_UNF_HDMI_ID_E enHdmi);
HI_U32 DRV_HDMI_SetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E enDeepColor);
HI_U32 DRV_HDMI_GetDeepColor(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DEEP_COLOR_E *penDeepColor);
HI_U32 DRV_HDMI_SetxvYCCMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bEnable);
HI_U32 DRV_HDMI_SetAVMute(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL bAvMute);
HI_U32 DRV_HDMI_PreFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enEncodingFormat);
HI_U32 DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_DISP_FMT_E enFmt, HI_DRV_DISP_STEREO_E enStereo);
HI_U32 DRV_HDMI_Force_GetEDID(HDMI_EDID_S *pEDID);
HI_U32 DRV_HDMI_Read_EDID(HDMI_EDID_S *pEDID);
HI_U32 DRV_HDMI_GetPlayStatus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus);
HI_U32 DRV_HDMI_GetCECAddress(HI_U8 *pPhyAddr, HI_U8 *pLogicalAddr);
HI_U32 DRV_HDMI_LoadKey(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_LOAD_KEY_S *pstLoadKey);

HI_S32 DRV_HDMI_GetProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32ProcID);
HI_S32 DRV_HDMI_ReleaseProcID(HI_UNF_HDMI_ID_E enHdmi, HI_U32 u32ProcID);

HI_S32 DRV_HDMI_AudioChange(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);
HI_S32 DRV_HDMI_GetAOAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);
//HI_S32 DRV_HDMI_AdjustInfoFrame(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_INFOFRAME_S *pstInfoFrame);

HI_S32 DRV_HDMI_GetInitNum(HI_UNF_HDMI_ID_E enHdmi);
HI_S32 DRV_HDMI_GetProcNum(HI_UNF_HDMI_ID_E enHdmi);
HI_S32 DRV_HDMI_GetUserInitNum(HI_UNF_HDMI_ID_E enHdmi);

HI_S32 DRV_HDMI_SetAPPAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_APP_ATTR_S *pstHDMIAppAttr,HI_BOOL UpdateFlag);
HI_S32 DRV_HDMI_SetAOAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_AUDIO_ATTR_S *pstHDMIAOAttr,HI_BOOL UpdateFlag);
HI_S32 DRV_HDMI_SetVOAttr(HI_UNF_HDMI_ID_E enHdmi,HDMI_VIDEO_ATTR_S *pstHDMIVOAttr,HI_BOOL UpdateFlag);
//HI_S32 DRV_HDMI_SetHDMIAttr(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_HDMI_ATTR_S *pstHDMIAttr);
//HI_S32 DRV_HDMI_ConfigAttr(HI_UNF_HDMI_ID_E enHdmi);

HI_S32 DRV_HDMI_GetStatus(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_STATUS_S *pHdmiStatus);

HI_S32 DRV_HDMI_GetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pHdmiDelay);
HI_S32 DRV_HDMI_SetDelay(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_DELAY_S *pHdmiDelay);


void HDMI_IRQ_Setup(void);
void HDMI_IRQ_Exit(void);
HI_S32 DRV_HDMI_Register(HI_VOID);
HI_S32 DRV_HDMI_UnRegister(HI_VOID);

HI_VOID DRV_O5_HDMI_PutBinInfoFrame(HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType,HI_VOID* infor_ptr);
HI_VOID DRV_O5_HDMI_GetBinInfoFrame(HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType, HI_VOID *infor_ptr);


typedef enum VIDEO_SAMPLE_TYPE_E_S
{
    VIDEO_SAMPLE_TYPE_UNKNOWN,                  /**<Unknown*/ /**<CNcomment: 未知采样方式*/
    VIDEO_SAMPLE_TYPE_PROGRESSIVE,              /**<Progressive*/ /**<CNcomment: 采样方式为逐行*/
    VIDEO_SAMPLE_TYPE_INTERLACE,                /**<Interlaced*/ /**<CNcomment: 采样方式为隔行*/
    VIDEO_SAMPLE_TYPE_BUTT
}VIDEO_SAMPLE_TYPE_E;

enum hdmi_switch_state {
	STATE_PLUG_UNKNOWN = -1,
	STATE_HOTPLUGOUT = 0,
	STATE_HOTPLUGIN = 1
};



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /* __OPTM_HDMI_H__ */

/*------------------------------------END-------------------------------------*/
