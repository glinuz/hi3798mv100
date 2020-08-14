
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_disp_x.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#ifndef __DRV_WIN_PROC_H__
#define __DRV_WIN_PROC_H__

#include "drv_win_hal.h"
#include "drv_win_wbc.h"

typedef struct hiWIN_RROC_FOR_DEVELOPER_S
{
    /*sr dci location in layer, for debug use.*/
    VIDEO_LAYER_PROC_S stSrDciPhysicalInfo;
    /*the current win num, if > 2, sr not open.*/
    HI_U32 u32WinNum;

    /*only in 4k, sr can be opened now.*/
    HI_RECT_S eCurrentFmt;
    /*when scaled down ,sr should not be opened.*/
    HI_BOOL bExistScaleDown_WhenRatioRevise;
    /*whether in 3d or not.*/
    HI_BOOL bIn3DMode;

    /*sr effect activated or not;*/
    HI_BOOL bHorSrOpenInPreProcess;
    HI_BOOL bVerSrOpenInPreProcess;

    HI_BOOL bHorSrOpenInPostProcess;
    HI_BOOL bVerSrOpenInPostProcess;


    /*frame size.*/
    HI_RECT_S stOringinFrameSize;
    HI_RECT_S stFinalWinOutputSize;
    HI_RECT_S stOutputSizeOfV0;

    HI_RECT_S stVdpRequire;
    HI_RECT_S stVpssGive;

    HI_RECT_S stSrOutputSize;

    /*the information of dci.*/
    HI_BOOL   bDciOpen;
    HI_RECT_S stOriginDCIPositionInFrame;
    HI_RECT_S stDciFrameSize;

    /*dci input size, the finale position should give a revise.*/
    HI_RECT_S stDciEffecttiveContentInputSize;
    HI_RECT_S stWinFinalPosition;

}HI_WIN_RROC_FOR_DEVELOPER_S;

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */













#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /*  __DRV_DISP_X_H__  */










