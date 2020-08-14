/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_disp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

*******************************************************************************/


#ifndef __MPI_DISP_H__
#define __MPI_DISP_H__

#include "hi_drv_disp.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

#define CHECK_DISP_INIT()\
do{\
    HI_DISP_LOCK();\
    if (g_DispDevFd < 0)\
    {\
        HI_ERR_DISP("DISP is not init.\n");\
        HI_DISP_UNLOCK();\
        return HI_ERR_DISP_NO_INIT;\
    }\
    HI_DISP_UNLOCK();\
}while(0)


HI_S32 HI_MPI_DISP_Init(HI_VOID);
HI_S32 HI_MPI_DISP_DeInit(HI_VOID);

HI_S32 HI_MPI_DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
HI_S32 HI_MPI_DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);

HI_S32 HI_MPI_DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enFormat);
HI_S32 HI_MPI_DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E *penStereo,HI_DRV_DISP_FMT_E *penFormat);

HI_S32 HI_MPI_DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bRFirst);
HI_S32 HI_MPI_DISP_SetVirtualScreen(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Width, HI_U32 u32Height);
HI_S32 HI_MPI_DISP_GetVirtualScreen(HI_DRV_DISPLAY_E enDisp, HI_U32 *u32Width, HI_U32 *u32Height);
    
HI_S32 HI_MPI_DISP_SetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S *pstOffset);
HI_S32 HI_MPI_DISP_GetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S *pstOffset);

HI_S32 HI_MPI_DISP_SetTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming);
HI_S32 HI_MPI_DISP_GetTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming);

HI_S32 HI_MPI_DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
HI_S32 HI_MPI_DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);

HI_S32 HI_MPI_DISP_AddVDAC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf, HI_U32 u32Number);
HI_S32 HI_MPI_DISP_DelVDAC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);

HI_S32 HI_MPI_DISP_Open(HI_DRV_DISPLAY_E enDisp);
HI_S32 HI_MPI_DISP_Close(HI_DRV_DISPLAY_E enDisp);

HI_S32 HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
HI_S32 HI_MPI_DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);

HI_S32 HI_MPI_DISP_SetBGColor(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_COLOR_S *pstBGColor);
HI_S32 HI_MPI_DISP_GetBGColor(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_COLOR_S *pstBGColor);

//set color
HI_S32 HI_MPI_DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
HI_S32 HI_MPI_DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);

//set aspect ratio
HI_S32 HI_MPI_DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v);
HI_S32 HI_MPI_DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v);

HI_S32 HI_MPI_DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag);
HI_S32 HI_MPI_DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

//miracast
HI_S32 HI_MPI_DISP_CreateCast(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S *pstCfg, HI_HANDLE *phCast);
HI_S32 HI_MPI_DISP_DestroyCast(HI_HANDLE hCast);
HI_S32 HI_MPI_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable);
HI_S32 HI_MPI_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable);

HI_S32 HI_MPI_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 HI_MPI_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
HI_S32 HI_MPI_DISP_ExtAttach(HI_HANDLE hCast, HI_HANDLE hSink);
HI_S32 HI_MPI_DISP_ExtDeAttach(HI_HANDLE hCast, HI_HANDLE hSink);

//snapshot
HI_S32 HI_MPI_DISP_Snapshot_Acquire(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstFrame);
HI_S32 HI_MPI_DISP_Snapshot_Release(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstFrame);

//Macrovision
HI_S32 HI_MPI_DISP_TestMacrovisionSupport(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbSupport);
HI_S32 HI_MPI_DISP_SetMacrovisionCustomer(HI_DRV_DISPLAY_E enDisp, HI_VOID *pData);
HI_S32 HI_MPI_DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode);
HI_S32 HI_MPI_DISP_GetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E *penMode);

//cgms-a
HI_S32 HI_MPI_DISP_SetCGMS_A(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S *pstCfg);

//vbi
HI_S32 HI_MPI_DISP_CreateVBI(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_VBI_CFG_S *pstCfg, HI_HANDLE *phVbi);
HI_S32 HI_MPI_DISP_DestroyVBI(HI_HANDLE hVbi);
HI_S32 HI_MPI_DISP_SendVBIData(HI_HANDLE hVbi, const HI_DRV_DISP_VBI_DATA_S *pstVbiData);
HI_S32 HI_MPI_DISP_SetWss(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_WSS_DATA_S *pstWssData);

//may be deleted
//HI_S32 HI_MPI_DISP_SetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg);
//HI_S32 HI_MPI_DISP_GetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg);


//setting
//HI_S32 HI_MPI_DISP_SetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting);
//HI_S32 HI_MPI_DISP_GetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S *pstSetting);
//HI_S32 HI_MPI_DISP_ApplySetting(HI_DRV_DISPLAY_E enDisp);


HI_S32 HI_MPI_DISP_SetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Brightness);
HI_S32 HI_MPI_DISP_GetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Brightness);
HI_S32 HI_MPI_DISP_SetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Contrast);
HI_S32 HI_MPI_DISP_GetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Contrast);
HI_S32 HI_MPI_DISP_SetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Saturation);
HI_S32 HI_MPI_DISP_GetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Saturation);
HI_S32 HI_MPI_DISP_SetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 u32HuePlus);
HI_S32 HI_MPI_DISP_GetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32HuePlus);

HI_S32 HI_MPI_DISP_Suspend(HI_VOID);
HI_S32 HI_MPI_DISP_Resume(HI_VOID);
HI_S32 HI_MPI_DISP_SetDacOutputEnable(HI_BOOL bEnable);

HI_S32 HI_MPI_DISP_SetCgms(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_CGMSA_CFG_S *pstCgmsCgf);
HI_S32 HI_MPI_DISP_SetIsogenyAttr(const HI_DRV_DISP_ISOGENY_ATTR_S *pstIsogeny,const HI_U32 u32Num);
HI_S32 HI_MPI_DISP_GetIsogenyAttr(HI_DRV_DISP_ISOGENY_ATTR_S *pstIsogeny,const HI_U32 u32Num);

HI_S32 HI_MPI_DISP_SetDispOutput(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_OUT_TYPE_E enDispType);
HI_S32 HI_MPI_DISP_GetDispOutput(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OUT_TYPE_E *penDispType);
HI_S32 HI_MPI_DISP_SetBT2020Enable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
HI_S32 HI_MPI_DISP_GetBT2020Enable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
