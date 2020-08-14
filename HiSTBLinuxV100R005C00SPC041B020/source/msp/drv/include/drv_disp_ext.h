/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_ext.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __DRV_DISP_EXT_H__
#define __DRV_DISP_EXT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include "hi_type.h"
#include "hi_drv_video.h"
#include "hi_drv_disp.h"
#include "hi_drv_dev.h"


typedef HI_S32  (* FN_DISP_Init)(HI_VOID);
typedef HI_S32  (* FN_DISP_DeInit)(HI_VOID);
typedef HI_S32  (* FN_DISP_Attach)(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
typedef HI_S32  (* FN_DISP_Detach)(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave);
typedef HI_S32  (* FN_DISP_SetFormat)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEnFormat);
typedef HI_S32  (* FN_DISP_GetFormat)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E *penEnFormat);
typedef HI_S32  (* FN_DISP_SetHDMI420)(HI_DRV_DISPLAY_E enDisp,HI_BOOL bEnable);

typedef HI_S32  (* FN_DISP_SetCustomTiming)(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming);
typedef HI_S32  (* FN_DISP_GetCustomTiming)(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming);
typedef HI_S32  (* FN_DISP_AddIntf)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
typedef HI_S32  (* FN_DISP_DelIntf)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf);
typedef HI_S32  (* FN_DISP_Open)(HI_DRV_DISPLAY_E enDisp);
typedef HI_S32  (* FN_DISP_Close)(HI_DRV_DISPLAY_E enDisp);
typedef HI_S32  (* FN_DISP_SetEnable)(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
typedef HI_S32  (* FN_DISP_GetEnable)(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable);
typedef HI_S32  (* FN_DISP_SetRightEyeFirst)(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable);
typedef HI_S32  (* FN_DISP_SetBgColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
typedef HI_S32  (* FN_DISP_GetBgColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor);
typedef HI_S32  (* FN_DISP_SetColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);
typedef HI_S32  (* FN_DISP_GetColor)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS);

typedef HI_S32  (* FN_DISP_SetAspectRatio)(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v);
typedef HI_S32  (* FN_DISP_GetAspectRatio)(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Ratio_h, HI_U32 *pu32Ratio_v);
typedef HI_S32  (* FN_DISP_SetLayerZorder)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag);
typedef HI_S32  (* FN_DISP_GetLayerZorder)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

typedef HI_S32  (* FN_DISP_CreateCast) (HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast);
typedef HI_S32  (* FN_DISP_DestroyCast)(HI_HANDLE hCast);
typedef HI_S32  (* FN_DISP_SetCastEnable)(HI_HANDLE hCast, HI_BOOL bEnable);
typedef HI_S32  (* FN_DISP_GetCastEnable)(HI_HANDLE hCast, HI_BOOL *pbEnable);
typedef HI_S32  (* FN_DISP_AcquireCastFrame)(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);
typedef HI_S32  (* FN_DISP_ReleaseCastFrame)(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame);


typedef HI_S32  (* FN_DISP_GetInitFlag)(HI_BOOL *pbInited);
typedef HI_S32  (* FN_DISP_GetVersion)(HI_DRV_DISP_VERSION_S *pstVersion);
typedef HI_BOOL (* FN_DISP_IsOpened)(HI_DRV_DISPLAY_E enDisp);
typedef HI_S32  (* FN_DISP_GetSlave)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penSlave);
typedef HI_S32  (* FN_DISP_GetMaster)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E *penMaster);
typedef HI_S32  (* FN_DISP_GetDisplayInfo)(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S *pstInfo);

typedef HI_S32 (* FN_DISP_Ioctl)(HI_U32 cmd, HI_VOID *arg);
typedef HI_S32 (* FN_DISP_RegCallback)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                                   HI_DRV_DISP_CALLBACK_S *pstCallback);

typedef HI_S32 (* FN_DISP_UnRegCallback)(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                                     HI_DRV_DISP_CALLBACK_S *pstCallback);
typedef HI_S32 (* FN_DISP_ExtAttach)( HI_HANDLE hCast, HI_HANDLE hSink);
typedef HI_S32 (* FN_DISP_ExtDeAttach)( HI_HANDLE hCast, HI_HANDLE hSink);

