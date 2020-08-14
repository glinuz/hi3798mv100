
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_win_hal.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_HAL_H__
#define __DRV_WIN_HAL_H__


#include "hi_type.h"
#include "drv_disp_com.h"
#include "vdp_define.h"
#include "vdp_driver.h"


#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


#define HI_WIN_IN_RECT_X_ALIGN      0xFFFFFFFEul
#define HI_WIN_IN_RECT_WIDTH_ALIGN  0xFFFFFFFEul
#define HI_WIN_IN_RECT_Y_ALIGN      0xFFFFFFFCul
#define HI_WIN_IN_RECT_HEIGHT_ALIGN 0xFFFFFFF8ul

#define HI_WIN_OUT_RECT_X_ALIGN      0xFFFFFFFEul
#define HI_WIN_OUT_RECT_WIDTH_ALIGN  0xFFFFFFFEul
#define HI_WIN_OUT_RECT_Y_ALIGN      0xFFFFFFFCul
#define HI_WIN_OUT_RECT_HEIGHT_ALIGN 0xFFFFFFFCul


#define VIDEO_ZOOM_IN_VERTICAL_MAX   4
#define VIDEO_ZOOM_IN_HORIZONTAL_MAX 4

#define DEF_VIDEO_LAYER_MAX_NUMBER VDP_LAYER_VID_BUTT
#define DEF_VIDEO_LAYER_INVALID_ID 0xff

#define VIDEO_LAYER_SUPPORT_MIN_WIDTH   720
#define VIDEO_LAYER_SUPPORT_MIN_HEIGHT  576

typedef enum tagVIDEO_LAYER_ACC_MODE_E
{
    VIDEO_LAYER_ACC_DISABLE = 0,
    VIDEO_LAYER_ACC_AUTO,
    VIDEO_LAYER_ACC_MODE_BUTT
}VIDEO_LAYER_ACC_MODE_E;


typedef enum tagVIDEO_LAYER_ACM_MODE_E
{
    VIDEO_LAYER_ACM_DISABLE = 0,
    VIDEO_LAYER_ACM_AUTO,
    VIDEO_LAYER_ACM_MODE_BUTT
}VIDEO_LAYER_ACM_MODE_E;


typedef enum tagVIDEO_ALG_LOCATION
{
    ALG_LOCATION_IN_V0 = 0,
    ALG_LOCATION_IN_VP0,
    ALG_LOCATION_IN_BUTT
}VIDEO_ALG_LOCATION_E;

#if defined (CHIP_HIFONEB02)
typedef struct tagVIDEO_LAYER_STATUS_S
{
    HI_BOOL  bMultiMode;
    HI_U32   u32WinNum;
}VIDEO_LAYER_STATUS_S;
#endif
typedef struct tagVIDEO_LAYER_PROC_S
{
    HI_BOOL bDci;
    HI_BOOL bSR;
    VIDEO_ALG_LOCATION_E eSrLocation;
    VIDEO_ALG_LOCATION_E eDciLocation;
    HI_BOOL bSrBehindDci;
}VIDEO_LAYER_PROC_S;


typedef struct tagVIDEO_LAYER_CAPABILITY_S
{
    HI_BOOL bSupport;
    HI_U32  eId;
    HI_BOOL bZme;
    HI_BOOL bACC;
    HI_BOOL bACM;
    HI_BOOL bLTICTI;
    HI_BOOL bDcmp;

    /*although  DCI and SR may be  in SR, but it's mainly for window on V0
     *so we make it a spec of video layer.
     */
    HI_BOOL bDci;
    HI_BOOL bSR;

    HI_BOOL bHDIn;
    HI_BOOL bHDOut;

    /*the bitwidth of the layer. used for bg color setting.*/
    HI_U32 u32BitWidth;
#if defined (CHIP_HIFONEB02)	
    /*the read bitwidth from amba bus.*/
    HI_U32 u32BitWidthExternalBus;
#endif
    HI_U32 u32LayerWidthMax;
    HI_U32 u32LayerHeightMax;

    VIDEO_LAYER_PROC_S stLayerProcInfo;
}VIDEO_LAYER_CAPABILITY_S;

typedef struct tagVIDEO_LAYER_S
{
    HI_BOOL bWorking;
    HI_BOOL bInitial;
}VIDEO_LAYER_S;

typedef struct tagVIDEO_LAYER_FRAME_PARA_S
{
    HI_DRV_FRAME_TYPE_E  eFrmType;
    HI_DRV_PIX_FORMAT_E  eVideoFormat;

    HI_BOOL bInterlaced;
    HI_BOOL bTopFirst;

    HI_RECT_S stIn;
    HI_RECT_S stDisp;
    HI_RECT_S stVideo;

    HI_DRV_COLOR_SPACE_E eSrcCS;
    HI_DRV_COLOR_SPACE_E eDstCs;

    HI_U32 u32AddrNumber;
    HI_DRV_VID_FRAME_ADDR_S stAddr[2];

}VIDEO_LAYER_FRAME_PARA_S;


