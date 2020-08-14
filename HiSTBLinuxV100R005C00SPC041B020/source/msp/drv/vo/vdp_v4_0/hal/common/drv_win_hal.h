
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
#include "hi_drv_win.h"
#include "hi_type.h"
#include "drv_disp_com.h"
#include "vdp_define.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

#define DEF_VIDEO_LAYER_MAX_NUMBER VDP_LAYER_VID_BUTT
#define DEF_VIDEO_LAYER_INVALID_ID 0xff


#if defined(HI_NVR_SUPPORT)

#define HI_WIN_IN_RECT_X_ALIGN      0xFFFFFFFEul
#define HI_WIN_IN_RECT_WIDTH_ALIGN  0xFFFFFFFEul
#define HI_WIN_IN_RECT_Y_ALIGN      0xFFFFFFFCul
#define HI_WIN_IN_RECT_HEIGHT_ALIGN 0xFFFFFFFCul

#else

#define HI_WIN_IN_RECT_X_ALIGN      0xFFFFFFF8ul
#define HI_WIN_IN_RECT_WIDTH_ALIGN  0xFFFFFFF8ul
#define HI_WIN_IN_RECT_Y_ALIGN      0xFFFFFFF8ul
#define HI_WIN_IN_RECT_HEIGHT_ALIGN 0xFFFFFFF8ul
#endif


#define HI_WIN_OUT_RECT_X_ALIGN      0xFFFFFFFEul
#define HI_WIN_OUT_RECT_WIDTH_ALIGN  0xFFFFFFFEul
#define HI_WIN_OUT_RECT_Y_ALIGN      0xFFFFFFFCul
#define HI_WIN_OUT_RECT_HEIGHT_ALIGN 0xFFFFFFFCul


#define DEF_VIDEO_LAYER_MAX_NUMBER VDP_LAYER_VID_BUTT
#define DEF_VIDEO_LAYER_INVALID_ID 0xff

#define VIDEO_LAYER_SUPPORT_MIN_WIDTH   720
#define VIDEO_LAYER_SUPPORT_MIN_HEIGHT  576




typedef struct tagVIDEO_LAYER_PROC_S
{
    HI_BOOL bDci;
    HI_BOOL bSR;
    HI_BOOL bSrBehindDci;
}VIDEO_LAYER_PROC_S;


typedef struct tagVIDEO_TOTAL_CAPABILITY_S
{
    HI_U32 u32TotalLayerNum;
}VIDEO_TOTAL_CAPABILITY_S;

typedef struct tagVIDEO_LAYER_CAPABILITY_S
{
    HI_U32  eId;
    HI_BOOL bZme;
    HI_BOOL bZmePrior;
    HI_BOOL bDcmp;
    HI_BOOL bTnrSnrDbDMDrDs;
    HI_RECT_S stVideoReadSize;

    /*the bitwidth of the layer. used for bg color setting.*/
    HI_U32 u32BitWidth;
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
    HI_U32   u32LayerNO     ;
    HI_U32  u32RegionNum    ;

    HI_DRV_DISP_STEREO_E en3Dmode;
    HI_BOOL bRightEyeFirst;
    HI_DRV_VIDEO_FRAME_S *pstFrame;

    /* since some platform use complete width ,
        * some use cropped width,so we add a original one.
        */
    HI_RECT_S stIn;
    HI_RECT_S stVideo;
    HI_RECT_S stDisp;
    HI_RECT_S stOriRect;/*Vpss InRect,for ZME*/


    /*intermediate Rect for degrading the PQ  as a matter of TEE monitor*/
    HI_BOOL   bHdcpDegradePQ;
    HI_RECT_S stHdcpIntermediateRect;

    HI_DRV_WINDOW_FROST_E enLayerFrostMode;

    HI_U32 u32Fidelity;
    HI_DRV_COLOR_SPACE_E eOutPutColorSpace;
    HI_DISP_DISPLAY_INFO_S *pstDispInfo;

    HI_BOOL bRegionEnable;
    HI_BOOL bFlipEn;
    HI_BOOL bRegionMute;
    HI_BOOL bTnrOnVdp;
    HI_BOOL        bDolbyMode;
    DISP_MMZ_BUF_S stLayerAlgMmz;
}WIN_HAL_PARA_S;

typedef struct tagWBC_INPUT_S {
    /*wbc enable or not.*/
    HI_BOOL                        bWbcEnable                ;

    /*only OutFmt needed,  the input fmt depends on the wbc point.
     * bit width  is useless,  the wbc's input width is invisible for software.*/
    HI_DRV_PIX_FORMAT_E           eOutPutPixelFmt           ;
    /*when write back field mode. tell the cbb: top or bottom.*/
    HI_DRV_FIELD_MODE_E           eFieldMode;
    /*should give input and output  colorspace.*/

    HI_DRV_VID_FRAME_ADDR_S       stAddr;


    /**********************below is for  PQ's  policy.******************************/
    HI_RECT_S stHdWinInputSize;
    HI_RECT_S stHdWinOutputSize;
    HI_RECT_S stSdWinOutputSize;
    HI_DRV_COLOR_SPACE_E          enHdWinSrcColorSpace           ;
    HI_DRV_COLOR_SPACE_E          enHdWinDestColorSpace          ;

    /*when lowdelay mode,  field mode is needed, or frame is needed.*/
    HI_BOOL    bForceFieldMode;
    /*means  should set wbc point to vp.*/
    HI_BOOL    bComplexMode;
}WIN_WBC_CONFIG_S;


typedef struct tagWBC_POLICY_S {
    /*indicates the  output frame or field mode.*/
    HI_BOOL bWbcOutputFieldMode;

    /*PQ give the outsize of wbc  for v3's use.*/
    HI_RECT_S stWbcOutputSize;

    HI_DRV_COLOR_SPACE_E          enSrcColorSpace           ;
    HI_DRV_COLOR_SPACE_E          enDestColorSpace          ;
}WIN_WBC_POLICY_S;


typedef struct tagVIDEO_LAYER_FUNCTIONG_S
{
    HI_S32 (*PF_GetCapability)(HI_U32 u32Layer, VIDEO_LAYER_CAPABILITY_S *pstSurf);
    HI_S32 (*PF_AcquireLayerByDisplay)(HI_DRV_DISPLAY_E eDisp, HI_U32 *pu32Layer);
    HI_S32 (*PF_ReleaseLayer)(HI_U32 u32Layer);
    HI_S32 (*PF_SetEnable)(HI_U32 u32Layer, HI_U32 u32RegionNum, HI_BOOL bEnable);

    HI_S32 (*PF_InitialLayer)(HI_U32 u32Layer);
    HI_S32 (*PF_CheckLayerInit)(HI_U32 u32Layer);
    HI_S32 (*PF_SetAllLayerDefault)(HI_VOID);


    HI_S32 (*PF_MovTop)(HI_U32 u32Layer);
    HI_S32 (*PF_MovBottom)(HI_U32 u32Layer);
    HI_S32 (*PF_GetZorder)(HI_U32 u32Layer, HI_U32 *pZOrder);

    HI_S32 (*PF_SetFramePara)(HI_U32 u32Layer, WIN_HAL_PARA_S *pstPara);
    HI_S32 (*PF_GetWbcPolicy)(WIN_WBC_CONFIG_S *pstPara, WIN_WBC_POLICY_S *pstWbcPolicy);
    HI_S32 (*PF_SetWbcParam) (WIN_WBC_CONFIG_S *pstPara);
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