typedef HI_S32 (* FN_DISP_SetCastAttr)( HI_HANDLE hCast, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32FrmRate);
typedef HI_S32 (* FN_DISP_GetCastAttr)( HI_HANDLE hCast, HI_DRV_DISP_Cast_Attr_S *pstCastAttr);
typedef HI_S32 (*FN_DISP_UpdatePqData)(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);
typedef HI_S32 (*FN_DISP_Suspend)(PM_BASEDEV_S *pdev, pm_message_t state);
typedef HI_S32 (*FN_DISP_Resume)(PM_BASEDEV_S *pdev);
typedef HI_S32 (*FN_DISP_FreeLogoMem)(HI_VOID);
typedef HI_S32 (*FN_DISP_GetLogoInfor)(HI_DRV_DISP_VideoLogoInfo *pstLogoInfor);

typedef struct
{
    FN_DISP_Init                 pfnDispInit;
    FN_DISP_DeInit               pfnDispDeInit;
    FN_DISP_Attach               pfnDispAttach;
    FN_DISP_Detach               pfnDispDetach;
    FN_DISP_SetFormat            pfnDispSetFormat;
    FN_DISP_GetFormat            pfnDispGetFormat;
	FN_DISP_SetHDMI420			 pfnDispSetHDMI420;
    FN_DISP_SetCustomTiming      pfnDispSetCustomTiming;
    FN_DISP_GetCustomTiming      pfnDispGetCustomTiming;
    FN_DISP_AddIntf              pfnDispAddIntf;
    FN_DISP_DelIntf              pfnDispDeIntf;
    FN_DISP_Open                 pfnDispOpen;
    FN_DISP_Close                pfnDispClose;
    FN_DISP_SetEnable            pfnDispSetEnable;
    FN_DISP_GetEnable            pfnDispGetEnable;
    FN_DISP_SetRightEyeFirst     pfnDispSetRightEyeFirst;
    FN_DISP_SetBgColor           pfnDispSetBgColor;
    FN_DISP_GetBgColor           pfnDispGetBgColor;
    FN_DISP_SetColor             pfnDispSetColor;
    FN_DISP_GetColor             pfnDispGetColor;
    FN_DISP_SetAspectRatio       pfnDispSetAspectRatio;
    FN_DISP_GetAspectRatio       pfnDispGetAspectRatio;
    FN_DISP_SetLayerZorder       pfnDispSetLayerZorder;
    FN_DISP_GetLayerZorder       pfnDispGetLayerZorder;
    FN_DISP_CreateCast           pfnDispCreatCast;
    FN_DISP_DestroyCast          pfnDispDestoryCast;
    FN_DISP_SetCastEnable        pfnDispSetCastEnable;
    FN_DISP_GetCastEnable        pfnDispGetCastEnable;
    FN_DISP_AcquireCastFrame     pfnDispAcquireCastFrm;
    FN_DISP_ReleaseCastFrame     pfnDispRlsCastFrm;
    FN_DISP_SetCastAttr          pfnDispSetCastAttr;
    FN_DISP_GetCastAttr          pfnDispGetCastAttr;
    FN_DISP_ExtAttach            pfnDispExtAttach;
    FN_DISP_ExtAttach            pfnDispExtDeAttach;
    FN_DISP_GetInitFlag          pfnDispGetInitFlag;
    FN_DISP_GetVersion           pfnDispGetVersion;
    FN_DISP_IsOpened             pfnDispIsOpen;
    FN_DISP_GetSlave             pfnDispGetSlave;
    FN_DISP_GetMaster            pfnDispGetMaster;
    FN_DISP_GetDisplayInfo       pfnDispGetDispInfo;
    FN_DISP_Ioctl                pfnDispIoctl;
    FN_DISP_RegCallback          pfnDispRegCallback;
    FN_DISP_UnRegCallback        pfnDispUnRegCallback;
	
	#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a) 
    FN_DISP_UpdatePqData         pfnDispUpdatePqData;
	#endif
	
	FN_DISP_Suspend pfnDispSuspend;
    FN_DISP_Resume pfnDispResume;
	
	FN_DISP_FreeLogoMem          pfnDispFreeLogoMem;
	FN_DISP_GetLogoInfor         pfnDispGetLogoInfor;
}DISP_EXPORT_FUNC_S;

HI_S32  DRV_DISP_Register(HI_VOID);
HI_VOID DRV_DISP_UnRegister(HI_VOID);

HI_S32  VDP_DRV_ModInit(HI_VOID);
HI_VOID VDP_DRV_ModExit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __DRV_DISP_EXT_H__ */