typedef struct tagWIN_HAL_PARA_S
{
    HI_DRV_DISP_STEREO_E en3Dmode;
    HI_BOOL bRightEyeFirst;
	HI_BOOL bSecure;

    HI_DRV_VIDEO_FRAME_S *pstFrame;

    HI_BOOL bZmeUpdate;
    HI_BOOL bZmeSupport;

    /* since some platform use complete width ,
     * some use cropped width,so we add a original one.
     */
    HI_RECT_S stInOrigin;
    HI_RECT_S stIn;
    HI_RECT_S stVideo;
    HI_RECT_S stDisp;
    HI_RECT_S stOriRect;/*Vpss InRect,for ZME*/


    /*for chiptype which has a second zme module such as SR.*/
    HI_RECT_S stFinalZmeRect;
    /*to judge the second zme module action.*/
    HI_BOOL bSecondHorZmeEnable;
    HI_BOOL bSecondVerZmeEnable;

    HI_U32 u32Fidelity;
    HI_DISP_DISPLAY_INFO_S *pstDispInfo;
    HI_DRV_DISP_FIELD_FLAG_E eField;
    HI_BOOL bRegionMute;
    HI_U32  u32RegionNum;
}WIN_HAL_PARA_S;


typedef struct tagVIDEO_LAYER_FUNCTIONG_S
{
    HI_S32 (*PF_GetCapability)(HI_U32 u32Layer, VIDEO_LAYER_CAPABILITY_S *pstSurf);
#if defined (CHIP_HIFONEB02)
    HI_S32 (*PF_GetLayerAttr) (HI_U32 u32Layer, VIDEO_LAYER_STATUS_S *pstSurf);
#endif
    HI_S32 (*PF_AcquireLayerByDisplay)(HI_DRV_DISPLAY_E eDisp, HI_U32 *pu32Layer);
    HI_S32 (*PF_ReleaseLayer)(HI_U32 u32Layer);

    HI_S32 (*PF_SetEnable)(HI_U32 u32Layer, HI_U32 u32RegionNum, HI_BOOL bEnable);

    HI_S32 (*PF_VP0ParaUpd)(HI_U32 u32Layer);
    HI_S32 (*PF_Update)(HI_U32 u32Layer);

    HI_S32 (*PF_SetDefault)(HI_U32 u32Layer);
    HI_S32 (*PF_ChckLayerInit)(HI_U32 u32Layer);

    HI_S32 (*PF_SetAllLayerDefault)(HI_VOID);

    HI_S32 (*PF_SetDispMode)(HI_U32 u32Layer, HI_DRV_DISP_STEREO_MODE_E eMode);
    HI_S32 (*PF_SetColor)(HI_U32 u32Layer, HI_DRV_DISP_COLOR_SETTING_S *pstColor);

    HI_S32 (*PF_MovUp)(HI_U32 u32Layer);
    HI_S32 (*PF_MovTop)(HI_U32 u32Layer);
    HI_S32 (*PF_MovDown)(HI_U32 u32Layer);
    HI_S32 (*PF_MovBottom)(HI_U32 u32Layer);

    HI_S32 (*PF_GetZorder)(HI_U32 u32Layer, HI_U32 *pZOrder);
    HI_S32 (*PF_SetDebug)(HI_U32 u32Layer, HI_BOOL bEnable);

    HI_S32 (*PF_SetFramePara)(HI_U32 u32Layer, WIN_HAL_PARA_S *pstPara);
    HI_S32 (*PF_Get3DOutRect)(HI_DRV_DISP_STEREO_E en3DMode, HI_RECT_S *pstOutRect, HI_RECT_S *pstReviseOutRect);
	HI_BOOL (*PF_GetLayerRevisedPixelFmt)(HI_U32 u32LayerId, HI_RECT_S *pstOutRect, HI_DRV_PIX_FORMAT_E *penFmt,HI_DISP_DISPLAY_INFO_S *pstDispInfo);	
    HI_S32 (*PF_GetCSCReg)(HI_U32 u32Data, HI_U32 *pdata);
    HI_S32 (*PF_SetCSCReg)(HI_U32 u32Data, HI_U32 *pdata);
    HI_S32 (*PF_SetZMEPhase)(HI_U32 u32Data, HI_S32 s32PhaseL, HI_S32 s32PhaseC);
}VIDEO_LAYER_FUNCTIONG_S;

HI_S32 VideoLayer_Init(HI_VOID);
HI_S32 VideoLayer_DeInit(HI_VOID);

HI_S32 VideoLayer_GetFunction(VIDEO_LAYER_FUNCTIONG_S *pstFunc);

VIDEO_LAYER_FUNCTIONG_S *VideoLayer_GetFunctionPtr(HI_VOID);



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_VO_HAL_H__  */










